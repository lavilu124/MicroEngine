#pragma once
#include "ObjectViewer.h"

class SceneContent : public Walnut::Layer
{
public:
	SceneContent(const std::shared_ptr<ObjectViewer>& viewer, const std::shared_ptr<ProjectDirectory>& directory);
	void OnDetach() override;

	void OnUIRender() override;

	std::vector<GameObject>& GetGameObjects();
	std::vector<LightObject>& GetLights();

	std::vector<TextObject>& GetTexts();

	std::string GetCurrentScene();

	std::shared_ptr<ProjectDirectory> GetDir();

private:
	void Window();

	void RenderObjectList();

	void RenderLightList();

	void RenderUiList();

	void SetNewScene(std::string NewScene);
	

	std::string GetDirForSprite(std::string sprite, std::string dir);

	
private:
	std::vector<GameObject> m_gameObjects;
	std::vector <LightObject> m_lightObjects;
	std::vector <TextObject> m_textObjects;

	int m_newLightIndex = -1;
	int m_NewGameIndex = -1;
	int m_newTextIndex = -1;

	int m_indexOfCurrentOb = -1;

	std::shared_ptr<ObjectViewer> m_viewer;
	std::shared_ptr<ProjectDirectory> m_directory;

	std::string m_currentScene = "";
};
