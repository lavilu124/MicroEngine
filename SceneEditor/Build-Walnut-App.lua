project "SceneEditor"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "../vendor/imgui",
      "../vendor/GLFW/include",

      "../Walnut/Source",
      "../Walnut/Platform/GUI",

      "%{IncludeDir.VulkanSDK}",
      "%{IncludeDir.glm}",

      -- Include Core
      "../MicroEngine/src",
      "../dependencies/SFML/SFML-2.6.1/include"
   }

    links
    {
        "Walnut",
        "Micro"
    }

    prebuildcommands {
      'if exist "$(OutDir)Micro.dll" del /q "$(OutDir)Micro.dll"',
      'xcopy /y "$(SolutionDir)Binaries\\' .. outputdir .. '\\Micro\\Micro.dll" "$(OutDir)"'
     }
  
     postbuildcommands {
      'xcopy /y "$(SolutionDir)dependencies\\SFML\\SFML-2.6.1\\bin\\*.dll" "$(OutDir)"'
     }

    targetdir ("../Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "WL_PLATFORM_WINDOWS", "WINDOWS", "ENGINE" }

   filter "configurations:Debug"
      defines { "WL_DEBUG","MC_PLATFORM_WINDOWS" }
      runtime "Debug"
      symbols "On"
      links {
         "../dependencies/SFML/SFML-2.6.1/lib/sfml-graphics-d.lib",
         "../dependencies/SFML/SFML-2.6.1/lib/sfml-window-d.lib",
         "../dependencies/SFML/SFML-2.6.1/lib/sfml-system-d.lib",
         "../dependencies/SFML/SFML-2.6.1/lib/sfml-audio-d.lib",
         "../dependencies/SFML/SFML-2.6.1/lib/sfml-network-d.lib"
     }

   filter "configurations:Release"
      defines { "WL_RELEASE","MC_PLATFORM_WINDOWS" }
      runtime "Release"
      optimize "On"
      symbols "On"
      links {
         "../dependencies/SFML/SFML-2.6.1/lib/sfml-graphics.lib",
         "../dependencies/SFML/SFML-2.6.1/lib/sfml-window.lib",
         "../dependencies/SFML/SFML-2.6.1/lib/sfml-system.lib",
         "../dependencies/SFML/SFML-2.6.1/lib/sfml-audio.lib",
         "../dependencies/SFML/SFML-2.6.1/lib/sfml-network.lib"
     }

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "WL_DIST","MC_PLATFORM_WINDOWS" }
      runtime "Release"
      optimize "On"
      symbols "Off"
      links {
         "../dependencies/SFML/SFML-2.6.1/lib/sfml-graphics.lib",
         "../dependencies/SFML/SFML-2.6.1/lib/sfml-window.lib",
         "../dependencies/SFML/SFML-2.6.1/lib/sfml-system.lib",
         "../dependencies/SFML/SFML-2.6.1/lib/sfml-audio.lib",
         "../dependencies/SFML/SFML-2.6.1/lib/sfml-network.lib"
     }