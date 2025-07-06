#pragma once
#include "SceneContent.h"
#include <Micro.h>
#include <lightInclude.h>

class SceneViewer : public Walnut::Layer
{
public:
	SceneViewer(const std::shared_ptr<SceneContent>& sceneContent, const char* mainPath);

	void OnDetach() override;

	void OnUIRender() override;

	void OnAttach() override;
	bool* Open() { return &m_isOpen; }

	bool DoneRun();

	std::shared_ptr<SceneContent> sceneContent;

private:
	void Window();
	void SaveWindow();

	void RenderGameObjects(ImVec2 contentRegion) const;
	void RenderLights(ImVec2 contentRegion);

	void renderTexts(ImVec2 contentRegion);

	void renderButtons(ImVec2 contentRegion) const;

	void GenerateLightImage(LightObject& light);
	void GenerateTextImage(TextObject& text);


	void ExecutePlayCommand();
	
	void RenderHeader(const ImVec2& contentRegion);

	
	
private:
	
	sf::RenderTexture renderTexture;

	std::shared_ptr<Walnut::Image> m_playButtonImage;
	std::shared_ptr<Walnut::Image> m_saveButtonImage;

	bool m_first = true;

	bool m_saving = false;
	bool m_doneSaving = false;
	std::string m_savePath;
	std::string m_saveName;
	std::string m_mainPath;

	float m_zoom = 1.0f;
	sf::Vector2f m_offset = { 0.0f, 0.0f };
	bool m_dragging = false;
	ImVec2 m_lastMousePos = { 0.0f, 0.0f };

	bool m_isOpen = true;


	bool m_doneWithRun = false;
};
