# Instance binding demo using C

Dependencies:
 * You need [Godot headers](https://github.com/godotengine/godot-headers),
   this is now a Git submodule of this repo.
 * `clang`, `gcc`, or any decent C compiler that's C11 compatible.

## Compile with SCons (cross platform)
You can use SCons to compile the library if you have it installed:

```
scons platform=PLATFORM
```

Where PLATFORM is: `windows`, `linuxbsd`, or `macos`.


## Manually compiling

### Linux
To compile the library on Linux, do

```
cd src
clang -std=c11 -fPIC -c -I../godot_headers instance_binding.c -o instance_binding.os
clang -shared instance_binding.os -o ../project/gdnative/linuxbsd/libinstance_binding.so
```

This creates the file `libinstance_binding.so` in the `project/gdnative/linuxbsd` directory.


### macOS
On macOS:

```
cd src
clang -std=c11 -fPIC -c -I../godot_headers instance_binding.c -o instance_binding.os -arch x86_64
clang -dynamiclib instance_binding.os -o ../project/gdnative/macos/libinstance_binding.dylib -arch x86_64
```

This creates the file `libinstance_binding.dylib` in the `project/gdnative/macos` directory.


### Windows
On Windows:

```
cd src
cl /Foinstance_binding.obj /c instance_binding.c /nologo -EHsc -DNDEBUG /MD /I. /I../godot_headers
link /nologo /dll /out:..\project\gdnative\windows\libinstance_binding.dll /implib:..\project\gdnative\windows\libinstance_binding.lib instance_binding.obj
```

This creates the file `libinstance_binding.dll` in the `project/gdnative/windows` directory.
