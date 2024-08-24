#include "InputAction.h"

namespace Micro {
    namespace Input {

        InputAction::InputAction(inputType type, KeyType key, std::string name, inputPart part,
            void(*onInput)(), void(*offInput)()) :
            m_type(type), m_key(key), m_name(name), m_part(part), m_onInput(onInput), m_offInput(offInput) {

            m_isPressed = false;
            if (m_type != inputType::KeyboardKey && m_type != inputType::ControllerButton && m_type != inputType::MouseButton) {
                throw std::invalid_argument("Invalid InputType specified.");
            }
        }

        bool InputAction::Active(sf::Event event) {
            bool returnValue = false;

            switch (m_part) {
            case inputPart::Started:
                returnValue = KeyStarted(event);
                break;
            case inputPart::Pressed:
                returnValue = KeyPressed(event);
                break;
            case inputPart::Released:
                returnValue = KeyReleased(event);
                break;
            default:
                throw std::invalid_argument("Unknown InputPart specified.");
            }

            if (returnValue) {
                m_onInput();
            }
            else if (m_offInput) {
                m_offInput();
            }

            return returnValue;
        }

        bool InputAction::KeyPressed(sf::Event event) {
            if (m_type == inputType::KeyboardKey) {
                sf::Keyboard::Key key = std::get<sf::Keyboard::Key>(m_key);
                return checkKeyboardEvent(key, event) == 1 || m_isPressed;
            }
            else if (m_type == inputType::ControllerButton) {
                unsigned int buttonIndex = std::get<sf::Joystick::Axis>(m_key);
                return checkControllerEvent(buttonIndex, event) == 1 || m_isPressed;
            }
            else if (m_type == inputType::MouseButton) {
                sf::Mouse::Button key = std::get<sf::Mouse::Button>(m_key);
                return checkMouseEvent(key, event) == 1 || m_isPressed;
            }
            return false;
        }

        bool InputAction::KeyReleased(sf::Event event) {
            if (m_type == inputType::KeyboardKey) {
                sf::Keyboard::Key key = std::get<sf::Keyboard::Key>(m_key);
                return checkKeyboardEvent(key, event) == 2;
            }
            else if (m_type == inputType::ControllerButton) {
                unsigned int buttonIndex = std::get<sf::Joystick::Axis>(m_key);
                return checkControllerEvent(buttonIndex, event) == 2;
            }
            else if (m_type == inputType::MouseButton) {
                sf::Mouse::Button key = std::get<sf::Mouse::Button>(m_key);
                return checkMouseEvent(key, event) == 2;
            }
            return false;
        }

        bool InputAction::KeyStarted(sf::Event event) {
            if (!m_isPressed && KeyPressed(event)) {
                return true;
            }
            KeyPressed(event);
            return false;
        }

        int InputAction::checkKeyboardEvent(sf::Keyboard::Key key, sf::Event event) {
            if (event.type == sf::Event::KeyPressed && event.key.code == key) {
                m_isPressed = true;
                return 1;
            }
            else if (event.type == sf::Event::KeyReleased && event.key.code == key) {
                m_isPressed = false;
                return 2;
            }
            return 0;
        }

        int InputAction::checkMouseEvent(sf::Mouse::Button button, sf::Event event) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == button) {
                m_isPressed = true;
                return 1;
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == button) {
                m_isPressed = false;
                return 2;
            }
            return 0;
        }

        int InputAction::checkControllerEvent(unsigned int buttonIndex, sf::Event event) {
            if (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == buttonIndex) {
                m_isPressed = true;
                return 1;
            }
            else if (event.type == sf::Event::JoystickButtonReleased && event.joystickButton.button == buttonIndex) {
                m_isPressed = false;
                return 2;
            }
            return 0;
        }


    }
}
