# Godot GDNative texture generator module

Building
--------
Open up a terminal and CD into the folder into which you've cloned this project

Install submodules
```
git submodule init
```

Building cpp_bindings:
```
cd cpp_bindings
scons platform=windows target=release generate_bindings=yes headers=../godot_headers godotbinpath=<pathtoexe>/godot.exe 
cd ..
```

And finally build our library:
```
scons platform=windows target=release
```
