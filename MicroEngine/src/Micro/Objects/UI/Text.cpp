#include "Text.h"
#include "../..//SystemLogic//SystemManager.h"

namespace Micro {
	Text::Text(const std::string& name, SystemManager* systemManager, const std::string& font) : m_name(name), m_systemManager(systemManager)
	{

		if (font != "") {
			if (!m_font.loadFromFile(systemManager->GetFileManager().GetFontPath(font))) {
				
			}
		}
		m_text.setFont(m_font);
		m_text.setOrigin(m_text.getGlobalBounds().width / 2, m_text.getGlobalBounds().height / 2);
	}

	sf::Text& Text::GetBase()
	{
		return m_text;
	}

	void Text::Delete()
	{
		m_systemManager->RemoveText(m_name);
	}

	bool Text::IsShown()
	{
		m_text.setOrigin(m_text.getGlobalBounds().width / 2, m_text.getGlobalBounds().height / 2);
		return m_shown;
	}
	void Text::SetShown(bool shown)
	{
		m_shown = shown;
	}
	std::string Text::GetName() const
	{
		return m_name;
	}

	bool Text::IsSceneObject() const
	{
		return m_isSceneObject;
	}

	void Text::SetIsSceneObject(bool val)
	{
		m_isSceneObject = val;
	}
}