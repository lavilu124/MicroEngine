/** 
BSD 2-Clause License

Copyright (c) 2017, DaiMysha
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef HEADER_DMUTILS_SFML
#define HEADER_DMUTILS_SFML

#include "../DMUtils/maths.hpp"
#include <SFML/Graphics.hpp>

namespace DMUtils {
namespace sfml {

    template <typename T>
    T norm2(const sf::Vector2<T>& v);

    template <typename T>
    T norm(const sf::Vector2<T>& v);
	
	template <typename T>
	T dot(const sf::Vector2<T>& a, const sf::Vector2<T>& b);

    template <typename T>
    float getAngleBetweenVectors(const sf::Vector2<T>& o, const sf::Vector2<T>& v);

    ///If you don't know the angle of your base vector use the function with 2 parameters
    ///if you do know this angle pass it to this function, it saves time
    /// rads
    template <typename T>
    sf::Vector2<T> rotate(const sf::Vector2<T>& v, float alpha, float vecRot);

    template <typename T>
    sf::Vector2<T> rotate(const sf::Vector2<T>& v, float alpha);

    ///Rotates the point v around the origin o of alpha degrees
    template <typename T>
    sf::Vector2<T> rotate(const sf::Vector2<T>& v, float alpha, const sf::Vector2<T>& o);

    ///Returns an AABB englobing all the pixels visible in the selected view on the world coordinates
	inline sf::IntRect getViewInWorldAABB(const sf::View& view) {
        sf::Vector2f p[4];
        sf::Vector2f c = view.getCenter();
        sf::Vector2f s2 = view.getSize()/2.0f;
        float alpha = view.getRotation();
        p[0] = c - s2;
        p[1] = c + sf::Vector2f(s2.x,-s2.y);
        p[2] = c + s2;
        p[3] = c + sf::Vector2f(-s2.x,s2.y);

        for(int i=0;i<4;++i) p[i] = DMUtils::sfml::rotate(p[i],DMUtils::maths::degToRad(-alpha),c);

        float xmin = DMUtils::maths::min(p[0].x,p[1].x,p[2].x,p[3].x);
        float xmax = DMUtils::maths::max(p[0].x,p[1].x,p[2].x,p[3].x);
        float ymin = DMUtils::maths::min(p[0].y,p[1].y,p[2].y,p[3].y);
        float ymax = DMUtils::maths::max(p[0].y,p[1].y,p[2].y,p[3].y);

        return sf::IntRect(sf::Vector2i(xmin,ymin),sf::Vector2i(xmax-xmin,ymax-ymin));
    }
	
	///tests if p2 is left of the line p0 -> p1
	///returns >0 if left, 0 if on the line, <0 if right
	template <typename T>
	T isLeft(const sf::Vector2<T>& p0, const sf::Vector2<T>& p1, const sf::Vector2<T>& p2);
	
	///returns true if the point p is inside the shape s
	template <typename T>
	bool contains(const sf::ConvexShape& s, sf::Vector2<T> p);
}
}

#include "sfml.tpl"
#endif // HEADER_DMUTILS_SFML
