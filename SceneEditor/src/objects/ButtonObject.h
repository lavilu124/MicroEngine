#pragma once
#include "Object.h"
#include "Walnut/Image.h"

class ButtonObject : public Object {
public:
	ButtonObject(std::string name,
		std::string img,
		std::string onclickfunc,
		std::string onClickImg = "",
		float rotation = 0,
		ImVec2 position = { 0.0f, 0.0f },
		ImVec2 scale = { 1.0f, 1.0f });

	void SetPathReg(std::string newPath);
	void SetPathClick(std::string newPath);

	std::string name;
	std::string img;
	std::string onClickImg;
	std::string onclickFunc;
	ImVec2 scale;

	std::shared_ptr<Walnut::Image> image = nullptr;
	std::shared_ptr<Walnut::Image> clickImage = nullptr;
};

