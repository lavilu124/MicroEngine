#include "LightObject.h"


LightObject::LightObject(std::string name, vector2f position, float rotation, int type, rgbaColor color, float radius) : Object(name, position, rotation), type(type), color(color), radius(radius) {

}