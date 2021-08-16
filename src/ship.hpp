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
        WAR_GALLEY,
        CARRACK,
        HULK,
        PIRATE_PINNACE,
        WAR_CANOES,
        ORC_HULK
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

        std::vector<Cargo::Type> Cargo = {};

        int MaximumCargo = 0;

        Base()
        {

        }

        Base(const char*name, Ship::Type type, Location::Type location, int fighting, int health, int maximumCargo)
        {
            Name = name;

            Type = type;

            Location = location;

            Fighting = fighting;

            Health = health;

            MaximumHealth = health;

            MaximumCargo = maximumCargo;
        }
    };
}

#endif