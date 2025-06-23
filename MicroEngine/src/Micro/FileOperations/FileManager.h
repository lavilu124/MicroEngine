#pragma once
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../SystemLogic/GameLogic/InputAction.h"
#include "../Objects/ObjectManager.h"
#include "../Core.h"

namespace Micro{
	#ifdef DEBUG
		#define MC_LOG(x) Micro::FileManager::Log(x)
	#else
		#define MC_LOG(x)
	#endif

	class MICRO_API FileManager
	{
	public:
		void LoadInput();
		void AddInputFunc(const std::string& name, void(*function)(Input::InputAction&));
		void AddButtonFunc(const std::string& name, void(*function)());
		void CreateInput(const std::string& name, Input::inputType type, Input::KeyType key, Input::inputPart part, const std::string& onInput = "", const std::string& offInput = "");

		void addInput(Input::InputAction& action);

		void SetPaths();

		std::vector<std::shared_ptr<GameObject>> GetObjects(const std::string& name, SystemManager* systemManger);

		sf::Sprite* GetSprite(const std::string& name);
		sf::Sound* GetSound(const std::string& name);
		std::map <std::string, Input::InputAction> inputs;

		static void Log(const std::string& msg);

		std::string GetFontPath(const std::string& name);

	private:
		void GetFilesInDir(const std::string& dir);
		
		void LoadAsset(const std::string& fileName);

		static void CreateLog();

	private:

		

		std::map <std::string, void(*)(Input::InputAction&)> m_functionMap;
		std::map <std::string, void(*)()> m_buttonFuncMap;
		std::map < std::string, sf::Sprite> m_sprites;
		std::map < std::string, sf::Sound> m_sounds;

		std::map <std::string, sf::Texture> m_textures;
		std::map <std::string, sf::SoundBuffer> m_buffers;

		static std::string s_mainPath;
		static int s_currentLog;
	};
}
