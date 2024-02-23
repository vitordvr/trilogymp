project "Client Proxy"
	language "C++"
	kind "SharedLib"
	targetname "proxy"
	targetdir(buildpath("binaries"))
	targetsuffix ""

	vpaths {
		["Headers/*"] = "**.h",
		["Sources"] = { "*.cpp" },
		["Resources/*"] = "*.def",
		["*"] = "premake5.lua"
	}

	files {
		"premake5.lua",
		"main.cpp",
	}

	includedirs {
		"../../Shared/sdk",
	}

	filter "system:windows"
		linkoptions { "/SAFESEH:NO" }
