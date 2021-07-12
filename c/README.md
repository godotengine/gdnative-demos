# C demos

These demos show how to use the GDNative C bindings.

Language: [GDNative C](https://docs.godotengine.org/en/latest/tutorials/scripting/gdnative/index.html)

Dependencies:
 * You need [Godot headers](https://github.com/godotengine/godot-headers),
   this is now a Git submodule of this repo.
 * `clang`, `gcc`, or any decent C compiler that's C11 compatible.

## Compiling

For each of the C demos, you can use SCons to compile the library:

```
scons platform=PLATFORM
```

Where PLATFORM is: `windows`, `linuxbsd`, or `macos`.

This creates the file `libsimple` in the respective
subfolders in the `project/gdnative` directory.
