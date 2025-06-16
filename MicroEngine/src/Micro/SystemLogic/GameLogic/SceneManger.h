#pragma once
#include "../../Objects/Camera.h"
#include "../../FileOperations/FileManager.h"
#include "../../../lightInclude.h"
#include "../../Objects/UI/Text.h"



namespace Micro{
	class MICRO_API SceneManger
	{
	public:
		SceneManger(sf::RenderWindow& window, float width, float height);
		SceneManger(sf::RenderWindow& window, sf::Vector2f windowSize);

		void LoadSceneFromFile(std::string Scene, SystemManager* systemManager, FileManager& fileManager);

	public:
		Camera camera;
		std::vector<std::shared_ptr<Micro::GameObject>> objects;
		std::vector<std::shared_ptr<LightSource>> lights;
		std::vector<Text> texts;
		EdgeVector edges;
	};
}