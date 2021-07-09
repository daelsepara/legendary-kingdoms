#ifndef __EQUIPMENT__HPP__
#define __EQUIPMENT__HPP__

#include "attribute.hpp"

namespace Equipment
{
    enum class Type
    {
        NONE = -1,
        NORMAL,
        WEAPON,
        ARMOUR,
        SHIELD
    };

    class Base
    {
    public:
        const char *Name = NULL;

        const char *Description = NULL;

        Equipment::Type Type = Equipment::Type::NORMAL;

        // Modifiers
        Attribute::Type Attribute = Attribute::Type::NONE;

        int Modifier = 0;

        Base()
        {
            
        }

        Base(const char *name, const char *description, Equipment::Type type, Attribute::Type modifies, int modifier)
        {
            Name = name;

            Description = description;

            Type = type;

            Attribute = modifies;

            Modifier = modifier;
        }
    };

    auto LEATHER_ARMOUR1 = Equipment::Base("LEATHER ARMOUR", "LEATHER ARMOUR", Equipment::Type::ARMOUR, Attribute::Type::ARMOUR, 1);
    auto SHIELD2 = Equipment::Base("SHIELD", "SHIELD", Equipment::Type::SHIELD, Attribute::Type::ARMOUR, 2);
}

#endif