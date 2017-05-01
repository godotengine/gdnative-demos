# GLFW Example using C

This is a small example using C to create a GDNative script that
exposes a smaaaaall part of the GLFW API.

## Compiling

Dependencies:
 * You need to have the [Godot headers](https://github.com/GodotNativeTools/godot_headers) saved somewhere on your system
 * You need to have GLFW and its headers installed
 * clang or any decent C compiler that's C11 or C99 compatible

To compile the library, do

	cd src
	clang -std=c11 -fPIC -c -I/PATH/TO/GODOT/HEADERS init.c -o init.os
	clang -shared -lglfw init.os -o libglfw.so

This creates the file `libglfw.so` in your `src` directory.
For windows you need to find out what compiler flags need to be used, I don't know which ones. (If you do, feel free to fork and update this project and README)

## Usage

Create a window object using `load("res://GLFW.gdn").new()`

This window object has following methods you can use:
 * `create(int width, int height, string title)`
 * `close()`
 * `is_closed()`
 * `poll_events()`

Also it has one signal, `closed`, which can be used to get notified when the window should be closed.
