#include "LightObject.h"


LightObject::LightObject(std::string name, ImVec2 position, float rotation, int type, ImColor color, float radius) : Object(name, position, rotation), type(type), color(color), radius(radius) {
}

void LightObject::UpdateVal()
{
	image = nullptr;
	Updated = false;
}

void LightObject::Updating()
{
	Updated = true;
}

bool LightObject::isUpdated()
{
	return Updated;
}

