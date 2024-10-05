#pragma once
#include <iostream>

#ifdef MC_PLATFORM_WINDOWS

extern Micro::Application* Micro::CreateApplication();

int main() {
	auto app = Micro::CreateApplication();

	if (!app) {
		return -1;
	}

	app->Run();

	delete app;

	std::cin.get();

	return 0;
}


#endif