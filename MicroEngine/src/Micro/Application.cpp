#include "Application.h"
#include <SFML/Graphics.hpp>

namespace Micro{
	Application::Application(float windowWidth, float windowHeight, float maxFPS) :m_window(sf::VideoMode(windowWidth, windowHeight), "new game",
		sf::Style::Default), m_systemManager(m_window) {
		
		m_window.setFramerateLimit(maxFPS);


		m_systemManager.LoadScene("DefualtScene");
		 
		m_camera = &m_systemManager.GetCamera();
	}

	Application::Application(sf::Vector2f windowSize, float maxFPS) :m_window(sf::VideoMode(windowSize.x, windowSize.y), "new game",
		sf::Style::Default), m_systemManager(m_window){
		m_window.setFramerateLimit(maxFPS);


		m_systemManager.LoadScene("DefualtScene");

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

	void Application::Display() {

		m_systemManager.GetLightSystem()->update();
		m_systemManager.GetLightSystem()->render(m_window.getView(), m_window);

		m_window.clear(sf::Color(60, 60, 60));

		m_systemManager.Render(m_window);
		

		m_systemManager.GetLightSystem()->draw(m_window.getView(), m_window);

		m_window.display();
	}
}
