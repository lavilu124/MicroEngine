#pragma once
#include "SFML/Graphics.hpp"
#include "../Core.h"
#include "../FileOperations/FileManager.h"

namespace Micro {
    namespace Light {

        enum class LightType {
            Circle,
            Directional
        };

        class MICRO_API LightSource {
        public:
            LightSource(LightType type, int id, sf::Color color, float size, float angle = 0.0f);

            void Update();
            sf::Drawable* GetLight();
            float getRadius() const;  

            sf::Vector2f position;
            sf::Vector2f scale = sf::Vector2f(1, 1);
            float rotation;

            int GetId() const;

			void SetColor(sf::Color color);

			sf::Glsl::Vec4 GetColor() const;
        private:
            LightType m_type;

            sf::CircleShape m_circleLight;
            sf::ConvexShape m_directionalLight;

            void createCircleLight(float diameter);
            void createDirectionalLight(float size, float angle);

            int m_id;
            float m_radius; 

            sf::Glsl::Vec4 m_color;
        };

        class MICRO_API LightSystem {
        public:
            LightSystem(sf::Vector2f windowSize, FileManager fileManger, sf::Color darkness = sf::Color(50, 50, 50, 150));
            LightSystem(float width, float height, FileManager fileManger, sf::Color darkness = sf::Color(50, 50, 50, 150));
            void update(sf::RenderWindow& window);
            void draw(sf::RenderWindow& window);

            sf::Color m_darkness;

            int AddLight(LightType type, sf::Color color, float size, float angle = 0.0f);
            void RemoveLight(int id);
            LightSource* getLight(int id);

        private:
            int GetLightIndex(int id);

        private:
            std::vector<LightSource> m_lights;
            sf::RenderTexture m_lightTexture;
			float m_windowHeight;

            int m_currentId = 0;

            sf::Shader m_lightShader;
        };
    }
}
