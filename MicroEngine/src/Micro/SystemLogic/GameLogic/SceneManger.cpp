#include "SceneManger.h"


namespace Micro{
	SceneManger::SceneManger(sf::RenderWindow& window, float width, float height) : camera(width, height) {
		camera.SetCam(window);
	}

	SceneManger::SceneManger(sf::RenderWindow& window, sf::Vector2f windowSize) : camera(windowSize) {
		camera.SetCam(window);
	}



	void SceneManger::LoadSceneFromFile(std::string Scene, SystemManager* systemManager, FileManager& fileManager) {
		objects.erase(std::remove_if(objects.begin(), objects.end(),
			[](const auto& obj) { return obj->IsSceneObject(); }), objects.end());

		lights.erase(std::remove_if(lights.begin(), lights.end(),
			[](const auto& light) { return light->IsSceneObject(); }), lights.end());

		texts.erase(std::remove_if(texts.begin(), texts.end(),
			[](const auto& text) { return text.IsSceneObject(); }), texts.end());

		buttons.erase(std::remove_if(buttons.begin(), buttons.end(),
			[](const auto& button) { return button.IsSceneObject(); }), buttons.end());


		objects = std::move(fileManager.GetObjects(Scene, systemManager));
	}

}
