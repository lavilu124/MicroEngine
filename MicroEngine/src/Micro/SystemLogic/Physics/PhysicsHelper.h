#pragma once
#include <functional>
#include <SFML/System/Vector2.hpp>
#include "Micro/Core.h"
#include "Micro/SystemLogic/Physics/PhysicsBody.h"


namespace Micro
{
	namespace Physics
	{
		struct Material;

		class MICRO_API PhysicsHelper
		{
		public:
			static float magnitude(sf::Vector2f vector);
			static float dot(const sf::Vector2f& a, const sf::Vector2f& b);
			static sf::Vector2f normalize(const sf::Vector2f& vector);
			static float getBounciness(const Material& material1, const Material& material2);
		};
	}

}


