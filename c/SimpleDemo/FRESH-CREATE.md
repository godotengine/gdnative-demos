# How to freshly create your own custom module

In case you start off by your own, and not start with this SimpleDemo project, follow along with the following guide to be able to create your own custom module ready for dynamically loaded by Godot 3.0.

1. Compile and build a share/dynamic library of your custom module. You can just use `simple.c` file as a starting point.
2. Open Godot 3.0
3. Create a new resource for "NativeScript".
4. Look at its Inspector panel.
5. Click "Save as" then select where you would like to save it at. It will have `.gdns` extension. Reccommend to create a new directory, then save all things in that directory.
6. You will notice `Path` has been set properly after we saved it.
7. Click on `<null>` value of `Library` property then select `New GDNativeLibrary`.
8. Click on it again to edit it.
9. Click on 'Save as' then enter the file name, and change its extension to `.gdnlib` for ease of remember and we will know it immediately that this is `GDNativeLibrary` stuff.
10. Currently there is no UI to edit shared library path for `GDNativeLibrary` resource. What we will do is to take a look at `bin/simple.gdnlib` of SimpleDemo project then copy its `[entry]` and `[dependencies]` sections to your `.gdnlib` file. You also have to edit this file via any of text editor outside of Godot 3.0. You can't edit this with Godot editor as it won't let you see anything.
11. Inside your `.gdnlib`, enter relevant path for your shared libraries for all platforms. Mainly `X11.64`, `Windows.64`, and `OSX.64`. Use format `res://` and enter correct path to each one.
12. Back to your `.gdns` by clicking on it. **This is important**. Enter a correct `Class Name` that you refer to in your C source file. If you enter it incorrectly, Godot won't be able to load your module. Then click 'Save'.
13. Now you can go back to your normal script then use the following code to load it.
    
    ```
    onready var my_custom_module = preload("res://PATH/TO/YOUR/MODULE/your_module.gdns")

    func _ready():
        my_custom_module.method_1()
    ```

    You then can use its methods normally.

Becareful when you try to save any changes made for `NativeScript` and `GDNativeLibrary` resource in Inspector panel. Sometimes it is lost due to it won't effectively save (hopefully it will be fixed). Better to confirm saving is to click on `Save`.
