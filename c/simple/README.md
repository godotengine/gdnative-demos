# Simple example using C

This is a small example using C to create a GDNative script
that just showcases some very simple bare bones calls.

Language: [GDNative C](https://docs.godotengine.org/en/latest/tutorials/scripting/gdnative/index.html)

Renderer: GLES 2

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
clang -std=c11 -fPIC -c -I../godot-headers simple.c -o simple.os
clang -shared simple.os -o ../project/gdnative/linuxbsd/libsimple.so
```

This creates the file `libsimple.so` in the `project/gdnative/linuxbsd` directory.


### macOS
On macOS:

```
cd src
clang -std=c11 -fPIC -c -I../godot-headers simple.c -o simple.os -arch x86_64
clang -dynamiclib simple.os -o ../project/gdnative/macos/libsimple.dylib -arch x86_64
```

This creates the file `libsimple.dylib` in the `project/gdnative/macos` directory.


### Windows
On Windows:

```
cd src
cl /Fosimple.obj /c simple.c /nologo -EHsc -DNDEBUG /MD /I. /I../godot-headers
link /nologo /dll /out:..\project\gdnative\windows\libsimple.dll /implib:..\project\gdnative\windows\libsimple.lib simple.obj
```

This creates the file `libsimple.dll` in the `project/gdnative/windows` directory.


## Usage

Create a new object using `load("res://simple.gdns").new()`

This object has following methods you can use:
 * `get_data()`
