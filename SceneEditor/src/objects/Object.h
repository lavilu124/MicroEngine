#pragma once
#include <string>

struct vector2f {
	vector2f(float x, float y) : x(x), y(y) {}
	float x,y;
};

class Object
{
public:
	Object(std::string name, vector2f position = vector2f(0,0), float rotation = 0);

	vector2f position;
	float rotation;
	std::string name;
};

