#include "PhysicsHelper.h"

float Micro::Physics::PhysicsHelper::magnitude(sf::Vector2f vector)
{
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

float Micro::Physics::PhysicsHelper::dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

sf::Vector2f Micro::Physics::PhysicsHelper::normalize(const sf::Vector2f& vector)
{
	float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	if (magnitude == 0)
		return { 0.f, 0.f };
	return vector / magnitude;
}

float Micro::Physics::PhysicsHelper::getBounciness(const Material& material1, const Material& material2)
{
	if (material1.bouncinessCalculation == material2.bouncinessCalculation)
	{
		switch (material1.bouncinessCalculation)
		{
		case AVERAGE:
			return (material1.bounciness + material2.bounciness) / 2.0f;
		case MINIMUM:
			return std::min(material1.bounciness, material2.bounciness);
		case MULTIPLY:
			return material1.bounciness * material2.bounciness;
		case MAXIMUM:
			return std::max(material1.bounciness, material2.bounciness);
		}
	}
	if (material1.bouncinessCalculation == AVERAGE || material2.bouncinessCalculation == AVERAGE)
		return (material1.bounciness + material2.bounciness) / 2.0f;
	if (material1.bouncinessCalculation == MINIMUM || material2.bouncinessCalculation == MINIMUM)
		return std::min(material1.bounciness, material2.bounciness);
	if (material1.bouncinessCalculation == MULTIPLY || material2.bouncinessCalculation == MULTIPLY)
		return material1.bounciness * material2.bounciness;
	if (material1.bouncinessCalculation == MAXIMUM || material2.bouncinessCalculation == MAXIMUM)
		return std::max(material1.bounciness, material2.bounciness);
}
