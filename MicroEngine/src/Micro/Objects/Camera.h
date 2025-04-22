#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "../Core.h"

namespace Micro{
	class MICRO_API Camera
	{
	public:
		Camera(const sf::Vector2f winodwSize);

		Camera(const float winodwWidth, const float windowHight);

		void Follow(std::string& ObjectName);
		void Unfollow();

		void SetCam(sf::RenderWindow& win);

		void ClearCam();

		void Update(SystemManager& systemManager);

		void SetWindowSize(sf::Vector2f NewSize);

	public:
		sf::Vector2f position;
		float rotation;
		float zoom;

		sf::View m_view;

	private:
		std::string m_objectName;
		sf::RenderWindow* m_window;
		sf::Vector2f m_windowSize;
	};
}

