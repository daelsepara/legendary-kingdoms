#ifndef __ARMY__HPP__
#define __ARMY__HPP__

#include <vector>

#include "location.hpp"

namespace Army
{
    enum class Type
    {
        NONE = -1,
        LHASBREATH_BERSERKERS
    };

    class Base
    {
    public:

        const char *Name = NULL;
        
        Army::Type Type = Army::Type::NONE;
        
        Location::Type Garrison = Location::Type::NONE;

        int Strength = 0;

        int MaximumStrength = 0;

        int Morale = 0;

        int MaximumMorale = 0;

        Location::BattleField Position = Location::BattleField::NONE;

        Base()
        {

        }

        Base(const char* name, Army::Type type, Location::Type garrison, int strength, int morale)
        {
            Name = name;

            Type = type;

            Garrison = garrison;

            Strength = strength;

            MaximumStrength = strength;

            Morale = morale;

            MaximumMorale = morale;
        }
    };
}

#endif