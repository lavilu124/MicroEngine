#pragma once
#include "Walnut/Application.h"
#include "ProjectDirectory.h"
#include "../objects/Objects.h"


enum currentObjectType {
	light,
	game,
	text,
	button
};

class ObjectViewer : public Walnut::Layer
{
public:
	ObjectViewer(std::shared_ptr<ProjectDirectory> projectDirectory);

	void OnUIRender() override;

	void SetObject(Object* newObject, currentObjectType type);
	currentObjectType GetCurrentObjectType();

	Object* GetObject() const;

	bool Delete();

	void OnAttach() override;

	void NewObject();

private:
	void Window();
	void DisplayGameObject();

	void DisplayLightObject();

	void DisplayTextObject();

	void DisplayButtonObject();

private:
	Object* m_currentObject = nullptr;
	std::string m_currentObjectName = "";
	currentObjectType m_currentObjectType;

	std::shared_ptr<ProjectDirectory> m_projectDirectory;

	bool m_toDelete = false;

	std::shared_ptr<Walnut::Image> m_deleteIcon;

	bool m_ObjectsLoaded = false;
	std::vector<std::string> m_typeList;


	float m_lightint = 0;
};
