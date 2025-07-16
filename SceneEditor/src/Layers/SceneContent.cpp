#include "SceneContent.h"

#include "ObjectViewer.h"

#include "../json/json.h"
#include "../include/json/value.h"
#include <fstream>

SceneContent::SceneContent(const std::shared_ptr<ObjectViewer>& viewer, const std::shared_ptr<ProjectDirectory>& directory) : m_viewer(viewer), m_directory(directory), m_camera({0,0},0)
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
	m_buttonObjects.clear();
	m_textObjects.clear();

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
		m_currentObName = "";
	}
}

bool ProjectDirectory::TryDeleteEntry(const std::filesystem::path& path)
{
	try {
		if (std::filesystem::exists(path)) {
			std::filesystem::remove(path);
			return true;
		}
	}
	catch (const std::exception& e) {
		ImGui::OpenPopup("Delete Error");
	}
	return false;
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

static int partition(std::vector<GameObject>& vec, int low, int high) {

	int pivot = vec[high].level;
	int i = (low - 1);

	for (int j = low; j <= high - 1; j++) {
		if (vec[j].level <= pivot) {
			i++;
			std::swap(vec[i], vec[j]);
		}
	}

	std::swap(vec[i + 1], vec[high]);

	return (i + 1);
}

static void quickSort(std::vector<GameObject>& vec, int low, int high) {
	if (low < high) {

		int pi = partition(vec, low, high);

		quickSort(vec, low, pi - 1);
		quickSort(vec, pi + 1, high);
	}
}

void SceneContent::Window()
{
	if (!m_isOpen) return;

	// Begin Hierarchy Window
	ImGui::Begin("Scene Content", &m_isOpen);

	// Set colors for hover and selection (Unity style)
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.25f, 0.5f, 0.75f, 0.1f));
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.25f, 0.5f, 1.0f, 1.5f));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0)); // Transparent background

	// Shrink item spacing and padding
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);

	ImGui::Indent();

	// Check for scene change
	std::string newScene = m_directory->GetNewScene();
	if (!newScene.empty())
		SetNewScene(newScene);

	// Update object names if the camera is selected
	if (m_viewer->GetCurrentObjectType() == camera)
	{
		std::vector<std::string> names;
		for (const auto& ob : m_gameObjects)
			names.push_back(ob.name);
		m_viewer->SetObjectNames(names);
	}

	ImGui::Spacing();

	// Camera entry
	ImVec2 windowSize = ImGui::GetWindowSize();
	if (windowSize.x > 300) windowSize.x = 300;

	bool selected = (m_viewer->GetCurrentObjectType() == camera);
	if (ImGui::Selectable("Camera", selected, 0, ImVec2(windowSize.x - 24.0f, 20)))
	{
		m_viewer->SetObject(&m_camera, camera);

		std::vector<std::string> names;
		for (const auto& ob : m_gameObjects)
			names.push_back(ob.name);
		m_viewer->SetObjectNames(names);
	}

	ImGui::Unindent();

	// Re-sort if needed
	if (m_NewGameIndex == -1) {
		for (auto& ob : m_gameObjects) {
			if (ob.IsLevelChanged()) {
				quickSort(m_gameObjects, 0, static_cast<int>(m_gameObjects.size()) - 1);
				if (!m_currentObName.empty() && m_viewer->GetCurrentObjectType() == currentObjectType::game) {
					for (int i = 0; i < m_gameObjects.size(); i++) {
						if (m_gameObjects[i].name == m_currentObName)
							m_viewer->SetObject(&m_gameObjects[i], currentObjectType::game);
					}
				}
			}
		}
	}

	// Render hierarchy lists
	RenderObjectList();
	RenderLightList();
	RenderUiList();

	// Context menu for creating new objects
	float footerHeight = 40.0f;
	ImGui::SetCursorPosY(ImGui::GetWindowSize().y - footerHeight * 2);

	if (ImGui::BeginPopupContextWindow("CreateObjectContext", ImGuiPopupFlags_MouseButtonRight))
	{
		if (ImGui::BeginMenu("Create Object"))
		{
			if (ImGui::MenuItem("Light Object"))
			{
				m_lightObjects.emplace_back("Light " + std::to_string(m_lightObjects.size() + 1),
					ImVec2(0, 0), 0, 360, 0, ImVec4(255, 255, 255, 255), 200, 200);
				m_newLightIndex = static_cast<int>(m_lightObjects.size()) - 1;
			}

			if (ImGui::MenuItem("Game Object"))
			{
				m_gameObjects.emplace_back("Object " + std::to_string(m_gameObjects.size() + 1), "");
				m_NewGameIndex = static_cast<int>(m_gameObjects.size()) - 1;
			}

			if (ImGui::BeginMenu("UI"))
			{
				if (ImGui::MenuItem("Text"))
				{
					m_textObjects.emplace_back("New Text", "Text " + std::to_string(m_textObjects.size() + 1),
						ImVec4(255, 255, 255, 255), "default", ImVec4(0, 0, 0, 255),
						1.0f, 24, ImVec2(1.0f, 1.0f), ImVec2(0, 0), 0.0f);
					m_newTextIndex = static_cast<int>(m_textObjects.size()) - 1;
				}
				if (ImGui::MenuItem("Button"))
				{
					m_buttonObjects.emplace_back("New Button", "", "");
					m_newButtonIndex = static_cast<int>(m_buttonObjects.size()) - 1;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndPopup();
	}


	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(3);
	ImGui::End();
}


void SceneContent::RenderObjectList()
{
	ImVec2 windowSize = ImGui::GetWindowSize();
	if (windowSize.x > 300) windowSize.x = 300;

	if (!m_gameObjects.empty()) {
		ImGui::Text("Game Objects:");
	}

	ImGui::Indent();

	static char GameObNameInput[256] = "";
	static int lastNewGameIndex = -1;

	for (int i = 0; i < m_gameObjects.size(); ++i)
	{
		bool isNew = (i == m_NewGameIndex);

		if (isNew)
		{
			// Focus input text on creation
			if (m_NewGameIndex != lastNewGameIndex) {
				snprintf(GameObNameInput, sizeof(GameObNameInput), "Object %d", m_NewGameIndex + 1);
				lastNewGameIndex = m_NewGameIndex;
				ImGui::SetKeyboardFocusHere();
			}

			if (ImGui::InputText("##NewGameObject", GameObNameInput, IM_ARRAYSIZE(GameObNameInput),
				ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{
				m_gameObjects[m_NewGameIndex].name = GameObNameInput;
				m_NewGameIndex = -1;
				lastNewGameIndex = -1;

				quickSort(m_gameObjects, 0, m_gameObjects.size() - 1);
				if (!m_currentObName.empty() && m_viewer->GetCurrentObjectType() == currentObjectType::game) {
					for (int j = 0; j < m_gameObjects.size(); j++) {
						if (m_gameObjects[j].name == m_currentObName)
							m_viewer->SetObject(&m_gameObjects[j], currentObjectType::game);
					}
				}
			}

			// Right-click cancel
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
				m_gameObjects.erase(m_gameObjects.begin() + m_NewGameIndex);
				m_NewGameIndex = -1;
				lastNewGameIndex = -1;
			}

		}
		else
		{
			const char* name = m_gameObjects[i].name.empty() ? "Unnamed Object" : m_gameObjects[i].name.c_str();
			bool selected = (m_indexOfCurrentOb == i && m_viewer->GetCurrentObjectType() == currentObjectType::game);
			if (ImGui::Selectable(name, selected, 0, ImVec2(windowSize.x - 24.0f, 20)))
			{
				m_viewer->SetObject(&m_gameObjects[i], currentObjectType::game);
				m_indexOfCurrentOb = i;
				m_currentObName = m_gameObjects[i].name;
			}
		}
	}

	ImGui::Unindent();
}


void SceneContent::RenderLightList()
{
	ImVec2 windowSize = ImGui::GetWindowSize();
	if (windowSize.x > 300) windowSize.x = 300;

	if (!m_lightObjects.empty()) {
		ImGui::Separator();
		ImGui::Text("Light Objects:");
	}

	ImGui::Indent();

	static char lightNameInput[256] = "";
	static int lastNewLightIndex = -1;

	for (int i = 0; i < m_lightObjects.size(); i++)
	{
		bool isNew = (i == m_newLightIndex);

		if (isNew) {
			if (m_newLightIndex != lastNewLightIndex) {
				snprintf(lightNameInput, sizeof(lightNameInput), "Light %d", m_newLightIndex + 1);
				lastNewLightIndex = m_newLightIndex;
				ImGui::SetKeyboardFocusHere();
			}

			if (ImGui::InputText("##NewLight", lightNameInput, IM_ARRAYSIZE(lightNameInput),
				ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{
				m_lightObjects[m_newLightIndex].name = lightNameInput;
				m_newLightIndex = -1;
				lastNewLightIndex = -1;
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
				m_lightObjects.erase(m_lightObjects.begin() + m_newLightIndex);
				m_newLightIndex = -1;
				lastNewLightIndex = -1;
			}
		}
		else {
			const char* name = m_lightObjects[i].name.empty() ? "Unnamed Light" : m_lightObjects[i].name.c_str();
			bool selected = (m_indexOfCurrentOb == i && m_viewer->GetCurrentObjectType() == currentObjectType::light);
			if (ImGui::Selectable(name, selected, 0, ImVec2(windowSize.x - 24.0f, 20))) {
				m_viewer->SetObject(&m_lightObjects[i], currentObjectType::light);
				m_indexOfCurrentOb = i;
				m_currentObName = m_lightObjects[i].name;
			}
		}
	}

	ImGui::Unindent();
}


void SceneContent::RenderUiList()
{
	ImVec2 windowSize = ImGui::GetWindowSize();
	if (windowSize.x > 300) windowSize.x = 300;

	// --- Text Objects ---
	if (!m_textObjects.empty()) {
		ImGui::Separator();
		ImGui::Text("UI Texts:");
	}

	ImGui::Indent();

	static char TextNameInput[256] = "";
	static int lastNewTextIndex = -1;

	for (int i = 0; i < m_textObjects.size(); i++)
	{
		bool isNew = (i == m_newTextIndex);

		if (isNew) {
			if (m_newTextIndex != lastNewTextIndex) {
				snprintf(TextNameInput, sizeof(TextNameInput), "Text %d", m_newTextIndex + 1);
				lastNewTextIndex = m_newTextIndex;
				ImGui::SetKeyboardFocusHere();
			}

			if (ImGui::InputText("##NewText", TextNameInput, IM_ARRAYSIZE(TextNameInput),
				ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{
				m_textObjects[m_newTextIndex].name = TextNameInput;
				m_newTextIndex = -1;
				lastNewTextIndex = -1;
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
				m_textObjects.erase(m_textObjects.begin() + m_newTextIndex);
				m_newTextIndex = -1;
				lastNewTextIndex = -1;
			}
		}
		else {
			const char* name = m_textObjects[i].name.empty() ? "Unnamed Text" : m_textObjects[i].name.c_str();
			bool selected = (m_indexOfCurrentOb == i && m_viewer->GetCurrentObjectType() == currentObjectType::text);
			if (ImGui::Selectable(name, selected, 0, ImVec2(windowSize.x - 24.0f, 20))) {
				m_viewer->SetObject(&m_textObjects[i], currentObjectType::text);
				m_indexOfCurrentOb = i;
				m_currentObName = m_textObjects[i].name;
			}
		}
	}

	ImGui::Unindent();

	// --- Button Objects ---
	if (!m_buttonObjects.empty()) {
		ImGui::Separator();
		ImGui::Text("UI Buttons:");
	}

	ImGui::Indent();

	static char ButtonNameInput[256] = "";
	static int lastNewButtonIndex = -1;

	for (int i = 0; i < m_buttonObjects.size(); i++)
	{
		bool isNew = (i == m_newButtonIndex);

		if (isNew) {
			if (m_newButtonIndex != lastNewButtonIndex) {
				snprintf(ButtonNameInput, sizeof(ButtonNameInput), "Button %d", m_newButtonIndex + 1);
				lastNewButtonIndex = m_newButtonIndex;
				ImGui::SetKeyboardFocusHere();
			}

			if (ImGui::InputText("##NewButton", ButtonNameInput, IM_ARRAYSIZE(ButtonNameInput),
				ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			{
				m_buttonObjects[m_newButtonIndex].name = ButtonNameInput;
				m_newButtonIndex = -1;
				lastNewButtonIndex = -1;
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
				m_buttonObjects.erase(m_buttonObjects.begin() + m_newButtonIndex);
				m_newButtonIndex = -1;
				lastNewButtonIndex = -1;
			}
		}
		else {
			const char* name = m_buttonObjects[i].name.empty() ? "Unnamed Button" : m_buttonObjects[i].name.c_str();
			bool selected = (m_indexOfCurrentOb == i && m_viewer->GetCurrentObjectType() == currentObjectType::button);
			if (ImGui::Selectable(name, selected, 0, ImVec2(windowSize.x - 24.0f, 20))) {
				m_viewer->SetObject(&m_buttonObjects[i], currentObjectType::button);
				m_indexOfCurrentOb = i;
				m_currentObName = m_buttonObjects[i].name;
			}
		}
	}

	ImGui::Unindent();
}

static bool isScene(Object* obj, currentObjectType type)
{
	if (obj->IsSceneObject) {
		switch (type)
		{
		case game:
			((GameObject*)obj)->sprite->~Image();
			break;
		case light:
			((LightObject*)obj)->image->~Image();
			break;
		case text:
			((TextObject*)obj)->image->~Image();
			break;
		case button:
			((ButtonObject*)obj)->image->~Image();
			break;
		}
		return true;
	}
	return false;
}

void SceneContent::SetNewScene(const std::string& newScene)
{
	m_viewer->SetObject(nullptr, game);
	m_currentObName = "";
	m_gameObjects.erase(std::remove_if(m_gameObjects.begin(), m_gameObjects.end(),
		[](auto& obj) { return isScene(&obj, game); }), m_gameObjects.end());

	m_lightObjects.erase(std::remove_if(m_lightObjects.begin(), m_lightObjects.end(),
		[](auto& light) { return isScene(&light, currentObjectType::light); }), m_lightObjects.end());

	m_textObjects.erase(std::remove_if(m_textObjects.begin(), m_textObjects.end(),
		[](auto& text) { return isScene(&text, currentObjectType::text); }), m_textObjects.end());

	m_buttonObjects.erase(std::remove_if(m_buttonObjects.begin(), m_buttonObjects.end(),
		[](auto& button) { return  isScene(&button, currentObjectType::button); }), m_buttonObjects.end());

	std::ifstream inputFile(newScene);
	Json::Value actualJson;
	Json::Reader Reader;

	//check if the input file exsist
	if (!inputFile.is_open())
		return;

	//checing if the file can be read
	if (!Reader.parse(inputFile, actualJson))
		return;

	m_currentScene = newScene;

	int count = 0;

	//going over the json and reading all the data

	//camera
	Json::Value cameraJson = actualJson["camera"];
	if (cameraJson.isNull())
	{
		m_camera = Camera({ 0,0 }, 0);
	}
	else
	{
		ImVec2 position = ImVec2(cameraJson["position"][0].asFloat(), cameraJson["position"][1].asFloat());
		float rotation = cameraJson["rotation"].asFloat();
		float zoom = cameraJson["zoom"].asFloat();
		int darknessPrecent = cameraJson["darknessPrecent"].asInt();
		m_camera = Camera(position, rotation);
		m_camera.zoom = zoom;
		m_camera.objectName = cameraJson["objectName"].asString();
		m_camera.darknessPrecent = darknessPrecent;
	}


	//game
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
		bool isSceneObject = currentObject["isSceneObject"].asBool();
		std::string type = currentObject["type"].asString();

		std::string pathToSprite = GetDirForSprite(spriteName, m_directory->GetMainPath());

		m_gameObjects.emplace_back(name, pathToSprite, position, scale, rotation, layer, level);
		m_gameObjects.back().IsSceneObject = isSceneObject;
		m_gameObjects.back().type = type;
	}
	quickSort(m_gameObjects, 0, m_gameObjects.size() -1);

	//lights
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
		float radius = currentObject["radius"].asFloat();
		float rotation = currentObject["rotation"].asFloat();
		float angle = currentObject["angle"].asFloat();
		bool fade = currentObject["fade"].asBool();
		bool isSceneObject = currentObject["isSceneObject"].asBool();

		m_lightObjects.emplace_back(name, position, rotation, angle, type, color, radius);
		m_lightObjects.back().fade = fade;
		m_lightObjects.back().IsSceneObject = isSceneObject;

	}

	//text
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
		bool isSceneObject = currentObject["isSceneObject"].asBool();


		m_textObjects.emplace_back(value, name, color, font, outlineColor, outlineThickness, size, scale, position, rotation);
		m_textObjects.back().IsSceneObject = isSceneObject;
	}

	//buttons
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
		bool isSceneObject = currentObject["isSceneObject"].asBool();

		std::string pathToImg1 = GetDirForSprite(img, m_directory->GetMainPath());
		std::string pathToImg2 = GetDirForSprite(onClickImg, m_directory->GetMainPath());


		m_buttonObjects.emplace_back(name, pathToImg1, onclickfunc, pathToImg2, rotation, position, scale);
		m_buttonObjects.back().IsSceneObject = isSceneObject;
	}

	//close the file
	inputFile.close();
}

std::string SceneContent::GetCurrentScene()
{
	return m_currentScene;
}

std::string SceneContent::GetDirForSprite(const std::string& sprite, const std::string& dirStr) {
	std::filesystem::path dirPath = dirStr;


	if (dirPath.string().find("Resources") == std::string::npos) {
		dirPath /= "Resources";
	}

	
	if (!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath)) {
		return "";
	}

	for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
		if (entry.is_directory()) {
			std::string result = GetDirForSprite(sprite, entry.path().string());
			if (!result.empty()) {
				return result;
			}
		}
		else {
			std::string filename = entry.path().filename().string();
			if (filename.find(sprite) != std::string::npos) {
				return entry.path().string();
			}
		}
	}

	return "";
}


std::shared_ptr<ProjectDirectory> SceneContent::GetDir()
{
	return m_directory;
}
