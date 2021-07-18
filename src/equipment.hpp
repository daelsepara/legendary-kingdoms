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
        ROBE,
        SHIELD
    };

    enum class Type
    {
        NONE = 0,
        LEATHER_ARMOUR1,
        SHIELD2,
        BLUESTONE,
        TITHE_REPORT,
        PRYBAR,
        HYGLIPH_FLOWER,
        JEWELLERY_BOX,
        CALLIGRAPHY_INK,
        SILVER_IDOL,
        GOLD_PORTRAIT,
        BRONZE_SCORPION,
        TROGLODYTE_HEAD,
        QUICKSILVER,
        GOLD_BULLION,
        DRAGON_HEAD,
        SCROLL_OF_RAGE,
        BRONZE_LOCKET
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

        bool TwoHanded = false;

        int Modifier = 0;

        Base()
        {
        }

        Base(const char *name, const char *description, Equipment::Class equipmentClass, Equipment::Type equipmentType, Attribute::Type modifies, int modifier, bool twoHanded)
        {
            Name = name;

            Description = description;

            Class = equipmentClass;

            Type = equipmentType;

            Attribute = modifies;

            Modifier = modifier;

            TwoHanded = twoHanded;
        }
    };

    auto LEATHER_ARMOUR1 = Equipment::Base("LEATHER ARMOUR", "LEATHER ARMOUR", Equipment::Class::ARMOUR, Equipment::Type::LEATHER_ARMOUR1, Attribute::Type::ARMOUR, 1, false);
    auto SHIELD2 = Equipment::Base("SHIELD", "SHIELD", Equipment::Class::SHIELD, Equipment::Type::SHIELD2, Attribute::Type::ARMOUR, 2, false);
    auto BLUESTONE = Equipment::Base("BLUESTONE", "BLUESTONE", Equipment::Class::NORMAL, Equipment::Type::BLUESTONE, Attribute::Type::NONE, 0, false);
    auto TITHE_REPORT = Equipment::Base("TITHE REPORT", "TITHE REPORT", Equipment::Class::NORMAL, Equipment::Type::TITHE_REPORT, Attribute::Type::NONE, 0, false);
    auto PRYBAR = Equipment::Base("PRYBAR", "PRYBAR", Equipment::Class::NORMAL, Equipment::Type::PRYBAR, Attribute::Type::NONE, 0, false);
    auto HYGLIPH_FLOWER = Equipment::Base("HYGLIPH FLOWER", "HYGLIPH FLOWER", Equipment::Class::NORMAL, Equipment::Type::HYGLIPH_FLOWER, Attribute::Type::NONE, 0, false);
    auto JEWELLERY_BOX = Equipment::Base("JEWELLERY BOX", "JEWELLERY BOX", Equipment::Class::NORMAL, Equipment::Type::JEWELLERY_BOX, Attribute::Type::NONE, 0, false);
    auto CALLIGRAPHY_INK = Equipment::Base("CALLIGRAPHY INK", "CALLIGRAPHY INK", Equipment::Class::NORMAL, Equipment::Type::CALLIGRAPHY_INK, Attribute::Type::NONE, 0, false);
    auto SILVER_IDOL = Equipment::Base("SILVER IDOL", "SILVER IDOL", Equipment::Class::NORMAL, Equipment::Type::SILVER_IDOL, Attribute::Type::NONE, 0, false);
    auto GOLD_PORTRAIT = Equipment::Base("GOLD PORTRAIT", "GOLD PORTRAIT", Equipment::Class::NORMAL, Equipment::Type::GOLD_PORTRAIT, Attribute::Type::NONE, 0, false);
    auto BRONZE_SCORPION = Equipment::Base("BRONZE SCORPION", "BRONZE SCORPION", Equipment::Class::NORMAL, Equipment::Type::BRONZE_SCORPION, Attribute::Type::NONE, 0, false);
    auto TROGLODYTE_HEAD = Equipment::Base("TROGLODYTE HEAD", "TROGLODYTE HEAD", Equipment::Class::NORMAL, Equipment::Type::TROGLODYTE_HEAD, Attribute::Type::NONE, 0, false);
    auto QUICKSILVER = Equipment::Base("QUICKSILVER", "QUICKSILVER", Equipment::Class::NORMAL, Equipment::Type::QUICKSILVER, Attribute::Type::NONE, 0, false);
    auto GOLD_BULLION = Equipment::Base("GOLD BULLION", "GOLD BULLION", Equipment::Class::NORMAL, Equipment::Type::GOLD_BULLION, Attribute::Type::NONE, 0, false);
    auto DRAGON_HEAD = Equipment::Base("DRAGON HEAD", "DRAGON HEAD", Equipment::Class::NORMAL, Equipment::Type::DRAGON_HEAD, Attribute::Type::NONE, 0, false);
    auto SCROLL_OF_RAGE = Equipment::Base("SCROLL OF RAGE", "SCROLL OF RAGE", Equipment::Class::NORMAL, Equipment::Type::SCROLL_OF_RAGE, Attribute::Type::NONE, 0, false);
    auto BRONZE_LOCKET = Equipment::Base("BRONZE LOCKET", "BRONZE LOCKET", Equipment::Class::NORMAL, Equipment::Type::BRONZE_LOCKET, Attribute::Type::NONE, 0, false);
}

#endif