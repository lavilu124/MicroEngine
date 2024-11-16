#include "Saves.h"


#include "../json/json.h"
#include "../include/json/value.h"
#include <fstream>

Saves::Saves(std::shared_ptr<SceneContent> sceneContent) : m_sceneContent(sceneContent)
{
}

void Saves::OnUIRender()
{
	Window();
}

void Saves::Save()
{
	m_saving = true;
	m_loading = false;
	done = false;
}

void Saves::Load()
{
	m_loading = true;
	m_saving = false;
	done = false;
}

void Saves::Window()
{
	if (m_saving)
	{
		SaveWindow();	
	}else if (m_loading)
	{
		LoadWindow();
	}
}

void Saves::SaveWindow()
{
	ImGui::Begin("Save");
	ImGui::Text("Save Scene");

	ImGui::Text("Path:");
	ImGui::Indent();

	static char pathBuffer[512];
	strcpy_s(pathBuffer, sizeof(pathBuffer), "C:\\github\\MicroEngine\\Resources\\Scenes");
	static char fileNameBuffer[128];
	strcpy_s(fileNameBuffer, sizeof(fileNameBuffer), m_path.substr(m_path.find_last_of('\\') + 1).c_str());

	//...

	if (ImGui::InputText("##Path", pathBuffer, sizeof(pathBuffer))) {
		// Update base path
		m_path = pathBuffer; // Assuming path is a std::string or similar
	}

	ImGui::Unindent();

	ImGui::Text("File Name:");
	ImGui::Indent();

	if (ImGui::InputText("##FileName", fileNameBuffer, sizeof(fileNameBuffer))) {
		// Reset path to base directory and then append file name with extension
		m_path = pathBuffer; // Reset to base directory
		m_path += "\\" + std::string(fileNameBuffer) + ".json";
	}
	ImGui::Unindent();

	bool existFlag = false;
	bool unkownPFlag = false;
	if (ImGui::Button("Save")) {

		if (std::filesystem::exists(m_path)) {
			existFlag = true;
		}
		else {
			// Serialize scene data to JSON
			Json::Value sceneJson;

			// Game Objects
			for (int i = 0; i < m_sceneContent->GetGameObjects().size(); i++) {
				std::string objectKey = "object" + std::to_string(i);
				Json::Value gameObjectJson;
				gameObjectJson["layer"] = m_sceneContent->GetGameObjects()[i].layer;
				gameObjectJson["position"][0] = m_sceneContent->GetGameObjects()[i].position.x;
				gameObjectJson["position"][1] = m_sceneContent->GetGameObjects()[i].position.y;
				gameObjectJson["rotation"] = m_sceneContent->GetGameObjects()[i].rotation;
				gameObjectJson["scale"][0] = m_sceneContent->GetGameObjects()[i].scale.x;
				gameObjectJson["scale"][1] = m_sceneContent->GetGameObjects()[i].scale.y;
				gameObjectJson["spriteName"] = m_sceneContent->GetGameObjects()[i].spritename;
				gameObjectJson["name"] = m_sceneContent->GetGameObjects()[i].name;
				sceneJson[objectKey] = gameObjectJson;
			}

			// Light Objects
			for (int i = 0; i < m_sceneContent->GetLights().size(); i++) {
				std::string lightKey = "lightSource" + std::to_string(i);
				Json::Value lightObjectJson;
				lightObjectJson["LightType"] = m_sceneContent->GetLights()[i].type;
				lightObjectJson["name"] = m_sceneContent->GetLights()[i].name;
				lightObjectJson["position"][0] = m_sceneContent->GetLights()[i].position.x;
				lightObjectJson["position"][1] = m_sceneContent->GetLights()[i].position.y;
				lightObjectJson["color"][0] = m_sceneContent->GetLights()[i].color.Value.x;
				lightObjectJson["color"][1] = m_sceneContent->GetLights()[i].color.Value.y;
				lightObjectJson["color"][2] = m_sceneContent->GetLights()[i].color.Value.z;
				lightObjectJson["color"][3] = m_sceneContent->GetLights()[i].color.Value.w; // Intensity
				lightObjectJson["angle"] = m_sceneContent->GetLights()[i].rotation; // Assuming rotation is the angle
				lightObjectJson["radius"] = m_sceneContent->GetLights()[i].radius;
				sceneJson[lightKey] = lightObjectJson;
			}

			// Write JSON to file
			std::ofstream outputFile(m_path);
			if (outputFile.is_open()) {
				outputFile << sceneJson.toStyledString();
				outputFile.close();
				done = true;
			}
			else {
				unkownPFlag = true;
			}
		}
	}

	if (ImGui::Button("close"))
	{
		m_saving = false;
		done = false;
		ImGui::End();
		return;
	}

	if (existFlag)
		ImGui::Text("this file already exist");
	else if (unkownPFlag)
		ImGui::Text("Unable to create scene");
	else if (done)
		ImGui::Text("Scene saved successfully");

	ImGui::End();
}

void Saves::LoadWindow()
{
	ImGui::Begin("Load");
	if (ImGui::Button("close"))
	{
		m_saving = false;
		done = false;
		ImGui::End();
		return;
	}
	ImGui::End();
}
