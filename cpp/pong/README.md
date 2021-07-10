# Pong with GDNative C++

A simple Pong game, made with GDNative C++. This demo shows best practices
for game development in Godot, including
[signals](https://docs.godotengine.org/en/latest/getting_started/step_by_step/signals.html).

Language: C++

Renderer: GLES 2

Note: There is a GDScript version available [here](https://github.com/godotengine/godot-demo-projects/tree/master/2d/pong).

Note: There is a C# version available [here](https://github.com/godotengine/godot-demo-projects/tree/master/mono/pong).

Note: There is a VisualScript version available [here](https://github.com/godotengine/godot-demo-projects/tree/master/visual_script/pong).

## Compiling

You can use SCons to compile the library:

```
scons platform=PLATFORM
```

Where PLATFORM is: `windows`, `linux`, or `osx`.

This creates the file `libsimple` in the respective
subfolders in the `project/gdnative` directory.

Dependencies:
 * You need [godot-cpp](https://github.com/godotengine/godot-cpp),
   this is now a Git submodule of this repo.
 * `clang`, `gcc`, or any decent C compiler that's C++14 compatible.

## How does it work?

The walls, paddle, and ball are all
[`Area2D`](https://docs.godotengine.org/en/latest/classes/class_area2d.html)
nodes. When the ball touches the walls or the paddles,
they emit signals and modify the ball.

## Screenshots

![Screenshot](screenshots/pong.png)
