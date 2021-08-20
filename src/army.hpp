#ifndef __ARMY__HPP__
#define __ARMY__HPP__

#include <map>
#include <vector>

#include "location.hpp"

namespace Army
{
    enum class Status
    {
        NONE = 0,
        STRENGTH1
    };

    std::map<Army::Status, const char *> Descriptions = {
        {Army::Status::NONE, "None"},
        {Army::Status::STRENGTH1, "+1 Strength"}};

    enum class Type
    {
        NONE = -1,
        BRONZEGUARD,
        CHALICE_ARCHERS,
        CITIZEN_ARCHERS,
        CLIFFTOP_INFANTRY,
        CURSITE_CITIZENS,
        CURSITE_FANATICS,
        CURSITE_INFANTRY,
        CURSITE_RIDERS,
        CURSITE_ZEALOTS,
        FAITHFUL_IRREGULARS,
        FIELD_OVERSEERS,
        KENSAI_WARRIORS,
        LANDSMEN_CAVALRY,
        LHASBREATH_BERSERKERS,
        LHASBREATH_BARBARIANS,
        LUUTANESH_SPEARS,
        MERCENARY_CROSSBOWS,
        MERCENARY_KNIGHTS,
        MERCENARY_MEN_AT_ARMS,
        MERCENARY_SPEARS,
        MERCENARY_SWORDS,
        NOMAD_HORSEMEN,
        PEASANT_LEVIES,
        SALTDAD_INFANTRY,
        SLAVE_LEVIES,
        SPEARMEN_OF_SALTDAD,
        SPEARS_OF_UNBRAAKI,
        TEMPLE_GUARD,
        THALSIAN_MERCENARIES
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

        // Special effects and the combat round they went into effect
        Army::Status Status = Army::Status::NONE;

        int StatusRound = -1;

        int StatusDuration = 0;

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