/*
Copyright DaiMysha (c) 2015, All rights reserved.
DaiMysha@gmail.com
https://github.com/DaiMysha

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef HEADER_DMGDVT_EMISSIVELIGHT
#define HEADER_DMGDVT_EMISSIVELIGHT
#include "../../../../Core.h"

#include "Light.hpp"

#include <SFML/Graphics.hpp>

namespace Micro
{
namespace ls
{

    class MICRO_API SpriteLight : public Light
    {
        public:
            SpriteLight(const sf::Vector2f& p, const sf::Color& c, float a, const sf::Sprite& spr, std::string name, int id = -1);
            virtual ~SpriteLight();

            virtual void render(const sf::IntRect& screen, sf::RenderTarget& target, sf::Shader* shader, const sf::RenderStates &states=sf::RenderStates::Default) override;
            virtual void preRender(sf::Shader* shader) override;
            virtual void debugRender(sf::RenderTarget& target, const sf::RenderStates &states) override;

            virtual void computeAABB() override;

            void setAngle(float a);
            float getAngle() const;

            void setSprite(const sf::Sprite& s);

        protected:
            float _angle;
    };

}
}

#endif // HEADER_DMGDVT_EMISSIVELIGHT

