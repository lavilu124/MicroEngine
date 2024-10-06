#include "LightSystem.h"
#include <cmath>

#define MINDIFF 2.25e-308
#define PI  3.14159f
#define POINT_COUNT  30
#define MAX_DISTANCE  255.f

namespace Micro {
    namespace Light {

        double sqroot(double square) {
            double root = square / 3, last, diff = 1;
            if (square <= 0) return 0;
            do {
                last = root;
                root = (root + square / root) / 2;
                diff = root - last;
            } while (diff > MINDIFF || diff < -MINDIFF);
            return root;
        }

        LightSource::LightSource(LightType type, int id, sf::Color color, float size, float angle)
            : m_type(type), m_id(id), m_radius(size / 2) {
            if (type == LightType::Circle) {
                createCircleLight(size);
            }
            else if (type == LightType::Directional) {
                createDirectionalLight(size, angle);
            }

            intensity = color.a / 25.5f;
            m_color = sf::Glsl::Vec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f , 1);
        }

        int LightSource::GetId() const { return m_id; }

        void LightSource::createCircleLight(float diameter) {
            m_circleLight = sf::CircleShape(diameter / 2);
            m_circleLight.setPointCount(diameter / 4);
            m_circleLight.setOrigin(diameter / 2, diameter / 2);
        }

        void LightSource::createDirectionalLight(float size, float angle) {
            m_directionalLight.setPointCount(POINT_COUNT + 2);
            m_directionalLight.setPoint(0, sf::Vector2f(0, 0));

            for (int i = 1; i <= POINT_COUNT + 1; ++i) {
                float angleDeg = 45.f + (angle - 45.0f) * (i - 1) / POINT_COUNT;
                float x = (size / 2) * std::cos(angleDeg * PI / 180);
                float y = (size / 2) * std::sin(angleDeg * PI / 180);
                m_directionalLight.setPoint(i, sf::Vector2f(x, y));
            }
        }

        void LightSource::Update() {
            if (m_type == LightType::Circle) {
                m_circleLight.setPosition(position);
                m_circleLight.setRotation(rotation);
                m_circleLight.setScale(scale);
            }
            else if (m_type == LightType::Directional) {
                m_directionalLight.setPosition(position);
                m_directionalLight.setRotation(rotation);
                m_directionalLight.setScale(scale);
            }
        }

        sf::Drawable* LightSource::GetLight() {
            if (m_type == LightType::Circle) {
                return &m_circleLight;
            }
            else if (m_type == LightType::Directional) {
                return &m_directionalLight;
            }
            return nullptr;
        }

		void LightSource::SetColor(sf::Color color) {
            m_color = sf::Glsl::Vec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1);
		}

        sf::Glsl::Vec4 LightSource::GetColor() const{
            return m_color;
        }

        float LightSource::getRadius() const {
            return m_radius;
        }

        LightSystem::LightSystem(sf::Vector2f windowSize, FileManager fileManger, sf::Color darkness)
            : m_darkness(darkness) {
            m_windowHeight = windowSize.y;
            m_lightTexture.create(windowSize.x, windowSize.y);
            m_lightShader.loadFromFile(fileManger.GetShaderPath((std::string)"lightShader.frag"), sf::Shader::Fragment);
        }

        LightSystem::LightSystem(float width, float height, FileManager fileManger, sf::Color darkness)
            : m_darkness(darkness) {
			m_windowHeight = height;
            m_lightTexture.create(width, height);
            m_lightShader.loadFromFile(fileManger.GetShaderPath((std::string)"lightShader.frag"), sf::Shader::Fragment);
        }

        void LightSystem::update(sf::RenderWindow& window, std::vector<LightSource>& lights) {
            m_lightTexture.clear(m_darkness);

            for (auto& light : lights) {
                light.Update();

                // Set shader uniforms for each light
                m_lightShader.setUniform("lightIntensity", light.intensity);
                m_lightShader.setUniform("windowHeight", m_windowHeight); 
                m_lightShader.setUniform("lightCenter", light.position);
                m_lightShader.setUniform("lightRadius", light.getRadius());
                m_lightShader.setUniform("lightColor", light.GetColor());  // Adjust as needed

                // Draw the light with the shader
                m_lightTexture.draw(*light.GetLight(), &m_lightShader);
            }

            m_lightTexture.display();
        }

        void LightSystem::draw(sf::RenderWindow& window) {
            sf::Sprite lightSprite(m_lightTexture.getTexture());
            lightSprite.setOrigin(lightSprite.getLocalBounds().width / 2.0f, lightSprite.getLocalBounds().height / 2.0f);
            lightSprite.setPosition(window.getView().getCenter());

            window.draw(lightSprite, sf::BlendMultiply);
        }

    }
}
