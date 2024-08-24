#pragma once
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../SystemLogic/GameLogic/InputAction.h"
#include "..//Objects/GameObject.h"
#include "../Core.h"
namespace Micro {
	class MICRO_API FileManager
	{
	public:
		void LoadInput();
		void AddInputFunc(std::string name, void(*function)());

		void SetPaths();

		std::vector<GameObject> GetObjects(std::string name, SystemManager* SystemManger);

		sf::Sprite* GetSprite(std::string name);
		sf::Sound* GetSound(std::string name);
		std::map <std::string, Input::InputAction> inputs;

	private:
		void GetFilesInDir(std::string Dir);
		void CreateInput(std::string Name, Input::inputType Type, Input::KeyType Key, Input::inputPart Part, std::string OnInput, std::string OffInput);

		void CreateInput(std::string Name, Input::inputType Type, Input::KeyType Key, Input::inputPart Part, std::string OnInput);
		void LoadAsset(std::string Path, std::string FileName);

	private:
		std::map <std::string, void(*)()> m_functionMap;
		std::map < std::string, sf::Sprite> m_sprites;
		std::map < std::string, sf::Sound> m_sounds;
	};
}
