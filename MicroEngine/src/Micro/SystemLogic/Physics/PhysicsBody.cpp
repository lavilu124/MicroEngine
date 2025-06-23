#include "PhysicsBody.h"
#include "../../Objects/GameObject.h"
#include <stdexcept>

namespace Micro
{
	namespace Physics
	{
		PhysicsBody::PhysicsBody(float mass, sf::Vector2f& position,
			std::function<void(sf::Vector2f newPos)> handlePositionChange)
			: m_position(position), m_mass(mass), m_handlePositionChange(handlePositionChange), hasGravitation(true),
			m_velocity(0, 0), m_acceleration(0, 0), m_netForces(0, 0), m_netImpulses(0, 0)
		{
			if (mass <= 0)
				m_mass = 1; // Default mass to 1 if invalid
			
		}
		
		void PhysicsBody::update(float dt)
		{
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

		void PhysicsBody::onCollision(const PhysicsBody& other)
		{
			sf::Vector2f v1 = m_velocity;
			sf::Vector2f v2 = other.m_velocity;

			float m1 = m_mass;
			float m2 = other.m_mass;
			// todo yw
			float n = 1;// ((v1 * (m1 - m2) + 2 * m2 * v2) / (v1 * m1 * 2.0f + v2 * (m2 - m1)));

			sf::Vector2f u1 = (n * (m1 * v1 + m2 * v2)) / (m1 * n + m2);
			sf::Vector2f u2 = (m1 * v1 + m2 * v2) / (m1 * n + m2);


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

