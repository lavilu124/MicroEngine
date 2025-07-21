#include "Application.h"
#include <SFML/Graphics.hpp>

static Micro::Application* s_Instance = nullptr;


namespace Micro{

	Application::Application(float windowWidth, float windowHeight, float maxFPS, const char* name, const char* scene){

		if (!std::string(scene).empty())
		{
			m_inGUiMode = true;
			m_systemManager.emplace(windowWidth, windowHeight);
		}else
		{
			m_window.emplace(sf::VideoMode(windowWidth, windowHeight), name, sf::Style::Default);
			m_window.value().setFramerateLimit(maxFPS);
			m_systemManager.emplace(*m_window);
		}
		


		s_Instance = this;
		 
		m_camera = &(*m_systemManager).GetCamera();
	}

	Application::Application(const sf::Vector2f& windowSize, float maxFPS, const char* name, const char* scene){
		if (!std::string(scene).empty())
		{
			m_inGUiMode = true;
			m_systemManager.emplace(windowSize.x, windowSize.y);
		}
		else
		{
			m_window.emplace(sf::VideoMode(windowSize.x, windowSize.y), name, sf::Style::Default);
			m_window.value().setFramerateLimit(maxFPS);
			m_systemManager.emplace(*m_window);
		}

		m_camera = &(*m_systemManager).GetCamera();
	}


	Application& Application::Get() {
		return *s_Instance;
	}

	void Application::Run() {
		while ((*m_window).isOpen() || m_inGUiMode) {
			InputFunc();

			(*m_systemManager).Update();

			Display();
		}
	}



	void Application::InputFunc() {
		if (!m_window)
			return;

		sf::Event event;
		while ((*m_window).pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				(*m_window).close();
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				(*m_window).close();
			}

			(*m_systemManager).RunInput(event, (*m_window));
		}
	}

	SystemManager* Application::GetSystemManager()
	{
		return &(*m_systemManager);
	}

	void Application::SetWindowSize(const sf::Vector2u& newSize)
	{
		if (m_window)
			(*m_window).setSize(newSize);
	}

	void Application::SetWindowTitle(const char* name)
	{
		if (m_window)
			(*m_window).setTitle(name);
	}



	sf::Vector2u Application::GetWindowSize() const
	{
		if (m_window)
			return (*m_window).getSize();

		return { 0,0 };
	}

	void Application::Display() {
		if (!m_window)
			return;

		(*m_window).clear();

		(*m_systemManager).Render((*m_window));

		(*m_window).display();
	}
}
