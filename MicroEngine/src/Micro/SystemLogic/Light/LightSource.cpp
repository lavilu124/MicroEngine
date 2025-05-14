#include "LightSource.hpp"

#include <algorithm>

#include "Constants.hpp"
#include "geometry/Line.hpp"
#include "geometry/Vector2.hpp"
#include "graphics/VertexArray.hpp"

namespace Micro{
    LightSource::LightSource(const char* name, int id)
        : m_color(sf::Color::White), m_id(id)
		, m_fade(true), m_name(name)
#ifdef CANDLE_DEBUG
        , m_debug(sf::Lines, 0)
#endif
        {}
    
    void LightSource::SetIntensity(float intensity){
        m_color.a = 255 * intensity;
        ResetColor();
    }
    
    float LightSource::GetIntensity() const{
        return (float)m_color.a/255.f;
    }

    int LightSource::GetID() const
    {
        return m_id;
    }

    void LightSource::SetColor(const sf::Color& c){
        m_color = {c.r, c.g, c.b, m_color.a};
        ResetColor();
    }
    
    sf::Color LightSource::GetColor() const{
        const sf::Color &c = m_color;
        return {c.r, c.g, c.b, 255};
    }
    
    void LightSource::SetFade(bool fade){
        m_fade = fade;
        ResetColor();
    }
    
    bool LightSource::GetFade() const{
        return m_fade;
    }
    
    void LightSource::SetRange(float r){
        m_range = r;
    }
    
    float LightSource::GetRange() const{
        return m_range;
    }
    

    const char* LightSource::GetName() const {
        return m_name;
    }
}
