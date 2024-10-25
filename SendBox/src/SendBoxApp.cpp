#include <Micro.h>
#include <iostream>

class App : public Micro::Application
{
public:
	App(float windowWidth, float windowHeight, float maxFPS) : Micro::Application(windowWidth, windowHeight, maxFPS) {
	}

	App(sf::Vector2f windowSize, float maxFPS) : Micro::Application(windowSize, maxFPS) {
	}

	void Run() override{
		m_systemManager.getLightSystem()->setAmbiantLight(sf::Color(20, 20, 20));

		Micro::ls::SpotLight light(sf::Vector2f(0, 0), 0, sf::Color::White);
		light.setRadius(200);
		light.setSpreadAngle(360);
		light.setColor(sf::Color::Yellow);
		light.setActive(true);
		int light1 = m_systemManager.AddLight(&light);

		
		Micro::ls::FlashLight lantern2(sf::Vector2f(-200, -200), 0, 0, sf::Color::White);
		lantern2.setRadius(200); 
		lantern2.setSpreadAngle(35); 
		lantern2.setColor(sf::Color::Yellow); 
		lantern2.setActive(true);
		lantern2.setDirectionAngle(90);
		int light2 = m_systemManager.AddLight(&lantern2);


		while (m_window.isOpen()) {
			InputFunc();

			m_systemManager.Update(m_camera);

			sf::Vector2i mousePositionWindow = sf::Mouse::getPosition(m_window);
			sf::Vector2u windowSize = m_window.getSize();

			int mouseXRelativeToCenter = mousePositionWindow.x - windowSize.x / 2;
			int mouseYRelativeToCenter = mousePositionWindow.y - windowSize.y / 2;

			m_systemManager.getLight(light2)->setPosition(sf::Vector2f(mouseXRelativeToCenter, mouseYRelativeToCenter));

			Display();
		}
	}

};

Micro::Application* Micro::CreateApplication() {
	return new App(960, 540, 144);
}