#include "Camera.h"

Camera::Camera(const sf::Vector2f WinodwSize) : m_windowSize(WinodwSize) {
	zoom = 1;
	m_spriteToFollow = nullptr;

	position = sf::Vector2f(0,0);
	m_view.setCenter(position);

	rotation = 0;
	m_view.setRotation(rotation);

	m_view.setSize(sf::Vector2f(m_windowSize.x / zoom, m_windowSize.y / zoom));

	m_window = nullptr;
}

Camera::Camera(const float winodwWidth, const float windowHight) : m_windowSize(sf::Vector2f(winodwWidth, windowHight)){
	zoom = 1;
	m_spriteToFollow = nullptr;

	position = sf::Vector2f(0, 0);
	m_view.setCenter(position);

	rotation = 0;
	m_view.setRotation(rotation);

	m_view.setSize(sf::Vector2f(m_windowSize.x / zoom, m_windowSize.y / zoom));

	m_window = nullptr;
}


void Camera::Follow(GameObject& objectToFollow) {
	m_spriteToFollow = const_cast<sf::Sprite*>(&objectToFollow.GetSprite());
}

void Camera::Unfollow() {
	m_spriteToFollow = nullptr;
}

void Camera::SetCam(sf::RenderWindow& win) {
	win.setView(m_view);
	m_window = &win;
}

void Camera::ClearCam() {
	m_window = nullptr;
}



void Camera::Update() {
	if (m_spriteToFollow != nullptr) {
		if (m_view.getCenter() != m_spriteToFollow->getPosition())
			m_view.setCenter(m_spriteToFollow->getPosition());
	}
	else if (m_view.getCenter() != position) {
		m_view.setCenter(position);
	}

	m_view.setRotation(rotation);
	if (zoom < 1) {
		zoom = 1;
	}
	m_view.setSize(sf::Vector2f(m_windowSize.x / zoom, m_windowSize.y / zoom));
	
	if (m_window != nullptr) {
		m_window->setView(m_view);
	}
}