#pragma once
#include "Core.h"



namespace Micro {
	class MICRO_API Application
	{
	public:
		Application();
		virtual ~Application();
		virtual void Run();
	};

	Application* CreateApplication();
}


