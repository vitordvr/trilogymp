project "Client Core"
  language "C++"
  kind "SharedLib"
  targetname "core"
  targetdir(buildpath("binaries"))
  
  filter {"system:windows"}
    linkoptions { "/SAFESEH:NO" }
    buildoptions { "-Zm130" }
  
  pchheader "StdInc.h"
	pchsource "StdInc.cpp"

  vpaths {
		["Headers/*"] = {"**.h", "**.hpp"},
		["Sources/*"] = "**.cpp",
		["Resources/*"] = {"**.rc", "../launch/resource/mtaicon.ico"},
		["*"] = "premake5.lua"
	}
  
  files {
		"premake5.lua",
		"**.h",
		"**.hpp",
		"**.cpp"
	}

  filter "system:not windows"
  flags { "ExcludeFromBuild" }