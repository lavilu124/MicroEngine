#pragma once


#ifdef MC_PLATFORM_WINDOWS

extern Micro::Application* Micro::CreateApplication(std::string scene);

int main() {

	auto app = Micro::CreateApplication("");

	if (!app) {
		return -1;
	}

	app->Run();

	delete app;


	return 0;
}


#endif