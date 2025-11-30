workspace "Toolchain"
    configurations { "Debug", "Release", "Profile" }
    location "build"

    -- everything for each project to inherit
    cppdialect "C++20"
    language "C++"
    staticruntime "Off"
    debugdir "."
    system "Windows"
    architecture "x86_64"
    warnings "Off"
    flags { "MultiProcessorCompile" }

    defines { "WINDOWS", "ENABLE_ASSERTS" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Profile"
        profile "On"
        symbols "On"
        optimize "On"

    filter "configurations:Release"
        optimize "On"

project "compiler"
    kind "ConsoleApp"
    location "build/%{prj.name}"

    files {
        "compiler/compiler.cpp",
    }

    filter "configurations:Debug"
        targetdir "build/bin/debug"

    filter "configurations:Profile"
        targetdir "build/bin/profile"

    filter "configurations:Release"
        targetdir "build/bin/release"

project "program"
    kind "ConsoleApp"
    location "build/%{prj.name}"

    files {
        "program/program.c",
        "program/output.asm",
    }

    filter "configurations:Debug"
        targetdir "build/bin/debug"

    filter "configurations:Profile"
        targetdir "build/bin/profile"

    filter "configurations:Release"
        targetdir "build/bin/release"
