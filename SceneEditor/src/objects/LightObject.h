#pragma once
#include "Object.h"

struct rgbaColor
{
	rgbaColor(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
	float r, g, b, a;
};

class LightObject : public Object
{
public:
	LightObject(std::string name, vector2f position = vector2f(0,0), float rotation = 0, int type = 0, rgbaColor color = rgbaColor(255,255,255,1), float radius = 200);
	int type;
	rgbaColor color;
	float radius;
};

