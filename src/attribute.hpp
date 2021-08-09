#ifndef __SKILLS__HPP__
#define __SKILLS__HPP__

#include <map>

namespace Attribute
{
    enum class Type
    {
        NONE = 0,
        FIGHTING,
        STEALTH,
        LORE,
        SURVIVAL,
        CHARISMA,
        ARMOUR,
        HEALTH,
        ALL_SKILLS
    };

    enum class Test
    {
        START,
        CHECK,
        CONFIRM,
        MAGIC,
        END
    };

    std::map<Attribute::Type, const char*> Descriptions = {
        {Attribute::Type::FIGHTING, "Fighting"},
        {Attribute::Type::STEALTH, "Stealth"},
        {Attribute::Type::LORE, "Lore"},
        {Attribute::Type::SURVIVAL, "Survival"},
        {Attribute::Type::CHARISMA, "Charisma"},
        {Attribute::Type::ARMOUR, "Armour"},
        {Attribute::Type::HEALTH, "Health"},
        {Attribute::Type::ALL_SKILLS, "All Skills"}
    };

    class Base
    {
    public:
        Attribute::Type Type = Attribute::Type::NONE;
        
        int Value = 0;

        Base(Attribute::Type type, int value)
        {
            Type = type;
            Value = value;
        }
    };
}

#endif