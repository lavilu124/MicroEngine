#include "Objects/Player.h"
#include <LightInclude.h>
#include <Micro/EnteryPoint.h>

#include <iostream>

void testing1() {
	std::cout << "testing123";
}

void test(Micro::Input::InputAction& in) {
	std::cout << "testing123";
}

class App : public Micro::Application
{
public:
	

	App(const float windowWidth, const float windowHeight, const float maxFPS, const char* scene) : Micro::Application(windowWidth, windowHeight, maxFPS, "new game") {
		//m_systemManager.GetFileManager().AddButtonFunc("testing1", testing1);
		m_systemManager.LoadScene("test");

		

		m_systemManager.SetDarkness(75);
     
		m_systemManager.GetFileManager().AddInputFunc("test", test);

		m_systemManager.GetObjectByName("1")->SetPhysicsBody(10, true, Micro::Physics::Material{0.6f, 0.3f, 0.5f , Micro::Physics::MINIMUM, Micro::Physics::AVERAGE});
		m_systemManager.GetObjectByName("s")->SetPhysicsBody(100000000000, false, Micro::Physics::Material{ 0.6f, 0.3f, 0.5f , Micro::Physics::MINIMUM, Micro::Physics::AVERAGE });
		
		
		
	}

	App(const sf::Vector2f windowSize, float maxFPS, const char* scene) : Micro::Application(windowSize, maxFPS, "new game") {
		m_systemManager.GetFileManager().AddButtonFunc("testing1", testing1);
		m_systemManager.LoadScene("DefualtScene");

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

			m_systemManager.Update();

			Display();
		}
	}

private:
	ls::LightId m_lightId;  

};

Micro::Application* Micro::CreateApplication(const char* scene) {
	return new App(1920, 1080, 165, scene);
}