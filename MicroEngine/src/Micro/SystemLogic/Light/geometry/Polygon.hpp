#ifndef __SFML_UTIL_GEOMETRY_POLYGON_HPP__
#define __SFML_UTIL_GEOMETRY_POLYGON_HPP__

#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "Line.hpp"

#include "../../../core.h"


namespace ls{
    /**
     * @brief Auxiliary class to represent a polygon as vector of lines.
     * Not meant to be used outside Candle.
     */
    struct MICRO_API Polygon{
        std::vector<ls::Line> lines;
        Polygon(const sf::Vector2f* points, int n);
        void initialize(const sf::Vector2f* points, int n);
        template <typename T>
        Polygon(const sf::Rect<T> rect){
            initialize(rect);
        }
        template <typename T>
        void initialize(const sf::Rect<T>& rect){
            sf::Vector2f lt(rect.left, rect.top);
            sf::Vector2f rt(rect.right, rect.top);
            sf::Vector2f lb(rect.left, rect.bottom);
            sf::Vector2f rb(rect.right, rect.bottom);
            lines.emplace_back(lt, rt);
            lines.emplace_back(rt, rb);
            lines.emplace_back(rb, lb);
            lines.emplace_back(lb, lt);
        }
    };
}

#endif
