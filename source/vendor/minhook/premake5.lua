project "MinHook"
   kind "StaticLib"
   language "C"
   targetdir "bin/%{cfg.buildcfg}"

   -- Inclui arquivos na raiz e na pasta hde
   files { "**.h", "**.c", "hde/*.h", "hde/*.c" }

   -- Inclui diretórios de inclusão
   includedirs { ".", "hde" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"