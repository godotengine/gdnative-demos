# Simple example using C

This is a small example using C to create a GDNative script that just showcases some very simple bare bones calls

## Compiling

Dependencies:
 * You need to have the [Godot headers](https://github.com/GodotNativeTools/godot_headers) saved somewhere on your system
 * clang or any decent C compiler that's C11 or C99 compatible

### Scons (cross platform)
You can use scons to compile the library if you have it installed:

    scons platform=PLATFORM

Where platform is: x11, osx or windows
Optionally you can specify:

    headers=/PATH/TO/GODOT/HEADERS

### Linux
To compile the library on Linux, do

    cd src
    clang -std=c11 -fPIC -c -I/PATH/TO/GODOT/HEADERS simple.c -o simple.os
    clang -shared simple.os -o ../bin/libsimple.so

This creates the file `libsimple.so` in your `src` directory.
For windows you need to find out what compiler flags need to be used, I don't know which ones. (If you do, feel free to fork and update this project and README)

### Mac OS X
On Mac OS X:

    cd src
    clang -std=c11 -fPIC -c -I/PATH/TO/GODOT/HEADERS simple.c -o simple.os -arch i386 -arch x86_64
    clang -dynamiclib simple.os -o ../bin/libsimple.dylib -arch i386 -arch x86_64

This creates the file 'libsimple.dylib' as a universal binary (or alternatively remove one of the -arch options from both commands if you want to just compile for one architecture).

### Windows
Cross-compile from macOS

You first need to install `mingw-w64` by executing the following command

    brew install mingw-w64

Please see [this](http://docs.godotengine.org/en/stable/development/compiling/compiling_for_windows.html#cross-compiling-for-windows-from-other-operating-systems) for instruction on how to test that you have installed it successfully. In short and general, you would be able to execute

    x86_64-w64-mingw32-gcc --version

Then if it shows version as output, you're good to go!

Next

    cd src
    x86_64-w64-mingw32-gcc -c -Wall -Werror -fpic simple.c
    x86_64-w64-mingw32-gcc -shared -o simple.dll -I/PATH/TO/GODOT/HEADERS

This will create `simple.o` at the same directory as `simple.c` then create a shared library that could be loaded dynamically at `bin/simple.dll`.

> Please note that for Windows we use `simple.dll` **not** `libsimple.dll` as this name is pre-set in `bin/simple.gdnlib`. Anyway, whichever name is ok to use.
    

## Usage

Create a new object using `load("res://bin/simple.gdns").new()`

This object has following methods you can use:
 * `get_data()`

For example

```
onready var data = preload("res://bin/simple.gdns").new()

func _ready():
    print(data)
```

You should see `World from GDNative!` as output from console from within Godot editor!

## Important Note

`simple.c` is coded to refer to `SIMPLE` class name for our custom module. The project is already set to refer to that name for `bin/simple.gdnlib` (see its inspector panel).

If you create your own custom module and specify wrong name different from what you use inside C source code, then Godot won't be able to load your module.

## How to freshly create your own custom module

See [FRESH-CREATE.md](https://github.com/haxpor/GDNative-demos/blob/master/c/SimpleDemo/FRESH-CREATE.md) for instructions.
