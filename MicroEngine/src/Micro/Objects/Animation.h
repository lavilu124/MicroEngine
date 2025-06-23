#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "GameObject.h"
#include "../Core.h"

namespace Micro{
    class MICRO_API Animation {
    public:
        //constructor for animation
        Animation(std::vector <sf::Sprite>& animationVector, float speed = 1);


        //all the pictures of the animation in order
        std::vector <sf::Sprite>& animationVector;

        //all the methods for running an animation
        void RunAnimation(float deltaTime, GameObject& object);
        void ResetAnimation();

        //setter and getter for animation speed
        float GetSpeed();
        void SetSpeed(float speed);

    private:
        //all the variable to keep track of the animation
        float m_speed;
        float m_counter = 0;
        int m_index = 0;
    };
}
