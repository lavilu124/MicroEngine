#include "FileManager.h"
#include "..//SystemLogic/GameLogic/Collision.hpp"
#include "../SystemLogic/SystemManager.h"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>
#include "json/json.h"
#include "value.h"

#include "../../lightInclude.h"

using std::filesystem::directory_iterator;


int Micro::FileManager::s_currentLog = 0;
std::string Micro::FileManager::s_mainPath = "";

namespace Micro{


    void FileManager::AddInputFunc(const std::string& name, void(*function)(Input::InputAction&)) {
        m_functionMap[name] = function;
        LoadInput();
    }

    void FileManager::AddButtonFunc(const std::string& name, void(*function)())
    {
        m_buttonFuncMap[name] = function;

    }
    void FileManager::CreateInput(const std::string& name, Input::inputType type, Input::KeyType key, Input::inputPart part, const std::string& onInput, const std::string& offInput) {
        if (!onInput.empty() && !offInput.empty()) {
            inputs.emplace(name, Input::InputAction(
                type,
                key,
                name,
                part,
                m_functionMap[onInput],
                m_functionMap[offInput]
            ));
        }
        else if (!onInput.empty()) {
            inputs.emplace(name, Input::InputAction(
                type,
                key,
                name,
                part,
                m_functionMap[onInput]
            ));
        }
        else if (!offInput.empty()){
            inputs.emplace(name, Input::InputAction(
                type,
                key,
                name,
                part,
                nullptr,
                m_functionMap[offInput]
            ));
        }
        else {
            inputs.emplace(name, Input::InputAction(
                type,
                key,
                name,
                part
            ));
        }
        
    }

    void FileManager::addInput(Input::InputAction& action)
    {
        inputs.emplace(action.GetName(), action);
    }
    void FileManager::LoadInput() {
        std::string path;

        std::filesystem::path dir = "../";

        while (!dir.empty()) {
            std::filesystem::path resourcesPath = dir / "Resources";

            if (std::filesystem::exists(resourcesPath) && std::filesystem::is_directory(resourcesPath)) {
                path = resourcesPath.string();
                break;
            }

            dir = dir.parent_path();
        }

        path += "\\settings\\Input.cfg";
        std::ifstream file(path);
        std::string line;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                // Skip empty lines and comments
                continue;
            }

            std::istringstream iss(line);
            std::string Type, Key, Name, Part, OnInput, OffInput;


            std::getline(iss, Type, ',');
            std::getline(iss, Key, ',');
            std::getline(iss, Name, ',');
            std::getline(iss, Part, ',');
            std::getline(iss, OnInput, ',');
            std::getline(iss, OffInput, ',');

            if (!Name.empty() && Name[0] == ' ') {
                Name.erase(0, 1);
            }

            if (!Part.empty() && Part[0] == ' ') {
                Part.erase(0, 1);
            }

            if (!OnInput.empty() && OnInput[0] == ' ') {
                OnInput.erase(0, 1);
            }

            if (!OffInput.empty() && OffInput[0] == ' ') {
                OffInput.erase(0, 1);
            }


            // Check if OnInput function exists in FunctionMap
            if (m_functionMap.find(OnInput) == m_functionMap.end()) {
                continue;
            }

            Input::inputPart inputPart = Input::inputPart::Pressed;
            if (Part == "started") {
                inputPart = Input::inputPart::Started;
            }
            else if (Part == "released") {
                inputPart = Input::inputPart::Released;
            }

            Input::inputType InputType = Input::inputType::KeyboardKey;
            Input::KeyType InputKey;
            if (Type == "MouseKey") {
                InputKey = static_cast<sf::Mouse::Button>(std::stoi(Key));
                InputType = Input::inputType::MouseButton;
            }
            else if (Type == "ControllerKey") {
                InputKey = static_cast<unsigned int>(std::stoi(Key));
                InputType = Input::inputType::ControllerButton;
            }
            else if (Type == "KeyboardKey") {
                InputKey = static_cast<sf::Keyboard::Key>(std::stoi(Key));
            }
            else if (Type == "MouseMove") {
                InputKey = static_cast<unsigned int>(std::stoi(Key));
                InputType = Input::inputType::MouseMove;
            }
            else {
                InputKey = static_cast<unsigned int>(std::stoi(Key));
                InputType = Input::inputType::JoystickMove;
            }

