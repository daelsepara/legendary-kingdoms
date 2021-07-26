#ifndef __ARMY__HPP__
#define __ARMY__HPP__

#include <vector>

#include "location.hpp"

namespace Army
{
    enum class Status
    {
        STRENGTH1 = 0
    };

    enum class Type
    {
        NONE = -1,
        LHASBREATH_BERSERKERS,
        BRONZEGUARD,
        CURSITE_INFANTRY,
        CURSITE_RIDERS,
        CURSITE_ZEALOTS,
        CITIZEN_ARCHERS,
        MERCENARY_KNIGHTS,
        MERCENARY_SPEARS
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

        bool Unique = false;

        Location::BattleField Position = Location::BattleField::NONE;

        Base()
        {
        }

        Base(const char *name, Army::Type type, Location::Type garrison, int strength, int morale)
        {
            Name = name;

            Type = type;

            Garrison = garrison;

            Strength = strength;

            MaximumStrength = strength;

            Morale = morale;

            MaximumMorale = morale;
        }

        Base(const char *name, Army::Type type, Location::Type garrison, int strength, int morale, bool unique)
        {
            Name = name;

            Type = type;

            Garrison = garrison;

            Strength = strength;

            MaximumStrength = strength;

            Morale = morale;

            MaximumMorale = morale;

            Unique = unique;
        }

        Base(const char *name, Army::Type type, Location::Type garrison, Location::BattleField position, int strength, int morale, bool unique)
        {
            Name = name;

            Type = type;

            Garrison = garrison;

            Position = position;

            Strength = strength;

            MaximumStrength = strength;

            Morale = morale;

            MaximumMorale = morale;

            Unique = unique;
        }
    };
}

#endif