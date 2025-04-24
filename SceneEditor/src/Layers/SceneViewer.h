#pragma once
#include "SceneContent.h"
#include <Micro.h>
#include <lightInclude.h>

class SceneViewer : public Walnut::Layer
{
public:
	SceneViewer(std::shared_ptr<SceneContent> sceneContent, const char* mainPath);

	void OnUIRender() override;

	void OnAttach() override;

private:
	void Window();
	void SaveWindow();

	void RenderGameObjects() const;
	void RenderLights();

	std::shared_ptr<Walnut::Image> GenerateLightImage(LightObject& light);

	void ExecutePlayCommand() const;

	void RenderHeader(const ImVec2& contentRegion);

private:
	std::shared_ptr<SceneContent> m_sceneContent;
	sf::RenderTexture renderTexture;

	std::shared_ptr<Walnut::Image> m_playButtonImage;
	std::shared_ptr<Walnut::Image> m_saveButtonImage;

	bool m_first = true;

	bool m_saving = false;
	bool m_doneSaving = false;
	std::string m_savePath;
	std::string m_saveName;
	std::string m_mainPath;
};
