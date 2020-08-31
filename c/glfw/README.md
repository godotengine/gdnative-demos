# GLFW example using C

This is a small example using C to create a GDNative script that
exposes a smaaaaall part of the GLFW API.

Dependencies:
 * You need [Godot headers](https://github.com/godotengine/godot_headers),
   this is now a submodule of this repo.
 * `clang`, `gcc`, or any decent C compiler that's C11 or C99 compatible.
 * You need to have GLFW and its headers installed.

## Compile with SCons (cross platform)
You can use SCons to compile the library if you have it installed:

```
scons platform=PLATFORM
```

Where platform is: windows, linuxbsd, or macos


## Manually compiling

### Linux
To compile the library on Linux, do

```
cd src
clang -std=c11 -fPIC -c -I../godot_headers init.c -o init.os
clang -shared -lglfw init.os -o ../project/gdnative/linuxbsd/libglfw.so

# Or use GCC.
gcc -std=c11 -fPIC -c -I../godot_headers init.c -o init.os
gcc -shared init.os -o ../project/gdnative/linuxbsd/libglfw.so -lglfw
```

This creates the file `libsimple.so` in the `project/gdnative/linuxbsd` directory.


### macOS
On macOS:

```
clang -std=c11 -fPIC -c -I../godot_headers -I/PATH/TO/GLFW/HEADERS init.c -o init.os -arch x86_64
clang -dynamiclib libglfw.dylib init.os -o project/gdnative/macos/libglfw_godot.dylib -arch x86_64 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -Wl,-undefined,dynamic_lookup
```

This creates the file `libsimple.dylib` in the `project/gdnative/macos` directory.

If you want to statically link GLFW in just replace libglfw.dylib by libglfw.a, make sure to copy these files from GLFW.


### Windows
On Windows: **TODO: Missing GLFW-specific instructions**

```
cd src
cl /Fosimple.obj /c simple.c /nologo -EHsc -DNDEBUG /MD /I. /I../godot_headers
link /nologo /dll /out:..\project\gdnative\windows\libsimple.dll /implib:..\project\gdnative\windows\libsimple.lib simple.obj
```

This creates the file `libsimple.dll` in the `project/gdnative/windows` directory.


## Usage

Create a window object using `load("res://GLFW.gdn").new()`

This window object has following methods you can use:
 * `create(int width, int height, string title)`
 * `close()`
 * `is_closed()`
 * `poll_events()`

Also it has one signal, `closed`, which can be used to get notified when the window should be closed.
