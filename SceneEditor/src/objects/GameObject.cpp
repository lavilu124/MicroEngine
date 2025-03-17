#include "GameObject.h"
#include <filesystem>
#include <iostream>

int GameObject::counter = 0;

GameObject::GameObject(std::string name, std::string imagePath, ImVec2 position, ImVec2 scale, float rotation, int layer) : Object(name, position ,rotation), layer(layer), scale(scale){
	SetPath(imagePath);
	
}

void GameObject::SetPath(std::string newPath) {
	if (newPath.empty()) return;

	if (!std::filesystem::exists(newPath)) {
		return;
	}

	spritename = std::filesystem::path(newPath).filename().string();
	path = newPath;
	sprite = std::make_shared<Walnut::Image>(newPath);
	std::cout << counter++ << "\n";
	
}
