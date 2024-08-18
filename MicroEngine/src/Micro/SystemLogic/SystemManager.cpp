#include "SystemManager.h"


sf::Clock SystemManager::clock;
sf::Time SystemManager::deltaTimeT;


float SystemManager::deltaTime = 0;


SystemManager::SystemManager(sf::RenderWindow& window) : m_currentScene(window, window.getSize().x, window.getSize().y){
    FileManager::SetPaths();
    FileManager::LoadInput();

    deltaTime = 0;
}

void SystemManager::Start() {
    std::vector<GameObject>& objects = m_currentScene.GetObjects();
    if (objects.empty()) return;

    for (std::vector<GameObject>::iterator it = objects.begin(); it != objects.end(); ++it)
        (*it).Start();
}

void SystemManager::Update(Camera* cam) {
    std::vector<GameObject>& objects = m_currentScene.GetObjects();
    if (objects.empty()) return;

    for (std::vector<GameObject>::iterator it = objects.begin(); it != objects.end(); ++it)
        (*it).Update(deltaTime);

    deltaTimeT = clock.restart();
    deltaTime = deltaTimeT.asSeconds();

    cam->Update();
}

void SystemManager::Render(sf::RenderWindow& window) {
    std::vector<GameObject>& objects = m_currentScene.GetObjects();
    for (std::vector<GameObject>::iterator it = objects.begin(); it != objects.end(); ++it)
        window.draw((*it).GetSprite());

}

int SystemManager::CreateGameObject(GameObject& ob) {
    return (m_currentScene.CheckExistingObject(ob.GetName()) != -1) ?
        m_currentScene.CheckExistingObject(ob.GetName()) :
        m_currentScene.CreateObject(ob);

    return -1;
}

bool SystemManager::CheckForCollision(sf::Sprite sprite, std::string name, Collision::collisionLayer layerToCollideWith, GameObject* collideInfo) {
    std::vector<GameObject>& objects = m_currentScene.GetObjects();
    for (int i = 0; i < objects.size(); ++i) {
        if (objects[i].GetName().compare(name) && (objects[i].GetLayer() == layerToCollideWith || (layerToCollideWith == Collision::ALL && objects[i].GetLayer() < 6))) {
            if (Collision::PixelPerfectCollision(sprite, objects[i].GetSprite())) {
                if (collideInfo != nullptr) {
                    collideInfo = &objects[i];
                }
                return true;
            }
        }
    }
    return false;
}

void SystemManager::RunInput(sf::Event event) {
    for (std::map<std::string, Input::InputAction>::iterator Input = FileManager::inputs.begin(); Input != FileManager::inputs.end(); ++Input)
        (*Input).second.Active(event);

}

void SystemManager::LoadScene(std::string scene) {
    m_currentScene.LoadSceneFromFile(scene, this);

    Start();
}

Camera& SystemManager::GetCamera() { return m_currentScene.camera; }

int SystemManager::GetObjectByName(const std::string& name) {
    return m_currentScene.CheckExistingObject(name);
}

void SystemManager::DestroyObject(std::string name) {
    m_currentScene.DestroyObject(name);
}

void SystemManager::DestroyObject(int index) {
    m_currentScene.DestroyObject(index);
}
