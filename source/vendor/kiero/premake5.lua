project "Kiero"
	targetname "Kiero"
	language "C++"
	kind "StaticLib"

	includedirs { "../minhook" }

  files {
		"premake5.lua",
		"**.h",
    "**.cpp"
	}

  links {"../minhook/lib/libMinHook-x64-v140-md"}
  