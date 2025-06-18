#pragma once
#include <Micro.h>


class Template : public Micro::GameObject 
{
public:
    Template (Micro::SystemManager* systemManager, const sf::Sprite& sprite, const std::string& name, Micro::Collision::collisionLayer layer = Micro::Collision::collisionLayer::COLLIDER, int level = 0)
; 



    void Start() override;
    void Update(float DeltaTime) override;
 
};