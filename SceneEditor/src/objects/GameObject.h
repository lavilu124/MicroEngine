#pragma once
#include "Object.h"
#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"

class GameObject : public Object
{
public:
	GameObject(std::string name, std::string imagePath, vector2f position = vector2f(0, 0), vector2f scale = vector2f(0, 0), float rotation = 0, int layer = 0);
	vector2f scale;
	int layer;
	std::string spritename;
	std::shared_ptr<Walnut::Image> sprite;

	void SetPath(std::string newPath);
private:
	std::string path;
};

