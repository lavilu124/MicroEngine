#include "LightSource.hpp"

#include <algorithm>

#include "Constants.hpp"
#include "geometry/Line.hpp"
#include "geometry/Vector2.hpp"
#include "graphics/VertexArray.hpp"

namespace Micro{
    LightSource::LightSource(const std::string& name, int id)
        : m_color(sf::Color::White), m_id(id)
		, m_fade(true), m_name(name)
#ifdef CANDLE_DEBUG
        , m_debug(sf::Lines, 0)
#endif
        {}
    
    void LightSource::setIntensity(float intensity){
        m_color.a = 255 * intensity;
        resetColor();
    }
    
    float LightSource::getIntensity() const{
        return (float)m_color.a/255.f;
    }

    int LightSource::getID() const
    {
        return m_id;
    }

    void LightSource::setColor(const sf::Color& c){
        m_color = {c.r, c.g, c.b, m_color.a};
        resetColor();
    }
    
    sf::Color LightSource::getColor() const{
        const sf::Color &c = m_color;
        return {c.r, c.g, c.b, 255};
    }
    
    void LightSource::setFade(bool fade){
        m_fade = fade;
        resetColor();
    }
    
    bool LightSource::getFade() const{
        return m_fade;
    }
    
    void LightSource::setRange(float r){
        m_range = r;
    }
    
    float LightSource::getRange() const{
        return m_range;
    }
    

	std::string LightSource::getName() const {
        return m_name;
    }
}
