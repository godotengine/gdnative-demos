#!python
import os

opts = Variables([], ARGUMENTS)

# Define the relative path to the Godot headers.
godot_headers_path = "godot-cpp/godot-headers"
godot_bindings_path = "godot-cpp"

# Gets the standard flags CC, CCX, etc.
env = DefaultEnvironment()

# Define our options. Use future-proofed names for platforms.
platform_array = ["", "windows", "linuxbsd", "macos", "x11", "linux", "osx"]
opts.Add(EnumVariable("target", "Compilation target", "debug", ["d", "debug", "r", "release"]))
opts.Add(EnumVariable("platform", "Compilation platform", "", platform_array))
opts.Add(EnumVariable("p", "Alias for 'platform'", "", platform_array))
opts.Add(BoolVariable("use_llvm", "Use the LLVM / Clang compiler", "no"))
opts.Add(PathVariable("target_path", "The path where the lib is installed.", "project/gdnative/"))
opts.Add(PathVariable("target_name", "The library name.", "libdodgethecreeps", PathVariable.PathAccept))

# Updates the environment with the option variables.
opts.Update(env)

# Process platform arguments. Here we use the same names as GDNative.
if env["p"] != "":
    env["platform"] = env["p"]

if env["platform"] == "macos":
    env["platform"] = "osx"
elif env["platform"] in ("x11", "linuxbsd"):
    env["platform"] = "linux"
elif env["platform"] == "bsd":
    env["platform"] = "freebsd"

if env["platform"] == "":
    print("No valid target platform selected.")
    quit()

platform = env["platform"]

# Check our platform specifics.
if platform == "osx":
    if not env["use_llvm"]:
        env["use_llvm"] = "yes"
    if env["target"] in ("debug", "d"):
        env.Append(CCFLAGS=["-g", "-O2", "-arch", "x86_64", "-std=c++14"])
        env.Append(LINKFLAGS=["-arch", "x86_64"])
    else:
        env.Append(CCFLAGS=["-g", "-O3", "-arch", "x86_64", "-std=c++14"])
        env.Append(LINKFLAGS=["-arch", "x86_64"])
elif platform == "linux":
    if env["target"] in ("debug", "d"):
        env.Append(CCFLAGS=["-fPIC", "-g3", "-Og"])
    else:
        env.Append(CCFLAGS=["-fPIC", "-g", "-O3"])
elif platform == "windows":
    # This makes sure to keep the session environment variables
    # on Windows, so that you can run scons in a VS 2017 prompt
    # and it will find all the required tools.
    env = Environment(ENV=os.environ)
    opts.Update(env)

    env.Append(CCFLAGS=["-DWIN32", "-D_WIN32", "-D_WINDOWS", "-W3", "-GR", "-D_CRT_SECURE_NO_WARNINGS"])
    if env["target"] in ("debug", "d"):
        env.Append(CCFLAGS=["-EHsc", "-D_DEBUG", "-MDd"])
    else:
        env.Append(CCFLAGS=["-O2", "-EHsc", "-DNDEBUG", "-MD"])

if env["use_llvm"] == "yes":
    env["CC"] = "clang"
    env["CXX"] = "clang++"

SConscript("godot-cpp/SConstruct")


def add_sources(sources, dir):
    for f in os.listdir(dir):
        if f.endswith(".cpp"):
            sources.append(dir + "/" + f)


env.Append(
    CPPPATH=[
        godot_headers_path,
        godot_bindings_path + "/include",
        godot_bindings_path + "/include/gen/",
        godot_bindings_path + "/include/core/",
    ]
)

env.Append(
    LIBS=[
        env.File(os.path.join("godot-cpp/bin", "libgodot-cpp.%s.%s.64%s" % (platform, env["target"], env["LIBSUFFIX"])))
    ]
)

env.Append(LIBPATH=[godot_bindings_path + "/bin/"])

sources = []
add_sources(sources, "src")

library = env.SharedLibrary(target=env["target_path"] + "/" + platform + "/" + env["target_name"], source=sources)
Default(library)
