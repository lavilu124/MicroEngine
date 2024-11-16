#include "SceneViewer.h"

SceneViewer::SceneViewer(std::shared_ptr<SceneContent> sceneContent) : m_sceneContent(sceneContent)
{
}

void SceneViewer::OnUIRender()
{
	Window();
}

void SceneViewer::Window()
{
	ImGui::Begin("Scene Viewer");
	//if (imgui::button("run game")) {
	//	std::string command = "c:\\github\\microengine\\binaries\\windows-x86_64\\debug\\sendbox\\sendbox.exe"; // assuming the exe is in the same directory
	//	system(command.c_str());
	//}

	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	ImGui::GetWindowDrawList()->AddRectFilled(
		ImGui::GetCursorScreenPos(),
		ImVec2(ImGui::GetCursorScreenPos().x + contentRegion.x, ImGui::GetCursorScreenPos().y + contentRegion.y),
		IM_COL32(0, 0, 0, 255) // Black
	);

	RenderGameObjects();
	//RenderLights();

	ImGui::End();
}

void SceneViewer::RenderGameObjects() const
{
	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	for (auto& gameObject : m_sceneContent->GetGameObjects()) {
		if (gameObject.sprite) {
			ImGui::SetCursorPos(ImVec2(
				gameObject.position.x + contentRegion.x / 2 - (float)gameObject.sprite->GetWidth() / 2, // Move 120px to the left
				gameObject.position.y + contentRegion.y / 2 - (float)gameObject.sprite->GetHeight() / 2
			));
			ImGui::Image(gameObject.sprite->GetDescriptorSet(), { gameObject.scale.x * (float)gameObject.sprite->GetWidth(), gameObject.scale.y * (float)gameObject.sprite->GetHeight() });
		}
	}
}

void SceneViewer::RenderLights() const
{
	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	for (auto& light : m_sceneContent->GetLights()) {
		ImVec2 lightPosition = ImVec2(
			light.position.x + contentRegion.x / 2,
			light.position.y + contentRegion.y / 2
		);
	}
}
