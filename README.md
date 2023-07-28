# This repository is no longer useful for Castle Game Engine, it is archived

We no longer build OpenAL libraries for OpenAL this way. Instead, CGE includes C++ source code of OpenAL, and we let Gradle compile them, as part of building Android package.

# Easily compile OpenAL for Android for Castle Game Engine

This repository allows to easily recompile OpenAL (sound playing library)
and copy the result over the _Castle Game Engine_ sources.
It assumes:

- You have installed Android NDK, and `ndk-build` is on $PATH.

- The source code of _Castle Game Engine_ is in `$CASTLE_ENGINE_PATH`.

Then just run `make build` .

It will recompile the library for
the appropriate Android versions and architectures used by _Castle Game Engine_:

- 32-bit ARM
- 64-bit ARM (aka Aarch64)
- 32-bit X86
- 64-bit X86 (x86_64)

# Credits

Forked from http://repo.or.cz/w/openal-soft/android.git .
