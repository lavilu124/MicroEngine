#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "GameObject.h"
#include "../Core.h"

namespace Micro{
    class MICRO_API Animation {
    public:
        //constructor for animation
        Animation(std::vector <sf::Sprite>& AnimationVector, float Speed = 1);


        //all the pictures of the animation in order
        std::vector <sf::Sprite>& AnimationVector;

        //all the methods for running an animation
        void RunAnimation(float Dletatime, GameObject Object);
        sf::Sprite GetCurrentSprite();
        void ResetAnimation();

        //setter and getter for animation speed
        float GetSpeed();
        void SetSpeed(float Speed);

    private:
        //all the variable to keep track of the animation
        float m_speed;
        float m_counter = 0;
        int m_index = 0;
        sf::Sprite m_currentSprite;
    };
}
