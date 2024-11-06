#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/UI/UI.h"

#include "vector"
#include "objects/Objects.h"

#include "json/json.h"
#include "include/json/value.h"
#include <fstream>

enum currentObjectType {
	light,
	game
};


class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer() {
		std::string directoryPath = "C:\\github\\MicroEngine\\Resources\\graphics";
		for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
			if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg" || entry.path().extension() == ".jpeg") {
				try {
					images.push_back({ std::make_shared<Walnut::Image>(entry.path().string()), entry.path() });
				}
				catch (const std::exception& e) {
					// Handle image loading failure
					ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to load: %s", entry.path().filename().c_str());
				}
			}
		}
	}

	virtual void OnUIRender() override
	{
		if (saving) {
			Save();
		}
		if (loading) {
			Load();
		}

		FileExplorer();
		SceneViewer();
		ObjectViewer();
		SceneObjects();

		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
			selectedPath = "";
		}
	}

	void ShowAboutModal()
	{
		m_AboutModalOpen = true;
	}

	void ExampleLayer::FileExplorer() {
    ImGui::Begin("FileExplorer");
    ImGui::Text("Graphics Directory:");
    ImGui::SameLine();
    ImGui::TextWrapped("C:\\github\\MicroEngine\\Resources\\graphics");

    if (true) {
        int columnIndex = 0;
        const float thumbnailSize = 100.0f;
        const float spacing = 10.0f;
        const float boxPadding = 5.0f;
        float windowWidth = ImGui::GetContentRegionAvail().x;
        const int imagesPerRow = (int)(windowWidth / (thumbnailSize + 2*boxPadding + spacing));

        for (auto& [image, imagePath] : images) {
            if (columnIndex >= imagesPerRow) {
                columnIndex = 0;
                ImGui::NewLine();
            }

            if (image) {
                ImGui::BeginChild(imagePath.filename().string().c_str(), ImVec2(thumbnailSize + 2 * boxPadding, thumbnailSize + 2 * boxPadding + ImGui::GetTextLineHeightWithSpacing()));
                {
					ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]);
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
					ImGui::SetCursorPosX(boxPadding);
					ImGui::Image(image->GetDescriptorSet(), ImVec2(thumbnailSize, thumbnailSize));
					if (ImGui::IsItemHovered()) {
						ImGui::BeginTooltip();
						ImGui::Text("%s", imagePath.filename().string().c_str());
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked()) {
						selectedPath = imagePath.string();
					}
					ImGui::PopStyleVar();
					ImGui::PopStyleColor();
				}
				ImGui::EndChild();
				ImGui::SameLine();
				columnIndex++;
			}
		}
	}
 else {
	 ImGui::Text("No images found in the directory.");
	}

	ImGui::End();
}

	void SceneViewer() {
	ImGui::Begin("SceneViewer");
	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	ImGui::GetWindowDrawList()->AddRectFilled(
		ImGui::GetCursorScreenPos(),
		ImVec2(ImGui::GetCursorScreenPos().x + contentRegion.x, ImGui::GetCursorScreenPos().y + contentRegion.y),
		IM_COL32(0, 0, 0, 255) // Black
	);

	RenderGameObjects();
	RenderLightObjects();

	ImGui::End();
}

	void RenderGameObjects() {

	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	for (auto& gameObject : m_GameObjects) {
		if (gameObject.sprite) {
			ImGui::SetCursorPos(ImVec2(
				gameObject.position.x + contentRegion.x / 2 - (float)gameObject.sprite->GetWidth() / 2, // Move 120px to the left
				gameObject.position.y + contentRegion.y / 2 - (float)gameObject.sprite->GetHeight() / 2
			));
			ImGui::Image(gameObject.sprite->GetDescriptorSet(), {gameObject.scale.x * (float)gameObject.sprite->GetWidth(), gameObject.scale.y * (float)gameObject.sprite->GetHeight()});
		}
	}
}

	void RenderLightObjects() {
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		for (auto& light : m_LightObjects) {
			ImVec2 lightPosition = ImVec2(
				light.position.x + contentRegion.x / 2,
				light.position.y + contentRegion.y / 2
			);
		}
	}

	void ObjectViewer() {
	ImGui::Begin("ObjectViewer");

	if (currentObject == nullptr) {
		ImGui::Text("No Object Was Selected");
	}
	else {
		switch (m_CurrentObjectType) {
		case currentObjectType::game:
			DisplayGameObject();
			break;
		case currentObjectType::light:
			DisplayLightObject();
		}
	}



	ImGui::End();
}

	void SceneObjects() {
	ImGui::Begin("SceneObjects");

	bool exist = false;

	if (m_GameObjects.size() == 0 && m_LightObjects.size() == 0) {
		ImGui::Text("No Objects");
	}
	else {
		exist = true;
		ImGui::Text("Game Object:");
		ImGui::Indent();
	}

	RenderObjectList();
	RenderLightList();

	float footerHeight = 40.0f;
	ImGui::SetCursorPosY(ImGui::GetWindowSize().y - footerHeight*2);


	ImGui::Separator();


	if (ImGui::Button("Create Light", ImVec2(ImGui::GetWindowSize().x, 30))) {

		LightObject newLight("New Light");
		m_LightObjects.push_back(newLight);


		m_NewLightIndex = m_LightObjects.size() - 1;
	}
	if (ImGui::Button("Create GameObject", ImVec2(ImGui::GetWindowSize().x, 30))){
		GameObject newGameObject("New Object","");
		m_GameObjects.push_back(newGameObject);

		m_NewGameObIndex = m_GameObjects.size() - 1;
	}


		ImGui::End();
	}

	void RenderObjectList() {
	ImVec2 windowSize = ImGui::GetWindowSize();
	if (windowSize.x > 300) windowSize.x = 300;
	for (int i = 0; i < m_GameObjects.size(); i++) {
		if (ImGui::Button(m_GameObjects[i].name.c_str(), ImVec2(windowSize.x - 20.0f, 30))) {
			currentObject = &m_GameObjects[i];
			m_CurrentObjectType = currentObjectType::game;
		}
	}

	if (m_NewGameObIndex > -1) {
		static char GameObNameInput[256] = "New GameObject";
		ImGui::InputText("##Enter GameObject Name", GameObNameInput, IM_ARRAYSIZE(GameObNameInput));
		if (ImGui::Button("Cancel")) {
			m_GameObjects.erase(m_GameObjects.begin() + m_NewGameObIndex);
			m_NewGameObIndex = -1;
		}
		ImGui::SameLine();
		if (ImGui::Button("Confirm")) {
			m_GameObjects[m_NewGameObIndex].name = GameObNameInput;
			m_NewGameObIndex = -1;
		}
	}
}

	void RenderLightList() {
	ImVec2 windowSize = ImGui::GetWindowSize();
	if (windowSize.x > 300) windowSize.x = 300;
	if (m_LightObjects.size() != 0) {
		ImGui::Unindent();
		ImGui::Separator();
		ImGui::Text("Light Object:");
		ImGui::Indent();
	}

	for (int i = 0; i < m_LightObjects.size(); i++) {
		if (ImGui::Button(m_LightObjects[i].name.c_str(), ImVec2(windowSize.x - 20.0f, 30))) {
			currentObject = &m_LightObjects[i];
			m_CurrentObjectType = currentObjectType::light;
		}
	}

	ImGui::Unindent();

	if (m_NewLightIndex > -1) {
		static char lightNameInput[256] = "New Light";
		ImGui::InputText("##Enter Light Name", lightNameInput, IM_ARRAYSIZE(lightNameInput));
		if (ImGui::Button("Cancel")) {
			m_LightObjects.erase(m_LightObjects.begin() + m_NewLightIndex);
			m_NewLightIndex = -1;
		}
		ImGui::SameLine();
		if (ImGui::Button("Confirm")) {
			m_LightObjects[m_NewLightIndex].name = lightNameInput;
			m_NewLightIndex = -1;
		}
	}
}

	void DisplayGameObject() {
		ImGui::Text("Object Details:");
		ImGui::Separator();

		GameObject* GameOj = (GameObject*)currentObject;

		ImGui::Text("Name:");
		ImGui::Indent();

		static char nameBuffer[128];
		strcpy_s(nameBuffer, sizeof(nameBuffer), currentObject->name.c_str());
		if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer)))
		{
			currentObject->name = nameBuffer;
		}

		ImGui::Text("Position:");
		ImGui::Indent();

		float positionValues[2] = { currentObject->position.x, currentObject->position.y };
		if (ImGui::InputFloat2("##Position", positionValues))
		{
			currentObject->position.x = positionValues[0];
			currentObject->position.y = positionValues[1];
		}
		ImGui::Unindent();

		ImGui::Text("Rotation (Degrees):");
		ImGui::Indent();
		if (ImGui::InputFloat("##Rotation", &currentObject->rotation))
		{
			currentObject->rotation = fmodf(currentObject->rotation, 360.0f);
		}
		ImGui::Unindent();

		ImGui::Text("Scale:");
		ImGui::Indent();

		float scaleValues[2] = { GameOj->scale.x, GameOj->scale.y };
		if (ImGui::InputFloat2("##Scale", scaleValues))
		{
			GameOj->scale.x = scaleValues[0];
			GameOj->scale.y = scaleValues[1];
		}
		ImGui::Unindent();

		ImGui::Text("Path:");
		ImGui::Indent();
		static char pathBuffer[256] = "";
		if (ImGui::InputText("##Path", pathBuffer, sizeof(pathBuffer)))
		{
			// If user manually edits the path
			GameOj->SetPath(selectedPath);
		}

		if (selectedPath != "" && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
			// If user drops the path by clicking
			strcpy_s(pathBuffer, sizeof(pathBuffer), selectedPath.c_str());
			GameOj->SetPath(selectedPath);
			selectedPath = ""; // Clear the selected path
		}

		// Visual indicator for drag-and-drop
		if (ImGui::IsItemHovered()) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		}
		ImGui::Unindent();

		ImGui::Text("Sprite:");
		ImGui::Indent();
		if (GameOj->sprite)
			ImGui::Image(GameOj->sprite->GetDescriptorSet(), {(float)GameOj->sprite->GetWidth(), (float)GameOj->sprite->GetHeight()});
		ImGui::Unindent();
	}

	void DisplayLightObject(){
		ImGui::Text("Object Details:");
		ImGui::Separator();

		LightObject* Light = (LightObject*) currentObject;

		ImGui::Text("Name:");
		ImGui::Indent();

		static char nameBuffer[128];
		strcpy_s(nameBuffer, sizeof(nameBuffer), currentObject->name.c_str());
		if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer)))
		{
			currentObject->name = nameBuffer;
		}

		ImGui::Text("Position:");
		ImGui::Indent();

		float positionValues[2] = { currentObject->position.x, currentObject->position.y };
		if (ImGui::InputFloat2("##Position", positionValues))
		{
			currentObject->position.x = positionValues[0];
			currentObject->position.y = positionValues[1];
		}
		ImGui::Unindent();

		ImGui::Text("Rotation (Degrees):");
		ImGui::Indent();
		if (ImGui::InputFloat("##Rotation", &currentObject->rotation))
		{
			currentObject->rotation = fmodf(currentObject->rotation, 360.0f);
		}
		ImGui::Unindent();

		ImGui::Text("Color:");
		ImGui::Indent();
		float colorValues[3] = { Light->color.r, Light->color.g, Light->color.b };
		if (ImGui::ColorEdit3("##Color", colorValues))
		{
			Light->color.r = colorValues[0];
			Light->color.g = colorValues[1];
			Light->color.b = colorValues[2];
		}
		ImGui::Unindent();

		ImGui::Text("Intensity:");
		ImGui::Indent();
		if (ImGui::InputFloat("##Intensity", &Light->color.a))
		{
			Light->color.a = fmodf(Light->color.a, 255.0f);
		}
		ImGui::Unindent();

		ImGui::Text("Type:");
		ImGui::Indent();
		if (ImGui::RadioButton("Spot", Light->type == 0)) {
			Light->type = 0;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Flash", Light->type == 1)) {
			Light->type = 1;
		}
		ImGui::Unindent();

		ImGui::Text("Radius");
		ImGui::Indent();
		if (ImGui::InputFloat("##Radius", &Light->radius))
		{
			Light->radius = fmodf(Light->radius, 1000000000000000000);
		}
		ImGui::Unindent();
	}

	void Save() {
	ImGui::Begin("Save");
	ImGui::Text("Save Scene");

	ImGui::Text("Path:");
	ImGui::Indent();
	std::string path = "C:\\github\\MicroEngine\\Resources\\Scenes\\scene.json";

	static char pathBuffer[512];
	strcpy_s(pathBuffer, sizeof(pathBuffer), "C:\\github\\MicroEngine\\Resources\\Scenes");
	if (ImGui::InputText("##Path", pathBuffer, sizeof(pathBuffer)))
	{
		path = pathBuffer;
	}
	ImGui::Unindent();

	ImGui::Text("File Name:");
	ImGui::Indent();

	static char fileNameBuffer[128];
	strcpy_s(fileNameBuffer, sizeof(fileNameBuffer), "scene");
	if (ImGui::InputText("##FileName", fileNameBuffer, sizeof(fileNameBuffer))) {
		path += "\\";
		path += fileNameBuffer;
		path += ".json";
	}
	ImGui::Unindent();

	bool existFlag = false;
	bool unkownPFlag = false;
	bool exit = false;
	if (ImGui::Button("Save")) {

		if (std::filesystem::exists(path)) {
			existFlag = true;
		}
		else {
			// Serialize scene data to JSON
			Json::Value sceneJson;

			// Game Objects
			for (int i = 0; i < m_GameObjects.size(); i++) {
				std::string objectKey = "object" + std::to_string(i);
				Json::Value gameObjectJson;
				gameObjectJson["layer"] = m_GameObjects[i].layer;
				gameObjectJson["position"][0] = m_GameObjects[i].position.x;
				gameObjectJson["position"][1] = m_GameObjects[i].position.y;
				gameObjectJson["rotation"] = m_GameObjects[i].rotation;
				gameObjectJson["scale"][0] = m_GameObjects[i].scale.x;
				gameObjectJson["scale"][1] = m_GameObjects[i].scale.y;
				gameObjectJson["spriteName"] = m_GameObjects[i].spritename;
				gameObjectJson["name"] = m_GameObjects[i].name;
				sceneJson[objectKey] = gameObjectJson;
			}

			// Light Objects
			for (int i = 0; i < m_LightObjects.size(); i++) {
				std::string lightKey = "lightSource" + std::to_string(i);
				Json::Value lightObjectJson;
				lightObjectJson["LightType"] = m_LightObjects[i].type;
				lightObjectJson["name"] = m_LightObjects[i].name;
				lightObjectJson["position"][0] = m_LightObjects[i].position.x;
				lightObjectJson["position"][1] = m_LightObjects[i].position.y;
				lightObjectJson["color"][0] = m_LightObjects[i].color.r;
				lightObjectJson["color"][1] = m_LightObjects[i].color.g;
				lightObjectJson["color"][2] = m_LightObjects[i].color.b;
				lightObjectJson["color"][3] = m_LightObjects[i].color.a; // Intensity
				lightObjectJson["angle"] = m_LightObjects[i].rotation; // Assuming rotation is the angle
				lightObjectJson["radius"] = m_LightObjects[i].radius;
				sceneJson[lightKey] = lightObjectJson;
			}

			// Write JSON to file
			std::ofstream outputFile(path);
			if (outputFile.is_open()) {
				outputFile << sceneJson.toStyledString();
				outputFile.close();
				exit = true;
			}
			else {
				unkownPFlag = true;
			}
		}
	}

	if (existFlag)
		ImGui::Text("this file already exist");
	else if (unkownPFlag)
		ImGui::Text("Unable to create scene");
	if (existFlag)
		saving = false;

	ImGui::End();
}

	void Load() {
		ImGui::Begin("Load");
		ImGui::End();
	}

