project "SendBox"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   local sfmlRoot = "../dependencies/SFML/SFML-2.6.1"

   includedirs
   {
      "src",

	  -- Include Core
      sfmlRoot .. "/include",
      "../MicroEngine/src"
   }

   links
   {
      "Micro"
   }

   prebuildcommands {
    'xcopy /y "../Binaries\\' .. outputdir .. '\\Micro\\Micro.dll" "$(OutDir)"'
   }

   postbuildcommands {
    'xcopy /y "../dependencies\\SFML\\SFML-2.6.1\\bin\\*.dll" "$(OutDir)"'
   }

   targetdir ("../Binaries/" .. outputdir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

   filter "configurations:Debug"
        defines { "DEBUG","MC_PLATFORM_WINDOWS"}
        runtime "Debug"
        symbols "On"
        links {
            sfmlRoot .. "/lib/sfml-graphics-d.lib",
            sfmlRoot .. "/lib/sfml-window-d.lib",
            sfmlRoot .. "/lib/sfml-system-d.lib",
            sfmlRoot .. "/lib/sfml-audio-d.lib",
            sfmlRoot .. "/lib/sfml-network-d.lib"
        }

   filter "configurations:Release"
       defines { "NDEBUG","MC_PLATFORM_WINDOWS"}
       runtime "Release"
       optimize "On"
       symbols "On"
       links {
           sfmlRoot .. "/lib/sfml-graphics.lib",
           sfmlRoot .. "/lib/sfml-window.lib",
           sfmlRoot .. "/lib/sfml-system.lib",
           sfmlRoot .. "/lib/sfml-audio.lib",
           sfmlRoot .. "/lib/sfml-network.lib"
       }

   filter "configurations:Dist"
       defines { "DIST","MC_PLATFORM_WINDOWS"}
       runtime "Release"
       optimize "On"
       symbols "Off"
       links {
           sfmlRoot .. "/lib/sfml-graphics.lib",
           sfmlRoot .. "/lib/sfml-window.lib",
           sfmlRoot .. "/lib/sfml-system.lib",
           sfmlRoot .. "/lib/sfml-audio.lib",
           sfmlRoot .. "/lib/sfml-network.lib"
       }