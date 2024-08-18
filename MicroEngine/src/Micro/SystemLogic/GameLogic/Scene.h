#pragma once
#include "../../Objects/Camera.h"
#include "..//..//FileOperations/FileManager.h"


class MICRO_API Scene
{
public:
	Scene(sf::RenderWindow& window, float width, float height);
	Scene(sf::RenderWindow& window, sf::Vector2f windowSize);

	std::vector<GameObject>& GetObjects();

	int CreateObject(GameObject object);

	void DestroyObject(int index);
	void DestroyObject(std::string name);
	int CheckExistingObject(std::string name);

	void LoadSceneFromFile(std::string Scene, SystemManager* systemManager);

public:
	Camera camera;

private:
	std::vector<GameObject> m_objects;
	std::string name;
};