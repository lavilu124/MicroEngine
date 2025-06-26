#pragma once
#include "../../Objects/Camera.h"
#include "../../FileOperations/FileManager.h"
#include "../../../lightInclude.h"
#include "../../Objects/UI/Text.h"
#include "../../Objects/UI/Button.h"



namespace Micro{
	class MICRO_API SceneManger
	{
	public:
		SceneManger(sf::RenderWindow& window, float width, float height);
		SceneManger(sf::RenderWindow& window, sf::Vector2f windowSize);

		void LoadSceneFromFile(std::string Scene, SystemManager* systemManager, FileManager& fileManager);

	public:
		Camera camera;

		std::vector<std::shared_ptr<GameObject>> objects;

		std::vector<std::shared_ptr<LightSource>> lights;
		EdgeVector edges;

		std::vector<Text> texts;
		std::vector<Button> buttons;

		
	};
}