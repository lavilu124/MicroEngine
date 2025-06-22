 #ifndef __CANDLE_CONSTANTS_HPP__
#define __CANDLE_CONSTANTS_HPP__

#include "../../Core.h"

namespace ls{
    /**
     * PI constant.
     */
	inline constexpr float PI = 3.1415926f;
    enum lightType { directed, radial };

	struct LightId
	{
		int id;
		ls::lightType type;
	};
}

#endif
