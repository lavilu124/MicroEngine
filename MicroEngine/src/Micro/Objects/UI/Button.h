#pragma once
#include <SFML/Graphics.hpp>
#include "../../Core.h"

namespace Micro {
	class SystemManager;

	class MICRO_API Button
	{
	public:
		Button(SystemManager* systemManager, const std::string& name, const std::string& img, const std::string& onClickImg = "", void (*onClick)() = nullptr);

		std::string GetName() const;

		bool IsPressed(sf::Vector2f mousePos);

		void SetPosition(sf::Vector2f pos);
		sf::Vector2f GetPosition() const;

		void SetScale(sf::Vector2f scale);
		sf::Vector2f GetScale() const;

		void SetRotation(float Rotation);
		float GetRotation() const;

		void SetImg(std::string name);
		void SetOnClickImg(std::string name);

		void Delete();


		sf::Sprite GetCurrentSprite();

		bool IsShown() const;
		void SetShown(bool shown);

		bool IsSceneObject() const;
		void SetIsSceneObject(bool val);

	private:
		std::string m_name;
		sf::Sprite* m_img;
		sf::Sprite* m_onClickImg;
		bool m_isOnClickSet = false;
		void (*m_onClick)();

		SystemManager* m_systemManager;
		
		bool m_isClicked;
		
		bool m_shown = true;

		bool m_isSceneObject;
	};
}

