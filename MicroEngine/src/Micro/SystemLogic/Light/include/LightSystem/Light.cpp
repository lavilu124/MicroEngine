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

#include "Light.hpp"

#include "LightSystem.hpp"
#include "ShadowSystem.hpp"

namespace Micro
{
namespace ls
{
    const char Light::LAS_PARAM_CENTER[] = "center";
    const char Light::LAS_PARAM_RADIUS[] = "radius";
    const char Light::LAS_PARAM_COLOR[] = "color";
    const char Light::LAS_PARAM_BLEED[] = "bleed";
    const char Light::LAS_PARAM_LINEARITY[] = "linearFactor";

	Light::Light(const sf::Vector2f& p, const sf::Color& c, std::string name, int id) : _aabb(), _position(sf::Vector2f(p.x, p.y * -1)), _color(c), _renderTexture(nullptr), _shadowTexture(nullptr), _attributes(Light::ACTIVE), _system(nullptr), m_name(name), id(id)
    {
    }

    Light::~Light()
    {
        setSystem(nullptr);

        if (_renderTexture != nullptr) {
            delete _renderTexture;
        }
        _renderTexture = nullptr;
    }

    void Light::debugRender(sf::RenderTarget& target, const sf::RenderStates &states)
    {
    }

    void Light::drawAABB(const sf::IntRect& screen, sf::RenderTarget& target)
    {
        sf::IntRect box = getAABB();
        sf::Vertex lines[] =
        {
            sf::Vertex(sf::Vector2f(box.left, box.top),_color),
            sf::Vertex(sf::Vector2f(box.left+box.width, box.top),_color),
            sf::Vertex(sf::Vector2f(box.left+box.width, box.top+box.height),_color),
            sf::Vertex(sf::Vector2f(box.left, box.top+box.height),_color),
            lines[0]
        };

        target.draw(lines,5,sf::LinesStrip);
    }

    void Light::calcShadow(const std::list<sf::ConvexShape>& walls)
    {
    }

    bool Light::isIsometric() const
    {
        return _attributes & ISOMETRIC;
    }

    sf::IntRect Light::getAABB()
    {
        return sf::IntRect(sf::Vector2i(_aabb.left+static_cast<int>(_position.x),_aabb.top+static_cast<int>(_position.y)),sf::Vector2i(_aabb.width,_aabb.height));
    }

    sf::FloatRect Light::getBoundaries()
    {
        const sf::IntRect rect = getAABB();
        return sf::FloatRect(rect.left,rect.top,rect.width,rect.height);
    }

    void Light::setPosition(const sf::Vector2f& c)
    {
        const sf::Vector2f& p = sf::Vector2f(c.x, c.y * -1);
        _position = p;
        _sprite.setPosition(p);
    }

    sf::Vector2f Light::getPosition() const
    {
        return _position;
    }

    void Light::move(const sf::Vector2f& delta)
    {
        _position += delta;
    }

    void Light::setColor(const sf::Color& c)
    {
        _color = c;
    }

    sf::Color Light::getColor() const
    {
        return _color;
    }

    void Light::setIsometric(bool i)
    {
        if(i) _setAttribute(ISOMETRIC);
        else _unsetAttribute(ISOMETRIC);
    }

    bool Light::isNegative()
    {
        return _attributes & NEGATIVE;
    }

    void Light::setNegative(bool n)
    {
        if(isEmissive()) return;//an emissive light cannot be negative
        if(n) _setAttribute(NEGATIVE);
        else _unsetAttribute(NEGATIVE);
    }

    int Light::getId() const {
        return id;
    }

    std::string Light::GetName() const {
		return m_name;
    }

	sf::Sprite Light::getSprite() const
	{
		return _sprite;
	}	

    void Light::assignID(int id) {
        if (this->id == -1) {
			this->id = id;
        }
    }

    void Light::setActive(bool a)
    {
        if(a) _setAttribute(ACTIVE);
        else _unsetAttribute(ACTIVE);
    }

    bool Light::isActive() const
    {
        return _attributes & ACTIVE;
    }

    void Light::setEmissive(bool e)
    {
        if(e)
        {
            _setAttribute(EMISSIVE);
            _unsetAttribute(NEGATIVE);//an emissive light cannot be negative
        }
        else _unsetAttribute(EMISSIVE);
    }

    bool Light::isEmissive() const
    {
        return _attributes & EMISSIVE;
    }

    void Light::removeFromSystem()
    {
        if(_system) _system->removeLight(this);
        setSystem(nullptr);
    }

    void Light::setSystem(LightSystem* ls)
    {
        _system = ls;
    }

    LightSystem* Light::getSystem() const
    {
        return _system;
    }

    /// PROTECTED

    void Light::_setAttribute(Light::Attributes a)
    {
        _attributes |= a;
    }

    void Light::_unsetAttribute(Light::Attributes a)
    {
        _attributes &= ~a;
    }

}
}
