#include <Micro.h>

class App : public Micro::Application
{
public:
	App(float windowWidth, float windowHeight, float maxFPS) : Micro::Application(windowWidth, windowHeight, maxFPS) {
	}

	App(sf::Vector2f windowSize, float maxFPS) : Micro::Application(windowSize, maxFPS) {
	}

	void Run() override{

		int light = m_lightSystem.AddLight(Micro::Light::LightType::Directional, sf::Color(255, 255, 255, 1), 200, 135);
		int light2 = m_lightSystem.AddLight(Micro::Light::LightType::Circle, sf::Color(255, 255, 251,1), 200);



		while (m_window.isOpen()) {
			InputFunc();

			m_systemManager.Update(m_camera);

			m_lightSystem.getLight(light2)->position = sf::Vector2f(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);
			 
			Display();
		}
	}

};

Micro::Application* Micro::CreateApplication() {
	return new App(920, 510, 144);
}