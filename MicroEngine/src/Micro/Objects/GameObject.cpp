#include "GameObject.h"
#include "..//SystemLogic//SystemManager.h"

namespace Micro{
    GameObject::GameObject(SystemManager* systemManager, const sf::Sprite& ObjectSprite, std::string& name, Collision::collisionLayer Layer)
        : m_objectSprite(ObjectSprite), m_layer(Layer), m_rotation(ObjectSprite.getRotation()), m_position(ObjectSprite.getPosition()), m_scale(ObjectSprite.getScale()), m_name(name), m_systemManager(systemManager)
    {
        m_systemManager->CreateGameObject(*this);

        SetCenter();
    }


    void GameObject::Flip() {
        m_scale = sf::Vector2f(m_scale.x * -1, m_scale.y);
    }

    bool GameObject::IsFacingRight() {
        return m_scale.x >= 0;
    }

    void GameObject::SetCenter() {
        const sf::Texture* texture = m_objectSprite.getTexture();

        sf::Vector2u textureSize = texture->getSize();

        int minX = textureSize.x;
        int minY = textureSize.y;
        int maxX = -1;
        int maxY = -1;

        // Iterate through pixels to find non-transparent coordinates
        for (unsigned int x = 0; x < textureSize.x; ++x) {
            for (unsigned int y = 0; y < textureSize.y; ++y) {
                sf::Color pixelColor = texture->copyToImage().getPixel(x, y);
                if (pixelColor.a > 0) {
                    // Update min and max coordinates
                    minX = std::min(minX, static_cast<int>(x));
                    minY = std::min(minY, static_cast<int>(y));
                    maxX = std::max(maxX, static_cast<int>(x));
                    maxY = std::max(maxY, static_cast<int>(y));
                }
            }
        }

        // Calculate the center based on the non-transparent coordinates
        float centerX = static_cast<float>(minX + maxX) / 2.0f;
        float centerY = static_cast<float>(minY + maxY) / 2.0f;

        m_objectSprite.setOrigin(sf::Vector2f(centerX, centerY));
    }

    void GameObject::Delete() {
        m_systemManager->DestroyObject(m_name);
    }

    void GameObject::OnCollision(GameObject* HitInfo) {}

    void GameObject::OnTrigger(GameObject* HitInfo) {}

    void GameObject::HandlePositionChange(sf::Vector2f NewPosition) {
        m_objectSprite.setPosition(NewPosition);

        GameObject* HitInfo = this;

        //checking if the new position collides with anything
        if (m_systemManager->CheckForCollision(m_objectSprite, m_name, Collision::ALL, HitInfo)) {
            if (m_layer < 6 && HitInfo->m_layer < 6) {
                OnCollision(HitInfo);
                HitInfo->OnCollision(this);
                m_objectSprite.setPosition(m_position);
            }
            else if (m_layer > 6) {
                OnTrigger(HitInfo);
                HitInfo->OnTrigger(this);
                m_position = NewPosition;
                return;
            }


            //testing if the object can move only along the x axies
            sf::Vector2f TestX = sf::Vector2f(NewPosition.x, NewPosition.y);
            m_objectSprite.setPosition(TestX);

            Collision::collisionLayer LayerToCollideWith = Collision::ALL;
            //checking collison for the movement along the x axies
            if (!m_systemManager->CheckForCollision(m_objectSprite, m_name, LayerToCollideWith)) {
                m_position = m_objectSprite.getPosition();
                return;
            }

            //testing if the object can move only along the y axies
            sf::Vector2f TestY = sf::Vector2f(m_position.x, NewPosition.y);
            m_objectSprite.setPosition(TestY);

            //checking collison for the movement along the y axies
            if (!m_systemManager->CheckForCollision(m_objectSprite, m_name, LayerToCollideWith)) {
                m_position = m_objectSprite.getPosition();
            }
        }
        else {
            m_position = NewPosition;
        }
    }

    void GameObject::HandleRotationChange(float NewRotation) {
        m_objectSprite.setRotation(NewRotation);

        GameObject* HitInfo = this;

        //checking if the new position collides with anything
        if (m_systemManager->CheckForCollision(m_objectSprite, m_name, Collision::ALL, HitInfo)) {
            if (HitInfo == this) {}
            else if (m_layer < 6 && HitInfo->m_layer < 6) {
                OnCollision(HitInfo);
                HitInfo->OnCollision(this);
                m_objectSprite.setRotation(m_rotation);
            }
            else if (m_layer > 6) {
                OnTrigger(HitInfo);
                HitInfo->OnTrigger(this);
                m_rotation = NewRotation;
            }
        }
        else {
            m_rotation = NewRotation;
        }
    }

    void GameObject::HandleScaleChange(sf::Vector2f NewScale) {
        m_objectSprite.setScale(NewScale);

        GameObject* HitInfo = this;

        //checking if the new position collides with anything
        if (m_systemManager->CheckForCollision(m_objectSprite, m_name, Collision::ALL, HitInfo)) {
            if (HitInfo == this) {}
            else if (m_layer < 6 && HitInfo->m_layer < 6) {
                OnCollision(HitInfo);
                HitInfo->OnCollision(this);
                m_objectSprite.setScale(m_scale);
            }
            else if (m_layer > 6) {
                OnTrigger(HitInfo);
                HitInfo->OnTrigger(this);
                m_scale = NewScale;
                return;
            }


        }
        else {
            m_scale = NewScale;
        }
    }

    sf::Vector2f GameObject::GetPosition() const {
        return m_position;
    }

    void GameObject::SetPosition(sf::Vector2f NewPosition) {
        if (NewPosition != m_position) {
            HandlePositionChange(NewPosition);
        }
    }

    sf::Vector2f GameObject::GetScale() const {
        return m_scale;
    }

    void GameObject::SetScale(sf::Vector2f NewScale) {
        if (NewScale != m_scale) {
            HandleScaleChange(NewScale);
        }
    }

    float GameObject::GetRotation() const {
        return m_rotation;
    }

    void GameObject::SetRotation(float NewRotation) {
        if (NewRotation != m_rotation) {
            HandleRotationChange(NewRotation);
        }
    }

    void GameObject::SetSprite(const sf::Sprite& Sprite) {
        m_objectSprite = Sprite;
    }

    const sf::Sprite& GameObject::GetSprite() const {
        return m_objectSprite;
    }

    Collision::collisionLayer GameObject::GetLayer() const {
        return m_layer;
    }

    void GameObject::Start() {

    }

    void GameObject::Update(float DeltaTime) {
    }

    std::string GameObject::GetName() {
        return m_name;
    }

}