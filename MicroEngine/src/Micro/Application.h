#pragma once
#include "Core.h"
#include "SystemLogic/SystemManager.h"



namespace Micro {
	class MICRO_API Application
	{
	public:
		Application(float windowWidth, float windowHeight, float maxFPS);
		Application(sf::Vector2f windowSize, float maxFps);

		virtual ~Application();
		virtual void Run();

		virtual void Display();

		virtual void InputFunc();

	protected:
		sf::RenderWindow m_window;
		Micro::SystemManager m_systemManager;
		Micro::Camera* m_camera;
		Micro::Light::LightSystem m_lightSystem;
	};

	Application* CreateApplication();
}


