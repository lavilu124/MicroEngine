#include "SceneContent.h"

#include "ObjectViewer.h"

#include "../json/json.h"
#include "../include/json/value.h"
#include <fstream>

SceneContent::SceneContent(const std::shared_ptr<ObjectViewer>& viewer, const std::shared_ptr<ProjectDirectory>& directory) : m_viewer(viewer), m_directory(directory)
{
}

void SceneContent::OnDetach()
{
	for (auto& light : m_lightObjects) {
		light.image.reset();
		light.imageData.reset();
	}
	m_lightObjects.clear();
	m_gameObjects.clear();
}

void SceneContent::OnUIRender()
{
	Window();

	if (m_viewer->Delete()) {
		if (m_viewer->GetCurrentObjectType() == currentObjectType::light) {
			m_lightObjects.erase(m_lightObjects.begin() + m_indexOfCurrentOb);
		}
		else if (m_viewer->GetCurrentObjectType() == currentObjectType::game){
			m_gameObjects.erase(m_gameObjects.begin() + m_indexOfCurrentOb);
		}
		else {
			m_textObjects.erase(m_textObjects.begin() + m_indexOfCurrentOb);
		}
		m_viewer->SetObject(nullptr, currentObjectType::light);
	}
}

std::vector<GameObject>& SceneContent::GetGameObjects()
{
	return m_gameObjects;
}

std::vector<LightObject>& SceneContent::GetLights()
{
	return m_lightObjects;
}

std::vector<TextObject>& SceneContent::GetTexts()
{
	return m_textObjects;
}

std::vector<ButtonObject>& SceneContent::GetButtons()
{
	return m_buttonObjects;
}

