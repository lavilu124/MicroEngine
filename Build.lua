-- premake5.lua
workspace "Micro Engine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "SendBox"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Core"
	include "MicroEngine/Build-Core.lua"
group ""

include "SendBox/Build-App.lua"
include "Testing/Build-Test.lua"