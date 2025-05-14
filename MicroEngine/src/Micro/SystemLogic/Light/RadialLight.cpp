#include <memory>
#include "RadialLight.hpp"

#include "SFML/Graphics.hpp"

#include "graphics/VertexArray.hpp"
#include "geometry/Vector2.hpp"
#include "geometry/Line.hpp"

namespace Micro{
    int RadialLight::s_instanceCount = 0;
    const float BASE_RADIUS = 400.0f;
    bool l_texturesReady(false);
    std::unique_ptr<sf::RenderTexture> l_lightTextureFade;
    std::unique_ptr<sf::RenderTexture> l_lightTexturePlain;

    void initializeTextures(){
        int points = 100;

        l_lightTextureFade.reset(new sf::RenderTexture);
        l_lightTexturePlain.reset(new sf::RenderTexture);
        l_lightTextureFade->create(BASE_RADIUS*2 + 2, BASE_RADIUS*2 + 2);
        l_lightTexturePlain->create(BASE_RADIUS*2 + 2, BASE_RADIUS*2 + 2);

        sf::VertexArray lightShape(sf::TriangleFan, points+2);
        float step = ls::PI*2.f/points;
        lightShape[0].position = {BASE_RADIUS + 1, BASE_RADIUS + 1};
        for(int i = 1; i < points+2; i++){
            lightShape[i].position = {
                (std::sin(step*(i)) + 1) * BASE_RADIUS + 1,
                (std::cos(step*(i)) + 1) * BASE_RADIUS + 1
            };
            lightShape[i].color.a = 0;
        }
        l_lightTextureFade->clear(sf::Color::Transparent);
        l_lightTextureFade->draw(lightShape);
        l_lightTextureFade->display();
        l_lightTextureFade->setSmooth(true);

        ls::setColor(lightShape, sf::Color::White);
        l_lightTexturePlain->clear(sf::Color::Transparent);
        l_lightTexturePlain->draw(lightShape);
        l_lightTexturePlain->display();
        l_lightTexturePlain->setSmooth(true);
    }

    float module360(float x){
        x = (float)fmod(x,360.f);
        if(x < 0.f) x += 360.f;
        return x;
    }

    RadialLight::RadialLight(const char* name, int id)
        : LightSource(name, id)
        {
        if(!l_texturesReady){
            // The first time we create a RadialLight, we must create the textures
            initializeTextures();
            l_texturesReady = true;
        }
        m_polygon.setPrimitiveType(sf::TriangleFan);
        m_polygon.resize(6);
        m_polygon[0].position =
        m_polygon[0].texCoords = {BASE_RADIUS+1, BASE_RADIUS+1};
        m_polygon[1].position =
        m_polygon[5].position =
        m_polygon[1].texCoords =
        m_polygon[5].texCoords = {0.f, 0.f};
        m_polygon[2].position =
        m_polygon[2].texCoords = {BASE_RADIUS*2 + 2, 0.f};
        m_polygon[3].position =
        m_polygon[3].texCoords = {BASE_RADIUS*2 + 2, BASE_RADIUS*2 + 2};
        m_polygon[4].position =
        m_polygon[4].texCoords = {0.f, BASE_RADIUS*2 + 2};
        Transformable::setOrigin(BASE_RADIUS, BASE_RADIUS);
        SetRange(1.0f);
        SetBeamAngle(360.f);
        // castLight();
        s_instanceCount++;
    }

    RadialLight::~RadialLight(){
        s_instanceCount--;
    }

    void RadialLight::draw(sf::RenderTarget& t, sf::RenderStates s) const{
        sf::Transform trm = Transformable::getTransform();
        trm.scale(m_range/BASE_RADIUS, m_range/BASE_RADIUS, BASE_RADIUS, BASE_RADIUS);
        s.transform *= trm;
        s.texture = m_fade ? &l_lightTextureFade->getTexture() : &l_lightTexturePlain->getTexture();
        if(s.blendMode == sf::BlendAlpha){
            s.blendMode = sf::BlendAdd;
        }
        t.draw(m_polygon, s);

    }
    void RadialLight::ResetColor(){
        ls::setColor(m_polygon, m_color);
    }

    void RadialLight::SetBeamAngle(float r){
        m_beamAngle = module360(r);
    }

    float RadialLight::GetBeamAngle() const{
        return m_beamAngle;
    }

