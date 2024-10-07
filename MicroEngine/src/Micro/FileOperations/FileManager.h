#pragma once
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../SystemLogic/GameLogic/InputAction.h"
#include "..//Objects/GameObject.h"
#include "../Core.h"

namespace Micro {
	#ifdef DEBUG
		#define MC_LOG(x) Micro::FileManager::Log(x)
	#else
		#define MC_LOG(x)
	#endif

	class MICRO_API FileManager
	{
	public:
		void LoadInput();
		void AddInputFunc(std::string name, void(*function)());

		void SetPaths();

		std::string GetShaderPath(std::string& shadername);

		std::vector<GameObject> GetObjects(std::string name, SystemManager* systemManger);

		sf::Sprite* GetSprite(std::string name);
		sf::Sound* GetSound(std::string name);
		std::map <std::string, Input::InputAction> inputs;

		static void Log(std::string msg);
		static void CreateLog();

	private:
		void GetFilesInDir(std::string Dir);
		void CreateInput(std::string Name, Input::inputType Type, Input::KeyType Key, Input::inputPart Part, std::string OnInput, std::string OffInput);

		void CreateInput(std::string Name, Input::inputType Type, Input::KeyType Key, Input::inputPart Part, std::string OnInput);
		void LoadAsset(std::string FileName);

	private:
		std::map <std::string, void(*)()> m_functionMap;
		std::map < std::string, sf::Sprite> m_sprites;
		std::map < std::string, sf::Sound> m_sounds;

		std::map <std::string, sf::Texture> m_textures;
		std::map <std::string, sf::SoundBuffer> m_buffers;

		static std::string m_mainPath;
		static int currentLog;
	};
}
