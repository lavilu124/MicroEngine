#pragma once
#include "Core.h"
#include "SystemLogic/SystemManager.h"



namespace Micro{
	class MICRO_API Application
	{
	public:
		Application(float windowWidth, float windowHeight, float maxFPS, const char* name);
		Application(const sf::Vector2f& windowSize, float maxFps, const char* name);


		static Application& Get();
		virtual void Run();

		virtual void Display();

		virtual void InputFunc();

		SystemManager* GetSystemManager();


		void SetWindowSize(const sf::Vector2u& newSize);
		sf::Vector2u GetWindowSize() const;
		void SetWindowTitle(const char* name);
		void SetResolution(const sf::Vector2f& newRes);

	protected:
		sf::RenderWindow m_window;
		Micro::SystemManager m_systemManager;
		Micro::Camera* m_camera;
	};

	Application* CreateApplication(const char* scene);
}


