#include "Camera.h"
#include "../SystemLogic/SystemManager.h"

namespace Micro{
	Camera::Camera(const sf::Vector2f WinodwSize) : m_windowSize(WinodwSize) {
		zoom = 1;

		position = sf::Vector2f(0, 0);
		m_view.setCenter(position);

		rotation = 0;
		m_view.setRotation(rotation);

		m_view.setSize(sf::Vector2f(m_windowSize.x / zoom, m_windowSize.y / zoom));

		m_window = nullptr;
	}

	Camera::Camera(const float winodwWidth, const float windowHight) : m_windowSize(sf::Vector2f(winodwWidth, windowHight)) {
		zoom = 1;

		position = sf::Vector2f(0, 0);
		m_view.setCenter(position);

		rotation = 0;
		m_view.setRotation(rotation);

		m_view.setSize(sf::Vector2f(m_windowSize.x / zoom, m_windowSize.y / zoom));

		m_window = nullptr;
	}


	void Camera::Follow(std::string ObjectName) {
		m_objectName = ObjectName;
	}

	void Camera::Unfollow() {
		m_objectName = "";
	}

	void Camera::SetCam(sf::RenderWindow& win) {
		win.setView(m_view);
		m_window = &win;
	}

	void Camera::ClearCam() {
		m_window = nullptr;
	}



	void Camera::Update(SystemManager& systemManager) {
		if (m_objectName != "") {
			if (m_view.getCenter() != systemManager.GetObjectByName(m_objectName.c_str())->GetPosition())
				m_view.setCenter(systemManager.GetObjectByName(m_objectName.c_str())->GetPosition());
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
	void Camera::SetWindowSize(sf::Vector2f NewSize)
	{
		m_windowSize = NewSize;
	}

	sf::View Camera::GetView() const
	{
		return m_view;
	}

	void Camera::SetView(sf::View newView)
	{
		m_view = newView;
	}


}