#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include "Micro/Objects/GameObject.h"




namespace Micro {
	class MICRO_API  ObjectManager
	{
	public:
		using CreatorFunc = std::function<std::unique_ptr<GameObject>(
			SystemManager* systemManager,
			const sf::Sprite& ObjectSprite,
			const std::string& name,
			Collision::collisionLayer layer,
			int level
		)>;

		static ObjectManager& Instance();

		void Register(const std::string& className, CreatorFunc creator);

		std::unique_ptr<GameObject> Create(
			const std::string& className,
			SystemManager* systemManager,
			const sf::Sprite& ObjectSprite,
			const std::string& name,
			Collision::collisionLayer layer = Collision::collisionLayer::COLLIDER,
			int level = 0
		) const;

	private:
		std::unordered_map<std::string, CreatorFunc> m_creators;
	};

}

#define REGISTER_GAMEOBJECT(classType) \
	namespace { \
		const bool registered_##classType = []() { \
			Micro::ObjectManager::Instance().Register(#classType, \
				[](Micro::SystemManager* systemManager, const sf::Sprite& sprite, const std::string& name, Micro::Collision::collisionLayer layer, int level) -> std::unique_ptr<Micro::GameObject> { \
					return std::make_unique<classType>(systemManager, sprite, name, layer, level); \
				}); \
			return true; \
		}(); \
	}
