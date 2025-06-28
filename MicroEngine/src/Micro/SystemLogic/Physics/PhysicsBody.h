#pragma once
#include <functional>
#include <SFML/System/Vector2.hpp>

#include "Constants.hpp"


namespace Micro
{
	namespace Physics
	{
		enum CalculationType
		{
			AVERAGE,
			MINIMUM,
			MULTIPLY,
			MAXIMUM
		};
		struct MICRO_API Material
		{
			float staticFriction;
			float kineticFriction;
			float bounciness;
			CalculationType bouncinessCalculation;
			CalculationType frictionCalculation;
		};

		class MICRO_API PhysicsBody final
		{
		public:
			PhysicsBody(float mass, sf::Vector2f& position, std::function<void(sf::Vector2f newPos)> handlePositionChange, const Material& material);
			void update(float dt);
			void onCollision(PhysicsBody& other, float collisionAngle);
			void addForce(sf::Vector2f force);
			void addImpulse(sf::Vector2f impulse);
		public:
			bool hasGravitation;
			bool isMoveable;
		private:
			float m_mass;
			Material m_material;
			sf::Vector2f& m_position;
			sf::Vector2f m_velocity;
			sf::Vector2f m_acceleration;
			sf::Vector2f m_netForces;
			sf::Vector2f m_netImpulses;

			std::function<void(sf::Vector2f newPos)> m_handlePositionChange;

			bool m_collisionBeenCalled;
		};

		
	}
	
}


