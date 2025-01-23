#include <Micro.h>
#include <LightInclude.h>

class App : public Micro::Application
{
public:
	App(const float windowWidth, const float windowHeight, const float maxFPS) : Micro::Application(windowWidth, windowHeight, maxFPS) {
		

	}

	App(const sf::Vector2f windowSize, float maxFPS) : Micro::Application(windowSize, maxFPS) {
	}

	void Run() override {
		

		sfu::LightId light = m_systemManager.AddLight(sfu::lightType::radial);
		m_systemManager.GetLight<sfu::lightType::radial>(light)->setRange(150);
		m_systemManager.GetLight<sfu::lightType::radial>(light)->setColor(sf::Color::Blue);



		Micro::EdgeVector edges;
		edges.emplace_back(sf::Vector2f(0.f, 0.f),
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
					m_systemManager.GetLight<sfu::lightType::radial>(light)->castLight(edges.begin(), edges.end());
				}
			}

			m_systemManager.Update(m_camera);

			m_window.clear();
			m_window.draw(*m_systemManager.GetLight<sfu::lightType::radial>(light));
			m_window.display();

			//Display();
		}
	}


};

Micro::Application* Micro::CreateApplication() {
	return new App(960, 540, 144);
}