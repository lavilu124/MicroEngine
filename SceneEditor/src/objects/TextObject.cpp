#include "TextObject.h"

TextObject::TextObject(std::string value, std::string name, ImVec4 color, std::string font, ImVec4 outlineColor, float outlineThickness,unsigned int size,ImVec2 scale,	ImVec2 position, float rotation)
	:Object(name, position, rotation), value(value), color(color), font(font), outlineColor(outlineColor), outlineThickness(outlineThickness), size(size), scale(scale){

}

void TextObject::UpdateVal()
{
	image.reset();
	m_updated = false;
}

void TextObject::Updating()
{
	m_updated = true;
}

bool TextObject::isUpdated()
{
	return m_updated;
}