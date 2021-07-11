#ifndef __SHIP__HPP__
#define __SHIP__HPP__

#include <vector>

#include "cargo.hpp"
#include "location.hpp"

namespace Ship
{
    enum class Type
    {
        NONE = -1,
        COG,
        CURSITE_WAR_GALLEY,
        CARRACK
    };

    class Base
    {
    public:

        const char *Name = NULL;
        
        Ship::Type Type = Ship::Type::NONE;
        
        Location::Type Location = Location::Type::NONE;

        int Fighting = 0;

        int Health = 0;

        int MaximumHealth = 0;

        std::vector<Cargo::Type> Cargo = std::vector<Cargo::Type>();

        int MaximumCargo = 0;

        Base()
        {

        }
    };
}

#endif