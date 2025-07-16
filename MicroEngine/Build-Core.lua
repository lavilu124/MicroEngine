project "Micro"
   kind "SharedLib"
   language "C++"
   cppdialect "C++17"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp", "src/**.hpp" }

   local sfmlRoot = "../dependencies/SFML/SFML-2.6.1"

   includedirs
   {
      "src",
      sfmlRoot .. "/include",
      "../dependencies/include/json"
   }
   
   targetdir ("../Binaries/" .. outputdir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
   
   filter "configurations:Debug"
       defines { "DEBUG","MC_PLATFORM_WINDOWS","MC_BUILD_DLL" }
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
       defines { "NDEBUG","MC_PLATFORM_WINDOWS","MC_BUILD_DLL" }
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
       defines { "DIST","MC_PLATFORM_WINDOWS","MC_BUILD_DLL" }
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
