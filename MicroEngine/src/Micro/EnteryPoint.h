#pragma once

#include "FileOperations/FileManager.h"

#ifdef MC_PLATFORM_WINDOWS

extern Micro::Application* Micro::CreateApplication(const char* scene);

int main(int argc, char** argv) {
	std::string scene = "";
	if (argc > 1)
	{
		scene = argv[1];
	}

	auto app = Micro::CreateApplication(scene.c_str());

	if (!app) {
		return -1;
	}

	app->Run();

	delete app;


	return 0;
}


#endif