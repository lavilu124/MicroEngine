#include "LightObject.h"



LightObject::LightObject(std::string name, ImVec2 position, float rotation, float beamAngle, int type, ImVec4 color, float radius, float width) : Object(name, position, rotation), type(type), color(color), radius(radius), beamAngle(beamAngle), width(width) {
	color.w = 1;
}


void LightObject::UpdateVal()
{
	image.reset();
	m_updated = false;
}

void LightObject::Updating()
{
	m_updated = true;
}

bool LightObject::isUpdated()
{
	return m_updated;
}

