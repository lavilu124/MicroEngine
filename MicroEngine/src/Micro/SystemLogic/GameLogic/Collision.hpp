#ifndef COLLISION_H
#define COLLISION_H


#include <vector>
#include "../../Core.h"

namespace Micro {
    namespace Collision {
        enum MICRO_API collisionLayer {
            COLLIDER,
            CUSTOMLAYERR_ONE,
            CUSTOMLAYERR_TWO,
            CUSTOMLAYERR_THREE,
            CUSTOMLAYERR_FOUR,
            CUSTOMLAYERR_FIVE,

            TRIGGER,
            IGNORE,
            CUSTOMLAYERR_SIX,
            CUSTOMLAYERR_SEVEN,
            CUSTOMLAYERR_EIGHT,
            CUSTOMLAYERR_NINE,
            CUSTOMLAYERR_TEN,
            ALL
        };


        bool MICRO_API PixelPerfectCollision(const sf::Sprite& Object1, const sf::Sprite& Object2, sf::Uint8 AlphaLimit = 0);


        bool MICRO_API CreateTextureAndBitmask(sf::Texture& LoadInto, const std::string& Filename);


        bool MICRO_API CircleTest(const sf::Sprite& Object1, const sf::Sprite& Object2);


        bool MICRO_API BoundingBoxTest(const sf::Sprite& Object1, const sf::Sprite& Object2);

        MICRO_API std::array<int, 4> GetcollisonPosition();
    }
}

#endif	/* COLLISION_H */
