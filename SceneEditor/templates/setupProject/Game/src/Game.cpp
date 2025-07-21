#include <Micro.h>
#include <LightInclude.h>

extern "C" __declspec(dllexport) Micro::Application * CreateApplication(const std::string& scene) {
	return new Micro::Application(1920, 1080, 144, "new game", scene);
}