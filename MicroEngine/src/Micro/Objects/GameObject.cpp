#include "GameObject.h"
#include "..//SystemLogic//SystemManager.h"

namespace Micro{
    GameObject::GameObject(SystemManager* systemManager, const sf::Sprite& ObjectSprite, const std::string& name, Collision::collisionLayer Layer, int level)
        : m_objectSprite(ObjectSprite), m_layer(Layer), m_rotation(ObjectSprite.getRotation()), m_position(ObjectSprite.getPosition()), m_scale(ObjectSprite.getScale()), m_name(name), m_systemManager(systemManager), m_level(level)
    {
        SetCenter();
    }


    void GameObject::Flip() {
        m_scale = sf::Vector2f(m_scale.x * -1, m_scale.y);
    }

    bool GameObject::IsFacingRight() const{
        return m_scale.x >= 0;
    }

    void GameObject::SetCenter() {
        const sf::Texture* texture = m_objectSprite.getTexture();

        sf::Vector2u textureSize = texture->getSize();

        m_objectSprite.setOrigin(sf::Vector2f(texture->getSize().x / 2, texture->getSize().y / 2));
    }

    void GameObject::Delete() {
        m_systemManager->DestroyObject(m_name.c_str());
    }

    void GameObject::OnCollision(GameObject* hitInfo) {}

    void GameObject::OnTrigger(GameObject* hitInfo) {}

    void GameObject::HandlePositionChange(const sf::Vector2f& newPosition) {
        m_objectSprite.setPosition(newPosition);

        GameObject* HitInfo = this;

        //checking if the new position collides with anything
        if (m_systemManager->CheckForCollision(m_objectSprite, m_name.c_str(), HitInfo, Collision::ALL)) {
            if (!HitInfo->IsShown())
            {
                m_position = newPosition;
                return;
            }

            if ((m_layer < 6 && HitInfo->m_layer < 6) || m_layer == 13 || HitInfo->m_layer == 13) { //13 is collison with all 6 is trigger 7 is none and 7-13 is custom
                OnCollision(HitInfo);
                HitInfo->OnCollision(this);
                m_objectSprite.setPosition(m_position);
            }
            else if ((m_layer == 6 || m_layer > 7) && (HitInfo->m_layer == 6 || HitInfo->m_layer > 7)) {
                OnTrigger(HitInfo);
                HitInfo->OnTrigger(this);
                m_position = newPosition;
                return;
            }


            //testing if the object can move only along the x axies
            sf::Vector2f TestX = sf::Vector2f(newPosition.x, m_position.y);
            m_objectSprite.setPosition(TestX);

            //checking collison for the movement along the x axies
            if (!m_systemManager->CheckForCollision(m_objectSprite, m_name.c_str(), HitInfo, Collision::ALL)) {
                m_position = m_objectSprite.getPosition();
                return;
            }

            //testing if the object can move only along the y axies
            sf::Vector2f TestY = sf::Vector2f(m_position.x, newPosition.y);
            m_objectSprite.setPosition(TestY);

            //checking collison for the movement along the y axies
            if (!m_systemManager->CheckForCollision(m_objectSprite, m_name.c_str(), HitInfo, Collision::ALL)) {
                m_position = m_objectSprite.getPosition();
                return;
            }
        }
    	m_position = newPosition;
    }

    void GameObject::HandleRotationChange(float newRotation) {
        m_objectSprite.setRotation(newRotation);

        GameObject* HitInfo = this;

        //checking if the new position collides with anything
        if (m_systemManager->CheckForCollision(m_objectSprite, m_name.c_str(), HitInfo, Collision::ALL)) {
            if (!HitInfo->IsShown())
            {
                m_rotation = newRotation;
                return;
            }
            if ((m_layer < 6 && HitInfo->m_layer < 6) || m_layer == 13 || HitInfo->m_layer == 13) {
                OnCollision(HitInfo);
                HitInfo->OnCollision(this);
                m_objectSprite.setRotation(m_rotation);
                return;
            }
        	if ((m_layer == 6 || m_layer > 7) && (HitInfo->m_layer == 6 || HitInfo->m_layer > 7)) {
                OnTrigger(HitInfo);
                HitInfo->OnTrigger(this);
                m_rotation = newRotation;
                return;
            }
        }

    	m_rotation = newRotation;
    }

    void GameObject::HandleScaleChange(const sf::Vector2f& newScale) {
        m_objectSprite.setScale(newScale);

        GameObject* HitInfo = this;

        //checking if the new position collides with anything
        if (m_systemManager->CheckForCollision(m_objectSprite, m_name.c_str(), HitInfo, Collision::ALL)) {
            if (!HitInfo->IsShown())
            {
                m_scale = newScale;
                return;
            }
            if ((m_layer < 6 && HitInfo->m_layer < 6) || m_layer == 13 || HitInfo->m_layer == 13) {
                OnCollision(HitInfo);
                HitInfo->OnCollision(this);
                m_objectSprite.setScale(m_scale);
                return;
            }
            if ((m_layer == 6 || m_layer > 7) && (HitInfo->m_layer == 6 || HitInfo->m_layer > 7)) {
                OnTrigger(HitInfo);
                HitInfo->OnTrigger(this);
                m_scale = newScale;
                return;
            }


        }
    	m_scale = newScale;
    }

    sf::Vector2f GameObject::GetPosition() const {
        return m_position;
    }

    void GameObject::SetPosition(const sf::Vector2f& newPosition) {
        if (newPosition != m_position) {
            HandlePositionChange(newPosition);
        }
    }

    sf::Vector2f GameObject::GetScale() const {
        return m_scale;
    }

    void GameObject::SetScale(const sf::Vector2f& newScale) {
        if (newScale != m_scale) {
            HandleScaleChange(newScale);
        }
    }

    float GameObject::GetRotation() const {
        return m_rotation;
    }

    void GameObject::SetRotation(float newRotation) {
        if (newRotation != m_rotation) {
            HandleRotationChange(newRotation);
        }
    }

    void GameObject::SetSprite(const sf::Sprite& sprite) {
        m_objectSprite = sprite;
    }

    const sf::Sprite& GameObject::GetSprite() const {
        return m_objectSprite;
    }

    Collision::collisionLayer GameObject::GetLayer() const {
        return m_layer;
    }

    void GameObject::Start() {

    }

    void GameObject::Update(float deltaTime) {
    }

    std::string GameObject::GetName() const{
        return m_name;
    }

    int GameObject::GetLevel() const
    {
        return m_level;
    }

    void GameObject::SetLevel(int level)
    {
        if (level == m_level || level < 0)
            return;
        m_level = level;
        m_systemManager->ChangedLevel();
    }

    bool GameObject::IsShown() const
    {
        return m_shown;
    }

    void GameObject::SetShown(bool shown)
    {
        m_shown = shown;
    }

    bool GameObject::IsSceneObject() const
    {
        return m_isSceneObject;
    }

    void GameObject::SetIsSceneObject(bool val)
    {
        m_isSceneObject = val;
    }


}