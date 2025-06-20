#include "InputManager.h"
#include <fstream>
#include "../json/json.h"
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "../defines/sfmlImguiKeys.h"

InputManager::InputManager(std::string pathToinput)
    : m_path(pathToinput)
{
}

void InputManager::OnAttach()
{
    LoadInputs();
}

void InputManager::LoadInputs() {
    m_inputs.clear();
    std::ifstream file(m_path);
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        InputDefinition def;

        std::getline(iss, def.type, ',');
        std::getline(iss, def.key, ',');
        std::getline(iss, def.name, ',');
        std::getline(iss, def.part, ',');
        std::getline(iss, def.onInput, ',');
        std::getline(iss, def.offInput, ',');

        auto trim = [](std::string& s) {
            while (!s.empty() && (s[0] == ' ' || s[0] == '\t')) s.erase(0, 1);
            };

        trim(def.name);
        trim(def.part);
        trim(def.onInput);
        trim(def.offInput);

        m_inputs.push_back(def);
    }
}


void InputManager::SaveInputs() {
    std::ofstream file(m_path);
    if (!file.is_open()) return;
    
    for (const auto& input : m_inputs) {
        file << input.type << ","
            << input.key << ","
            << input.name << ","
            << input.part << ","
            << input.onInput << ","
            << input.offInput << "\n";
    }
}

void InputManager::OnUIRender() {
    ImGui::Begin("Input Manager");

    // Left panel: Input list and Add button
    ImGui::BeginChild("List", ImVec2(200, 0), true);
    for (int i = 0; i < m_inputs.size(); ++i) {
        if (ImGui::Selectable(m_inputs[i].name.c_str(), m_selectedIndex == i)) {
            m_selectedIndex = i;
        }
    }

    if (ImGui::Button("+")) {
        m_inputs.push_back({ "KeyboardKey", "0", "NewInput", "Pressed", "", "" });
        m_selectedIndex = m_inputs.size() - 1;
    }
    ImGui::EndChild();

    // Right panel: Edit input properties
    ImGui::SameLine();
    ImGui::BeginGroup();
    if (m_selectedIndex >= 0 && m_selectedIndex < m_inputs.size()) {
        InputDefinition& def = m_inputs[m_selectedIndex];

        // Type dropdown
        static const char* typeOptions[] = { "MouseKey", "ControllerKey", "KeyboardKey", "MouseMove", "JoystickMove" };
        int currentTypeIndex = 2; // default to KeyboardKey
        for (int i = 0; i < IM_ARRAYSIZE(typeOptions); ++i) {
            if (def.type == typeOptions[i]) {
                currentTypeIndex = i;
                break;
            }
        }
        if (ImGui::Combo("Type", &currentTypeIndex, typeOptions, IM_ARRAYSIZE(typeOptions))) {
            def.type = typeOptions[currentTypeIndex];
        }

        
        if (def.type != "ControllerKey" && def.type != "JoystickMove") {
            ImGui::Text("Key: %s", def.key.c_str());
            ImGui::SameLine();
            if (!m_listeningForKey) {
                if (ImGui::Button("Listen")) {
                    m_listeningForKey = true;
                }
            }
            else {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Listening... Press any key/button");


                if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
                    m_listeningForKey = false;
                }

                for (int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; ++key)
                {
                    if (ImGui::IsKeyPressed((ImGuiKey)key))
                    {
                        if (ImGuiToSFMLKeyMap.find((ImGuiKey)key) != ImGuiToSFMLKeyMap.end())
                        {
                            // Key exists in the map
                            int sfKey = (int) ImGuiToSFMLKeyMap.at((ImGuiKey)key);
                            def.type = "KeyboardKey";
                            def.key = std::to_string(sfKey);
                            m_listeningForKey = false;
                            break;
                        }
                    }
                }



                for (int i = 0; i < 5; ++i) {
                    if (ImGui::IsMouseClicked(i)) {
                        def.type = "MouseKey";
                        def.key = std::to_string(i);
                        m_listeningForKey = false;
                        break;
                    }
                }




                ImVec2 delta = ImGui::GetIO().MouseDelta;
                if (delta.x != 0.0f || delta.y != 0.0f) {
                    def.type = "MouseMove";
                    def.key = "0";
                    m_listeningForKey = false;
                }


            }
        }
        else if (def.type == "ControllerKey"){
            ImGui::Text("Key: %s", def.key.c_str());
            ImGui::SameLine();
            // List of controller keys (example keys, modify as needed)
            static const char* controllerKeys[] = {
                "A",            // 0
                "B",            // 1
                "X",            // 2
                "Y",            // 3
                "LeftBumper",   // 4
                "RightBumper",  // 5
                "Back",         // 6
                "Start",        // 7
                "LeftStick",    // 8
                "RightStick",   // 9
                "DPadUp",       // 10
                "DPadDown",     // 11
                "DPadLeft",     // 12
                "DPadRight"     // 13
            };

            // Find currently selected index
            int currentIndex = 0;
            for (int i = 0; i < IM_ARRAYSIZE(controllerKeys); ++i) {
                if (def.key == controllerKeys[i]) {
                    currentIndex = i;
                    break;
                }
            }

            if (ImGui::BeginCombo("Controller Key", controllerKeys[currentIndex])) {
                for (int n = 0; n < IM_ARRAYSIZE(controllerKeys); n++) {
                    bool isSelected = (currentIndex == n);
                    if (ImGui::Selectable(controllerKeys[n], isSelected)) {
                        def.key = std::to_string(currentIndex);
                    }
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        }
        else {
            def.key == "0";
        }


        // Name
        char nameBuf[64];
        strcpy(nameBuf, def.name.c_str());
        ImGui::InputText("Name", nameBuf, sizeof(nameBuf));
        def.name = nameBuf;

        // Part dropdown
        static const char* partOptions[] = { "started", "Pressed", "released" };
        int currentPartIndex = 1; 
        for (int i = 0; i < IM_ARRAYSIZE(partOptions); ++i) {
            if (def.part == partOptions[i]) {
                currentPartIndex = i;
                break;
            }
        }
        if (ImGui::Combo("Part", &currentPartIndex, partOptions, IM_ARRAYSIZE(partOptions))) {
            def.part = partOptions[currentPartIndex];
        }

        // OnInput
        char onInputBuf[64];
        strcpy(onInputBuf, def.onInput.c_str());
        ImGui::InputText("OnInput (optional)", onInputBuf, sizeof(onInputBuf));
        def.onInput = onInputBuf;

        // OffInput
        char offInputBuf[64];
        strcpy(offInputBuf, def.offInput.c_str());
        ImGui::InputText("OffInput (optional)", offInputBuf, sizeof(offInputBuf));
        def.offInput = offInputBuf;

        // Delete button
        if (ImGui::Button("Delete")) {
            m_inputs.erase(m_inputs.begin() + m_selectedIndex);
            m_selectedIndex = -1;
        }
    }
    ImGui::EndGroup();

    // Save button
    ImGui::Separator();
    if (ImGui::Button("Save")) {
        SaveInputs();
    }

    ImGui::End();
}


