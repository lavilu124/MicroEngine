-- WalnutExternal-Headless.lua

IncludeDir = {}
IncludeDir["glm"] = "../Vendor/glm"
IncludeDir["spdlog"] = "../Vendor/spdlog/include"

group "Dependencies"
   include "Vendor/yaml-cpp"
group ""

group "Core"
    include "Walnut/Build-Walnut-Headless.lua"

    -- Optional modules
    if os.isfile("Walnut-Modules/Walnut-Networking/Build-Walnut-Networking.lua") then
        include "Walnut-Modules/Walnut-Networking/Build-Walnut-Networking.lua"
    end
group ""