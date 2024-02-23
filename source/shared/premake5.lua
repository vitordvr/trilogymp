project "Shared"
	language "C++"
	kind "None"
	targetname "shared"

	vpaths {
		["Headers/*"] = { "**.h", "**.hpp" },
		["*"] = "premake5.lua",
	}

	files {
		"premake5.lua",
		"**.h",
		"**.hpp",
	}

	flags { "ExcludeFromBuild" }