    sf::FloatRect RadialLight::GetLocalBounds() const{
        return sf::FloatRect(0.0f, 0.0f, BASE_RADIUS*2, BASE_RADIUS*2);
    }

    sf::FloatRect RadialLight::GetGlobalBounds() const{
        float scaledRange = m_range / BASE_RADIUS;
        sf::Transform trm = Transformable::getTransform();
        trm.scale(scaledRange, scaledRange, BASE_RADIUS, BASE_RADIUS);
        return trm.transformRect( GetLocalBounds() );
    }

    void RadialLight::CastLight(const EdgeVector::iterator& begin, const EdgeVector::iterator& end){
        float scaledRange = m_range / BASE_RADIUS;
        sf::Transform trm = Transformable::getTransform();
        trm.scale(scaledRange, scaledRange, BASE_RADIUS, BASE_RADIUS);
        std::vector<ls::Line> rays;

        rays.reserve(2 + std::distance(begin, end) * 2 * 3); // 2: beam angle, 4: corners, 2: pnts/sgmnt, 3 rays/pnt

        // Start casting
        float bl1 = module360(getRotation() - m_beamAngle/2);
        float bl2 = module360(getRotation() + m_beamAngle/2);
        bool beamAngleBigEnough = m_beamAngle < 0.1f;
        auto castPoint = Transformable::getPosition();
        float off = .001f;

        auto angleInBeam = [&](float a)-> bool {
            return beamAngleBigEnough
                   ||(bl1 < bl2 && a > bl1 && a < bl2)
                   ||(bl1 > bl2 && (a > bl1 || a < bl2));
        };

        for(float a = 45.f; a < 360.f; a += 90.f){
            if(beamAngleBigEnough || angleInBeam(a)){
                rays.emplace_back(castPoint, a);
            }
        }

        sf::FloatRect lightBounds = GetGlobalBounds();
        for(auto it = begin; it != end; it++){
            auto& s = *it;

            //Only cast a ray if the line is in range
            if( lightBounds.intersects( s.getGlobalBounds() ) ){
                ls::Line r1(castPoint, s.m_origin);
                ls::Line r2(castPoint, s.point(1.f));
                float a1 = ls::angle(r1.m_direction);
                float a2 = ls::angle(r2.m_direction);
                if(angleInBeam(a1)){
                    rays.push_back(r1);
                    rays.emplace_back(castPoint, a1 - off);
                    rays.emplace_back(castPoint, a1 + off);
                }
                if(angleInBeam(a2)){
                    rays.push_back(r2);
                    rays.emplace_back(castPoint, a2 - off);
                    rays.emplace_back(castPoint, a2 + off);
                }
            }
        }

        if(bl1 > bl2){
            std::sort(
                rays.begin(),
                rays.end(),
                [bl1, bl2] (ls::Line& r1, ls::Line& r2){
                    float _bl1 = bl1-0.1;
                    float _bl2 = bl2+0.1;
                    float a1 = ls::angle(r1.m_direction);
                    float a2 = ls::angle(r2.m_direction);
                    return (a1 >= _bl1 && a2 <= _bl2) || (a1 < a2 && (_bl1 <= a1 || a2 <= _bl2));
                }
            );
        }else{
            std::sort(
                rays.begin(),
                rays.end(),
                [bl1] (ls::Line& r1, ls::Line& r2){
                    return
                        ls::angle(r1.m_direction) < ls::angle(r2.m_direction);
                }
            );
        }
        if(!beamAngleBigEnough){
            rays.emplace(rays.begin(), castPoint, bl1);
            rays.emplace_back(castPoint, bl2);
        }

        sf::Transform tr_i = trm.getInverse();
        // keep only the ones within the area
        std::vector<sf::Vector2f> points;
        points.reserve(rays.size());
        for (auto& r: rays){
            points.push_back(tr_i.transformPoint(castRay(begin, end,    r, m_range*m_range)));
        }
        m_polygon.resize(points.size() + 1 + beamAngleBigEnough); // + center and last
        m_polygon[0].color = m_color;
        m_polygon[0].position = m_polygon[0].texCoords = tr_i.transformPoint(castPoint);
        for(unsigned i=0; i < points.size(); i++){
            sf::Vector2f p = points[i];
            m_polygon[i+1].position = p;
            m_polygon[i+1].texCoords = p;
            m_polygon[i+1].color = m_color;

        }
        if(beamAngleBigEnough){
            m_polygon[points.size()+1] = m_polygon[1];
        }
    }

}
