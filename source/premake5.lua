workspace "DMP"
    configurations { "Debug", "Release" }
    platforms { "x64" }

    location "../build"
    targetprefix ""
    startproject "Client Launcher"
    cppdialect "C++17"

    pic "On"
    symbols "Full"
    characterset "Unicode"
    flags { "No64BitChecks", "MultiProcessorCompile" }

    editandcontinue "Off"
    justmycode "Off"

	buildpath = function(p) return "%{wks.location}/../dist/" .. p .. "/" end
	copy = function(p) return "{COPY} %{cfg.buildtarget.abspath} \"%{wks.location}../dist/" .. p .. "/\"" end
	

    defines {
        "_CRT_SECURE_NO_WARNINGS",
        "_SCL_SECURE_NO_WARNINGS",
        "_CRT_NONSTDC_NO_DEPRECATE",
        "NOMINMAX",
        "_TIMESPEC_DEFINED"
    }

    filter "platforms:x64"
        architecture "x86_64"

    filter "configurations:Debug"
        defines { "DMP_DEBUG" }
        targetsuffix "_d"

    filter { "configurations:Release", "configurations:Nightly" }
        optimize "Speed"

    filter { 'language:C', 'language:C++' }
        defines { "GTEST_HAS_PTHREAD=0", "BOOST_ALL_NO_LIB", "BOOST_NULLPTR=nullptr" }

    filter { "system:windows", "configurations:Nightly", "kind:not StaticLib" }
        symbolspath "$(SolutionDir)Symbols\\$(Configuration)_$(Platform)\\$(ProjectName).pdb"

    filter "system:windows"
        toolset "v143"
        preferredtoolarchitecture "x86_64"
        staticruntime "On"
        buildoptions { "/Zc:__cplusplus" }

    filter { "system:windows", "configurations:Debug" }
        runtime "Release"
        defines { "DEBUG" }

    group "client"
        include "client/core"