#pragma once
#include "../Core.h"
#include "GameLogic/SceneManger.h"
#include "Light/include/LightSystem/LightSystem.hpp"
#include <vector>

namespace Micro{
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

		int AddLight(ls::lightType type, const sf::Vector2f& p, float r, const sf::Color& c, std::string name, float l = 0);
		void RemoveLight(int id);
		ls::LightValue getLight(int index);
		ls::LightValue getLight(std::string name);

		ls::LightSystem* getLightSystem();

	public:

		static sf::Clock clock;
		static sf::Time deltaTimeT;

		static float deltaTime;
	private:
		void Start();

		int CheckExistingObject(std::string name);

	private:

		ls::LightSystem m_lightSystem;

		SceneManger m_sceneManager;
		FileManager m_fileManager;

		int m_currentId = 0;
	};
}
