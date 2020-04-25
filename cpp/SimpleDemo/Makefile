#godot-cpp\godot_headers is expected at ..\..\
GENERATE_BINDINGS = no
TARGET = debug
USE_CLANG = no

BASE = scons use_llvm=$(USE_CLANG) target=$(TARGET)
LINUX = $(BASE) platform=linux
WINDOWS = $(BASE) platform=windows
OSX = $(BASE) platform=osx

all:
	make windows
	make android_armv7
	make android_arm64v8
  
linux:
	$(LINUX)

windows:
	$(WINDOWS)

osx:
	$(OSX)

android_armv7:
	make -B --makefile=MakefileAndroid ARCH=armv7 all
  
android_arm64v8:
	make -B --makefile=MakefileAndroid ARCH=arm64v8 all

