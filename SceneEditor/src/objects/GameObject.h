#pragma once
#include "Object.h"
#include "Walnut/Image.h"

class GameObject : public Object
{
public:
	GameObject(std::string name, std::string imagePath, ImVec2 position = ImVec2(0, 0), ImVec2 scale = ImVec2(0, 0), float rotation = 0, int layer = 0, int level = 0);
	ImVec2 scale;
	int layer;
	std::string spritename;
	std::shared_ptr<Walnut::Image> sprite = nullptr;
	std::string type;
	int level;

	void LevelChange();
	bool IsLevelChanged();

	void SetPath(std::string newPath);
private:
	std::string m_path;

	static int counter;

	bool m_levelChanged = false;
};

