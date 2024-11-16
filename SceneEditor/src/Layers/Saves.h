#pragma once
#include "SceneContent.h"

class Saves : public Walnut::Layer	
{
public:
	Saves(std::shared_ptr<SceneContent> sceneContent);

	void OnUIRender() override;

	void Save();
	void Load();
private:
	void Window();

	void SaveWindow();
	void LoadWindow();

private:
	bool m_saving = false;
	bool m_loading = false;
	bool done = false;

	std::shared_ptr<SceneContent> m_sceneContent;

	std::string m_path;
};
