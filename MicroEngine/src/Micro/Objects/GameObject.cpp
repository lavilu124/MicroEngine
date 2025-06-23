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

    void GameObject::OnCollision(GameObject* hitInfo)
    {
		if (m_physicsBody.has_value() && hitInfo->GetPhysicsBody().has_value())
		{
			m_physicsBody.value().onCollision(hitInfo->GetPhysicsBody().value());
		}
		
    }

    void GameObject::OnTrigger(GameObject* HitInfo) {}

    void GameObject::HandlePositionChange(sf::Vector2f NewPosition) {
        m_objectSprite.setPosition(NewPosition);

        GameObject* HitInfo = this;

        //checking if the new position collides with anything
        if (m_systemManager->CheckForCollision(m_objectSprite, m_name.c_str(), HitInfo, Collision::ALL)) {
            if ((m_layer < 6 && HitInfo->m_layer < 6) || m_layer == 13 || HitInfo->m_layer == 13) { //13 is collison with all 6 is trigger 7 is none and 7-13 is custom
                OnCollision(HitInfo);
                HitInfo->OnCollision(this);
                m_objectSprite.setPosition(m_position);
            }
            else if (m_layer == 6 && HitInfo->m_layer == 6) {
                OnTrigger(HitInfo);
                HitInfo->OnTrigger(this);
                m_position = NewPosition;
                return;
            }


            //testing if the object can move only along the x axies
            sf::Vector2f TestX = sf::Vector2f(NewPosition.x, m_position.y);
            m_objectSprite.setPosition(TestX);

            //checking collison for the movement along the x axies
            if (!m_systemManager->CheckForCollision(m_objectSprite, m_name.c_str(), HitInfo, Collision::ALL)) {
                m_position = m_objectSprite.getPosition();
                return;
            }

            //testing if the object can move only along the y axies
            sf::Vector2f TestY = sf::Vector2f(m_position.x, NewPosition.y);
            m_objectSprite.setPosition(TestY);

            //checking collison for the movement along the y axies
            if (!m_systemManager->CheckForCollision(m_objectSprite, m_name.c_str(), HitInfo, Collision::ALL)) {
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
        if (m_systemManager->CheckForCollision(m_objectSprite, m_name.c_str(), HitInfo, Collision::ALL)) {
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
        if (m_systemManager->CheckForCollision(m_objectSprite, m_name.c_str(), HitInfo, Collision::ALL)) {
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
        if (NewPosition != m_position && m_physicsBody.has_value() && m_physicsBody.value().isMoveable) {
            HandlePositionChange(NewPosition);
        }
    }

    sf::Vector2f GameObject::GetScale() const {
        return m_scale;
    }

    void GameObject::SetScale(sf::Vector2f NewScale) {
        if (NewScale != m_scale && m_physicsBody.has_value() && m_physicsBody.value().isMoveable) {
            HandleScaleChange(NewScale);
        }
    }

    float GameObject::GetRotation() const {
        return m_rotation;
    }

    void GameObject::SetRotation(float NewRotation ) {
        if (NewRotation != m_rotation && m_physicsBody.has_value() && m_physicsBody.value().isMoveable) {
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
        if (m_physicsBody.has_value())
        {
	        m_physicsBody.value().update(DeltaTime);
        }
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

    bool GameObject::IsShowen() const
    {
        return m_showen;
    }

    void GameObject::SetShowen(bool showen)
    {
        m_showen = showen;
    }

    bool GameObject::IsSceneObject() const
    {
        return m_isSceneObject;
    }

    void GameObject::SetIsSceneObject(bool val)
    {
        m_isSceneObject = val;
    }

    void GameObject::SetPhysicsBody(float mass, bool hasGravity)
    {
        m_physicsBody.emplace(mass, m_position, [this](sf::Vector2f newPos)
        {
        	this->SetPosition(newPos);
        });
        m_physicsBody.value().hasGravitation = hasGravity;
    }

    std::optional<Physics::PhysicsBody> GameObject::GetPhysicsBody()
    {
        return m_physicsBody;
    }
}
