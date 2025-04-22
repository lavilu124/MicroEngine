#include "Application.h"
#include <SFML/Graphics.hpp>

namespace Micro{
	Application::Application(float windowWidth, float windowHeight, float maxFPS, const char* name, std::string scene) :m_window(sf::VideoMode(windowWidth, windowHeight), name,
		sf::Style::Default), m_systemManager(m_window) {
		
		m_window.setFramerateLimit(maxFPS);

		(scene == "")? m_systemManager.LoadScene("DefualtScene") : m_systemManager.LoadScene(scene);
		
		 
		m_camera = &m_systemManager.GetCamera();
	}

	Application::Application(sf::Vector2f windowSize, float maxFPS, const char* name, std::string scene) :m_window(sf::VideoMode(windowSize.x, windowSize.y), name,
		sf::Style::Default), m_systemManager(m_window){
		m_window.setFramerateLimit(maxFPS);

		(scene == "") ? m_systemManager.LoadScene("DefualtScene") : m_systemManager.LoadScene(scene);

		m_camera = &m_systemManager.GetCamera();
	}

	Application::~Application() {}
	void Application::Run() {
		while (m_window.isOpen()) {
			InputFunc();

			m_systemManager.Update(m_camera);

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

			m_systemManager.RunInput(event);
		}
	}

	void Application::SetWindowSize(sf::Vector2u newSize)
	{
		m_window.setSize(newSize);
	}

	sf::Vector2u Application::GetWindowSize()
	{
		return m_window.getSize();
	}

	void Application::Display() {

		m_window.clear();

		m_systemManager.Render(m_window);

		m_window.display();
	}
}
