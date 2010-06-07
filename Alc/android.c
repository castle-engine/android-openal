/**
 * OpenAL cross platform audio library
 * Copyright (C) 2010 by Chris Robinson
 * This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the
 *  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *  Boston, MA  02111-1307, USA.
 * Or go to http://www.gnu.org/copyleft/lgpl.html
 */

#include "config.h"

#include <stdlib.h>
#include <jni.h>
#include <pthread.h>
#include "alMain.h"
#include "AL/al.h"
#include "AL/alc.h"

static JavaVM* javaVM;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    javaVM = vm;
    return JNI_VERSION_1_2;
}

typedef struct
{
    ALCdevice* device;

    pthread_t thread;
    volatile int running;

    int buffers;
    int frequency;
    int channels;
    int bytes;

} AndroidData;

#define STREAM_MUSIC 3
#define CHANNEL_CONFIGURATION_MONO 2
#define CHANNEL_CONFIGURATION_STEREO 3
#define ENCODING_PCM_8BIT 3
#define ENCODING_PCM_16BIT 2
#define MODE_STREAM 1

static void* thread_function(void* arg)
{
    AndroidData* data = (AndroidData*)arg;

    JNIEnv* env;
    (*javaVM)->AttachCurrentThread(javaVM, &env, NULL);

    (*env)->PushLocalFrame(env, 3);

    jclass cAudioTrack = (*env)->FindClass(env, "android/media/AudioTrack");
    jmethodID mAudioTrack = (*env)->GetMethodID(env, cAudioTrack, "<init>", "(IIIIII)V");
    jmethodID mGetMinBufferSize = (*env)->GetStaticMethodID(env, cAudioTrack, "getMinBufferSize", "(III)I");
    jmethodID mPlay = (*env)->GetMethodID(env, cAudioTrack, "play", "()V");
    jmethodID mStop = (*env)->GetMethodID(env, cAudioTrack, "stop", "()V");
    jmethodID mRelease = (*env)->GetMethodID(env, cAudioTrack, "release", "()V");
    jmethodID mWrite = (*env)->GetMethodID(env, cAudioTrack, "write", "([BII)I");

    int sampleRateInHz = data->frequency;
    int channelConfig = data->channels == 1 ? CHANNEL_CONFIGURATION_MONO : CHANNEL_CONFIGURATION_STEREO;
    int audioFormat = data->bytes == 1 ? ENCODING_PCM_8BIT : ENCODING_PCM_16BIT;

    int bufferSizeInBytes = (*env)->CallStaticIntMethod(env, cAudioTrack, 
        mGetMinBufferSize, sampleRateInHz, channelConfig, audioFormat);

    int bufferSizeInSamples = bufferSizeInBytes / data->channels / data->bytes;

    jobject track = (*env)->NewObject(env, cAudioTrack, mAudioTrack,
        STREAM_MUSIC, sampleRateInHz, channelConfig, audioFormat, data->buffers * bufferSizeInBytes, MODE_STREAM);

    (*env)->CallNonvirtualVoidMethod(env, track, cAudioTrack, mPlay);

    jarray buffer = (*env)->NewByteArray(env, bufferSizeInBytes);

    while (data->running)
    {
        void* pBuffer = (*env)->GetPrimitiveArrayCritical(env, buffer, NULL);

        if (pBuffer)
        {
            aluMixData(data->device, pBuffer, bufferSizeInSamples);
            (*env)->ReleasePrimitiveArrayCritical(env, buffer, pBuffer, 0);

            (*env)->CallNonvirtualIntMethod(env, track, cAudioTrack, mWrite, buffer, 0, bufferSizeInBytes);
        }
        else
        {
            AL_PRINT("Failed to get pointer to array bytes");
        }
    }
    
    (*env)->CallNonvirtualVoidMethod(env, track, cAudioTrack, mStop);
    (*env)->CallNonvirtualVoidMethod(env, track, cAudioTrack, mRelease);

    (*env)->PopLocalFrame(env, NULL);

    (*javaVM)->DetachCurrentThread(javaVM);
    return NULL;
}

static const ALCchar android_device[] = "Android Software";

static ALCboolean android_open_playback(ALCdevice *device, const ALCchar *deviceName)
{
    AndroidData* data;
    int channels;
    int bytes;

    if (!deviceName)
    {
        deviceName = android_device;
    }
    else if(strcmp(deviceName, android_device) != 0)
    {
        return ALC_FALSE;
    }

    data = (AndroidData*)calloc(1, sizeof(*data));
    data->device = device;
    device->szDeviceName = strdup(deviceName);
    device->ExtraData = data;
    return ALC_TRUE;
}

static void android_close_playback(ALCdevice *device)
{
    AndroidData* data = (AndroidData*)device->ExtraData;
    if (data != NULL)
    {
        free(data);
        device->ExtraData = NULL;
    }
}

static ALCboolean android_reset_playback(ALCdevice *device)
{
    AndroidData* data = (AndroidData*)device->ExtraData;

    int channels = aluChannelsFromFormat(device->Format);
    int bytes = aluBytesFromFormat(device->Format);

    if (channels != 1 && channels != 2)
    {
        AL_PRINT("Output with %d channels is not suppported", channels);
        return ALC_FALSE;
    }

    if (bytes != 1 && bytes != 2)
    {
        AL_PRINT("Output with %d bytes per sample is not supported", bytes);
        return ALC_FALSE;
    }

    SetDefaultChannelOrder(device);

    data->buffers = device->NumUpdates;
    data->frequency = device->Frequency;
    data->channels = channels;
    data->bytes = bytes;

    data->running = 1;
    pthread_create(&data->thread, NULL, thread_function, data);

    return ALC_TRUE;
}

static void android_stop_playback(ALCdevice *device)
{
    AndroidData* data = (AndroidData*)device->ExtraData;

    if (data->running)
    {
        data->running = 0;

        pthread_join(data->thread, NULL);
    }
}

static ALCboolean android_open_capture(ALCdevice *pDevice, const ALCchar *deviceName)
{
    (void)pDevice;
    (void)deviceName;
    return ALC_FALSE;
}

static void android_close_capture(ALCdevice *pDevice)
{
    (void)pDevice;
}

static void android_start_capture(ALCdevice *pDevice)
{
    (void)pDevice;
}

static void android_stop_capture(ALCdevice *pDevice)
{
    (void)pDevice;
}

static void android_capture_samples(ALCdevice *pDevice, ALCvoid *pBuffer, ALCuint lSamples)
{
    (void)pDevice;
    (void)pBuffer;
    (void)lSamples;
}

static ALCuint android_available_samples(ALCdevice *pDevice)
{
    (void)pDevice;
    return 0;
}

static const BackendFuncs android_funcs = {
    android_open_playback,
    android_close_playback,
    android_reset_playback,
    android_stop_playback,
    android_open_capture,
    android_close_capture,
    android_start_capture,
    android_stop_capture,
    android_capture_samples,
    android_available_samples
};

void alc_android_init(BackendFuncs *func_list)
{
    *func_list = android_funcs;
}

void alc_android_deinit(void)
{
}

void alc_android_probe(int type)
{
    if (type == DEVICE_PROBE)
    {
        AppendDeviceList(android_device);
    }
    else if (type == ALL_DEVICE_PROBE)
    {
        AppendAllDeviceList(android_device);
    }
}
