# GDNative example of a kinematic character

This is a demo showing how to use GDNative together with the cpp_bindings layer to implement a couple of new nodes.
This demo was originally created by Ramesh Ravone.

Before compiling this module you must clone https://github.com/GodotNativeTools/godot_headers and https://github.com/GodotNativeTools/godot-cpp
Note that both are git submodule in Scripts/src so you can initialise these with git submodule but at you'll likely use these for other projects as well I advise giving them their own location.

After both repositories have been downloaded you need to compile the cpp bindings, for this follow the instructions in the godot-cpp repository.

Note that if you are on the latest master of Godot it is possible godot_headers is outdated. Instead of using this repository you can also use <godot-source>/modules/gdnative/include
Make sure that you compile both godot-cpp and this repository using the same headers.

Once you are then ready to compile this module simply cd into the Scripts subfolder and execute:
scons platform=xyz
where xyz is your platform (linux, window and osx are currently supported).

If you have installed godot_headers and/or godot-cpp in a different location you can use the swiches headers and cpp_bindings respectively. You can also set these paths using the environmental variables GODOT_HEADERS and CPP_BINDINGS, that is particularly useful if you have multiple projects.

Finally you can switch between release and debug builds by adding the switch target. This must line up with the setting you use for compiling godot-cpp.
