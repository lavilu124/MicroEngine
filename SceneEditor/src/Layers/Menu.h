#pragma once
#include "Walnut/Application.h"

class Menu : public Walnut::Layer
{
public:
	Menu(std::string ProjectRootPath);

	void NewObjectPressed();

	void OnUIRender() override;

	void NewObjectWindow();
private:
	bool m_newObjectOpen = false;
	std::string m_rootPath;
	char m_ClassNameBuffer[256];
};

