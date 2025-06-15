#pragma once
#include "Object.h"
#include <lightInclude.h>
#include "Walnut/Image.h"

class LightObject : public Object
{
public:
	LightObject(std::string name, ImVec2 position = ImVec2(0,0), float rotation = 0, float Angle = 360, int type = 0, ImVec4 color = ImVec4(255,255,255,1), float radius = 200, float width = 200);
	void UpdateVal();
	void Updating();
	bool isUpdated();

	int type = 0;
	ImVec4 color;
	float radius;
	float beamAngle;
	float width;
	bool fade = true;
	
	std::shared_ptr<Walnut::Image> image = nullptr;
	std::shared_ptr<uint8_t[]> imageData;

private:
	bool Updated = false;
};

