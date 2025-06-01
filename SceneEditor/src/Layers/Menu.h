#pragma once
#include "Walnut/Application.h"
#include "ObjectViewer.h"

class Menu : public Walnut::Layer
{
public:
	Menu(std::string projectRootPath, std::shared_ptr<ObjectViewer> objectViewer);

	void NewObjectPressed();

	void OnUIRender() override;

	void NewObjectWindow();

	bool ObjectExist(const std::string& name) const;
private:
	bool m_newObjectOpen = false;


	std::string m_rootPath;
	char m_ClassNameBuffer[256];

	std::shared_ptr<ObjectViewer> m_objectViewer;
};

