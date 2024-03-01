project "imgui"
	language "C++"
	kind "StaticLib"
	targetname "imgui"
	targetdir(buildpath("binaries"))

	includedirs {
		".",
	}

    defines {
      "IMGUI_IMPL_WIN32_DISABLE_GAMEPAD",
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

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"