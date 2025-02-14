#include <iostream>
#include <Micro.h>
#include <LightInclude.h>

class App : public Micro::Application
{
public:
	App(const float windowWidth, const float windowHeight, const float maxFPS, std::string scene) : Micro::Application(windowWidth, windowHeight, maxFPS, scene) {
		

	}

	App(const sf::Vector2f windowSize, float maxFPS, std::string scene) : Micro::Application(windowSize, maxFPS, scene) {
	}

	void Run() override {

		m_systemManager.SetDarkness(75);

		sfu::LightId light = m_systemManager.AddLight(sfu::lightType::radial, "testrgjgi");
		m_systemManager.GetLight<sfu::lightType::radial>(light)->setRange(150);
		m_systemManager.GetLight<sfu::lightType::radial>(light)->setColor(sf::Color::Blue);



		m_systemManager.AddEdge(sf::Vector2f(0.f, 0.f),
			sf::Vector2f(0.f, 300.f));


		while (m_window.isOpen()) {
			sf::Event e;
			while (m_window.pollEvent(e)) {
				if (e.type == sf::Event::Closed) {
					m_window.close();
				}
				else if (e.type == sf::Event::MouseMoved) {
					sf::Vector2i mousePositionWindow = sf::Mouse::getPosition(m_window);
					sf::Vector2u windowSize = m_window.getSize();

					int mouseXRelativeToCenter = mousePositionWindow.x - windowSize.x / 2;
					int mouseYRelativeToCenter = mousePositionWindow.y - windowSize.y / 2;
					m_systemManager.GetLight<sfu::lightType::radial>(light)->setPosition(mouseXRelativeToCenter, mouseYRelativeToCenter);
				}
			}

			m_systemManager.Update(m_camera);

			Display();
		}
	}


};

Micro::Application* Micro::CreateApplication(std::string scene) {
	return new App(960, 540, 144, scene);
}