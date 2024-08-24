#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <variant>
#include "../../Core.h"

namespace Micro {
    namespace Input {

        using KeyType = std::variant<sf::Keyboard::Key, sf::Joystick::Axis, sf::Mouse::Button>;

        enum class MICRO_API inputType {
            KeyboardKey,
            ControllerButton,
            MouseButton
        };

        enum class MICRO_API inputPart {
            Started,
            Pressed,
            Released
        };

        class MICRO_API InputAction {
        public:
            explicit InputAction(inputType type, KeyType key, std::string name, inputPart part,
                void (*onInput)(), void(*offInput)() = nullptr);

            bool Active(sf::Event event);

        private:
            bool KeyPressed(sf::Event event);
            bool KeyReleased(sf::Event event);
            bool KeyStarted(sf::Event event);

            int checkKeyboardEvent(sf::Keyboard::Key key, sf::Event event);
            int checkMouseEvent(sf::Mouse::Button button, sf::Event event);
            int checkControllerEvent(unsigned int buttonIndex, sf::Event event);

            inputType m_type;
            KeyType m_key;
            std::string m_name;
            inputPart m_part;
            bool m_isPressed;
            void (*m_onInput)();
            void (*m_offInput)();
        };

    }
}



