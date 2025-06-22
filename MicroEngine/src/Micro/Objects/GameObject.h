 #pragma once
#include <optional>
#include <SFML/Graphics.hpp>
#include "../SystemLogic/GameLogic/Collision.hpp"
#include "../Core.h"
#include "../SystemLogic/Physics/PhysicsBody.h"

 namespace Micro{
	class SystemManager;

	class MICRO_API GameObject
	{
	public:
		GameObject(SystemManager* systemManager, const sf::Sprite& ObjectSprite, const std::string& name, Collision::collisionLayer Layer = Collision::collisionLayer::COLLIDER, int level = 0);

		bool IsFacingRight() const;

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

		void SetPhysicsBody(float mass, bool hasGravity);

	private:
		void SetCenter();

		void HandlePositionChange(sf::Vector2f NewPosition);
		void HandleRotationChange(float NewRotation);
		void HandleScaleChange(sf::Vector2f NewScale);

		std::string m_name;
		sf::Vector2f m_position;
		sf::Vector2f m_scale;
		float m_rotation;
		sf::Sprite m_objectSprite;

	protected:
		virtual void OnCollision(GameObject* HitInfo);
		virtual void OnTrigger(GameObject* HitInfo);

		std::optional<Physics::PhysicsBody> m_physicsBody;

		Collision::collisionLayer m_layer;


		SystemManager* m_systemManager;

		int m_level = 0;

		bool m_showen = true;

		bool m_isSceneObject;
	};
}

