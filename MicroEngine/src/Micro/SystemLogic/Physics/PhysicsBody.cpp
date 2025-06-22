#include "PhysicsBody.h"

namespace Micro
{
	namespace Physics
	{
		PhysicsBody::PhysicsBody(float mass, sf::Vector2f& position,
			std::function<void(sf::Vector2f newPos)> handlePositionChange)
			: m_position(position), m_mass(mass), m_handlePositionChange(handlePositionChange), hasGravitation(true)
		{
		}

		void PhysicsBody::update(float dt)
		{
			if (hasGravitation)
				addForce(sf::Vector2f(0,  GRAVITY_ACC * m_mass));
			m_acceleration = m_netForces / m_mass;
			m_handlePositionChange(m_position + m_velocity * dt + sf::Vector2f{ m_acceleration.x * dt * dt * 0.5f, m_acceleration.y * dt * dt * 0.5f });
			m_velocity += m_acceleration * dt;
			m_velocity += m_netImpulses / m_mass;

			m_netForces = { 0,0 };
			m_netImpulses = { 0,0 };
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

