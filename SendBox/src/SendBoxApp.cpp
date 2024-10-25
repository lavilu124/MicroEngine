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
		//Micro::ls::SpotLight light(sf::Vector2f(0, 0), 0, sf::Color::White);
		//light.setRadius(200);
		//light.setSpreadAngle(360);
		//light.setColor(sf::Color::Yellow);
		//light.setActive(true);
		//int light1 = m_systemManager.AddLight(&light);
		//
		//Micro::ls::FlashLight lantern2(sf::Vector2f(-200, -200), 0, 0, sf::Color::White);
		//lantern2.setRadius(200); // Light radius
		//lantern2.setSpreadAngle(35); // Full circle
		//lantern2.setColor(sf::Color::Yellow); // Light color
		//lantern2.setActive(true); // Ensure it's active
		//lantern2.setDirectionAngle(90);
		//int light2 = m_systemManager.AddLight(&lantern2);

		float x;
		float y;

		while (m_window.isOpen()) {
			InputFunc();

			m_systemManager.Update(m_camera);

			MC_LOG(std::to_string(sf::Mouse::getPosition(m_window).x) + ", " + std::to_string(sf::Mouse::getPosition(m_window).y));
			/*std::cin >> x;
			std::cin >> y;*/
			//lantern2.setPosition(sf::Mouse::getPosition);

			Display();
		}
	}

};

Micro::Application* Micro::CreateApplication() {
	return new App(960, 540, 144);
}