#include "Text.h"
#include "../..//SystemLogic//SystemManager.h"

Text::Text(std::string name, SystemManager* systemManager, std::string font = "") : m_name(name), m_systemManager(systemManager)
{
	if (font == "") {
		m_text = sf::Text();
		return;
	}
	
}

sf::Text& Text::GetBase()
{
	return m_text;
}

void Text::Delete()
{
}
