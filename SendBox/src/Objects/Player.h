#pragma once
#include <Micro.h>
//#include <iostream>

class Player : public Micro::GameObject
{
public:
    Player(Micro::SystemManager* systemManager, const sf::Sprite& sprite, const std::string& name, Micro::Collision::collisionLayer layer, int level)
        : GameObject(systemManager, sprite, name, layer, level) {}

    void Start() override {
        //std::cout << "test";
    }

    void test() {
        //std::cout << "testing";
    }
};

