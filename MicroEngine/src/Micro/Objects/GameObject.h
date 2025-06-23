 #pragma once
#include <SFML/Graphics.hpp>
#include "../SystemLogic/GameLogic/Collision.hpp"
#include "../Core.h"

namespace Micro{
	class SystemManager;

	class MICRO_API GameObject
	{
	public:
		GameObject(SystemManager* systemManager, const sf::Sprite& objectSprite, const std::string& name, Collision::collisionLayer layer = Collision::collisionLayer::COLLIDER, int level = 0);

		bool IsFacingRight() const;

		void Flip();

		void Delete();

		sf::Vector2f GetPosition() const;
		void SetPosition(const sf::Vector2f& newPosition);

		sf::Vector2f GetScale() const;
		void SetScale(const sf::Vector2f& newScale);

		float GetRotation() const;
		void SetRotation(float rotation);

		const sf::Sprite& GetSprite() const;
		void SetSprite(const sf::Sprite& sprite);

		Collision::collisionLayer GetLayer() const;

		virtual void Start();
		virtual void Update(float deltaTime);

		std::string GetName() const;

		int GetLevel() const;

		void SetLevel(int level);

		bool IsShown() const;

		void SetShown(bool shown);

		bool IsSceneObject() const;
		void SetIsSceneObject(bool val);

	private:
		void SetCenter();

		void HandlePositionChange(const sf::Vector2f& newPosition);
		void HandleRotationChange(float newRotation);
		void HandleScaleChange(const sf::Vector2f& newScale);

		std::string m_name;
		sf::Vector2f m_position;
		sf::Vector2f m_scale;
		float m_rotation;
		sf::Sprite m_objectSprite;

	protected:
		virtual void OnCollision(GameObject* hitInfo);
		virtual void OnTrigger(GameObject* hitInfo);

		Collision::collisionLayer m_layer;


		SystemManager* m_systemManager;

		int m_level = 0;

		bool m_shown = true;

		bool m_isSceneObject;
	};
}

