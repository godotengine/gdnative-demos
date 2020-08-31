# Simple example using C

This is a small example using C to create a GDNative script that just showcases some very simple bare bones calls

## Compiling

Dependencies:

 * You need [Godot headers](https://github.com/GodotNativeTools/godot_headers), this is now a submodule of this repo
 * clang or any decent C compiler that's C11 or C99 compatible

### Scons (cross platform)
You can use scons to compile the library if you have it installed:

```
scons platform=PLATFORM
```

Where platform is: windows, linuxbsd, or macos

### Linux
To compile the library on Linux, do

```
cd src
clang -std=c11 -fPIC -c -I../godot_headers simple.c -o simple.os
clang -shared simple.os -o ../project/gdnative/linuxbsd/libsimple.so
```

This creates the file `libsimple.so` in your `project/gdnative/linuxbsd` directory.
For windows you need to find out what compiler flags need to be used, I don't know which ones. (If you do, feel free to fork and update this project and README)

### Mac OS X
On Mac OS X:

```
cd src
clang -std=c11 -fPIC -c -I../godot_headers simple.c -o simple.os -arch i386 -arch x86_64
clang -dynamiclib simple.os -o ../project/gdnative/macos/libsimple.dylib -arch i386 -arch x86_64
```

This creates the file 'libsimple.dylib' as a universal binary (or alternatively remove one of the -arch options from both commands if you want to just compile for one architecture).

### Windows
On Windows:

```
cd src
cl /Fosimple.obj /c simple.c /nologo -EHsc -DNDEBUG /MD /I. /I../godot_headers
link /nologo /dll /out:..\project\gdnative\windows\libsimple.dll /implib:..\project\gdnative\windows\libsimple.lib simple.obj
```

This creates the file `libsimple.dll` in your `project/gdnative/windows` directory.


## Usage

Create a new object using `load("res://SIMPLE.gdns").new()`

This object has following methods you can use:
 * `get_data()`

