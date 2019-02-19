# Trivial Makefile to build OpenAL shared libraries for Android,
# and copy them over to Castle Game Engine subdirs.

.PHONY: default
default:
	cd android && ndk-build

.PHONY: clean
clean:
	cd android && rm -Rf libs/ obj/

.PHONY: build
build: clean default
	cp -f android/libs/armeabi-v7a/libopenal.so \
	  $(CASTLE_ENGINE_PATH)/tools/build-tool/data/android/integrated-services/sound/app/src/main/jni/armeabi-v7a/libopenal.so
	cp -f android/libs/arm64-v8a/libopenal.so \
	  $(CASTLE_ENGINE_PATH)/tools/build-tool/data/android/integrated-services/sound/app/src/main/jni/arm64-v8a/libopenal.so
