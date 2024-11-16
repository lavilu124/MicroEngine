#pragma once
#include "SceneContent.h"

class SceneViewer : public Walnut::Layer
{
public:
	SceneViewer(std::shared_ptr<SceneContent> sceneContent);

	virtual void OnUIRender() override;

private:
	void Window();

	void RenderGameObjects() const;
	void RenderLights() const;
private:
	std::shared_ptr<SceneContent> m_sceneContent;
};
