#pragma once
#include "Walnut/Application.h"
#include <map>

struct InputDefinition {
    std::string type;       // "KeyboardKey", "MouseKey", etc.
    std::string key;        // as string (int)
    std::string name;       // "Jump", "Shoot"
    std::string part;       // "pressed", "started", "released"
    std::string onInput;    // function name to call on input
    std::string offInput;   // function name to call when input is off
};


class InputManager : public Walnut::Layer
{
public:
    InputManager(const std::string& pathToinput);
    void OnUIRender() override;
    void OnAttach() override;

	bool* Open() { return &m_isOpen; }

private:
    std::string m_path;
    std::vector<InputDefinition> m_inputs;
    int m_selectedIndex = -1;
    char m_newInputName[64] = "";
    char m_newInputKey[64] = "";
    bool m_shouldSave = false;
    bool m_listeningForKey = false;

    void SaveInputs() const;
    void LoadInputs();

    bool m_isOpen = true;
};

