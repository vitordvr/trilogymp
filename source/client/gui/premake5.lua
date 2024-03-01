project "GUI"
	language "C++"
	kind "SharedLib"
	targetname "gui"
	targetdir(buildpath("binaries"))

  filter {}
	 includedirs {
		"../../shared/sdk",
		"../sdk",
		"../../vendor/spdlog/include",
		"../../vendor/imgui",
		"../../vendor/imgui/backends",
		"../../vendor/kiero",
		"../core/app",
	}

	pchheader "StdInc.h"
	pchsource "StdInc.cpp"

  vpaths {
		["Headers/*"] = {"**.h", "**.hpp"},
		["Sources/*"] = "**.cpp",
		["Resources/*"] = {"**.rc", "../launch/resource/mtaicon.ico"},
		["*"] = "premake5.lua"
	}

  links {  "ImGui", "imgui", "kiero",  "d3d11.lib", "dxgi.lib", "d3dcompiler.lib" }

  files {
		"premake5.lua",
		"**.h",
		"**.hpp",
		"**.cpp"	
	}

  filter {"system: windows"}
  buildoptions { "-Zm180" }