public:
	std::vector<GameObject> m_GameObjects;
	std::vector <LightObject> m_LightObjects;

	bool saving = false;
	bool loading = false;
private:
	bool m_AboutModalOpen = false;
	std::vector<std::pair<std::shared_ptr<Walnut::Image>, std::filesystem::path>> images;

	Object* currentObject = nullptr;
	currentObjectType m_CurrentObjectType;

	int m_NewLightIndex = -1;
	int m_NewGameObIndex = -1;

	std::string selectedPath;

};

void SaveMap(std::shared_ptr<ExampleLayer> layer) {
	layer->saving = true;
}

void LoadMap(std::shared_ptr<ExampleLayer> layer) {
	layer->loading = true;
}

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Scene Editor";
	spec.CustomTitlebar = true;


	Walnut::Application* app = new Walnut::Application(spec);
	std::shared_ptr<ExampleLayer> exampleLayer = std::make_shared<ExampleLayer>();
	exampleLayer->m_GameObjects.push_back(GameObject("test1", ""));
	exampleLayer->m_LightObjects.push_back(LightObject("light1"));
	app->PushLayer(exampleLayer);


	app->SetMenubarCallback([app, exampleLayer]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save")) {
				SaveMap(exampleLayer);
			}

			if (ImGui::MenuItem("Load")) {
				LoadMap(exampleLayer);
			}

			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}