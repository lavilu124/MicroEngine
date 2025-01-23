#ifndef __CANDLE_CONSTANTS_HPP__
#define __CANDLE_CONSTANTS_HPP__

#include "../../Core.h"

namespace sfu{
    /**
     * PI constant.
     */
    MICRO_API extern const float PI;

    enum lightType { directed, radial };

	struct LightId
	{
		int id;
		sfu::lightType type;
	};
}

#endif
