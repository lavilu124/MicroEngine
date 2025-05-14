#include "Animation.h"

namespace Micro{
    //set all animation values in constructor
    Animation::Animation(std::vector<sf::Sprite>& AnimationVector, float Speed) : AnimationVector(AnimationVector), m_speed(Speed){
    }

    //run the animation based on a timer
    void Animation::RunAnimation(float Dletatime, GameObject Object) {
        m_counter -= Dletatime;

        //reset the animation if reach the end
        if (m_index >= AnimationVector.size() - 1) {
            m_index = 0;
        }
        if (m_counter <= 0) {//reset the timer and switch the frame
            m_index++;
            m_counter = m_speed;
        }

        //set currentSprtie vales match the player's values
        m_currentSprite = AnimationVector[m_index];
        m_currentSprite.setScale(Object.GetScale());
        m_currentSprite.setPosition(Object.GetPosition());
        m_currentSprite.setRotation(Object.GetRotation());
    }

    //return the current frame of the animation
    sf::Sprite Animation::GetCurrentSprite() {
        return m_currentSprite;
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
    void Animation::SetSpeed(float Speed) {
        this->m_speed = Speed;
    }
}