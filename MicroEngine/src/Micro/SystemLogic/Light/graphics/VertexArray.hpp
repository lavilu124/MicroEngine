#ifndef __SFML_UTIL_GRAPHICS_VERTEXARRAY_HPP__
#define __SFML_UTIL_GRAPHICS_VERTEXARRAY_HPP__

#include "SFML/Graphics/VertexArray.hpp"
#include "../../../core.h"


namespace sfu{
    MICRO_API void setColor(sf::VertexArray& va, const sf::Color& color);
    MICRO_API void transform(sf::VertexArray& va, const sf::Transform& t);
    MICRO_API void move(sf::VertexArray& va, const sf::Vector2f& d);
    MICRO_API void darken(sf::VertexArray& va, float r);
    MICRO_API void lighten(sf::VertexArray& va, float r);
    MICRO_API void interpolate(sf::VertexArray& va, const sf::Color& c, float r);
    MICRO_API void complementary(sf::VertexArray& va);
}

#endif
