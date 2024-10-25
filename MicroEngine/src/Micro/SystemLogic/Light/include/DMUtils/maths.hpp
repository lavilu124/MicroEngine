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

#ifndef HEADER_DMUTILS_MATH
#define HEADER_DMUTILS_MATH

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

#define M_PIf       3.14159265358979323846f

namespace DMUtils {
namespace maths {

    template <typename T>
    inline const T& min(const T& a, const T& b);

    template <typename T, typename ... Args>
    inline const T& min(const T& a, const T& b, const T& c, const Args& ... args);

    template <typename T>
    inline const T& max(const T& a, const T& b);

    template <typename T, typename ... Args>
    inline const T& max(const T& a, const T& b, const T& c, const Args& ... args);

    template <int N>
    struct power;

    template <typename T>
    inline T clamp(const T& val, const T& min, const T& max);

    template <typename T>
    inline T abs(const T& val);

    template <typename T>
    inline int signOf(const T& val);

    template <typename T>
    constexpr inline T degToRad(const T& a);

    template <typename T>
    constexpr inline T radToDeg(const T& a);
	
	template <int N>
	struct fact;

	/*
	inline float sqrt(float number) {
		long i;
		float x, y;
		const float f = 1.5F;
		x = number * 0.5F;
		y = number;
		i = * ( long * ) &y;
		i = 0x5f3759df - ( i >> 1 );
		y = * ( float * ) &i;
		y = y * ( f - ( x * y * y ) );
		y = y * ( f - ( x * y * y ) );
		return number * y;
	}
	//*/

}
}

#include "maths.tpl"
#endif // HEADER_DMUTILS_MATH
