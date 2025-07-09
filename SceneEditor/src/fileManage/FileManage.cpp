#include "FileManage.h"

#include <filesystem>
#include "../json/json.h"
#include "../include/json/value.h"
#include <fstream>

namespace fs = std::filesystem;

void FileManage::SaveScene(const std::string& path, SceneContent* content)
{
	if (fs::exists(path)) {
		std::remove(path.c_str());
	}

	Json::Value sceneJson;

	//Camera
	const auto& camera = content->GetCam();
	Json::Value cameraJson;
	cameraJson["position"][0] = camera.position.x;
	cameraJson["position"][1] = camera.position.y;
	cameraJson["rotation"] = camera.rotation;
	cameraJson["zoom"] = camera.zoom;
	cameraJson["darknessPrecent"] = camera.darknessPrecent;
	cameraJson["objectName"] = camera.objectName;
	sceneJson["camera"] = cameraJson;

	// Game 
	for (int i = 0; i < content->GetGameObjects().size(); i++) {
		std::string objectKey = "object" + std::to_string(i);
		const auto& obj = content->GetGameObjects()[i];
		Json::Value gameObjectJson;
		gameObjectJson["layer"] = obj.layer;
		gameObjectJson["level"] = obj.level;
		gameObjectJson["position"][0] = obj.position.x;
		gameObjectJson["position"][1] = obj.position.y;
		gameObjectJson["rotation"] = obj.rotation;
		gameObjectJson["scale"][0] = obj.scale.x;
		gameObjectJson["scale"][1] = obj.scale.y;
		gameObjectJson["spriteName"] = obj.spritename;
		gameObjectJson["name"] = obj.name;
		gameObjectJson["type"] = (obj.type == "") ? "none" : obj.type;
		gameObjectJson["isSceneObject"] = obj.IsSceneObject;
		sceneJson[objectKey] = gameObjectJson;
	}

	// Light 
	for (int i = 0; i < content->GetLights().size(); i++) {
		std::string lightKey = "lightSource" + std::to_string(i);
		const auto& light = content->GetLights()[i];
		Json::Value lightObjectJson;
		lightObjectJson["LightType"] = light.type;
		lightObjectJson["name"] = light.name;
		lightObjectJson["position"][0] = light.position.x;
		lightObjectJson["position"][1] = light.position.y;
		lightObjectJson["color"][0] = light.color.x;
		lightObjectJson["color"][1] = light.color.y;
		lightObjectJson["color"][2] = light.color.z;
		lightObjectJson["color"][3] = light.color.w;
		lightObjectJson["rotation"] = light.rotation;
		lightObjectJson["radius"] = light.radius;
		lightObjectJson["fade"] = light.fade;
		if (light.type == 0) {
			lightObjectJson["angle"] = light.beamAngle;
		}
		else {
			lightObjectJson["width"] = light.width;
		}
		lightObjectJson["isSceneObject"] = light.IsSceneObject;
		sceneJson[lightKey] = lightObjectJson;
	}

	// Text 
	for (int i = 0; i < content->GetTexts().size(); i++) {
		std::string textKey = "text" + std::to_string(i);
		const auto& text = content->GetTexts()[i];
		Json::Value textJson;
		textJson["name"] = text.name;
		textJson["font"] = text.font;
		textJson["rotation"] = text.rotation;
		textJson["position"][0] = text.position.x;
		textJson["position"][1] = text.position.y;
		textJson["color"][0] = text.color.x;
		textJson["color"][1] = text.color.y;
		textJson["color"][2] = text.color.z;
		textJson["color"][3] = text.color.w;
		textJson["outlineColor"][0] = text.outlineColor.x * 255.0f;
		textJson["outlineColor"][1] = text.outlineColor.y * 255.0f;
		textJson["outlineColor"][2] = text.outlineColor.z * 255.0f;
		textJson["outlineThickness"] = text.outlineThickness;
		textJson["size"] = text.size;
		textJson["scale"][0] = text.scale.x;
		textJson["scale"][1] = text.scale.y;
		textJson["value"] = text.value;
		textJson["isSceneObject"] = text.IsSceneObject;
		sceneJson[textKey] = textJson;
	}

	// Button 
	for (int i = 0; i < content->GetButtons().size(); i++) {
		std::string buttonKey = "button" + std::to_string(i);
		const auto& button = content->GetButtons()[i];
		Json::Value buttonJson;
		buttonJson["name"] = button.name;
		buttonJson["img"] = button.img;
		buttonJson["onClickImg"] = button.onClickImg;
		buttonJson["onClickFunc"] = button.onClickFunc;
		buttonJson["rotation"] = button.rotation;
		buttonJson["position"][0] = button.position.x;
		buttonJson["position"][1] = button.position.y;
		buttonJson["scale"][0] = button.scale.x;
		buttonJson["scale"][1] = button.scale.y;
		buttonJson["isSceneObject"] = button.IsSceneObject;
		sceneJson[buttonKey] = buttonJson;
	}


	std::ofstream outputFile(path);
	if (outputFile.is_open()) {
		outputFile << sceneJson.toStyledString();
		outputFile.close();
	}

	content->GetDir()->HandleFile(std::filesystem::directory_entry(path));
}

