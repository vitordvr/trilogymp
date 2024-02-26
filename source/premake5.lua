workspace "TrilogyMP"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    startproject "Client Launcher"

    location "../build"
    cppdialect "C++17"
    characterset "MBCS"
    pic "On"
    symbols "On"
    flags "MultiProcessorCompile"

    defines {
        "_CRT_SECURE_NO_WARNINGS",
        "_SCL_SECURE_NO_WARNINGS",
        "_CRT_NONSTDC_NO_DEPRECATE",
        "NOMINMAX",
        "_TIMESPEC_DEFINED"
    }

    buildpath = function(p) return "%{wks.location}/../dist/" .. p .. "/" end
	copy = function(p) return "{COPY} %{cfg.buildtarget.abspath} \"%{wks.location}../dist/" .. p .. "/\"" end

    filter "configurations:Debug"
        defines { "TRILOGY_DEBUG" }
        targetsuffix "_d"

    filter "configurations:Release"
        optimize "Speed"

    filter "system:windows"
        architecture "x86_64"
        toolset "v143"
        staticruntime "On"
        buildoptions { "/Zc:__cplusplus" }
        defines { "WIN32", "_WIN32", "_WIN32_WINNT=0x601", "_MSC_PLATFORM_TOOLSET=$(PlatformToolsetVersion)" }

    filter { "system:windows", "configurations:Debug" }
        runtime "Release"
        defines { "DEBUG" }

    group "Client"
        include "client/launcher"
        include "client/loader"
        include "client/core"
        include "client/sdk"

	group "Shared"
		include "shared"
