#include "SceneManger.h"

SceneManger::SceneManger(sf::RenderWindow& window, float width, float height) : camera(width, height){
	camera.SetCam(window);
}

SceneManger::SceneManger(sf::RenderWindow& window, sf::Vector2f windowSize) :  camera(windowSize) {
	camera.SetCam(window);
}

void SceneManger::LoadSceneFromFile(std::string Scene, SystemManager* systemManager, FileManager& fileManager) {
	objects.clear();
	objects = fileManager.GetObjects(Scene, systemManager);
}