#pragma once
#include "SceneContent.h"
#include <Micro.h>
#include <lightInclude.h>

class SceneViewer : public Walnut::Layer
{
public:
	SceneViewer(std::shared_ptr<SceneContent> sceneContent);

	void OnUIRender() override;

	void OnAttach() override;

private:
	void Window();

	void RenderGameObjects() const;
	void RenderLights();

	std::shared_ptr<Walnut::Image> GenerateLightImage(LightObject& light);

	void ExecutePlayCommand() const;

	void RenderPlayButton(const ImVec2& contentRegion);

private:
	std::shared_ptr<SceneContent> m_sceneContent;
	sf::RenderTexture renderTexture;

	std::shared_ptr<Walnut::Image> m_playButtonImage;

	bool m_first = true;
};
