#include "Button.h"
#include "../..//SystemLogic//SystemManager.h"


namespace Micro {
	Button::Button(SystemManager* systemManager, const std::string& name, const std::string& img, const std::string& onClickImg, void (*onClick)())
		: m_name(name), m_systemManager(systemManager), m_onClick(onClick) {

		m_img = systemManager->GetFileManager().GetSprite(img);
		sf::Vector2u size = m_img->getTexture()->getSize();
		m_img->setOrigin(size.x / 2, size.y / 2);



		if (!onClickImg.empty()) {
			m_onClickImg = systemManager->GetFileManager().GetSprite(onClickImg);
			m_isOnClickSet = true;
			sf::Vector2u size2 = m_onClickImg->getTexture()->getSize();
			m_onClickImg->setOrigin(size2.x / 2, size2.y / 2);
		}
			
	}

	std::string Button::GetName() const
	{
		return m_name;
	}

	bool Button::IsPressed(sf::Vector2f mousePos)
	{
		if (m_img->getGlobalBounds().contains(mousePos)) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (!m_isClicked) m_onClick();
				m_isClicked = true;
				return true;
			}
		}


		m_isClicked = false;
		return false;
	}

	void Button::SetPosition(sf::Vector2f pos)
	{
		m_img->setPosition(pos);
		m_onClickImg->setPosition(pos);
	}

	sf::Vector2f Button::GetPosition() const
	{
		return m_img->getPosition();
	}

	void Button::SetScale(sf::Vector2f scale)
	{
		m_img->setScale(scale);
		m_onClickImg->setScale(scale);
	}

	sf::Vector2f Button::GetScale() const
	{
		return m_img->getScale();
	}

	void Button::SetRotation(float Rotation)
	{
		m_img->setRotation(Rotation);
		m_onClickImg->setRotation(Rotation);
	}

	float Button::GetRotation() const
	{
		return m_img->getRotation();
	}

	void Button::SetImg(std::string name)
	{
		m_img = m_systemManager->GetFileManager().GetSprite(name);
		m_img->setOrigin(m_img->getGlobalBounds().width / 2, m_img->getGlobalBounds().height / 2);
	}

	void Button::SetOnClickImg(std::string name)
	{
		m_onClickImg = m_systemManager->GetFileManager().GetSprite(name);
		m_onClickImg->setOrigin(m_onClickImg->getGlobalBounds().width / 2, m_onClickImg->getGlobalBounds().height / 2);
	}

	void Button::Delete()
	{
		m_systemManager->RemoveButton(m_name);
	}

	sf::Sprite Button::GetCurrentSprite()
	{
		if (!m_isClicked || !m_isOnClickSet) return *m_img;

		return *m_onClickImg;
	}

	bool Button::IsShown() const
	{
		return m_shown;
	}
	void Button::SetShown(bool showen)
	{
		m_shown = showen;
	}

	bool Button::IsSceneObject() const
	{
		return m_isSceneObject;
	}

	void Button::SetIsSceneObject(bool val)
	{
		m_isSceneObject = val;
	}
}