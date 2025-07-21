#pragma once
#include <optional>

#include "Core.h"
#include "SystemLogic/SystemManager.h"



namespace Micro{
	class MICRO_API Application
	{
	public:
		Application(float windowWidth, float windowHeight, float maxFPS, const char* name, const std::string& scene = "");
		Application(const sf::Vector2f& windowSize, float maxFps, const char* name, const std::string& scene = "");


		static Application& Get();
		virtual void Run();

		virtual void Display();

		virtual void InputFunc();

		SystemManager* GetSystemManager();


		void SetWindowSize(const sf::Vector2u& newSize);
		sf::Vector2u GetWindowSize() const;
		void SetWindowTitle(const char* name);


		void Close();


	protected:
		std::optional<sf::RenderWindow> m_window;
		std::optional<SystemManager> m_systemManager;
		Camera* m_camera;

		bool m_inGUiMode = false;
	};

	Application* CreateApplication(const char* scene);
}


