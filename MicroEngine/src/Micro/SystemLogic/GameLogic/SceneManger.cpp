#include "SceneManger.h"


namespace Micro{
	SceneManger::SceneManger(sf::RenderWindow& window, float width, float height) : camera(width, height) {
		camera.SetCam(window);
	}

	SceneManger::SceneManger(sf::RenderWindow& window, sf::Vector2f windowSize) : camera(windowSize) {
		camera.SetCam(window);
	}



	void SceneManger::LoadSceneFromFile(std::string Scene, SystemManager* systemManager, FileManager& fileManager) {
		objects.clear();
		lights.clear();
		objects = std::move(fileManager.GetObjects(Scene, systemManager));
	}

}
