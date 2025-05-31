#include "ObjectManager.h"

namespace Micro {
	ObjectManager& ObjectManager::Instance()
	{
		static ObjectManager instance;
		return instance;
	}

	void ObjectManager::Register(const std::string& className, CreatorFunc creator)
	{
		m_creators[className] = creator;
	}
	std::unique_ptr<GameObject> ObjectManager::Create(const std::string& className, SystemManager* systemManager, const sf::Sprite& ObjectSprite, const std::string& name, Collision::collisionLayer layer) const
	{
		auto it = m_creators.find(className);
		if (it != m_creators.end()) {
			return it->second(systemManager, ObjectSprite, name, layer);
		}
		return nullptr;
	}
}