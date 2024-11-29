project "SceneEditor"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "../vendor/imgui",
      "../vendor/glfw/include",

      "../Walnut/Source",
      "../Walnut/Platform/GUI",

      "%{IncludeDir.VulkanSDK}",
      "%{IncludeDir.glm}",

      -- Include Core
      "../MicroEngine/Source",
      "$(SolutionDir)dependencies/SFML/SFML-2.6.1/include",
      "$(SolutionDir)MicroEngine/src"
   }

    links
    {
        "Walnut",
        "Micro"
    }

    prebuildcommands {
      'del /q "$(OutDir)Micro.dll"',
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
         "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-graphics-d.lib",
         "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-window-d.lib",
         "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-system-d.lib",
         "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-audio-d.lib",
         "$(SolutionDir)dependencies/SFML/SFML-2.6.1/lib/sfml-network-d.lib"
     }

   filter "configurations:Release"
      defines { "WL_RELEASE","MC_PLATFORM_WINDOWS" }
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
      kind "WindowedApp"
      defines { "WL_DIST","MC_PLATFORM_WINDOWS" }
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