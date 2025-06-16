#pragma once
#include "../Core.h"
#include "GameLogic/SceneManger.h"
#include "../../lightInclude.h"
#include <vector>

namespace Micro{
	

	class MICRO_API SystemManager
	{
	public:
		SystemManager(sf::RenderWindow& window);

		void Update(Camera* cam);

		void Render(sf::RenderWindow& window);

		void CreateGameObject(std::shared_ptr<GameObject> ob);

		void DestroyObject(const char* name);

		std::vector<std::shared_ptr<GameObject>>& GetObjects();

		GameObject* GetObjectByName(const char* name);

		bool CheckForCollision(sf::Sprite sprite, const char* name, GameObject*& collideInfo , Collision::collisionLayer layerToCollideWith = Collision::ALL);

		void RunInput(sf::Event event);

		void AddInput(Micro::Input::InputAction& action);

		void LoadScene(const char* scene);

		Camera& GetCamera();

		FileManager& GetFileManager();

		ls::LightId AddLight(ls::lightType type, const char* name);


		void RemoveLight(ls::LightId id);

		template <ls::lightType T>
		auto GetLight(ls::LightId Id);

		template <ls::lightType T>
		auto GetLight(const char* name);

		void AddEdge(sf::Vector2f a, sf::Vector2f b);
		void RemoveEdge(sf::Vector2f a, sf::Vector2f b);

		void SetDarkness(int precent);

		void ChangedLevel();

		void AddText(const std::string& name, std::string font = "");
		void RemoveText(const std::string& name);
		Text* GetText(const std::string& name);

	public:

		static sf::Clock clock;
		static sf::Time deltaTimeT;

		static float deltaTime;
	private:
		void Start();

		int CheckExistingObject(const char* name);

	private:

		SceneManger m_sceneManager;
		FileManager m_fileManager;

		LightingArea m_lighting;
		sf::Texture m_drakness;


		int m_currentLightId = 0;

		sf::Vector2f m_windowSize;
	};

	

	template <ls::lightType T>
	auto SystemManager::GetLight(ls::LightId Id) {

		for (int i = 0; i < m_sceneManager.lights.size(); i++)
		{
			if (m_sceneManager.lights[i]->GetID() == Id.id)
			{
				if constexpr (T == ls::lightType::radial)
					return static_cast<RadialLight*>(m_sceneManager.lights[i].get());
				if constexpr (T == ls::lightType::directed)
					return static_cast<DirectedLight*>(m_sceneManager.lights[i].get());
			}
		}
	}

	template<ls::lightType T>
	auto SystemManager::GetLight(const char* name)
	{
		for (int i = 0; i < m_sceneManager.lights.size(); i++)
		{
			if (m_sceneManager.lights[i]->getName() == name)
			{
				if constexpr (T == ls::lightType::radial)
					return static_cast<RadialLight*>(m_sceneManager.lights[i].get());
				if constexpr (T == ls::lightType::directed)
					return static_cast<DirectedLight*>(m_sceneManager.lights[i].get());
			}
		}
	}
	
}
