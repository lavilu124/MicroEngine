#include <Micro.h>
#include <SFML/Graphics.hpp>

class App : public Micro::Application
{
public:
	App(float windowWidth, float windowHeight, float maxFPS) : 
		m_lightSystem(windowWidth, windowHeight, sf::Color(50, 50, 50, 150)),
  		m_window(sf::VideoMode(windowWidth, windowHeight),"new game",
			sf::Style::Default) , m_systemManager(m_window){


		m_window.setFramerateLimit(maxFPS);


		m_systemManager.LoadScene("Scene1");

		m_camera = &m_systemManager.GetCamera();
	}

	void Run() override{

		int light = m_lightSystem.AddLight(Light::LightType::Directional, 400, 135.f);

		while (m_window.isOpen()) {
			InputFunc();

			m_systemManager.Update(m_camera);

			m_lightSystem.getLight(light)->position = sf::Vector2f(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);

			Display();
		}
	}

private:
	void InputFunc() {
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

	void Display() {
		m_lightSystem.update();

		m_window.clear();

		m_systemManager.Render(m_window);

		m_lightSystem.draw(m_window);

		m_window.display();
	}
private:

	sf::RenderWindow m_window;
	SystemManager m_systemManager;
	Camera* m_camera;
	Light::LightSystem m_lightSystem;
};

Micro::Application* Micro::CreateApplication() {
	return new App(920, 510, 144);
}