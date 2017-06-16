# Simple example using C

This is a small example using C to create a GDNative script that just showcases some very simple bare bones calls

## Compiling

Dependencies:
 * You need to have the [Godot headers](https://github.com/GodotNativeTools/godot_headers) saved somewhere on your system
 * clang or any decent C compiler that's C11 or C99 compatible

To compile the library, do

  cd src
  clang -std=c11 -fPIC -c -I/PATH/TO/GODOT/HEADERS simple.c -o simple.os
  clang -shared simple.os -o libsimple.so

This creates the file `libsimple.so` in your `src` directory.
For windows you need to find out what compiler flags need to be used, I don't know which ones. (If you do, feel free to fork and update this project and README)

On mac:

  cd src
  clang -std=c11 -fPIC -c -I/PATH/TO/GODOT/HEADERS simple.c -o simple.os -arch i386 -arch x86_64
  clang -dynamiclib simple.os -o libsimple.dylib -arch i386 -arch x86_64 -framework Cocoa -framework IOKit -framework CoreVideo -Wl,-undefined,dynamic_lookup

This creates the file 'libsimple.dylib' as a universal binary (or alternatively remove one of the -arch options from both commands if you want to just compile for one architecture).

## Usage

Create a new object using `load("res://SIMPLE.gdn").new()`

This object has following methods you can use:
 * `get_data()`

