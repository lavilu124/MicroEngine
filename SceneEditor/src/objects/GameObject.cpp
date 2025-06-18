#include "GameObject.h"
#include <filesystem>


int GameObject::counter = 0;

GameObject::GameObject(std::string name, std::string imagePath, ImVec2 position, ImVec2 scale, float rotation, int layer, int level) : Object(name, position ,rotation), layer(layer), scale(scale), level(level){
	SetPath(imagePath);
	
}

void GameObject::LevelChange()
{
	m_levelChanged = true;
}

bool GameObject::IsLevelChanged()
{
	return true;
}

void GameObject::SetPath(std::string newPath) {
	if (newPath.empty()) return;

	if (!std::filesystem::exists(newPath)) {
		return;
	}

	spritename = std::filesystem::path(newPath).filename().string();
	m_path = newPath;
	sprite = std::make_shared<Walnut::Image>(newPath);
	
}
