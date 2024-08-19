#pragma once
#include "../Core.h"
#include "GameLogic/SceneManger.h"
#include "LightSystem.h"
#include <vector>

class MICRO_API SystemManager
{
public:
	SystemManager(sf::RenderWindow& window);

	void Update(Camera* cam);

	void Render(sf::RenderWindow& window);

	void CreateGameObject(GameObject& ob);

	void DestroyObject(std::string name);
	void DestroyObject(int index);

	bool CheckForCollision(sf::Sprite sprite, std::string name, Collision::collisionLayer layerToCollideWith = Collision::ALL, GameObject* collideInfo = nullptr);

	void RunInput(sf::Event event);

	void LoadScene(std::string scene);

	 Camera& GetCamera(); 

	 int GetObjectByName(const std::string& name);

public:

	static sf::Clock clock;
	static sf::Time deltaTimeT;

	static float deltaTime;
private:
	void Start();

	SystemManager(const SystemManager&) = delete;
	void operator=(const SystemManager&) = delete;

	int CheckExistingObject(std::string name);

private:
	SceneManger m_sceneManager;
	FileManager m_fileManager;
};


