#include <Micro.h>
#include <iostream>

class App : public Micro::Application
{
public:
	App(const float windowWidth, const float windowHeight, const float maxFPS) : Micro::Application(windowWidth, windowHeight, maxFPS) {
		//m_systemManager.LoadScene("de");

	}

	App(const sf::Vector2f windowSize, float maxFPS) : Micro::Application(windowSize, maxFPS) {
	}

	void Run() override{
		m_systemManager.GetLightSystem()->setAmbiantLight(sf::Color(50, 40, 50));

		const int light2 = m_systemManager.AddLight(Micro::ls::Flash, sf::Vector2f(-200, -200), 200, sf::Color::Yellow, "lan", 0);
		m_systemManager.GetLight(light2).flash->setActive(true);
		m_systemManager.GetLight(light2).flash->setSpreadAngle(35);
		m_systemManager.GetLight(light2).flash->setDirectionAngle(90);


		while (m_window.isOpen()) {
			InputFunc();

			m_systemManager.Update(m_camera);

			sf::Vector2i mousePositionWindow = sf::Mouse::getPosition(m_window);
			sf::Vector2u windowSize = m_window.getSize();

			int mouseXRelativeToCenter = mousePositionWindow.x - windowSize.x / 2;
			int mouseYRelativeToCenter = mousePositionWindow.y - windowSize.y / 2;

			m_systemManager.GetLight(light2).flash->setPosition(sf::Vector2f(mouseXRelativeToCenter, mouseYRelativeToCenter));

			Display();
		}
	}


};

Micro::Application* Micro::CreateApplication() {
	return new App(960, 540, 144);
}