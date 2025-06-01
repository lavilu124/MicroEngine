#include <Micro.h>
#include <LightInclude.h>

class App : public Micro::Application
{
public:
	App(const float windowWidth, const float windowHeight, const float maxFPS, const char* scene) : Micro::Application(windowWidth, windowHeight, maxFPS, "new game", scene) {
	}

	App(const sf::Vector2f windowSize, float maxFPS, const char* scene) : Micro::Application(windowSize, maxFPS, "new game", scene) {
	}

	void Run() override {
		while (m_window.isOpen()) {
			InputFunc();

			m_systemManager.Update(m_camera);

			Display();
		}
	}

};

Micro::Application* Micro::CreateApplication(const char* scene) {
	return new App(1920, 1080, 144, scene);
}