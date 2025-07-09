#include "Application.h"
#include <SFML/Graphics.hpp>

static Micro::Application* s_Instance = nullptr;


namespace Micro{
	Application::Application(float windowWidth, float windowHeight, float maxFPS, const char* name) : m_window(sf::VideoMode(windowWidth, windowHeight), name,
		sf::Style::Default), m_systemManager(m_window) {
		
		m_window.setFramerateLimit(maxFPS);


		s_Instance = this;
		 
		m_camera = &m_systemManager.GetCamera();
	}

	Application::Application(const sf::Vector2f& windowSize, float maxFPS, const char* name) :m_window(sf::VideoMode(windowSize.x, windowSize.y), name,
		sf::Style::Default), m_systemManager(m_window){
		m_window.setFramerateLimit(maxFPS);

		m_camera = &m_systemManager.GetCamera();
	}


	Application& Application::Get() {
		return *s_Instance;
	}

	void Application::Run() {
		while (m_window.isOpen()) {
			InputFunc();

			m_systemManager.Update();

			Display();
		}
	}



	void Application::InputFunc() {
		sf::Event event;
		while (m_window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				m_window.close();
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				m_window.close();
			}

			m_systemManager.RunInput(event, m_window);
		}
	}

	SystemManager* Application::GetSystemManager()
	{
		return &m_systemManager;
	}

	void Application::SetWindowSize(const sf::Vector2u& newSize)
	{
		m_window.setSize(newSize);
	}

	void Application::SetWindowTitle(const char* name)
	{
		m_window.setTitle(name);
	}



	sf::Vector2u Application::GetWindowSize() const
	{
		return m_window.getSize();
	}

	void Application::Display() {

		m_window.clear();

		m_systemManager.Render(m_window);

		m_window.display();
	}
}
