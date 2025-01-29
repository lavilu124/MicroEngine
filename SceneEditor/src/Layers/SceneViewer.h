#pragma once
#include "SceneContent.h"
#include <Micro.h>
class SceneViewer : public Walnut::Layer
{
public:
	SceneViewer(std::shared_ptr<SceneContent> sceneContent);

	virtual void OnUIRender() override;

private:
	void Window();

	void RenderGameObjects() const;
	void RenderLights();

	//void setImage(LightObject& light);

	void test();

private:
	std::shared_ptr<SceneContent> m_sceneContent;
	//Micro::ls::LightSystem ls;

	sf::View view;
};
