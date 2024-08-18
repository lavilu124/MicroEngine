#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "../Core.h"

class MICRO_API Camera
{
public:
	Camera(const sf::Vector2f winodwSize);

	Camera(const float winodwWidth, const float windowHight);

	void Follow(GameObject& objectToFollow);
	void Unfollow();

	void SetCam(sf::RenderWindow& win);

	void ClearCam();
	
	void Update();

public:
	sf::Vector2f position;
	float rotation;
	float zoom;

	sf::View m_view;

private:
	sf::Sprite* m_spriteToFollow;
	sf::RenderWindow* m_window;
	sf::Vector2f m_windowSize;
};

