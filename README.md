# Easily compile OpenAL for Android for Castle Game Engine

This repository allows to easily recompile OpenAL (sound playing library)
and copy the result over the _Castle Game Engine_ sources.
It assumes:

- You have installed Android NDK, and `ndk-build` is on $PATH.

- The source code of _Castle Game Engine_ is in `$CASTLE_ENGINE_PATH`.

Then just run `make build` .

It will be recompile the library for
the appropriate Android versions and architectures
-- the ones used by _Castle Game Engine_.

# Credits

Forked from http://repo.or.cz/w/openal-soft/android.git .
