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
        Attribute::Type Modifies = Attribute::Type::NONE;

        int Modifier = 0;
    };
}

#endif