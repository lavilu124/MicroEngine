#include "Objects/Player.h"
#include <LightInclude.h>
#include <Micro/EnteryPoint.h>


class App : public Micro::Application
{
public:
	
	
	App(const float windowWidth, const float windowHeight, const float maxFPS, const char* scene)
	: Application(windowWidth, windowHeight, maxFPS, "new game") {
		m_systemManager.LoadScene("DefualtScene");
		
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
	return new App(1920, 1080, 144, scene);
}