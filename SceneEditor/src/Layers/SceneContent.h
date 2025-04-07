#pragma once
#include "ObjectViewer.h"

class SceneContent : public Walnut::Layer
{
public:
	SceneContent(const std::shared_ptr<ObjectViewer>& viewer, const std::shared_ptr<ProjectDirectory>& directory);

	void OnUIRender() override;

	std::vector<GameObject>& GetGameObjects() ;
	std::vector<LightObject>& GetLights();

	std::string GetCurrentScene();

private:
	void Window();

	void RenderObjectList();

	void RenderLightList();

	void SetNewScene(std::string NewScene);
	

	std::string GetDirForSprite(std::string sprite, std::string dir);
private:
	std::vector<GameObject> m_gameObjects;
	std::vector <LightObject> m_lightObjects;

	int m_newLightIndex = -1;
	int m_NewGameObIndex = -1;

	int indexOfCurrentOb = -1;
	bool isCurrentObjectLight = false;

	std::shared_ptr<ObjectViewer> m_viewer;
	std::shared_ptr<ProjectDirectory> m_Directory;

	std::string m_currentScene = "";
};
