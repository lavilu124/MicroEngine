#include "Object.h"
#include "Walnut/Image.h"

class TextObject : public Object {
public:
	TextObject(std::string value, std::string name,
	ImVec4 color = { 1,1,1,1 },
	std::string font = "",
	ImVec4 outlineColor = {0,0,0,0},
	float outlineThickness = 0,
	unsigned int size = 30,
	ImVec2 scale = {1, 1},
	ImVec2 position = ImVec2(0, 0),
	float rotation = 0
	);

	std::string font;
	ImVec4 color;
	ImVec4 outlineColor;
	float outlineThickness;
	unsigned int size;
	ImVec2 scale;
	std::string value;

	void UpdateVal();
	void Updating();
	bool isUpdated();

	std::shared_ptr<Walnut::Image> image = nullptr;
	std::shared_ptr<uint8_t[]> imageData;

private:
	bool Updated = false;
};