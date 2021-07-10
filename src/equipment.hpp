#ifndef __EQUIPMENT__HPP__
#define __EQUIPMENT__HPP__

#include "attribute.hpp"

namespace Equipment
{
    enum class Class
    {
        NONE = -1,
        NORMAL,
        WEAPON,
        ARMOUR,
        SHIELD
    };

    enum class Type
    {
        NONE = 0,
        LEATHER_ARMOUR1,
        SHIELD2,
        BLUESTONE
    };

    class Base
    {
    public:
        const char *Name = NULL;

        const char *Description = NULL;

        Equipment::Class Class = Equipment::Class::NORMAL;

        Equipment::Type Type = Equipment::Type::NONE;

        // Modifiers
        Attribute::Type Attribute = Attribute::Type::NONE;

        int Modifier = 0;

        Base()
        {
        }

        Base(const char *name, const char *description, Equipment::Class equipmentClass, Equipment::Type equipmentType, Attribute::Type modifies, int modifier)
        {
            Name = name;

            Description = description;

            Class = equipmentClass;

            Type = equipmentType;

            Attribute = modifies;

            Modifier = modifier;
        }
    };

    auto LEATHER_ARMOUR1 = Equipment::Base("LEATHER ARMOUR", "LEATHER ARMOUR", Equipment::Class::ARMOUR, Equipment::Type::LEATHER_ARMOUR1, Attribute::Type::ARMOUR, 1);
    auto SHIELD2 = Equipment::Base("SHIELD", "SHIELD", Equipment::Class::SHIELD, Equipment::Type::SHIELD2, Attribute::Type::ARMOUR, 2);
    auto BLUESTONE = Equipment::Base("BLUESTONE", "BLUESTONE", Equipment::Class::NORMAL, Equipment::Type::BLUESTONE, Attribute::Type::NONE, 0);
}

#endif