## C++

### Initial setup

You need to clone the GDNative C++ bindings in the root of this project:

```
git clone --recursive https://github.com/godotengine/godot-cpp
```


### Updating the api.json
Our api.json file contains meta data of all the classes that are part of the Godot core and are needed to generate the C++ binding classes for use in GDNative modules.

This file is supplied with our godot_headers repository for your convinience but if you are running a custom build of Godot and need access to classes that have recent changes a new api.json file must be generated. You do this by starting your Godot executable with the following parameters:

```
$ godot --gdnative-generate-json-api api.json
```

Now copy the api.json file into your folder structure so its easy to access. **Note** the remark below for the extra ```custom_api_file``` command line parameter needed to tell scons where to find your file.

### Compiling the cpp bindings library
The final step is to compile our cpp bindings library:
```
$ cd godot-cpp
$ scons platform=<your platform> generate_bindings=yes
$ cd ..
```

> Replace `<your platform>` with either `windows`, `linux` or `osx`.

> Include `use_llvm=yes` for using clang++

> Include `target=runtime` to build a runtime build (windows only at the moment)

> The resulting library will be created in `godot-cpp/bin/`, take note of its name as it will be different depending on platform.

> If you want to use an alternative api.json file add `use_custom_api_file=yes custom_api_file=../api.json`, be sure to specify the correct location of where you placed your file.

> Example: `scons platform=windows generate_bindings=yes target=release use_custom_api_file=yes custom_api_file=../api.json -j12`

### Compile the demos

A `Makefile` is provided in each demo to make the process more convenient.
