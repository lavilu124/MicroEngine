#pragma once
#include <SFML/Graphics.hpp>

class Text 
{
	class SystemManager;
public:
	Text(std::string name, SystemManager* systemManager, std::string font = "");

	sf::Text& GetBase();

	void Delete();
private:
	sf::Text m_text;

	std::string m_name;
	bool IsShowen = true;

	SystemManager* m_systemManager;
};

