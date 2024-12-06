#pragma once
#include "SceneContent.h"
#include <Micro.h>

struct ImageData {
	uint32_t width = 0;
	uint32_t height = 0;
	const void* data = nullptr;
};

class SceneViewer : public Walnut::Layer
{
public:
	SceneViewer(std::shared_ptr<SceneContent> sceneContent);

	virtual void OnUIRender() override;

private:
	void Window();

	void RenderGameObjects() const;
	void RenderLights();

	void SetImage(LightObject& light);

	void RenderPlayButton(const ImVec2& contentRegion);

	void SceneViewer::ImageRotated(ImTextureID user_texture_id, const ImVec2& size, float angle, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col) const;

	ImageData ExtractImageData(const sf::Sprite& sprite) const;

	void SceneViewer::RenderObject(GameObject& object) const;
private:
	std::shared_ptr<SceneContent> m_sceneContent;
	Micro::ls::LightSystem ls;

	sf::View view;
};
