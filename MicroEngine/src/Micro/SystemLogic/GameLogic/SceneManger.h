#pragma once
#include "../../Objects/Camera.h"
#include "../../FileOperations/FileManager.h"
#include "../Light/include/LightSystem/LightSystem.hpp"


namespace Micro{
	class MICRO_API SceneManger
	{
	public:
		SceneManger(sf::RenderWindow& window, float width, float height);
		SceneManger(sf::RenderWindow& window, sf::Vector2f windowSize);

		void LoadSceneFromFile(std::string Scene, SystemManager* systemManager, FileManager& fileManager);


	public:
		Camera camera;
		std::vector<GameObject> objects;
		std::vector<Micro::ls::Light*> lights;

	private:
		std::string name;
	};
}