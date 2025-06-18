 #pragma once
#include <SFML/Graphics.hpp>
#include "../SystemLogic/GameLogic/Collision.hpp"
#include "../Core.h"

namespace Micro{
	class SystemManager;

	class MICRO_API GameObject
	{
	public:
		GameObject(SystemManager* systemManager, const sf::Sprite& ObjectSprite, const std::string& name, Collision::collisionLayer Layer = Collision::collisionLayer::COLLIDER, int level = 0);

		bool IsFacingRight();

		void Flip();

		void Delete();

		sf::Vector2f GetPosition() const;
		void SetPosition(sf::Vector2f NewPosition);

		sf::Vector2f GetScale() const;
		void SetScale(sf::Vector2f NewScale);

		float GetRotation() const;
		void SetRotation(float Rotation);

		const sf::Sprite& GetSprite() const;
		void SetSprite(const sf::Sprite& Sprite);

		Collision::collisionLayer GetLayer() const;

		virtual void Start();
		virtual void Update(float DeltaTime);

		std::string GetName() const;

		int GetLevel() const;

		void SetLevel(int level);

		bool IsShowen() const;

		void SetShowen(bool showen);

		bool IsSceneObject() const;
		void SetIsSceneObject(bool val);

	private:
		void SetCenter();

		void HandlePositionChange(sf::Vector2f NewPosition);
		void HandleRotationChange(float NewRotation);
		void HandleScaleChange(sf::Vector2f NewScale);

	private:
		Collision::collisionLayer m_layer;
		sf::Vector2f m_position;
		sf::Vector2f m_scale;
		float m_rotation;

		sf::Sprite m_objectSprite;

		virtual void OnCollision(GameObject* HitInfo);
		virtual void OnTrigger(GameObject* HitInfo);

		std::string m_name;

		SystemManager* m_systemManager;

		int m_level = 0;

		bool m_showen = true;

		bool m_isSceneObject;
	};
}

