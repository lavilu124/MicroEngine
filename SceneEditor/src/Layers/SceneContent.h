#pragma once
#include "ObjectViewer.h"

class SceneContent : public Walnut::Layer
{
public:
	SceneContent(const std::shared_ptr<ObjectViewer>& viewer);

	virtual void OnUIRender() override;

	std::vector<GameObject>& GetGameObjects() ;
	std::vector<LightObject>& GetLights() ;

private:
	void Window();

	void RenderObjectList();

	void RenderLightList();
private:
	std::vector<GameObject> m_gameObjects;
	std::vector <LightObject> m_lightObjects;

	int m_newLightIndex = -1;
	int m_NewGameObIndex = -1;

	std::shared_ptr<ObjectViewer> m_viewer;
};
