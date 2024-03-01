project "Kiero"
	targetname "Kiero"
	language "C++"
	kind "StaticLib"

	includedirs { "../minHook" }

	links { "MinHook" }
	
  files {
		"premake5.lua",
		"**.h",
    "**.cpp"
	}