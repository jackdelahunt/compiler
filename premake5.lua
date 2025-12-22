workspace "Toolchain"
    configurations { "Debug", "Release", "Profile" }
    location "build"

    cppdialect "C++20"
    language "C++"
    staticruntime "Off"
    debugdir "."
    system "Windows"
    architecture "x86_64"
    warnings "Off"
    flags { "MultiProcessorCompile" }

    defines { "OS_WINDOWS", "COMPILER_MSVC" }
    -- defines { "OS_LINUX", "COMPILER_GCC" }
    -- defines { "OS_MACOS", "COMPILER_CLANG" }

    filter "configurations:Debug"
        defines { "BUILD_DEBUG", "ENABLE_ASSERTS" }
        targetdir "build/bin/debug"
        symbols "On"

    filter "configurations:Release"
        defines { "BUILD_RELEASE" }
        targetdir "build/bin/release"
        optimize "On"

    filter "configurations:Profile"
        defines { "BUILD_DEBUG", "BUILD_RELEASE" }
        targetdir "build/bin/profile"
        profile "On"
        symbols "On"
        optimize "On"

project "compiler"
    kind "ConsoleApp"
    location "build/%{prj.name}"

    files {
        "compiler/compiler.cpp",
    }

project "program"
    kind "ConsoleApp"
    location "build/%{prj.name}"

    files {
        "program/program.c",
        "program/output.asm",
    }
