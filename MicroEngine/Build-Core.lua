project "Micro"
   kind "SharedLib"
   language "C++"
   cppdialect "C++17"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "src",
      "$(SolutionDir)dependencies/SFML/SFML-2.6.1/include",
      "$(SolutionDir)dependencies/include/json"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"

   filter "configurations:Debug"
       defines { "DEBUG","MC_PLATFORM_WINDOWS","MC_BUILD_DLL" }
       runtime "Debug"
       symbols "On"
       links {
           "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-graphics-d.lib",
           "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-window-d.lib",
           "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-system-d.lib",
           "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-audio-d.lib",
           "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-network-d.lib"
       }

   filter "configurations:Release"
       defines { "NDEBUG","MC_PLATFORM_WINDOWS","MC_BUILD_DLL" }
       runtime "Release"
       optimize "On"
       symbols "On"
       links {
           "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-graphics.lib",
           "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-window.lib",
           "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-system.lib",
           "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-audio.lib",
           "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-network.lib"
       }

   filter "configurations:Dist"
       defines { "DIST","MC_PLATFORM_WINDOWS","MC_BUILD_DLL" }
       runtime "Release"
       optimize "On"
       symbols "Off"
       links {
           "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-graphics.lib",
           "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-window.lib",
           "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-system.lib",
           "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-audio.lib",
           "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-network.lib"
       }