            CreateInput(Name, InputType, InputKey, inputPart, OnInput, OffInput);
        }
    }

    void FileManager::SetPaths() {
        std::string Folders[] = { "\\Resources\\sounds" }; // "\\Resources\\graphics", 
        std::string path = std::filesystem::current_path().string();

        std::ofstream CurFile(path);

        for (int i = 0; i < sizeof(Folders) / sizeof(Folders[0]); i++)
        {
            if (!std::filesystem::directory_entry{ path + Folders[i] }.exists()) {
                const std::string::size_type pos = path.find_last_of("\\");
                if (pos != std::string::npos) {
                    path.erase(pos);
                }
                else {
                    break;
                }
                i -= 1;
                continue;
            }

            s_mainPath = path;
            GetFilesInDir(path + Folders[i]);
        }
    }

    int GetFileCount(const std::string& directory) {

        int FileCount = 0;
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                FileCount++;
            }
        }

        return FileCount;
    }

    void FileManager::GetFilesInDir(const std::string& dir) {

        for (const auto& File : directory_iterator(dir)) {
            std::string pathString{ File.path().u8string() };

            std::string fileName = pathString.substr(pathString.find_last_of("\\") + 1, pathString.length() - 1);
            LoadAsset(fileName);
        }
    }

    void FileManager::LoadAsset(const std::string& fileName) {
        if (m_sprites.find(fileName) != m_sprites.end() || m_sounds.find(fileName) != m_sounds.end()) {
            return;
        }

        for (const auto& entry : std::filesystem::recursive_directory_iterator(s_mainPath + "\\Resources\\")) {
            if (entry.is_regular_file() && entry.path().extension() == ".png" && entry.path().filename().string() == fileName) {

                sf::Texture NewTexture;
                if (NewTexture.loadFromFile(entry.path().string())) {
                    m_textures[fileName] = NewTexture;

                    sf::Sprite NewSprite;
                    NewSprite.setTexture(m_textures[fileName]);
                    m_sprites[fileName] = NewSprite;
                }

            }
            else if ((entry.is_regular_file() && entry.path().extension() == ".wav" || entry.is_regular_file() && entry.path().extension() == ".ogg" || entry.is_regular_file() && entry.path().extension() == ".mp3")
                && entry.path().filename().string() == fileName) {

                sf::SoundBuffer NewBuffer;
                if (NewBuffer.loadFromFile(entry.path().string())) {
                    m_buffers[fileName] = NewBuffer;

                    sf::Sound newSound;
                    newSound.setBuffer(m_buffers[fileName]);
                    m_sounds[fileName] = newSound;
                }
            }
        }
    }

    std::vector<std::shared_ptr<GameObject>> FileManager::GetObjects(const std::string& name, SystemManager* systemManager) {
        std::string path;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(s_mainPath + "\\Resources\\"))
        {
            if (entry.is_regular_file() &&  entry.path().filename().string() == name + ".McScene")
            {
				path = entry.path().string();
            }
        }

        std::ifstream inputFile(path);
        Json::Value actualJson;
        Json::Reader Reader;

        std::vector<std::shared_ptr<Micro::GameObject>> returnVector;

        //check if the input file exsist
        if (!inputFile.is_open())
            return returnVector;

        //checing if the file can be read
        if (!Reader.parse(inputFile, actualJson))
            return returnVector;

        m_sprites.clear();

		int count = 0;

        //going over the json and reading all the data

        //objects
        for (int i = 0; i < actualJson.size(); i++) {
            std::stringstream ss;
            ss << i;
            Json::Value currentObject = actualJson["object" + ss.str()];

            
            if (currentObject.isNull()) {
                count = actualJson.size() - i -1;
                break;
            }

            Collision::collisionLayer layer = static_cast<Collision::collisionLayer>(currentObject["layer"].asInt());
            sf::Vector2f position = sf::Vector2f(currentObject["position"][0].asFloat(), currentObject["position"][1].asFloat());
            float rotation = currentObject["rotation"].asFloat();
            sf::Vector2f scale = sf::Vector2f(currentObject["scale"][0].asFloat(), currentObject["scale"][1].asFloat());
            std::string spriteName = currentObject["spriteName"].asString();
            std::string name = currentObject["name"].asString();
            int level = currentObject["level"].asInt();
            LoadAsset(spriteName);
            bool isSceneObject = currentObject["isSceneObject"].asBool();

            std::string type = currentObject["type"].asString();
            if (type != "none") {
               
                auto obj = ObjectManager::Instance().Create(type, systemManager, m_sprites[spriteName], name, layer, level);
                returnVector.push_back(std::move(obj)); 
            }
            else {
                auto plain = std::make_unique<GameObject>(systemManager, m_sprites[spriteName], name, layer, level);
                returnVector.push_back(std::move(plain));
            }
            returnVector.back()->SetPosition(position);
            returnVector.back()->SetScale(scale);
            returnVector.back()->SetRotation(rotation);
            returnVector.back()->SetIsSceneObject(isSceneObject);
        }
        
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
            sf::Vector2f position = sf::Vector2f(currentObject["position"][0].asFloat(), currentObject["position"][1].asFloat());
            sf::Color color = sf::Color(currentObject["color"][0].asFloat(), currentObject["color"][1].asFloat(), currentObject["color"][2].asFloat(), currentObject["color"][3].asFloat());
            float rotation = currentObject["rotation"].asFloat();
            float radius = currentObject["radius"].asFloat();
            bool fade = currentObject["fade"].asBool();
            ls::LightId light;
            bool isSceneObject = currentObject["isSceneObject"].asBool();

			switch (type) {
			case 0: //Radial light
                light = systemManager->AddLight(ls::radial, name.c_str());
                systemManager->GetLight<ls::lightType::radial>(light)->SetRange(radius);
                systemManager->GetLight<ls::lightType::radial>(light)->SetColor(color);
                systemManager->GetLight<ls::lightType::radial>(light)->setPosition(position);
                systemManager->GetLight<ls::lightType::radial>(light)->setRotation(rotation);
                systemManager->GetLight<ls::lightType::radial>(light)->SetBeamAngle(currentObject["angle"].asFloat());
                systemManager->GetLight<ls::lightType::radial>(light)->SetFade(fade);
                systemManager->GetLight<ls::lightType::radial>(light)->SetIsSceneObject(isSceneObject);
				break;
			case 1: //Directional light
                light = systemManager->AddLight(ls::directed, name.c_str());
                systemManager->GetLight<ls::lightType::directed>(light)->SetRange(radius);
                systemManager->GetLight<ls::lightType::directed>(light)->SetColor(color);
				systemManager->GetLight<ls::lightType::directed>(light)->setRotation(rotation);
                systemManager->GetLight<ls::lightType::directed>(light)->SetBeamWidth(currentObject["width"].asFloat());
                systemManager->GetLight<ls::lightType::directed>(light)->SetFade(fade);
                systemManager->GetLight<ls::lightType::directed>(light)->SetIsSceneObject(isSceneObject);
				break;
			default:
				MC_LOG(((std::string)"unknown light type in " + name));
				break;
			} 
        }

        //texts
        for (int i = 0; i < count; i++) {
            std::stringstream ss;
            ss << i;
            Json::Value currentObject = actualJson["text" + ss.str()];

            if (currentObject.isNull()) {
                count = count - i;
                break;
            }
            if (currentObject.isNull()) {
                MC_LOG("undfined object in place " + std::to_string(i));
                continue;
            }


            std::string name = currentObject["name"].asString();
            std::string font = currentObject["font"].asString();
            float rotation = currentObject["rotation"].asFloat();
            sf::Vector2f position = sf::Vector2f(currentObject["position"][0].asFloat(), currentObject["position"][1].asFloat());
            sf::Color color = sf::Color(currentObject["color"][0].asFloat(), currentObject["color"][1].asFloat(), currentObject["color"][2].asFloat(), currentObject["color"][3].asFloat());
            color.a = 255;
            sf::Color outlineColor = sf::Color(currentObject["outlineColor"][0].asFloat(), currentObject["outlineColor"][1].asFloat(), currentObject["outlineColor"][2].asFloat(), currentObject["outlineColor"][3].asFloat());
            outlineColor.a = 255;
            float outlineThickness = currentObject["outlineThickness"].asFloat();
            unsigned int size = currentObject["size"].asInt();
            sf::Vector2f scale = sf::Vector2f(currentObject["scale"][0].asFloat(), currentObject["scale"][1].asFloat());
            std::string value = currentObject["value"].asString();
            bool isSceneObject = currentObject["isSceneObject"].asBool();

            systemManager->AddText(name, font);
            systemManager->GetText(name)->GetBase().setString(value);
            systemManager->GetText(name)->GetBase().setRotation(rotation);
            systemManager->GetText(name)->GetBase().setPosition(position);
            systemManager->GetText(name)->GetBase().setColor(color);
            systemManager->GetText(name)->GetBase().setOutlineColor(outlineColor);
            systemManager->GetText(name)->GetBase().setOutlineThickness(outlineThickness);
            systemManager->GetText(name)->GetBase().setCharacterSize(size);
            systemManager->GetText(name)->GetBase().setScale(scale);
            systemManager->GetText(name)->SetIsSceneObject(isSceneObject);
        }

        //buttons
        for (int i = 0; i < count; i++) {
            std::stringstream ss;
            ss << i;
            Json::Value currentObject = actualJson["button" + ss.str()];


            if (currentObject.isNull()) {
                MC_LOG("undfined object in place " + std::to_string(i));
                continue;
            }

            std::string name = currentObject["name"].asString();
            std::string img = currentObject["img"].asString();
            std::string onClickImg = currentObject["onClickImg"].asString();
            std::string onclickfunc = currentObject["onClickFunc"].asString();
            float rotation = currentObject["rotation"].asFloat();
            sf::Vector2f position = sf::Vector2f(currentObject["position"][0].asFloat(), currentObject["position"][1].asFloat());
            sf::Vector2f scale = sf::Vector2f(currentObject["scale"][0].asFloat(), currentObject["scale"][1].asFloat());
            bool isSceneObject = currentObject["isSceneObject"].asBool();


            auto it = m_buttonFuncMap.find(onclickfunc);
            auto it2 = m_functionMap.find(onclickfunc);
            if (it == m_buttonFuncMap.end()) {
                MC_LOG("No function registered with name: " + onclickfunc);
                continue; // or handle error
            }

            auto x = it->second;
            systemManager->AddButton(name, img, onClickImg, x);
            systemManager->GetButton(name)->SetRotation(rotation);
            systemManager->GetButton(name)->SetPosition(position);
            systemManager->GetButton(name)->SetScale(scale);
            systemManager->GetButton(name)->SetIsSceneObject(isSceneObject);
        }

        //close the file
        inputFile.close();

        return returnVector;
    }

    sf::Sprite* FileManager::GetSprite(const std::string& name) {
        if (m_sprites.find(name) == m_sprites.end()) {
            LoadAsset(name);
        }
        if (m_sprites.find(name) == m_sprites.end()) return nullptr;

        return &m_sprites[name];
    }

    sf::Sound* FileManager::GetSound(const std::string& name) {
        if (m_sounds.find(name) == m_sounds.end()) {
            LoadAsset(name);
        }
        if (m_sounds.find(name) == m_sounds.end()) return nullptr;

    	return &m_sounds[name];
    }

    void FileManager::CreateLog() {
        std::string logDir = s_mainPath + "\\Logs";

        if (!std::filesystem::exists(logDir)) {
            return;
        }

        std::regex logPattern(R"(log(\d+)\.log)");
        int maxLogNumber = 0;

        for (const auto& entry : std::filesystem::directory_iterator(logDir)) {
            std::string fileName = entry.path().filename().string();
            std::smatch match;
            if (std::regex_match(fileName, match, logPattern)) {
                int logNumber = std::stoi(match[1].str());
                if (logNumber > maxLogNumber) {
                    maxLogNumber = logNumber;
                }
            }
        }

        s_currentLog = maxLogNumber + 1;
        std::string logFileName = logDir + "\\log" + std::to_string(s_currentLog) + ".log";
        std::ofstream logFile(logFileName);

        if (!logFile) {
            return;
        }

        logFile.close();
    }

    void FileManager::Log(const std::string& msg) {
		if (s_currentLog == 0) 
			CreateLog();
		

        std::string logFileName = s_mainPath + "\\logs\\log" + std::to_string(s_currentLog) + ".log";

        std::ofstream logFile(logFileName, std::ios::app);

        if (!logFile) {
            return;
        }

        logFile << msg << std::endl;

        logFile.close();
    }

    std::string FileManager::GetFontPath(const std::string& name)
    {
        return s_mainPath + "\\Resources\\fonts\\" + name + ".ttf";
    }
}