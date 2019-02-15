# Same as Castle Game Engine minimal platform version
TARGET_PLATFORM := android-16

ROOT_PATH := $(call my-dir)

########################################################################################################

include $(CLEAR_VARS)

LOCAL_MODULE     := openal
LOCAL_ARM_MODE   := arm
LOCAL_PATH       := $(ROOT_PATH)
LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/../../include $(LOCAL_PATH)/../../OpenAL32/Include
LOCAL_SRC_FILES  := ../../OpenAL32/alAuxEffectSlot.c \
                    ../../OpenAL32/alBuffer.c        \
                    ../../OpenAL32/alEffect.c        \
                    ../../OpenAL32/alError.c         \
                    ../../OpenAL32/alExtension.c     \
                    ../../OpenAL32/alFilter.c        \
                    ../../OpenAL32/alListener.c      \
                    ../../OpenAL32/alSource.c        \
                    ../../OpenAL32/alState.c         \
                    ../../OpenAL32/alThunk.c         \
                    ../../Alc/ALc.c                  \
                    ../../Alc/alcConfig.c            \
                    ../../Alc/alcDedicated.c         \
                    ../../Alc/alcEcho.c              \
                    ../../Alc/alcModulator.c         \
                    ../../Alc/alcReverb.c            \
                    ../../Alc/alcRing.c              \
                    ../../Alc/alcThread.c            \
                    ../../Alc/ALu.c                  \
                    ../../Alc/bs2b.c                 \
                    ../../Alc/helpers.c              \
                    ../../Alc/hrtf.c                 \
                    ../../Alc/mixer.c                \
                    ../../Alc/panning.c              \
                    ../../Alc/backends/android.c     \
                    ../../Alc/backends/loopback.c    \
                    ../../Alc/backends/null.c        \

#                    ../../Alc/backends/opensl.c     \

LOCAL_CFLAGS     := -ffast-math -DAL_BUILD_LIBRARY -DAL_ALEXT_PROTOTYPES
LOCAL_LDLIBS     := -llog -Wl,-s

include $(BUILD_SHARED_LIBRARY)

# Michalis commented out Tremolo,
# doesn't compile anymore (with NDK platform 16),
# assembler fails.
# Doesn't matter in our case -- we have tremolo from
# https://github.com/michaliskambi/tremolo-android

# ########################################################################################################

# include $(CLEAR_VARS)

# LOCAL_MODULE     := tremolo
# LOCAL_ARM_MODE   := arm
# LOCAL_PATH       := $(ROOT_PATH)/tremolo
# LOCAL_SRC_FILES  := bitwise.c      \
#                     bitwiseARM.s   \
#                     codebook.c     \
#                     dpen.s         \
#                     dsp.c          \
#                     floor0.c       \
#                     floor1.c       \
#                     floor1ARM.s    \
#                     floor1LARM.s   \
#                     floor_lookup.c \
#                     framing.c      \
#                     info.c         \
#                     mapping0.c     \
#                     mdct.c         \
#                     mdctARM.s      \
#                     mdctLARM.s     \
#                     misc.c         \
#                     res012.c       \
#                     speed.s        \
#                     vorbisfile.c   \

# LOCAL_CFLAGS     := -ffast-math -D_ARM_ASSEM_

# include $(BUILD_STATIC_LIBRARY)

# ########################################################################################################

# include $(CLEAR_VARS)

# LOCAL_MODULE     := example
# LOCAL_ARM_MODE   := arm
# LOCAL_PATH       := $(ROOT_PATH)
# LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include $(LOCAL_PATH)/tremolo
# LOCAL_SRC_FILES  := example.c
# LOCAL_LDLIBS     := -llog -Wl,-s

# LOCAL_STATIC_LIBRARIES := libtremolo
# LOCAL_SHARED_LIBRARIES := libopenal

# # Michalis+, following
# # http://stackoverflow.com/questions/19986523/shared-library-text-segment-is-not-shareable
# # to avoid errors;
# # [armeabi-v7a] SharedLibrary  : libexample.so
# # /home/michalis/installed/android/ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/../lib/gcc/arm-linux-androideabi/4.9.x/../../../../arm-linux-androideabi/bin/ld: warning: shared library text segment is not shareable
# # /home/michalis/installed/android/ndk/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/../lib/gcc/arm-linux-androideabi/4.9.x/../../../../arm-linux-androideabi/bin/ld: error: treating warnings as errors
# # collect2: error: ld returned 1 exit status
# LOCAL_LDLIBS += -Wl,--no-warn-shared-textrel

# include $(BUILD_SHARED_LIBRARY)

# ########################################################################################################
