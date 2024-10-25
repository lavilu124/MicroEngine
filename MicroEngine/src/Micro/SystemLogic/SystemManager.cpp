#include "SystemManager.h"


namespace Micro{
    sf::Clock SystemManager::clock;
    sf::Time SystemManager::deltaTimeT;


    float SystemManager::deltaTime = 0;



    SystemManager::SystemManager(sf::RenderWindow& window) : m_sceneManager(window, window.getSize().x, window.getSize().y) {
        m_fileManager.SetPaths();
        m_fileManager.LoadInput();

        deltaTime = 0;

        m_lightSystem.setView(window.getView());
    }

    void SystemManager::Start() {
        if (m_sceneManager.objects.empty()) return;

        for (std::vector<GameObject>::iterator it = m_sceneManager.objects.begin(); it != m_sceneManager.objects.end(); ++it)
            (*it).Start();
    }

    void SystemManager::Update(Camera* cam) {
        if (m_sceneManager.objects.empty()) return;

        for (std::vector<GameObject>::iterator it = m_sceneManager.objects.begin(); it != m_sceneManager.objects.end(); ++it)
            (*it).Update(deltaTime);

        deltaTimeT = clock.restart();
        deltaTime = deltaTimeT.asSeconds();

        cam->Update(*this);
    }

    void SystemManager::Render(sf::RenderWindow& window) {
        m_lightSystem.setView(window.getView());
        for (std::vector<GameObject>::iterator it = m_sceneManager.objects.begin(); it != m_sceneManager.objects.end(); ++it)
            window.draw((*it).GetSprite());

    }

    int SystemManager::CheckExistingObject(std::string name) {
        for (int i = 0; i < m_sceneManager.objects.size(); ++i) {
            if (m_sceneManager.objects[i].GetName() == name) {
                return i;
            }
        }

        return -1;
    }

    bool SystemManager::CheckForCollision(sf::Sprite sprite, std::string name, Collision::collisionLayer layerToCollideWith, GameObject* collideInfo) {
        for (int i = 0; i < m_sceneManager.objects.size(); ++i) {
            if (m_sceneManager.objects[i].GetName().compare(name) && (m_sceneManager.objects[i].GetLayer() == layerToCollideWith || (layerToCollideWith == Collision::ALL && m_sceneManager.objects[i].GetLayer() < 6))) {
                if (Collision::PixelPerfectCollision(sprite, m_sceneManager.objects[i].GetSprite())) {
                    if (collideInfo != nullptr) {
                        collideInfo = &m_sceneManager.objects[i];
                    }

                    return true;
                }
            }
        }
        return false;
    }

    void SystemManager::RunInput(sf::Event event) {
        for (std::map<std::string, Input::InputAction>::iterator Input = m_fileManager.inputs.begin(); Input != m_fileManager.inputs.end(); ++Input)
            (*Input).second.Active(event);

    }

    void SystemManager::LoadScene(std::string scene) {
        m_sceneManager.LoadSceneFromFile(scene, this, m_fileManager);

        Start();
    }

    Camera& SystemManager::GetCamera() { return m_sceneManager.camera; }

    void SystemManager::CreateGameObject(GameObject& ob) {
        if (!CheckExistingObject(ob.GetName()))
            m_sceneManager.objects.push_back(ob);
    }

    GameObject& SystemManager::GetObjectByName(const std::string& name) {
        return m_sceneManager.objects[CheckExistingObject(name)];
    }

    FileManager& SystemManager::GetFileManager() {
        return m_fileManager;
    }

    void SystemManager::DestroyObject(std::string name) {
        m_sceneManager.objects.erase(m_sceneManager.objects.begin() + CheckExistingObject(name));
    }

    ls::LightSystem* SystemManager::getLightSystem() {
        return &m_lightSystem;
    }

    int SystemManager::AddLight(ls::Light* light) {
		m_lightSystem.addLight(light);
        m_sceneManager.lights.push_back(light);
        light->assignID(m_currentId);

        m_currentId++;
        return m_currentId - 1;
    }

    void SystemManager::RemoveLight(int id) {
        int index = GetLightIndex(id);
        m_lightSystem.removeLight(m_sceneManager.lights[index]);
		m_sceneManager.lights.erase(m_sceneManager.lights.begin() + index);
    }

    ls::Light* SystemManager::getLight(int id) {
        int index = GetLightIndex(id);
        if (index == -1)
            return nullptr;

        return m_sceneManager.lights[index];
    }

    int SystemManager::GetLightIndex(int id) {
        for (int i = 0; i < m_sceneManager.lights.size(); i++) {
            if (m_sceneManager.lights[i]->getId() == id) {
                return i;
            }
        }
        return -1;
    }
}
