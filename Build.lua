-- premake5.lua
workspace "Micro Engine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "SendBox"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

outputdir  = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Engine-Core"
	include "MicroEngine/Build-Core.lua"
group ""


include "Build-Walnut-External.lua"
include "SceneEditor/Build-Walnut-App.lua"
include "SendBox/Build-App.lua"