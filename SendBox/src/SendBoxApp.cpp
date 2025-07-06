#include "Objects/Player.h"
#include <LightInclude.h>
#include <Micro/EnteryPoint.h>

#include <iostream>

//void testing1() {
//	std::cout << "testing123";
//}

//void test(Micro::Input::InputAction& in) {
//
//	auto obj = Micro::Application::Get().GetSystemManager()->GetObjectByName("Object 1");
//	auto pos = obj->GetPosition();
//	obj->SetPosition({pos.x, pos.y + 100});
//}

class App : public Micro::Application
{
public:
	
	
	App(const float windowWidth, const float windowHeight, const float maxFPS, const char* scene)
	: Application(windowWidth, windowHeight, maxFPS, "new game") {
		m_systemManager.LoadScene("testingAng");



		//m_systemManager.GetFileManager().AddInputFunc("test", test);

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