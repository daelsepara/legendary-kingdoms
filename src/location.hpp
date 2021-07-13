#ifndef __LOCATION__HPP__
#define __LOCATION__HPP__

#include <map>

#include "location.hpp"

namespace Location
{
    enum class Type
    {
        NONE = -1,
        CLIFFTOP,
        SALTDAD,
        CURSUS
    };

    std::map<Location::Type, const char *> Description = {
        {Location::Type::NONE, "Unknown"},
        {Location::Type::CLIFFTOP, "Clifftop"},
        {Location::Type::SALTDAD, "Saltdad"},
        {Location::Type::CURSUS, "Cursus"}};

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