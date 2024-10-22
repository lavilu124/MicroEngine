#pragma once
#include "../Core.h"
#include "GameLogic/SceneManger.h"
#include "LightSystem.h"
#include <vector>

namespace Micro {
	class MICRO_API SystemManager
	{
	public:
		SystemManager(sf::RenderWindow& window);

		void Update(Camera* cam);

		void Render(sf::RenderWindow& window);

		void CreateGameObject(GameObject& ob);

		void DestroyObject(std::string name);

		std::vector<GameObject>& GetObjects();

		bool CheckForCollision(sf::Sprite sprite, std::string name, Collision::collisionLayer layerToCollideWith = Collision::ALL, GameObject* collideInfo = nullptr);

		void RunInput(sf::Event event);

		void LoadScene(std::string scene);

		Camera& GetCamera();

		GameObject& GetObjectByName(const std::string& name);

		FileManager& GetFileManager();

		int AddLight(Light::LightType type, sf::Color color, float size, float angle = 0.0f);
		void RemoveLight(int id);
		Light::LightSource* getLight(int id);

		std::vector<Light::LightSource>& SystemManager::getLights();

	public:

		static sf::Clock clock;
		static sf::Time deltaTimeT;

		static float deltaTime;
	private:
		void Start();

		int CheckExistingObject(std::string name);

	private:

		SceneManger m_sceneManager;
		FileManager m_fileManager;

		int m_currentId = 0;

		int GetLightIndex(int id);
	};
}
