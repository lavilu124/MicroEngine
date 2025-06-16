#include "Text.h"
#include "../..//SystemLogic//SystemManager.h"

namespace Micro {
	Text::Text(std::string name, SystemManager* systemManager, std::string font) : m_name(name), m_systemManager(systemManager)
	{

		if (font != "") {
			if (!m_font.loadFromFile(systemManager->GetFileManager().GetFontPath(font))) {
				
			}
		}
		m_text.setFont(m_font);
	}

	sf::Text& Text::GetBase()
	{
		return m_text;
	}

	void Text::Delete()
	{
		m_systemManager->RemoveText(m_name);
	}

	bool Text::IsShowen() const
	{
		return m_showen;
	}
	void Text::SetShowen(bool showen)
	{
		m_showen = showen;
	}
	std::string Text::GetName() const
	{
		return m_name;
	}
}