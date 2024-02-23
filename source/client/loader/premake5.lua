project "Client Loader"
	language "C++"
	kind "SharedLib"
	targetname "loader"
	targetdir(buildpath("binaries"))

	filter "system:windows"
		linkoptions { "/SAFESEH:NO" }

    includedirs {
      "../../Shared/sdk",
    }

	links {}

	pchheader "StdInc.h"
	pchsource "StdInc.cpp"
	forceincludes { "StdInc.h" }

	vpaths {
		["Headers/*"] = "**.h",
		["Sources"] = "*.c",
		["*"] = "premake5.lua"
	}

	files {
		"premake5.lua",
		"*.h",
		"*.cpp"
	}

	filter "system:not windows"
		flags { "ExcludeFromBuild" }
