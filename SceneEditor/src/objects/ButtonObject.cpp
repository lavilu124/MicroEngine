#include "ButtonObject.h"
#include <filesystem>

ButtonObject::ButtonObject(std::string name, std::string img, std::string onclickFunc, std::string onClickImg, float rotation, ImVec2 position, ImVec2 scale)
	:Object(name, position, rotation), name(name), onclickFunc(onclickFunc), scale(scale)
{
	SetPathReg(img);
	SetPathClick(onClickImg);
}

void ButtonObject::SetPathReg(std::string newPath) {
	if (newPath.empty()) return;

	if (!std::filesystem::exists(newPath)) {
		return;
	}

	img = std::filesystem::path(newPath).filename().string();
	image = std::make_shared<Walnut::Image>(newPath);

}

void ButtonObject::SetPathClick(std::string newPath) {
	if (newPath.empty()) return;

	if (!std::filesystem::exists(newPath)) {
		return;
	}

	onClickImg = std::filesystem::path(newPath).filename().string();
	clickImage = std::make_shared<Walnut::Image>(newPath);

}
