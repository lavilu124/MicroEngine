#pragma once
#include "../../Objects/Camera.h"
#include "..//..//FileOperations/FileManager.h"


class MICRO_API SceneManger
{
public:
	SceneManger(sf::RenderWindow& window, float width, float height);
	SceneManger(sf::RenderWindow& window, sf::Vector2f windowSize);

	void LoadSceneFromFile(std::string Scene, SystemManager* systemManager, FileManager& fileManager);


public:
	Camera camera;
	std::vector<GameObject> objects;

private:
	std::string name;
};