#ifndef __SHIP__HPP__
#define __SHIP__HPP__

#include "location.hpp"

namespace Ship
{
    enum class Type
    {
        NONE = -1
    };

    class Base
    {
    public:
        Ship::Type Type = Ship::Type::NONE;
        
        Location::Type Location = Location::Type::NONE;

        Base()
        {

        }
    };
}

#endif