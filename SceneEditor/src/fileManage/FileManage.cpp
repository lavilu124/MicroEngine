#include "FileManage.h"

#include <filesystem>
#include "../json/json.h"
#include "../include/json/value.h"
#include <fstream>

namespace fs = std::filesystem;

void FileManage::SaveScene(std::string path, SceneContent* content)
{
	if (fs::exists(path)) {
		std::remove(path.c_str());
	}

	// Serialize scene data to JSON
	Json::Value sceneJson;

	// Game Objects
	for (int i = 0; i < content->GetGameObjects().size(); i++) {
		std::string objectKey = "object" + std::to_string(i);
		Json::Value gameObjectJson;
		gameObjectJson["layer"] = content->GetGameObjects()[i].layer;
		gameObjectJson["level"] = content->GetGameObjects()[i].level;
		gameObjectJson["position"][0] = content->GetGameObjects()[i].position.x;
		gameObjectJson["position"][1] = content->GetGameObjects()[i].position.y;
		gameObjectJson["rotation"] = content->GetGameObjects()[i].rotation;
		gameObjectJson["scale"][0] = content->GetGameObjects()[i].scale.x;
		gameObjectJson["scale"][1] = content->GetGameObjects()[i].scale.y;
		gameObjectJson["spriteName"] = content->GetGameObjects()[i].spritename;
		gameObjectJson["name"] = content->GetGameObjects()[i].name;
		std::string type = content->GetGameObjects()[i].type;
		gameObjectJson["type"] = (type == "")? "none" : type;
		sceneJson[objectKey] = gameObjectJson;
	}

	// Light Objects
	for (int i = 0; i < content->GetLights().size(); i++) {
		std::string lightKey = "lightSource" + std::to_string(i);
		Json::Value lightObjectJson;
		lightObjectJson["LightType"] = content->GetLights()[i].type;
		lightObjectJson["name"] = content->GetLights()[i].name;
		lightObjectJson["position"][0] = content->GetLights()[i].position.x;
		lightObjectJson["position"][1] = content->GetLights()[i].position.y;
		lightObjectJson["color"][0] = content->GetLights()[i].color.x;
		lightObjectJson["color"][1] = content->GetLights()[i].color.y;
		lightObjectJson["color"][2] = content->GetLights()[i].color.z;
		lightObjectJson["color"][3] = content->GetLights()[i].color.w * 255.0f; 
		lightObjectJson["rotation"] = content->GetLights()[i].rotation;
		if (content->GetLights()[i].type == 0) {
			lightObjectJson["angle"] = content->GetLights()[i].beamAngle;
		}
		else {
			lightObjectJson["width"] = content->GetLights()[i].width;
		}
		lightObjectJson["radius"] = content->GetLights()[i].radius;
		lightObjectJson["fade"] = content->GetLights()[i].fade;
		sceneJson[lightKey] = lightObjectJson;
	}

	// Write JSON to file
	std::ofstream outputFile(path);
	if (outputFile.is_open()) {
		outputFile << sceneJson.toStyledString();
		outputFile.close();
	}
	content->GetDir()->HandleFile(std::filesystem::directory_entry(path));
}
