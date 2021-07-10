# Dodge the Creeps with GDNative C++

This is a simple game where your character must move
and avoid the enemies for as long as possible, made in GDNative C++.

This is a finished version of the game featured in the
["Your first game"](https://docs.godotengine.org/en/latest/getting_started/step_by_step/your_first_game.html)
tutorial in the documentation. For more details,
consider following the tutorial in the documentation.

Language: GDScript

Renderer: GLES 3 (particles are not available in GLES 2)

Note: There is a GDScript version available [here](https://github.com/godotengine/godot-demo-projects/tree/master/2d/dodge_the_creeps).

Note: There is a C# version available [here](https://github.com/godotengine/godot-demo-projects/tree/master/mono/dodge_the_creeps).

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

## Screenshots

![GIF from the documentation](https://docs.godotengine.org/en/latest/_images/dodge_preview.gif)

## Copying

`art/House In a Forest Loop.ogg` Copyright &copy; 2012 [HorrorPen](https://opengameart.org/users/horrorpen), [CC-BY 3.0: Attribution](http://creativecommons.org/licenses/by/3.0/). Source: https://opengameart.org/content/loop-house-in-a-forest

Images are from "Abstract Platformer". Created in 2016 by kenney.nl, [CC0 1.0 Universal](http://creativecommons.org/publicdomain/zero/1.0/). Source: https://www.kenney.nl/assets/abstract-platformer

Font is "Xolonium". Copyright &copy; 2011-2016 Severin Meyer <sev.ch@web.de>, with Reserved Font Name Xolonium, SIL open font license version 1.1. Details are in `fonts/LICENSE.txt`.
