#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <variant>
#include <string>
#include <stdexcept>
#include "../../Core.h"

namespace Micro {
    namespace Input {

        // Updated to distinguish between buttons (unsigned int) and axis (sf::Joystick::Axis)
        using KeyType = std::variant<sf::Keyboard::Key, sf::Mouse::Button, unsigned int, sf::Joystick::Axis>;

        enum class MICRO_API inputType {
            KeyboardKey,
            ControllerButton,
            MouseButton,
            MouseMove,
            JoystickMove
        };

        enum class MICRO_API inputPart {
            Started,
            Pressed,
            Released
        };

        class MICRO_API InputAction {
        public:
            explicit InputAction(inputType type, KeyType key, std::string name, inputPart part,
                void (*onInput)(InputAction&) = nullptr, void(*offInput)(InputAction&) = nullptr);

            bool Active(sf::Event event);

            void SetOnInput(void (*onInput)(InputAction&));
            void SetOffInput(void(*offInput)(InputAction&));

            template <typename T>
            auto GetVal();

            std::string GetName() const;

            void SetControllerId(int id);

            void SetContollerThreshold(float th);

            void SetColtrllerExis(int ex);

        private:
            bool KeyPressed(sf::Event event);
            bool KeyReleased(sf::Event event);
            bool KeyStarted(sf::Event event);

            int CheckKeyboardEvent(sf::Keyboard::Key key, sf::Event event);
            int CheckMouseEvent(sf::Mouse::Button button, sf::Event event);
            int CheckControllerEvent(unsigned int buttonIndex, sf::Event event);

            inputType m_type;
            KeyType m_key;
            std::string m_name;
            inputPart m_part;
            bool m_isPressed;
            void (*m_onInput)(InputAction&);
            void (*m_offInput)(InputAction&);

            int m_controllerId = 0;
            float m_controllerThreshold = 5;
            int m_controllerExisIndex = 0;
        };

        template <typename T>
        auto InputAction::GetVal() {
            if constexpr (std::is_same_v<T, sf::Vector2i>) {
                if (m_type == inputType::MouseMove) {
                    return sf::Mouse::getPosition();
                }
            }
            else if constexpr (std::is_same_v<T, sf::Vector2f>) {
                if (m_type == inputType::JoystickMove) {
                    if (!std::holds_alternative<unsigned int>(m_key))
                        throw std::runtime_error("JoystickMove expects unsigned int joystick ID.");
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
                    return sf::Vector2f(
                        x,
                        y
                    );
                }
            }
            throw std::runtime_error("Unsupported type or mismatched inputType.");
        }

    }
}
