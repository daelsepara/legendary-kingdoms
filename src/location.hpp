#ifndef __LOCATION__HPP__
#define __LOCATION__HPP__

#include "location.hpp"

namespace Location
{
    enum class Type
    {
        NONE = -1,
        SALTAD
    };

    enum class BattleField
    {
        NONE = -1,
        LEFT_FLANK_FRONT,
        LEFT_FLANK_SUPPORT,
        CENTRE_FRONT,
        CENTRE_SUPPORT,
        RIGHT_FLANK_FRONT,
        RIGHT_FLANK_SUPPORT
    };
}

#endif