#include "Animation.h"

namespace Micro {
    //set all animation values in constructor
    Animation::Animation(std::vector<sf::Sprite>& AnimationVector, float Speed) {
        this->AnimationVector = AnimationVector;
        this->Speed = Speed;
    }

    //run the animation based on a timer
    void Animation::RunAnimation(float Dletatime, GameObject Object) {
        Counter -= Dletatime;

        //reset the animation if reach the end
        if (Index >= AnimationVector.size() - 1) {
            Index = 0;
        }
        if (Counter <= 0) {//reset the timer and switch the frame
            Index++;
            Counter = Speed;
        }

        //set currentSprtie vales match the player's values
        CurrentSprite = AnimationVector[Index];
        CurrentSprite.setScale(Object.GetScale());
        CurrentSprite.setPosition(Object.GetPosition());
        CurrentSprite.setRotation(Object.GetRotation());
    }

    //return the current frame of the animation
    sf::Sprite Animation::GetCurrentSprite() {
        return CurrentSprite;
    }

    //reset the animation
    void Animation::ResetAnimation() {
        Index = 0;
        Counter = Speed;
    }

    //return the speed of the animaiton
    float Animation::GetSpeed() {
        return Speed;
    }

    //set the speed of the animation
    void Animation::SetSpeed(float Speed) {
        this->Speed = Speed;
    }
}