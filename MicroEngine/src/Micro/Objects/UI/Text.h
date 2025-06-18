#pragma once
#include <SFML/Graphics.hpp>
#include "../../Core.h"


namespace Micro {
	class SystemManager;

	class MICRO_API Text
	{

	public:
		Text(std::string name, SystemManager* systemManager, std::string font = "");

		sf::Text& GetBase();

		void Delete();

		bool IsShowen();
		void SetShowen(bool showen);

		std::string GetName() const;

		bool IsSceneObject() const;
		void SetIsSceneObject(bool val);
	private:
		sf::Text m_text;
		sf::Font m_font;

		std::string m_name;
		bool m_showen = true;

		SystemManager* m_systemManager;

		bool m_isSceneObject;
	};
}