void SceneContent::Window()
{
	ImGui::Begin("Scene Content");

	if (m_gameObjects.size() == 0 && m_lightObjects.size() == 0) {
		ImGui::Text("No Objects");
	}

	std::string newScene = m_directory->getNewScene();
	if (newScene != "") {
		SetNewScene(newScene);
	}

	RenderObjectList();
	RenderLightList();
	RenderUiList();

	float footerHeight = 40.0f;
	ImGui::SetCursorPosY(ImGui::GetWindowSize().y - footerHeight * 2);

	if (ImGui::BeginPopupContextWindow("CreateObjectContext", ImGuiPopupFlags_MouseButtonRight))
	{
		if (ImGui::BeginMenu("Create Object"))
		{
			if (ImGui::MenuItem("Light Object"))
			{
				m_lightObjects.push_back(LightObject(
					"Light " + std::to_string(m_lightObjects.size() + 1),
					ImVec2(0, 0),
					0,
					360,
					0,
					ImVec4(255, 255, 255, 255),
					200,
					200
				));

				m_newLightIndex = m_lightObjects.size() - 1;
				if (m_viewer->GetCurrentObjectType() == currentObjectType::light && m_viewer->GetObject() != nullptr)
					m_viewer->SetObject(&m_gameObjects[m_indexOfCurrentOb], currentObjectType::light);
			}

			if (ImGui::MenuItem("Game Object"))
			{
				GameObject newGameObject("Object " + std::to_string(m_gameObjects.size() + 1), "");
				m_gameObjects.push_back(newGameObject);
				m_NewGameIndex = m_gameObjects.size() - 1;
				if (m_viewer->GetCurrentObjectType() == currentObjectType::game && m_viewer->GetObject() != nullptr)
					m_viewer->SetObject(&m_gameObjects[m_indexOfCurrentOb], currentObjectType::game);
			}

			if (ImGui::BeginMenu("UI"))
			{
				if (ImGui::MenuItem("Text"))
				{
					m_textObjects.push_back(TextObject(
						"New Text",
						"Text " + std::to_string(m_textObjects.size() + 1),
						ImVec4(255, 255, 255, 255),
						"default",
						ImVec4(0, 0, 0, 255),
						1.0f,
						24,
						ImVec2(1.0f, 1.0f),
						ImVec2(0, 0),
						0.0f
					));
					m_newTextIndex = m_textObjects.size() - 1;
					if (m_viewer->GetCurrentObjectType() == currentObjectType::text && m_viewer->GetObject() != nullptr)
						m_viewer->SetObject(&m_textObjects[m_indexOfCurrentOb], currentObjectType::text);
				}
				if (ImGui::MenuItem("Button")) {
					m_buttonObjects.push_back(ButtonObject(
						"New Button",
						"",
						""
					));
					m_newButtonIndex = m_buttonObjects.size() - 1;
					if (m_viewer->GetCurrentObjectType() == currentObjectType::button && m_viewer->GetObject() != nullptr)
						m_viewer->SetObject(&m_buttonObjects[m_indexOfCurrentOb], currentObjectType::button);
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}

	ImGui::End();
}

void SceneContent::RenderObjectList()
{
	ImVec2 windowSize = ImGui::GetWindowSize();
	if (windowSize.x > 300) windowSize.x = 300;
	if (m_gameObjects.size() != 0) {
		ImGui::Separator();
		ImGui::Text("Game Objects:");
	}

	ImGui::Indent();
	for (int i = 0; i < m_gameObjects.size(); i++) {
		if (ImGui::Button(m_gameObjects[i].name.c_str(), ImVec2(windowSize.x - 20.0f, 30))) {
			m_viewer->SetObject(&m_gameObjects[i], currentObjectType::game);
			m_indexOfCurrentOb = i;
		}
	}
	ImGui::Unindent();

	if (m_NewGameIndex > -1) {
		static char GameObNameInput[256] = "";
		snprintf(GameObNameInput, sizeof(GameObNameInput), "Object %d", m_NewGameIndex + 1);

		ImGui::InputText("##Enter GameObject Name", GameObNameInput, IM_ARRAYSIZE(GameObNameInput));
		if (ImGui::Button("Cancel")) {
			m_gameObjects.erase(m_gameObjects.begin() + m_NewGameIndex);
			m_NewGameIndex = -1;
		}
		ImGui::SameLine();
		if (ImGui::Button("Confirm")) {
			m_gameObjects[m_NewGameIndex].name = GameObNameInput;
			m_NewGameIndex = -1;
		}
	}
}

void SceneContent::RenderLightList()
{
	ImVec2 windowSize = ImGui::GetWindowSize();
	if (windowSize.x > 300) windowSize.x = 300;
	if (m_lightObjects.size() != 0) {
		ImGui::Separator();
		ImGui::Text("Light Objects:");
	}

	ImGui::Indent();
	for (int i = 0; i < m_lightObjects.size(); i++) {
		if (ImGui::Button(m_lightObjects[i].name.c_str(), ImVec2(windowSize.x - 20.0f, 30))) {
			m_viewer->SetObject(&m_lightObjects[i], currentObjectType::light);
			m_indexOfCurrentOb = i;
		}
	}
	ImGui::Unindent();

	if (m_newLightIndex > -1) {
		static char lightNameInput[256] = "";
		snprintf(lightNameInput, sizeof(lightNameInput), "Light %d", m_newLightIndex + 1);

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

void SceneContent::RenderUiList() {
	ImVec2 windowSize = ImGui::GetWindowSize();
	if (windowSize.x > 300) windowSize.x = 300;

	// Text objects
	if (!m_textObjects.empty()) {
		ImGui::Separator();
		ImGui::Text("UI Texts:");
	}

	ImGui::Indent();
	for (int i = 0; i < m_textObjects.size(); i++) {
		if (ImGui::Button(m_textObjects[i].name.c_str(), ImVec2(windowSize.x - 20.0f, 30))) {
			m_viewer->SetObject(&m_textObjects[i], currentObjectType::text);
			m_indexOfCurrentOb = i;
		}
	}
	ImGui::Unindent();

	if (m_newTextIndex > -1) {
		static char TextNameInput[256] = "";
		snprintf(TextNameInput, sizeof(TextNameInput), "Text %d", m_newTextIndex + 1);

		ImGui::InputText("##Enter Text Name", TextNameInput, IM_ARRAYSIZE(TextNameInput));
		if (ImGui::Button("Cancel")) {
			m_textObjects.erase(m_textObjects.begin() + m_newTextIndex);
			m_newTextIndex = -1;
		}
		ImGui::SameLine();
		if (ImGui::Button("Confirm")) {
			m_textObjects[m_newTextIndex].name = TextNameInput;
			m_newTextIndex = -1;
		}
	}

	// Button objects
	if (!m_buttonObjects.empty()) {
		ImGui::Separator();
		ImGui::Text("UI Buttons:");
	}

	ImGui::Indent();
	for (int i = 0; i < m_buttonObjects.size(); i++) {
		if (ImGui::Button(m_buttonObjects[i].name.c_str(), ImVec2(windowSize.x - 20.0f, 30))) {
			m_viewer->SetObject(&m_buttonObjects[i], currentObjectType::button);
			m_indexOfCurrentOb = i;
		}
	}
	ImGui::Unindent();

	if (m_newButtonIndex > -1) {
		static char ButtonNameInput[256] = "";
		snprintf(ButtonNameInput, sizeof(ButtonNameInput), "Button %d", m_newButtonIndex + 1);

		ImGui::InputText("##Enter Button Name", ButtonNameInput, IM_ARRAYSIZE(ButtonNameInput));
		if (ImGui::Button("Cancel##ButtonCancel")) {
			m_buttonObjects.erase(m_buttonObjects.begin() + m_newButtonIndex);
			m_newButtonIndex = -1;
		}
		ImGui::SameLine();
		if (ImGui::Button("Confirm##ButtonConfirm")) {
			m_buttonObjects[m_newButtonIndex].name = ButtonNameInput;
			m_newButtonIndex = -1;
		}
	}
}


void SceneContent::SetNewScene(std::string NewScene)
{
	m_viewer->SetObject(nullptr, currentObjectType::game);
	m_gameObjects.clear();
	m_lightObjects.clear();

	std::ifstream inputFile(NewScene);
	Json::Value actualJson;
	Json::Reader Reader;

	//check if the input file exsist
	if (!inputFile.is_open())
		return;

	//checing if the file can be read
	if (!Reader.parse(inputFile, actualJson))
		return;

	m_currentScene = NewScene;

	int count = 0;

	//going over the json and reading all the data
	for (int i = 0; i < actualJson.size(); i++) {
		std::stringstream ss;
		ss << i;
		Json::Value currentObject = actualJson["object" + ss.str()];


		if (currentObject.isNull()) {
			count = actualJson.size() - i - 1;
			break;
		}

		//object values
		int layer = currentObject["layer"].asInt();
		ImVec2 position = ImVec2(currentObject["position"][0].asFloat(), currentObject["position"][1].asFloat());
		float rotation = currentObject["rotation"].asFloat();
		ImVec2 scale = ImVec2(currentObject["scale"][0].asFloat(), currentObject["scale"][1].asFloat());
		std::string spriteName = currentObject["spriteName"].asString();
		std::string name = currentObject["name"].asString();
		int level = currentObject["level"].asInt();

		std::string pathToSprite = GetDirForSprite(spriteName, std::filesystem::current_path().string());

		m_gameObjects.push_back(GameObject(name, pathToSprite, position, scale, rotation, layer, level));


	}

	for (int i = 0; i < count; i++) {
		std::stringstream ss;
		ss << i;
		Json::Value currentObject = actualJson["lightSource" + ss.str()];

		if (currentObject.isNull()) {
			count = count - i;
			break;
		}

		int type = currentObject["LightType"].asInt();
		std::string name = currentObject["name"].asString();
		ImVec2 position = ImVec2(currentObject["position"][0].asFloat(), currentObject["position"][1].asFloat());
		ImColor color = ImColor(currentObject["color"][0].asFloat() , currentObject["color"][1].asFloat() , currentObject["color"][2].asFloat() , currentObject["color"][3].asFloat());
		float radius = currentObject["radius"].asFloat() /1.5;
		float rotation = currentObject["rotation"].asFloat();
		float angle = currentObject["angle"].asFloat();
		bool fade = currentObject["fade"].asBool();
		ls::LightId light;

		m_lightObjects.push_back(LightObject(name, position, rotation, angle, type, color, radius));
		m_lightObjects.back().fade = fade;

	}

	for (int i = 0; i < count; i++) {
		std::stringstream ss;
		ss << i;
		Json::Value currentObject = actualJson["text" + ss.str()];

		if (currentObject.isNull()) {
			count = count - i;
			break;
		}

		std::string name = currentObject["name"].asString();
		std::string font = currentObject["font"].asString();
		float rotation = currentObject["rotation"].asFloat();
		ImVec2 position = ImVec2(currentObject["position"][0].asFloat(), currentObject["position"][1].asFloat());
		ImVec4 color = ImVec4(currentObject["color"][0].asFloat(), currentObject["color"][1].asFloat(), currentObject["color"][2].asFloat(), 1);
		ImVec4 outlineColor = ImVec4(currentObject["outlineColor"][0].asFloat() /255, currentObject["outlineColor"][1].asFloat(), currentObject["outlineColor"][2].asFloat(), 1);
		float outlineThickness = currentObject["outlineThickness"].asFloat();
		unsigned int size = currentObject["size"].asInt();
		ImVec2 scale = ImVec2(currentObject["scale"][0].asFloat(), currentObject["scale"][1].asFloat());
		std::string value = currentObject["value"].asString();


		m_textObjects.push_back(TextObject(value, name, color, font, outlineColor, outlineThickness, size, scale, position, rotation));
	}

	for (int i = 0; i < count; i++) {
		std::stringstream ss;
		ss << i;
		Json::Value currentObject = actualJson["button" + ss.str()];

		if (currentObject.isNull()) {
			count = count - i;
			break;
		}

		std::string name = currentObject["name"].asString();
		std::string img = currentObject["img"].asString();
		std::string onClickImg = currentObject["onClickImg"].asString();
		std::string onclickfunc = currentObject["onClickFunc"].asString();
		float rotation = currentObject["rotation"].asFloat();
		ImVec2 position = ImVec2(currentObject["position"][0].asFloat(), currentObject["position"][1].asFloat());
		ImVec2 scale = ImVec2(currentObject["scale"][0].asFloat(), currentObject["scale"][1].asFloat());

		std::string pathToImg1 = GetDirForSprite(img, std::filesystem::current_path().string());
		std::string pathToImg2 = GetDirForSprite(onClickImg, std::filesystem::current_path().string());


		m_buttonObjects.push_back(ButtonObject(name, pathToImg1, onclickfunc, pathToImg2, rotation, position, scale));
	}

	//close the file
	inputFile.close();
}

std::string SceneContent::GetCurrentScene()
{
	return m_currentScene;
}

std::string SceneContent::GetDirForSprite(std::string sprite, std::string dir)
{
	if (dir.find("SceneEditor") != std::string::npos) {
		dir = dir.substr(0, dir.find_last_of('\\'));
		dir += "\\Resources";
	}

	for (const auto& entry : std::filesystem::directory_iterator(dir)) {
		if (entry.is_directory())
		{
			std::string returnVal = GetDirForSprite(sprite, entry.path().string());
			if (returnVal != "") {
				return returnVal;
			}
		}
		else if (entry.path().filename().string().find(sprite) != std::string::npos) {
			return entry.path().string();
		}
	}
	return "";
}

std::shared_ptr<ProjectDirectory> SceneContent::GetDir()
{
	return m_directory;
}
