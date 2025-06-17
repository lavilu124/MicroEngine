#include "TextObject.h"

TextObject::TextObject(std::string value, std::string name, ImVec4 color, std::string font, ImVec4 outlineColor, float outlineThickness,unsigned int size,ImVec2 scale,	ImVec2 position, float rotation)
	:Object(name, position, rotation), value(value), color(color), font(font), outlineColor(outlineColor), outlineThickness(outlineThickness), size(size), scale(scale){

}

void TextObject::UpdateVal()
{
	image.reset();
	Updated = false;
}

void TextObject::Updating()
{
	Updated = true;
}

bool TextObject::isUpdated()
{
	return Updated;
}