#pragma once
#include "Core.h"
#include "SystemLogic/SystemManager.h"



namespace Micro{
	class MICRO_API Application
	{
	public:
		Application(float windowWidth, float windowHeight, float maxFPS, const char* name, std::string scene = "");
		Application(sf::Vector2f windowSize, float maxFps, const char* name, std::string scene = "");

		virtual ~Application();
		virtual void Run();

		virtual void Display();

		virtual void InputFunc();


		void SetWindowSize(sf::Vector2u newSize);
		sf::Vector2u GetWindowSize();

	protected:
		sf::RenderWindow m_window;
		Micro::SystemManager m_systemManager;
		Micro::Camera* m_camera;
	};

	Application* CreateApplication(std::string scene);
}


