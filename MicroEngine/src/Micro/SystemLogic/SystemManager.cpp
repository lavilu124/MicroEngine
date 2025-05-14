#include "SystemManager.h"


namespace Micro {
    sf::Clock SystemManager::clock;
    sf::Time SystemManager::deltaTimeT;


    float SystemManager::deltaTime = 0;



    SystemManager::SystemManager(sf::RenderWindow& window)
        : m_sceneManager(window, window.getSize().x, window.getSize().y), m_lighting(LightingArea::FOG, { ((float)window.getSize().x) / -2, ((float)window.getSize().y) / -2 }, { ((float)window.getSize().x), ((float)window.getSize().y) }) {


        m_windowSize = sf::Vector2f(window.getSize().x, window.getSize().y);

        SetDarkness(0);

        m_lighting.SetAreaTexture(&m_drakness);
        m_lighting.scale(960 / m_drakness.getSize().x, 540 / m_drakness.getSize().y);
        m_lighting.Clear();


        m_fileManager.SetPaths();
        m_fileManager.LoadInput();

        deltaTime = 0;
    }

    void SystemManager::Start() {
        if (m_sceneManager.objects.empty()) return;

        for (std::vector<GameObject>::iterator it = m_sceneManager.objects.begin(); it != m_sceneManager.objects.end(); ++it)
            (*it).Start();
    }

    void SystemManager::Update(Camera* cam) {
        if (m_sceneManager.objects.empty()) return;

        for (auto light : m_sceneManager.lights)
        {
            light->CastLight(m_sceneManager.edges.begin(), m_sceneManager.edges.end());
        }


        for (std::vector<GameObject>::iterator it = m_sceneManager.objects.begin(); it != m_sceneManager.objects.end(); ++it)
            (*it).Update(deltaTime);

        deltaTimeT = clock.restart();
        deltaTime = deltaTimeT.asSeconds();

        cam->Update(*this);

        m_lighting.Clear();

        for (auto light : m_sceneManager.lights)
        {
            m_lighting.Draw(*light);
        }

        m_lighting.Display();
    }

    void SystemManager::Render(sf::RenderWindow& window) {
        //m_lightSystem.setView(window.getView());
        for (std::vector<GameObject>::iterator it = m_sceneManager.objects.begin(); it != m_sceneManager.objects.end(); ++it)
            window.draw((*it).GetSprite());

        window.draw(m_lighting);

        for (auto light : m_sceneManager.lights)
        {
            window.draw(*light);
        }
    }

    int SystemManager::CheckExistingObject(const char* name) {
        for (int i = 0; i < m_sceneManager.objects.size(); ++i) {
            if (m_sceneManager.objects[i].GetName() == name) {
                return i;
            }
        }

        return -1;
    }

    bool SystemManager::CheckForCollision(sf::Sprite sprite, const char* name, Collision::collisionLayer layerToCollideWith, GameObject* collideInfo){
        for (int i = 0; i < m_sceneManager.objects.size(); ++i) {
            if (m_sceneManager.objects[i].GetName() != name && (m_sceneManager.objects[i].GetLayer() == layerToCollideWith || (layerToCollideWith == Collision::ALL && m_sceneManager.objects[i].GetLayer() < 6))) {
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

    void SystemManager::RunInput(sf::Event event)  {
        for (std::map<std::string, Input::InputAction>::iterator Input = m_fileManager.inputs.begin(); Input != m_fileManager.inputs.end(); ++Input)
            (*Input).second.Active(event);

    }

    void SystemManager::LoadScene(const char* scene) {
        m_sceneManager.LoadSceneFromFile(scene, this, m_fileManager);

        Start();
    }

    Camera& SystemManager::GetCamera() { return m_sceneManager.camera; }

    void SystemManager::CreateGameObject(GameObject& ob) {
        if (!CheckExistingObject(ob.GetName().c_str()))
            m_sceneManager.objects.push_back(ob);
    }

    std::vector<GameObject>& SystemManager::GetObjects() {
        return m_sceneManager.objects;
    }


    GameObject* SystemManager::GetObjectByName(const char* name) {
        return &m_sceneManager.objects[CheckExistingObject(name)];
    }

    void SystemManager::DestroyObject(const char* name) {
        m_sceneManager.objects.erase(m_sceneManager.objects.begin() + CheckExistingObject(name));
    }


    FileManager& SystemManager::GetFileManager() {
        return m_fileManager;
    }

    ls::LightId SystemManager::AddLight(ls::lightType type, const char* name)
    {
        switch (type)
        {
        case ls::lightType::directed:
            m_sceneManager.lights.push_back(std::make_shared<DirectedLight>(DirectedLight(name, m_currentLightId)));
            break;
        case ls::radial:
            m_sceneManager.lights.push_back(std::make_shared<RadialLight>(RadialLight(name, m_currentLightId)));
            break;
        }

        return { m_currentLightId++, type };
    }

    void SystemManager::RemoveLight(ls::LightId id)
    {
        for (int i = 0; i < m_sceneManager.lights.size(); i++)
        {
            if (m_sceneManager.lights[i]->GetID() == id.id)
                m_sceneManager.lights.erase(m_sceneManager.lights.begin() + i);
        }
    }

    void SystemManager::AddEdge(sf::Vector2f a, sf::Vector2f b)
    {
        m_sceneManager.edges.emplace_back(a, b);
    }

    void SystemManager::RemoveEdge(sf::Vector2f a, sf::Vector2f b)
    {
        for (int i = 0; i < m_sceneManager.edges.size(); i++)
        {
            if (a == m_sceneManager.edges[i].m_origin
                && b == m_sceneManager.edges[i].m_direction)
            {
                m_sceneManager.edges.erase(m_sceneManager.edges.begin() + i);
            }
        }
    }

    void SystemManager::SetDarkness(int precent)
    {
        std::vector<sf::Uint8> pixels(m_windowSize.x * m_windowSize.y * 4);

        for (size_t i = 0; i < pixels.size(); i += 4) {
            pixels[i] = 0;
            pixels[i + 1] = 0;
            pixels[i + 2] = 0;
            pixels[i + 3] = static_cast<sf::Uint8>(255 * (precent / 100.0f));
        }

        m_drakness.create(m_windowSize.x, m_windowSize.y);
        m_drakness.update(pixels.data());
    }

}

