#include <Micro.h>

class App : public Micro::Application
{
public:
	App(float windowWidth, float windowHeight, float maxFPS) : Micro::Application(windowWidth, windowHeight, maxFPS) {
	}

	App(sf::Vector2f windowSize, float maxFPS) : Micro::Application(windowSize, maxFPS) {
	}

	void Run() override{

		int light1 = m_systemManager.AddLight(Micro::Light::LightType::Circle, sf::Color(255, 255, 255,25.5), 200);
		int light2 = m_systemManager.AddLight(Micro::Light::LightType::Circle, sf::Color(255, 255, 255, 25.5), 200);

		m_systemManager.getLight(light2)->position = sf::Vector2f(-200.0, -200.0);


		while (m_window.isOpen()) {
			InputFunc();

			m_systemManager.Update(m_camera);

			m_systemManager.getLight(light1)->position = sf::Vector2f(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);

			Display();
		}
	}

};

Micro::Application* Micro::CreateApplication() {
	return new App(960, 540, 144);
}