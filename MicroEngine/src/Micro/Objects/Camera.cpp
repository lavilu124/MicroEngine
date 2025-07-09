#include "Camera.h"
#include "../SystemLogic/SystemManager.h"

namespace Micro{
	Camera::Camera(const sf::Vector2f& winodwSize) : m_windowSize(winodwSize) {
		m_zoom = 1;

		m_position = sf::Vector2f(0, 0);
		m_view.setCenter(m_position);

		m_rotation = 0;
		m_view.setRotation(m_rotation);

		m_view.setSize(sf::Vector2f(m_windowSize.x / m_zoom, m_windowSize.y / m_zoom));

		m_window = nullptr;
	}

	Camera::Camera(float windowWidth, float windowHight) : m_windowSize(sf::Vector2f(windowWidth, windowHight)) {
		m_zoom = 1;

		m_position = sf::Vector2f(0, 0);
		m_view.setCenter(m_position);

		m_rotation = 0;
		m_view.setRotation(m_rotation);

		m_view.setSize(sf::Vector2f(m_windowSize.x / m_zoom, m_windowSize.y / m_zoom));

		m_window = nullptr;
	}


	void Camera::Follow(const std::string& objectName) {
		m_objectName = objectName;
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



	void Camera::Update(SystemManager& systemManager){

		if (m_window != nullptr) {
			m_window->setView(m_view);
		}

		if (m_objectName != "")
		{
			if (m_view.getCenter() != systemManager.GetObjectByName(m_objectName.c_str())->GetPosition())
				m_view.setCenter(systemManager.GetObjectByName(m_objectName.c_str())->GetPosition());

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

	void Camera::SetPosition(sf::Vector2f newPos)
	{
		if (m_objectName != "") {
			return;
		}

		if (m_view.getCenter() != m_position) {
			m_view.setCenter(m_position);
		}

	}

	sf::Vector2f Camera::GetPosition() const
	{
		return m_position;
	}

	void Camera::SetRotation(float newRot)
	{
		m_view.setRotation(newRot);
		m_rotation = newRot;
	}

	float Camera::GetRotation() const
	{
		return m_rotation;
	}

	void Camera::SetZoom(float newZoom)
	{
		if (newZoom < 1 && newZoom > 0) {
			m_zoom = newZoom;
			m_view.setSize(sf::Vector2f(m_windowSize.x / m_zoom, m_windowSize.y / m_zoom));
		}
	}

	float Camera::GetZoom() const
	{
		return m_zoom;
	}
}
