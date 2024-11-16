#include "SceneContent.h"

#include "ObjectViewer.h"

SceneContent::SceneContent(const std::shared_ptr<ObjectViewer>& viewer) : m_viewer(viewer)
{
}

void SceneContent::OnUIRender()
{
	Window();
}

std::vector<GameObject> SceneContent::GetGameObjects() const
{
	return m_gameObjects;
}

std::vector<LightObject> SceneContent::GetLights() const
{
	return m_lightObjects;
}

void SceneContent::Window()
{
	ImGui::Begin("Scene Content");



	if (m_gameObjects.size() == 0 && m_lightObjects.size() == 0) {
		ImGui::Text("No Objects");
	}

	RenderObjectList();
	RenderLightList();

	float footerHeight = 40.0f;
	ImGui::SetCursorPosY(ImGui::GetWindowSize().y - footerHeight * 2);


	ImGui::Separator();


	if (ImGui::Button("Create Light", ImVec2(ImGui::GetWindowSize().x, 30))) {

		LightObject newLight("New Light");
		m_lightObjects.push_back(newLight);


		m_newLightIndex = m_lightObjects.size() - 1;
	}

	if (ImGui::Button("Create GameObject", ImVec2(ImGui::GetWindowSize().x, 30))) {
		GameObject newGameObject("New Object", "");
		m_gameObjects.push_back(newGameObject);

		m_NewGameObIndex = m_gameObjects.size() - 1;
	}


	ImGui::End();
}

void SceneContent::RenderObjectList()
{
	ImVec2 windowSize = ImGui::GetWindowSize();
	if (windowSize.x > 300) windowSize.x = 300;
	if (m_gameObjects.size() != 0) {
		ImGui::Unindent();
		ImGui::Separator();
		ImGui::Text("Game Objects:");
		ImGui::Indent();
	}

	for (int i = 0; i < m_gameObjects.size(); i++) {
		if (ImGui::Button(m_gameObjects[i].name.c_str(), ImVec2(windowSize.x - 20.0f, 30))) {
			m_viewer->SetObject(&m_gameObjects[i], currentObjectType::game);
		}
	}

	if (m_NewGameObIndex > -1) {
		static char GameObNameInput[256] = "New GameObject";
		ImGui::InputText("##Enter GameObject Name", GameObNameInput, IM_ARRAYSIZE(GameObNameInput));
		if (ImGui::Button("Cancel")) {
			m_gameObjects.erase(m_gameObjects.begin() + m_NewGameObIndex);
			m_NewGameObIndex = -1;
		}
		ImGui::SameLine();
		if (ImGui::Button("Confirm")) {
			m_gameObjects[m_NewGameObIndex].name = GameObNameInput;
			m_NewGameObIndex = -1;
		}
	}
}

void SceneContent::RenderLightList()
{
	ImVec2 windowSize = ImGui::GetWindowSize();
	if (windowSize.x > 300) windowSize.x = 300;
	if (m_lightObjects.size() != 0) {
		ImGui::Unindent();
		ImGui::Separator();
		ImGui::Text("Light Objects:");
		ImGui::Indent();
	}

	for (int i = 0; i < m_lightObjects.size(); i++) {
		if (ImGui::Button(m_lightObjects[i].name.c_str(), ImVec2(windowSize.x - 20.0f, 30))) {
			m_viewer->SetObject(&m_lightObjects[i], currentObjectType::light);
		}
	}

	ImGui::Unindent();

	if (m_newLightIndex > -1) {
		static char lightNameInput[256] = "New Light";
		ImGui::InputText("##Enter Light Name", lightNameInput, IM_ARRAYSIZE(lightNameInput));
		if (ImGui::Button("Cancel")) {
			m_lightObjects.erase(m_lightObjects.begin() + m_newLightIndex);
			m_newLightIndex = -1;
		}
		ImGui::SameLine();
		if (ImGui::Button("Confirm")) {
			m_lightObjects[m_newLightIndex].name = lightNameInput;
			m_newLightIndex = -1;
		}
	}
}
