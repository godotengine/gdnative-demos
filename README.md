# GDNative demos

The top level folders split this repo into demos for different languages.
Each folder inside of those is a GDNative project. Once compiled, the
`project.godot` file can be opened with
[Godot Engine](https://godotengine.org), the open source 2D and 3D game engine.

**Important:** Each demo depends on submodules for the `godot-headers` and
`godot-cpp` GDNative dependencies. You should initialize this repository by
checking out the submodules recursively:

```
git submodule update --init --recursive
```

For non-GDNative demos (GDScript, VisualScript, and C#), please see the
[Godot demo projects](https://github.com/godotengine/godot-demo-projects/) repo.

## Godot versions

The [`master`](https://github.com/godotengine/gdnative-demos/tree/master) branch
is compatible with the `3.x` development branch of Godot.

Numbered branches are compatible with the corresponding Godot branch.
The following numbered branches are available:

- [`3.3`](https://github.com/godotengine/gdnative-demos/tree/3.3)
- [`3.4`](https://github.com/godotengine/gdnative-demos/tree/3.4)

Older Godot versions are not supported by this repo.

## Useful links

- [Main website](https://godotengine.org)
- [Source code](https://github.com/godotengine/godot)
- [Documentation](http://docs.godotengine.org)
- [Community hub](https://godotengine.org/community)
- [Godot headers](https://github.com/godotengine/godot_headers)
- [Godot C++ bindings](https://github.com/godotengine/godot-cpp)

## License

Those demos are distributed under the terms of the MIT license, as
described in the [LICENSE.md](LICENSE.md) file.
