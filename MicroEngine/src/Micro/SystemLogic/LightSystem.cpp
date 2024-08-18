#include "LightSystem.h"
#include <cmath>

#define MINDIFF 2.25e-308
#define PI  3.14159f
#define POINT_COUNT  30
#define MAX_DISTANCE  255.f

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

    LightSource::LightSource(LightType type, float size, float angle)
        : m_type(type) {
        if (type == LightType::Circle) {
            createCircleLight(size);
        }
        else if (type == LightType::Directional) {
            createDirectionalLight(size, angle);
        }
    }

    void LightSource::createCircleLight(float diameter) {
        m_circleLight = sf::CircleShape(diameter / 2);
        m_circleLight.setPointCount(diameter / 4);
        m_circleLight.setOrigin(diameter / 2, diameter / 2);

        m_gradientImage.create(diameter, diameter);

        for (unsigned y = 0; y < diameter; ++y) {
            for (unsigned x = 0; x < diameter; ++x) {
                float distance = sqroot((x - (diameter / 2)) * (x - (diameter / 2)) + (y - (diameter / 2)) * (y - (diameter / 2)));
                float intensity = std::max(0.f, 1.f - distance / (diameter / 2));
                sf::Uint8 alpha = static_cast<sf::Uint8>(MAX_DISTANCE * intensity);
                m_gradientImage.setPixel(x, y, sf::Color(255, 255, 255, alpha));
            }
        }

        m_gradientTexture.loadFromImage(m_gradientImage);
        m_circleLight.setTexture(&m_gradientTexture);
    }

    void LightSource::createDirectionalLight(float size, float angle) {
        m_directionalLight.setPointCount(POINT_COUNT + 2);

        m_directionalLight.setPoint(0, sf::Vector2f(0, 0));

        for (int i = 1; i <= POINT_COUNT + 1; ++i) {
            float angleDeg = 45.0f + (angle - 45.0f) * (i - 1) / POINT_COUNT;
            float x = (size / 2) * std::cos(angleDeg * PI / 180);
            float y = (size / 2) * std::sin(angleDeg * PI / 180);
            m_directionalLight.setPoint(i, sf::Vector2f(x, y));
        }

        m_gradientImage.create(size, size);

        for (unsigned y = 0; y < size; ++y) {
            for (unsigned x = 0; x < size; ++x) {
                float distance = std::sqrt((x - (size / 2)) * (x - (size / 2)) + (y - m_directionalLight.getPosition().y) * (y - m_directionalLight.getPosition().y));
                float intensity = std::max(0.f, 1.f - distance / (size / 2.f));
                sf::Uint8 alpha = static_cast<sf::Uint8>(255 * intensity);
                m_gradientImage.setPixel(x, y, sf::Color(255, 255, 255, alpha));
            }
        }

        m_gradientTexture.loadFromImage(m_gradientImage);
        m_directionalLight.setTexture(&m_gradientTexture);
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

    LightSystem::LightSystem(sf::Vector2f windowSize, sf::Color darkness) : m_darkness(darkness) {
        m_lightTexture.create(windowSize.x, windowSize.y);
    }

    LightSystem::LightSystem(float width, float height, sf::Color darkness) : m_darkness(darkness) {
        m_lightTexture.create(width, height);
    }

    void LightSystem::update() {
        m_lightTexture.clear(m_darkness);

        for (int i = 0; i < m_lights.size(); i++) {
            m_lights[i].Update();
            m_lightTexture.draw(*m_lights[i].GetLight(), sf::BlendAdd);
        }

        m_lightTexture.display();
    }

    void LightSystem::draw(sf::RenderWindow& window) {
        sf::Sprite lightSprite(m_lightTexture.getTexture());
        lightSprite.setOrigin(lightSprite.getLocalBounds().width / 2.0f, lightSprite.getLocalBounds().height / 2.0f);
        lightSprite.setPosition(window.getView().getCenter());

        window.draw(lightSprite, sf::BlendMultiply);
    }

    int LightSystem::AddLight(LightType type, float size, float angle) {
		LightSource light(type, size, angle);
		m_lights.push_back(light);
		return m_lights.size() - 1;
	}

    void LightSystem::RemoveLight(int index) {
        m_lights.erase(m_lights.cbegin() + index);
        m_lights.shrink_to_fit();
    }

    LightSource* LightSystem::getLight(int index) {
        return &m_lights[index];
    }
}
