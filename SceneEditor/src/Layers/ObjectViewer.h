#pragma once
#include "Walnut/Application.h"
#include "ProjectDirectory.h"
#include "../objects/Objects.h"


enum currentObjectType {
	light,
	game
};

class ObjectViewer : public Walnut::Layer
{
public:
	ObjectViewer(std::shared_ptr<ProjectDirectory> projectDirectory);

	virtual void OnUIRender() override;

	void SetObject(Object* newObject, currentObjectType type);

	Object* GetObject() const;

private:
	void Window();
	void DisplayGameObject();

	void DisplayLightObject();

private:
	Object* currentObject = nullptr;
	currentObjectType m_CurrentObjectType;
	std::shared_ptr<ProjectDirectory> m_ProjectDirectory;
};
