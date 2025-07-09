#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "../Core.h"

namespace Micro{
	class MICRO_API Camera
	{
	public:
		Camera(const sf::Vector2f& winodwSize);

		Camera(float winodwWidth, float windowHight);

		void Follow(const std::string& objectName);
		void Unfollow();

		void SetCam(sf::RenderWindow& win);

		void ClearCam();

		void Update(SystemManager& systemManager);

		void SetWindowSize(sf::Vector2f newSize);

		sf::View GetView() const;
		void SetView(sf::View newView);


		void SetPosition(sf::Vector2f newPos);
		sf::Vector2f GetPosition() const;


		void SetRotation(float newRot);
		float GetRotation() const;

		void SetZoom(float newZoom);
		float GetZoom() const;

	private:
		sf::Vector2f m_position;
		float m_rotation;
		float m_zoom;


		std::string m_objectName;
		sf::RenderWindow* m_window;
		sf::Vector2f m_windowSize;
		sf::View m_view;
	};
}

