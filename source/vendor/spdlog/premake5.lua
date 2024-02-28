project "spdlog"
    cppdialect "C++20"
    kind "StaticLib"
    targetname "spdlog"
    warnings "Off"

    vpaths {
        ["Headers/*"] = {"include/spdlog/**.*",},
        ["Sources/*"] = {"src/**.c*",},
        ["*"] = {"premake5.lua", "CMakeLists.txt"}
    }

    files {
        "src/spdlog.cpp", 
        "src/stdout_sinks.cpp", 
        "src/color_sinks.cpp", 
        "src/file_sinks.cpp", 
        "src/async.cpp", 
        "src/cfg.cpp",
        "include/**"
    }

    includedirs {
        "include"
    }

    defines {
        "SPDLOG_COMPILED_LIB"
    }