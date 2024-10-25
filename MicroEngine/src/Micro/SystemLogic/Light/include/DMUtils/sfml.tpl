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

#include <cmath>

#include <SFML/Graphics.hpp>

namespace DMUtils {
namespace sfml {

    template <typename T>
    T norm2(const sf::Vector2<T>& v) {
        return (v.x*v.x + v.y*v.y);
    }

    template <typename T>
    T norm(const sf::Vector2<T>& v) {
        return sqrt(norm2(v));
    }

	template <typename T>
	T dot(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
		return a.x*b.x + a.y*b.y;
	}

    template <typename T>
    float getAngleBetweenVectors(const sf::Vector2<T>& o, const sf::Vector2<T>& v) {
        return -(atan2(static_cast<double>(o.y),static_cast<double>(o.x)) - atan2(static_cast<double>(v.y),static_cast<double>(v.x)));
    }

    ///If you don't know the angle of your base vector use the function with 2 parameters
    ///if you do know this angle pass it to this function, it saves time
    template <typename T>
    sf::Vector2<T> rotate(const sf::Vector2<T>& v, float alpha, float vecRot) {
        float totAngle = alpha + vecRot;
        float n = sqrt(static_cast<float>(norm2<T>(v)));
        return sf::Vector2<T>(static_cast<T>(-sin(totAngle) * n),static_cast<T>(cos(totAngle) * n));
    }

    template <typename T>
    sf::Vector2<T> rotate(const sf::Vector2<T>& v, float alpha) {
        return rotate(v,alpha,getAngleBetweenVectors(sf::Vector2f(0.0f,1.0f),v));
    }

	template <typename T>
	sf::Vector2<T> rotate(const sf::Vector2<T>& v, float alpha, const sf::Vector2<T>& o) {
		return rotate(v-o,alpha)+o;
	}

	template <typename T>
	T isLeft(const sf::Vector2<T>& p0, const sf::Vector2<T>& p1, const sf::Vector2<T>& p2)
	{
		if(p0.y < p1.y)
			return ( (p1.x - p0.x) * (p2.y - p0.y) - (p2.x -  p0.x) * (p1.y - p0.y) );
		else
			return ( (p0.x - p1.x) * (p2.y - p1.y) - (p2.x -  p1.x) * (p0.y - p1.y) );
	}

	template <typename T>
	bool contains(const sf::ConvexShape& shapeParam, sf::Vector2<T> point)
	{
		sf::ConvexShape shape;
		shape.setPointCount(shapeParam.getPointCount());
		sf::Transform t = shapeParam.getTransform();
		for(size_t i = 0; i < shape.getPointCount(); ++i)
		{
			shape.setPoint(i, t.transformPoint(shapeParam.getPoint(i)));
		}
		
		size_t minyi = 0, maxyi = 0;
		T miny = shape.getPoint(0).y;
		T maxy = miny;

		for(size_t i = 0; i < shape.getPointCount(); ++i)
		{
			T y = shape.getPoint(i).y;
			if(y < miny)
			{
				minyi = i;
				miny = y;
			}
			else if(y > maxy)
			{
				maxyi = i;
				maxy = y;
			}
		}

		if(point.y < miny || point.y > maxy)
		{
			return false;
		}

		//find the two segments that surround the point in y axis
		//first going right side
		size_t rightSide1 = minyi, leftSide1 = minyi;
		size_t rightSide2 = minyi, leftSide2 = minyi;
		int i = minyi;
		int ip1;
		while(i != maxyi)
		{
			ip1 = i+1;
			if(ip1 >= shape.getPointCount())
			{
				ip1 = 0;
			}

			int minYtmp, maxYtmp;
			if(shape.getPoint(i).y < shape.getPoint(ip1).y)
			{
				minYtmp = i;
				maxYtmp = ip1;
			}
			else
			{
				maxYtmp = i;
				minYtmp = ip1;
			}

			if(point.y >= shape.getPoint(minYtmp).y && point.y <= shape.getPoint(maxYtmp).y)
			{
				rightSide1 = minYtmp;
				rightSide2 = maxYtmp;
			}
			++i;
			if(i >= shape.getPointCount())
			{
				i = 0;
			}
		}

		//leftSide
		i = minyi;
		while(i != maxyi)
		{
			ip1 = i-1;
			if(ip1 < 0)
			{
				ip1 = shape.getPointCount()-1;
			}

			int minYtmp, maxYtmp;
			if(shape.getPoint(i).y < shape.getPoint(ip1).y)
			{
				minYtmp = i;
				maxYtmp = ip1;
			}
			else
			{
				maxYtmp = i;
				minYtmp = ip1;
			}

			if(point.y >= shape.getPoint(minYtmp).y && point.y <= shape.getPoint(maxYtmp).y)
			{
				leftSide1 = minYtmp;
				leftSide2 = maxYtmp;
			}
			--i;
			if(i < 0)
			{
				i = shape.getPointCount()-1;
			}
		}

		return (isLeft(shape.getPoint(rightSide1), shape.getPoint(rightSide2), point) >= 0
			&& isLeft(shape.getPoint(leftSide1), shape.getPoint(leftSide2), point) <= 0)
			||
			(isLeft(shape.getPoint(rightSide1), shape.getPoint(rightSide2), point) <= 0
			&& isLeft(shape.getPoint(leftSide1), shape.getPoint(leftSide2), point) >= 0);
	}
}
}
