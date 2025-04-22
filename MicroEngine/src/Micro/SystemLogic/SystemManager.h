#pragma once
#include "../Core.h"
#include "GameLogic/SceneManger.h"
//#include "Light/include/LightSystem/LightSystem.hpp"
#include "../../lightInclude.h"
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

		GameObject* GetObjectByName(const std::string& name);

		bool CheckForCollision(sf::Sprite sprite, std::string name, Collision::collisionLayer layerToCollideWith = Collision::ALL, GameObject* collideInfo = nullptr);

		void RunInput(sf::Event event);

		void LoadScene(std::string scene);

		Camera& GetCamera();

		FileManager& GetFileManager();

		sfu::LightId AddLight(sfu::lightType type, const std::string& name);


		void RemoveLight(sfu::LightId id);

		template <sfu::lightType T>
		auto GetLight(sfu::LightId Id);

		template <sfu::lightType T>
		auto GetLight(const std::string& name);

		void AddEdge(sf::Vector2f a, sf::Vector2f b);
		void RemoveEdge(sf::Vector2f a, sf::Vector2f b);

		void SetDarkness(int precent);

		/*
		
		ls::LightValue GetLight(int index);

		ls::LightSystem* GetLightSystem();*/

	public:

		static sf::Clock clock;
		static sf::Time deltaTimeT;

		static float deltaTime;
	private:
		void Start();

		int CheckExistingObject(std::string name);

	private:

		//ls::LightSystem m_lightSystem;

		SceneManger m_sceneManager;
		FileManager m_fileManager;

		LightingArea m_lighting;
		sf::Texture m_drakness;


		int m_currentLightId = 0;

		sf::Vector2f m_windowSize;
	};

	

	template <sfu::lightType T>
	auto SystemManager::GetLight(sfu::LightId Id) {

		for (int i = 0; i < m_sceneManager.lights.size(); i++)
		{
			if (m_sceneManager.lights[i]->getID() == Id.id)
			{
				if constexpr (T == sfu::lightType::radial)
					return static_cast<RadialLight*>(m_sceneManager.lights[i].get());
				if constexpr (T == sfu::lightType::directed)
					return static_cast<DirectedLight*>(m_sceneManager.lights[i].get());
			}
		}
	}

	template<sfu::lightType T>
	auto SystemManager::GetLight(const std::string& name)
	{
		for (int i = 0; i < m_sceneManager.lights.size(); i++)
		{
			if (m_sceneManager.lights[i]->getName() == name)
			{
				if constexpr (T == sfu::lightType::radial)
					return static_cast<RadialLight*>(m_sceneManager.lights[i].get());
				if constexpr (T == sfu::lightType::directed)
					return static_cast<DirectedLight*>(m_sceneManager.lights[i].get());
			}
		}
	}
	
}
