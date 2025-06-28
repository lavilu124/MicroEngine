#include "PhysicsBody.h"
#include "../../Objects/GameObject.h"
#include <stdexcept>

#include "PhysicsHelper.h"
#include "Micro/FileOperations/FileManager.h"

namespace Micro
{
	namespace Physics
	{
		PhysicsBody::PhysicsBody(float mass, sf::Vector2f& position, std::function<void(sf::Vector2f newPos)> handlePositionChange, const Material& material)
			: m_position(position), m_mass(mass), m_handlePositionChange(handlePositionChange), hasGravitation(true),
			m_velocity(0, 0), m_acceleration(0, 0), m_netForces(0, 0), m_netImpulses(0, 0),
			isMoveable(true), m_collisionBeenCalled(false), m_material(material)
		{
			if (mass <= 0)
				m_mass = 1; // Default mass to 1 if invalid
			
		}
		
		void PhysicsBody::update(float dt)
		{
			m_collisionBeenCalled = false;
			if (!isMoveable)
				return;
			if (hasGravitation)
				addForce(sf::Vector2f(0,  GRAVITY_ACC * m_mass));
			m_acceleration = m_netForces / m_mass;
			m_handlePositionChange(m_position + m_velocity * dt + sf::Vector2f{ m_acceleration.x * dt * dt * 0.5f, m_acceleration.y * dt * dt * 0.5f });
			m_velocity += m_acceleration * dt;
			m_velocity += m_netImpulses / m_mass;

			m_netForces = { 0,0 };
			m_netImpulses = { 0,0 };
		}

		void PhysicsBody::onCollision(PhysicsBody& other, float collisionAngle)
		{
			if (m_collisionBeenCalled || other.m_collisionBeenCalled)
				return;
			m_collisionBeenCalled = true;
			float restitution = PhysicsHelper::getBounciness(m_material, other.m_material); // 1.0: אלסטית, 0.0: פלסטית, ערכים באמצע: חצי-אלסטית וכו'

			sf::Vector2f v1 = m_velocity;
			sf::Vector2f v2 = other.m_velocity;

			float m1 = m_mass;
			float m2 = other.m_mass;

			sf::Vector2f normal = PhysicsHelper::normalize(other.m_position - m_position);
			sf::Vector2f relativeVelocity = v2 - v1;

			float velAlongNormal = PhysicsHelper::dot(relativeVelocity, normal);

			if (velAlongNormal > 0)
				return;

			float e = restitution;
			float j = -(1 + e) * velAlongNormal;
			j /= (1 / m1) + (1 / m2);
			//MC_LOG(std::to_string(collisionAngle));
			sf::Vector2f impulse = j * normal;
			addImpulse(-impulse);
			other.addImpulse(impulse);
			
		}

		void PhysicsBody::addForce(sf::Vector2f force)
		{
			m_netForces += force;
		}

		void PhysicsBody::addImpulse(sf::Vector2f impulse)
		{
			m_netImpulses += impulse;
		}
	}
}

