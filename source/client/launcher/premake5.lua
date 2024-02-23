project "Client Launcher"
	language "C++"
	kind "WindowedApp"
	targetname "TrilogyMP"
	targetdir(buildpath("."))
	debugdir(buildpath("."))

	includedirs {
		"../../shared/sdk",
	}

	pchheader "StdInc.h"
	pchsource "StdInc.cpp"

	entrypoint "WinMainCRTStartup"

	vpaths {
		["Headers/*"] = "**.h",
		["Sources/*"] = "**.cpp",
		["Resources/*"] = {"*.rc", "**.ico", "**.xml", "**.manifest"},
		["*"] = "premake5.lua"
	}

	files {
		"premake5.lua",
		"*.h",
		"*.cpp"
	}

	filter "system:windows"
		staticruntime "On"
		files {
			"NEU/trilogymp.ico",
			"NEU/TrilogyMP.gdf.xml",
			"launch.rc",
			"TrilogyMP.rc",
			"TrilogyMP.manifest",
			"resource/trilogymp.ico"
		}

	filter "system:not windows"
		flags { "ExcludeFromBuild" }
