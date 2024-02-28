project "imgui"
	language "C++"
	kind "StaticLib"
	targetname "imgui"
	targetdir(buildpath("binaries"))

	includedirs {
		".",
	}

	vpaths {
		["Headers/*"] = "**.h",
		["Sources/*"] = "**.cpp",
		["*"] = "premake5.lua"
	}

	files {
		"premake5.lua",
		"**.h",
    "**.cpp",
	}

  cppdialect "C++17"
	staticruntime "On"