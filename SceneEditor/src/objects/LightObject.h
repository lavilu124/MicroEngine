#pragma once
#include "Object.h"
#include <lightInclude.h>



class LightObject : public Object
{
public:
	LightObject(std::string name, ImVec2 position = ImVec2(0,0), float rotation = 0, int type = 0, ImColor color = ImColor(255,255,255,1), float radius = 200);
	void UpdateVal();
	int type;
	ImColor color;
	float radius;
	std::shared_ptr<Walnut::Image> image = nullptr;
	int counter = 0;
	//sf::RenderTexture renderTexture;
};

