#pragma once
#include "Walnut/UI/UI.h"

class Object
{
public:
	Object(std::string name, ImVec2 position = ImVec2(0,0), float rotation = 0);



	ImVec2 position;
	std::string name;
	float rotation;

	bool isVisable = true;

	bool IsSceneObject = true;
};

