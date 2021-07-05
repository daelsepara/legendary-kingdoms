#ifndef __SKILLS__HPP__
#define __SKILLS__HPP__

namespace Attribute
{
    enum class Type
    {
        NONE = 0,
        FIGTING,
        STEALTH,
        LORE,
        SURVIVAL,
        CHARISMA,
        ARMOUR
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