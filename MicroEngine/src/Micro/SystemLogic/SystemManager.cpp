#include "SystemManager.h"


namespace Micro {
    sf::Clock SystemManager::clock;
    sf::Time SystemManager::deltaTimeT;


    float SystemManager::deltaTime = 0;

    int partition(std::vector<std::shared_ptr<GameObject>>& vec, int low, int high) {

        int pivot = vec[high]->GetLevel();
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++) {
            if (vec[j]->GetLevel() <= pivot) {
                i++;
                std::swap(vec[i], vec[j]);
            }
        }

        std::swap(vec[i + 1], vec[high]);

        return (i + 1);
    }

    void quickSort(std::vector<std::shared_ptr<GameObject>>& vec, int low, int high) {
        if (low < high) {

            int pi = partition(vec, low, high);

            quickSort(vec, low, pi - 1);
            quickSort(vec, pi + 1, high);
        }
    }

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

        for (auto it = m_sceneManager.objects.begin(); it != m_sceneManager.objects.end(); ++it)
            (*it)->Start();
    }

    void SystemManager::Update() {
        if (m_sceneManager.objects.empty()) return;

        for (auto light : m_sceneManager.lights)
        {
            light->CastLight(m_sceneManager.edges.begin(), m_sceneManager.edges.end());
        }


        for (int i = 0; i < m_sceneManager.objects.size(); i++)
            m_sceneManager.objects[i]->Update(deltaTime);

        deltaTimeT = clock.restart();
        deltaTime = deltaTimeT.asSeconds();

        m_sceneManager.camera.Update();

        m_lighting.Clear();

        for (auto light : m_sceneManager.lights)
        {
            m_lighting.Draw(*light);
        }

        m_lighting.Display();
    }

    void SystemManager::Render(sf::RenderWindow& window) {
        for (auto it = m_sceneManager.objects.begin(); it != m_sceneManager.objects.end(); ++it)
            if ((*it)->IsShown()) window.draw((*it)->GetSprite());

        window.draw(m_lighting);

        for (auto light : m_sceneManager.lights)
            if (light->IsShown()) window.draw(*light);

        for (auto& button : m_sceneManager.buttons)
            if (button.IsShown()) window.draw(button.GetCurrentSprite());

        for (auto& text : m_sceneManager.texts) 
            if (text.IsShown()) window.draw(text.GetBase());
        
    }

    int SystemManager::CheckExistingObject(const char* name) {
        auto it = std::find_if(m_sceneManager.objects.begin(), m_sceneManager.objects.end(),
            [&](const std::shared_ptr<GameObject>& obj) {
                return obj->GetName() == name;
            });

        return (it != m_sceneManager.objects.end()) ? std::distance(m_sceneManager.objects.begin(), it) : -1;
    }


    bool SystemManager::CheckForCollision(const sf::Sprite& sprite, const char* name, GameObject*& collideInfo, Collision::collisionLayer layerToCollideWith, const sf::Uint8& alpahLimit) const
    {
        if (layerToCollideWith == 7)
            return false;

        for (int i = 0; i < m_sceneManager.objects.size(); ++i) {
            if (m_sceneManager.objects[i]->GetName() != name &&
                (m_sceneManager.objects[i]->GetLayer() == layerToCollideWith ||
                    (layerToCollideWith == Collision::ALL && m_sceneManager.objects[i]->GetLayer() != 7))) {

                if (Collision::PixelPerfectCollision(sprite, m_sceneManager.objects[i]->GetSprite())) {
                    collideInfo = m_sceneManager.objects[i].get();
                    return true;
                }
            }
        }

        return false;
    }


    void SystemManager::RunInput(sf::Event event, sf::RenderWindow& window)  {
        for (std::map<std::string, Input::InputAction>::iterator Input = m_fileManager.inputs.begin(); Input != m_fileManager.inputs.end(); ++Input)
            (*Input).second.Active(event);

        sf::Vector2f worldMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        for (auto& button : m_sceneManager.buttons) {
           
            button.IsPressed(worldMousePos);
        }
    }

    void SystemManager::AddInput(Micro::Input::InputAction& action)
    {
        m_fileManager.addInput(action);
    }

    void SystemManager::LoadScene(const char* scene) {
        m_sceneManager.LoadSceneFromFile(scene, this, m_fileManager);
        quickSort(m_sceneManager.objects, 0, m_sceneManager.objects.size() - 1);

        Start();
    }

    Camera& SystemManager::GetCamera() { return m_sceneManager.camera; }

    void SystemManager::CreateGameObject(std::shared_ptr<GameObject> ob) {
        if (CheckExistingObject(ob->GetName().c_str()) == -1) {
            m_sceneManager.objects.push_back(std::move(ob));
            quickSort(m_sceneManager.objects, 0, m_sceneManager.objects.size() - 1);
        }
            
    }

    std::vector<std::shared_ptr<GameObject>>& SystemManager::GetObjects() {
        return m_sceneManager.objects;
    }


    GameObject* SystemManager::GetObjectByName(const char* name) {
        int index = CheckExistingObject(name);
        if (index != -1)
            return m_sceneManager.objects[index].get();
        return nullptr;
    }

    void SystemManager::DestroyObject(const char* name) {
        m_sceneManager.objects.erase(m_sceneManager.objects.begin() + CheckExistingObject(name));
    }


    FileManager& SystemManager::GetFileManager() {
        return m_fileManager;
    }

    ls::LightId SystemManager::AddLight(ls::lightType type, const char* name)
    {
        for (int i = 0; i < m_sceneManager.lights.size(); i++)
        {
            if (m_sceneManager.lights[i]->GetName() == name)
                throw std::runtime_error("a light with this name already exist");
        }
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
            if (m_sceneManager.lights[i]->GetID() == id.id) {
                m_sceneManager.lights.erase(m_sceneManager.lights.begin() + i);
                break;
            }
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

    void SystemManager::ChangedLevel()
    {
        quickSort(m_sceneManager.objects, 0, m_sceneManager.objects.size() - 1);
    }

    void SystemManager::AddText(const std::string& name, const std::string& font)
    {
        for (int i = 0; i < m_sceneManager.texts.size(); i++) {
            if (name == m_sceneManager.texts[i].GetName()) return;
        }
        m_sceneManager.texts.emplace_back(name, this, font);
    }

    void SystemManager::RemoveText(const std::string& name)
    {
        for (int i = 0; i < m_sceneManager.texts.size(); i++) {
            if (name == m_sceneManager.texts[i].GetName()) {
                m_sceneManager.texts.erase(m_sceneManager.texts.begin() + i);
            }
        }
    }

    Text* SystemManager::GetText(const std::string& name)
    {
        for (auto& text : m_sceneManager.texts) {
            if (name == text.GetName()) {
                return &text;
            }
        }

        return nullptr;
    }

    void SystemManager::AddButton(const std::string& name, const std::string& img, const std::string& onClickImg, void(*onClick)())
    {
        for (int i = 0; i < m_sceneManager.buttons.size(); i++) {
            if (name == m_sceneManager.buttons[i].GetName()) return;
        }
        m_sceneManager.buttons.emplace_back(this, name, img, onClickImg, onClick);
    }

    void SystemManager::RemoveButton(const std::string& name)
    {
        for (int i = 0; i < m_sceneManager.buttons.size(); i++) {
            if (name == m_sceneManager.buttons[i].GetName()) {
                m_sceneManager.buttons.erase(m_sceneManager.buttons.begin() + i);
            }
        }
    }

    Button* SystemManager::GetButton(const std::string& name)
    {
        for (auto& button : m_sceneManager.buttons) {
            if (name == button.GetName()) {
                return &button;
            }
        }

        return nullptr;
    }

}

