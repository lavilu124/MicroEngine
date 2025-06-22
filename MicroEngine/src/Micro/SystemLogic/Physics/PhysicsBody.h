#pragma once
#include <functional>
#include <SFML/System/Vector2.hpp>

#include "Constants.hpp"


namespace Micro
{
	namespace Physics
	{
		class MICRO_API PhysicsBody
		{
		public:
			PhysicsBody(float mass, sf::Vector2f& position, std::function<void(sf::Vector2f newPos)> handlePositionChange);
			void update(float dt);
			void addForce(sf::Vector2f force);
			void addImpulse(sf::Vector2f impulse);
		public:
			bool hasGravitation;
		private:
			float m_mass;
			sf::Vector2f& m_position;
			sf::Vector2f m_velocity;
			sf::Vector2f m_acceleration;
			sf::Vector2f m_netForces;
			sf::Vector2f m_netImpulses;
			std::function<void(sf::Vector2f newPos)> m_handlePositionChange;
		};
	}
	
}


