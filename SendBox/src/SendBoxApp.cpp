#include <Micro.h>
#include <LightInclude.h>

class App : public Micro::Application
{
public:
	App(const float windowWidth, const float windowHeight, const float maxFPS, const char* scene) : Micro::Application(windowWidth, windowHeight, maxFPS, "new game", scene) {
		m_systemManager.SetDarkness(75);

		//add a light to the scene
		m_lightId = m_systemManager.AddLight(ls::lightType::radial, "testrgjgi");
		m_systemManager.GetLight<ls::lightType::radial>(m_lightId)->SetRange(150);
		m_systemManager.GetLight<ls::lightType::radial>(m_lightId)->SetColor(sf::Color::Blue);

		//add a wall to block light
		m_systemManager.AddEdge(sf::Vector2f(0.f, 0.f),
			sf::Vector2f(0.f, 300.f));
	}

	App(const sf::Vector2f windowSize, float maxFPS, const char* scene) : Micro::Application(windowSize, maxFPS, "new game", scene) {
		m_systemManager.SetDarkness(75);

		//add a light to the scene
		m_lightId = m_systemManager.AddLight(ls::lightType::radial, "testrgjgi");
		m_systemManager.GetLight<ls::lightType::radial>(m_lightId)->SetRange(150);
		m_systemManager.GetLight<ls::lightType::radial>(m_lightId)->SetColor(sf::Color::Blue);

		//add a wall to block light
		m_systemManager.AddEdge(sf::Vector2f(0.f, 0.f),
			sf::Vector2f(0.f, 300.f));
	}

	void Run() override {
		while (m_window.isOpen()) {
			InputFunc();

			m_systemManager.Update(m_camera);

			Display();
		}
	}

private:
	ls::LightId m_lightId;  
};

Micro::Application* Micro::CreateApplication(const char* scene) {
	return new App(1920, 1080, 144, scene);
}