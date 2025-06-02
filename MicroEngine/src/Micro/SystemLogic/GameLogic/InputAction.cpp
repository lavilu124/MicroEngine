#include "InputAction.h"


namespace Micro {
    namespace Input {

        InputAction::InputAction(inputType type, KeyType key, std::string name, inputPart part,
            void(*onInput)(InputAction&), void(*offInput)(InputAction&)) :
            m_type(type), m_key(key), m_name(std::move(name)), m_part(part), m_onInput(onInput), m_offInput(offInput) {

            m_isPressed = false;
        }

        bool InputAction::Active(sf::Event event) {
            bool returnValue = false;

            if (m_type == inputType::MouseMove && event.type == sf::Event::MouseMoved) {
                if (m_onInput) m_onInput(*this);
                return true;
            }
            else if (m_type == inputType::JoystickMove && std::holds_alternative<unsigned int>(m_key)) {

                if (sf::Joystick::isConnected(m_controllerId)) {
                    sf::Joystick::update();

                    float x;
                    float y; 

                    switch (m_controllerExisIndex)
                    {
                    case 0:
                        x = sf::Joystick::getAxisPosition(m_controllerId, sf::Joystick::X);
                        y = sf::Joystick::getAxisPosition(m_controllerId, sf::Joystick::Y);
                        break;
                    case 1:
                        x = sf::Joystick::getAxisPosition(m_controllerId, sf::Joystick::Z);
                        y = sf::Joystick::getAxisPosition(m_controllerId, sf::Joystick::R);
                        break;
                    case 2:
                        x = sf::Joystick::getAxisPosition(m_controllerId, sf::Joystick::U);
                        y = sf::Joystick::getAxisPosition(m_controllerId, sf::Joystick::V);
                        break;
                    case 3:
                        x = sf::Joystick::getAxisPosition(m_controllerId, sf::Joystick::PovX);
                        y = sf::Joystick::getAxisPosition(m_controllerId, sf::Joystick::PovY);
                        break;

                    }

                    if (std::abs(x) > m_controllerThreshold || std::abs(y) > m_controllerThreshold) {
                        if (m_onInput) m_onInput(*this);
                        return true;
                    }
                }
            }


            switch (m_part) {
            case inputPart::Started:
                if (KeyReleased(event)) 
                    m_isPressed = false;
                returnValue = KeyStarted(event);
                if (returnValue) m_isPressed = true;
                break;
            case inputPart::Pressed:
                returnValue = KeyPressed(event);
                if (returnValue) m_isPressed = true;
                break;
            case inputPart::Released:
                returnValue = KeyReleased(event);
                break;
            default:
                throw std::invalid_argument("Unknown InputPart specified.");
            }

            if (returnValue) {
                if (m_onInput) m_onInput(*this);
            }
            else if (m_offInput) {
                m_offInput(*this);
            }

            return returnValue;
        }

        void InputAction::SetOnInput(void(*onInput)(InputAction&)) {
            m_onInput = onInput;
        }

        void InputAction::SetOffInput(void(*offInput)(InputAction&)) {
            m_offInput = offInput;
        }

        std::string InputAction::GetName() const
        {
            return m_name;
        }

        void InputAction::SetControllerId(int id)
        {
            if (id >= 0)
                m_controllerId = id;
   
        }

        void InputAction::SetContollerThreshold(float th)
        {
            if (th > 0)
                m_controllerThreshold = th;
            
        }

        void InputAction::SetColtrllerExis(int ex)
        {
            if (ex >= 0 && ex <= 3)
                m_controllerExisIndex = ex;
        }

        bool InputAction::KeyPressed(sf::Event event) {
            if (m_type == inputType::KeyboardKey && std::holds_alternative<sf::Keyboard::Key>(m_key)) {
                auto key = std::get<sf::Keyboard::Key>(m_key);
                return CheckKeyboardEvent(key, event) == 1;
            }
            else if (m_type == inputType::ControllerButton && std::holds_alternative<unsigned int>(m_key)) {
                auto buttonIndex = std::get<unsigned int>(m_key);
                return CheckControllerEvent(buttonIndex, event) == 1;
            }
            else if (m_type == inputType::MouseButton && std::holds_alternative<sf::Mouse::Button>(m_key)) {
                auto button = std::get<sf::Mouse::Button>(m_key);
                return CheckMouseEvent(button, event) == 1;
            }
            return false;
        }

        bool InputAction::KeyReleased(sf::Event event) {
            if (m_type == inputType::KeyboardKey && std::holds_alternative<sf::Keyboard::Key>(m_key)) {
                auto key = std::get<sf::Keyboard::Key>(m_key);
                return CheckKeyboardEvent(key, event) == 2;
            }
            else if (m_type == inputType::ControllerButton && std::holds_alternative<unsigned int>(m_key)) {
                auto buttonIndex = std::get<unsigned int>(m_key);
                return CheckControllerEvent(buttonIndex, event) == 2;
            }
            else if (m_type == inputType::MouseButton && std::holds_alternative<sf::Mouse::Button>(m_key)) {
                auto button = std::get<sf::Mouse::Button>(m_key);
                return CheckMouseEvent(button, event) == 2;
            }
            return false;
        }

        bool InputAction::KeyStarted(sf::Event event) {
            return !m_isPressed && KeyPressed(event);
        }

        int InputAction::CheckKeyboardEvent(sf::Keyboard::Key key, sf::Event event) {
            if (event.type == sf::Event::KeyPressed && event.key.code == key) {
                return 1;
            }
            else if (event.type == sf::Event::KeyReleased && event.key.code == key) {
                m_isPressed = false;
                return 2;
            }
            return 0;
        }

        int InputAction::CheckMouseEvent(sf::Mouse::Button button, sf::Event event) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == button) {
                return 1;
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == button) {
                m_isPressed = false;
                return 2;
            }
            return 0;
        }

        int InputAction::CheckControllerEvent(unsigned int buttonIndex, sf::Event event) {
            if (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == buttonIndex) {
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
