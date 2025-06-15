#pragma once
#include "ObjectViewer.h"

class SceneContent : public Walnut::Layer
{
public:
	SceneContent(const std::shared_ptr<ObjectViewer>& viewer, const std::shared_ptr<ProjectDirectory>& directory);
	~SceneContent();

	void OnUIRender() override;

	std::vector<GameObject>& GetGameObjects();
	std::vector<LightObject>& GetLights();

	std::string GetCurrentScene();

	std::shared_ptr<ProjectDirectory> GetDir();

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

	int m_indexOfCurrentOb = -1;
	bool m_isCurrentObjectLight = false;

	std::shared_ptr<ObjectViewer> m_viewer;
	std::shared_ptr<ProjectDirectory> m_directory;

	std::string m_currentScene = "";
};
