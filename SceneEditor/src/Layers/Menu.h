#pragma once
#include "Walnut/Application.h"
#include "ObjectViewer.h"

class Menu : public Walnut::Layer
{
public:
	Menu(const std::string& projectRootPath, std::shared_ptr<ObjectViewer> objectViewer);

	void NewObjectPressed();
	void DeleteObjectPressed(const char* name);

	void OnUIRender() override;

	void OnAttach() override;

	void NewObjectWindow();
	void DeleteObjectWindow();

	bool ObjectExist(const std::string& name) const;
private:
	bool m_newObjectOpen = false;

	bool m_deleteObjectOpen = false;
	const char* m_deleteObjectName;


	std::string m_rootPath;
	char m_ClassNameBuffer[256];

	std::shared_ptr<ObjectViewer> m_objectViewer;
};

