#pragma once
#include "Object.h"
class Camera :
    public Object
{
    public:
		Camera(ImVec2 position = ImVec2(0, 0), float rotation = 0);

	float zoom = 1.0f;
	std::string objectName = ""; //follow

	int darknessPrecent = 0;
};

