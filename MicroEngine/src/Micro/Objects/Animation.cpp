#include "Animation.h"

namespace Micro{
    //set all animation values in constructor
    Animation::Animation(std::vector<sf::Sprite>& animationVector, float speed) : animationVector(animationVector), m_speed(speed){
    }

    //run the animation based on a timer
    void Animation::RunAnimation(float deltaTime, GameObject& object) {
        m_counter -= deltaTime;

        //reset the animation if reach the end
        if (m_index >= animationVector.size() - 1) {
            m_index = 0;
        }
        if (m_counter <= 0) {//reset the timer and switch the frame
            m_index++;
            m_counter = m_speed;
        }

        object.SetSprite(animationVector[m_index]);
    }

    //reset the animation
    void Animation::ResetAnimation() {
        m_index = 0;
        m_counter = m_speed;
    }

    //return the speed of the animaiton
    float Animation::GetSpeed() {
        return m_speed;
    }

    //set the speed of the animation
    void Animation::SetSpeed(float speed) {
        this->m_speed = speed;
    }
}