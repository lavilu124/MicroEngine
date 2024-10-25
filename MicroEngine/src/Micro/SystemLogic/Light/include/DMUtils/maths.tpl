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

namespace DMUtils {
namespace maths {

    template <typename T>
    inline const T& min(const T& a, const T& b) {
        return a<b?a:b;
    }

    template <typename T, typename ... Args>
    inline const T& min(const T& a, const T& b, const T& c, const Args& ... args) {
        return min(min(a,b),c,args ...);
    }

    template <typename T>
    inline const T& max(const T& a, const T& b) {
        return a>b?a:b;
    }

    template <typename T, typename ... Args>
    inline const T& max(const T& a, const T& b, const T& c, const Args& ... args) {
        return max(max(a,b),c,args ...);
    }


    template <int N>
    struct power {
		template <typename T>
		static inline T of(const T& val) {
			return val*power<N-1>::of(val);
		}
	};

	template <>
	struct power<0> {
		template <typename T>
		static inline T of(const T& val) {
			return T(1);
		}
	};

    template <typename T>
    inline T clamp(const T& val, const T& min, const T& max) {
		return val<min?min:(val>max?max:val);
	}

    template <typename T>
    inline T abs(const T& val) {
		return val<T(0)?-val:val;
	}

    template <typename T>
    inline int signOf(const T& val) {
		return (T(0)<val)-(val<T(0));
	}

    template <typename T>
    constexpr inline T degToRad(const T& a) {
		return a * T(M_PI) / T(180.0);
	}

    template <typename T>
    constexpr inline T radToDeg(const T& a) {
		return a * T(180.0) / T(M_PI);
	}

	template <int N>
	struct fact {
		enum {value = N*fact<N-1>::value};
	};

	template<>
	struct fact<0> {
		enum {value = 1};
	};

}
}
