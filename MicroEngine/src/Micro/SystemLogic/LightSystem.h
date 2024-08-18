#pragma once
#include "SFML/Graphics.hpp"
#include "../Core.h"

namespace Light {

    enum class LightType {
        Circle,
        Directional
    };

    class MICRO_API LightSource {
    public:
        LightSource(LightType type, float size, float angle = 0.0f);

        void Update();
        sf::Drawable* GetLight();

        sf::Vector2f position;
        sf::Vector2f scale = sf::Vector2f(1, 1);
        float rotation;

    private:
        LightType m_type;

        sf::Texture m_gradientTexture;
        sf::Image m_gradientImage;

        sf::CircleShape m_circleLight;
        sf::ConvexShape m_directionalLight;

        void createCircleLight(float diameter);
        void createDirectionalLight(float size, float angle);
    };

    class MICRO_API LightSystem {
    public:
        LightSystem(sf::Vector2f windowSize, sf::Color darkness);
        LightSystem(float width, float height, sf::Color darkness);
        void update();
        void draw(sf::RenderWindow& window);

        sf::Color m_darkness;

        int AddLight(LightType type, float size, float angle);
		void RemoveLight(int index);
		LightSource* getLight(int index);

    private:
        std::vector<LightSource> m_lights;
        sf::RenderTexture m_lightTexture;
    };
}
