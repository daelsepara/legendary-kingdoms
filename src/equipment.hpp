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
        SILVER_COINS,
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
        BRONZE_LOCKET,
        SEAL_OF_HOUSE_ROSS,
        VIAL_OF_POISON,
        FINE_BOOTS2,
        REGISTRY_PAPERS,
        GREY_TALISMAN,
        BARBARIAN_BODY,
        CRUDE_BLADE,
        MAUL,
        INCENSE,
        TALISMAN_OF_ST_ELIAS,
        ENGAGEMENT_RING,
        DRAGONYAK_HORN,
        PYRAMIDAL_KEY,
        RING_OF_THE_PATRIARCH,
        RUNESWORD3,
        SILVER_SKULL,
        POTION_OF_INVULNERABILITY,
        IRON_SHORTSWORD1,
        IRON_GREATAXE2,
        STEEL_LONGSWORD2,
        STEEL_GREATSWORD3,
        MASTERWORK_BLADE3,
        MASTERWORK_GREATSWORD4,
        HIDE_ARMOUR1,
        BONE_ARMOUR2,
        BRONZE_ARMOUR4,
        SOFT_BOOTS1,
        REFERENCE_BOOK1,
        WARM_CLOAK1,
        HANDSOME_BROOCH1,
        GOLDWAX_CANDLE,
        LIZARD_HIDE,
        IRON_SCEPTRE,
        CRIER_BIRD,
        RUSTY_KEY,
        CLOAK_OF_PROTECTION3,
        MAGICAL_SHORTSWORD4,
        STONECUTTER_SWORD2,
        SPEAR,
        PRECIOUS_TOMES,
        CLUB,
        RUGGED_CLOAK2,
        AMULET_OF_HEALTH1,
        BEAUTIFUL_LETTER,
        CHAIN_ARMOUR2,
        GLITTERING_NECKLACE2,
        MAGICAL_WEAVE,
        MADSHARD,
        AMULET_OF_DEFENSE1,
        FAIRBROTHER_FAMILY_CREST,
        CLOAK_OF_WONDER3,
        BLACK_PRISM,
        WAYFINDER_ROD,
        TOME_OF_KNOWLEDGE2,
        SCROLLS_OF_LORE3,
        SKALLOS_RUNEBLADE,
        ROYAL_LEDGER,
        KENSAI_ROBES3,
        GOLDEN_CANDLESTICK,
        IRON_SWORD1,
        IRON_KEY
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

        bool TwoHanded = false;

        int AdditionalSlots = 0;

        int Value = 0;

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

        Base(const char *name, const char *description, Equipment::Class equipmentClass, Equipment::Type equipmentType, Attribute::Type modifies, int modifier, int additionalSlots, bool twoHanded)
        {
            Name = name;

            Description = description;

            Class = equipmentClass;

            Type = equipmentType;

            Attribute = modifies;

            Modifier = modifier;

            AdditionalSlots = additionalSlots;

            TwoHanded = twoHanded;
        }

        Base(const char *name, const char *description, Equipment::Class equipmentClass, Equipment::Type equipmentType, Attribute::Type modifies, int modifier, int additionalSlots, int value, bool twoHanded)
        {
            Name = name;

            Description = description;

            Class = equipmentClass;

            Type = equipmentType;

            Attribute = modifies;

            Modifier = modifier;

            AdditionalSlots = additionalSlots;

            Value = value;

            TwoHanded = twoHanded;
        }
    };

    auto NONE = Equipment::Base();
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
    auto SEAL_OF_HOUSE_ROSS = Equipment::Base("SEAL OF HOUSE ROSS", "SEAL OF HOUSE ROSS", Equipment::Class::NORMAL, Equipment::Type::SEAL_OF_HOUSE_ROSS, Attribute::Type::NONE, 0, false);
    auto VIAL_OF_POISON = Equipment::Base("VIAL OF POISON", "VIAL OF POISON", Equipment::Class::NORMAL, Equipment::Type::VIAL_OF_POISON, Attribute::Type::NONE, 0, false);
    auto FINE_BOOTS2 = Equipment::Base("FINE BOOTS", "FINE BOOTS", Equipment::Class::NORMAL, Equipment::Type::FINE_BOOTS2, Attribute::Type::STEALTH, 2, false);
    auto REGISTRY_PAPERS = Equipment::Base("REGISTRY PAPERS", "REGISTRY PAPERS", Equipment::Class::NORMAL, Equipment::Type::REGISTRY_PAPERS, Attribute::Type::NONE, 0, false);
    auto GREY_TALISMAN = Equipment::Base("GREY TALISMAN", "GREY TALISMAN", Equipment::Class::NORMAL, Equipment::Type::GREY_TALISMAN, Attribute::Type::NONE, 0, false);
    auto BARBARIAN_BODY = Equipment::Base("BARBARIAN BODY", "BARBARIAN BODY", Equipment::Class::NORMAL, Equipment::Type::BARBARIAN_BODY, Attribute::Type::NONE, 0, 4, false);
    auto CRUDE_BLADE = Equipment::Base("CRUDE BLADE", "CRUDE BLADE", Equipment::Class::WEAPON, Equipment::Type::CRUDE_BLADE, Attribute::Type::FIGHTING, 0, false);
    auto MAUL = Equipment::Base("MAUL", "MAUL", Equipment::Class::WEAPON, Equipment::Type::MAUL, Attribute::Type::FIGHTING, 1, true);
    auto INCENSE = Equipment::Base("INCENSE", "INCENSE", Equipment::Class::NORMAL, Equipment::Type::INCENSE, Attribute::Type::NONE, 0, false);
    auto TALISMAN_OF_ST_ELIAS = Equipment::Base("TALISMAN of St. Elias", "TALISMAN of St. Elias", Equipment::Class::NORMAL, Equipment::Type::TALISMAN_OF_ST_ELIAS, Attribute::Type::NONE, 0, false);
    auto ENGAGEMENT_RING = Equipment::Base("ENGAGEMENT RING", "ENGAGEMENT RING", Equipment::Class::NORMAL, Equipment::Type::ENGAGEMENT_RING, Attribute::Type::NONE, 0, false);
    auto DRAGONYAK_HORN = Equipment::Base("DRAGONYAK HORN", "DRAGONYAK HORN", Equipment::Class::NORMAL, Equipment::Type::DRAGONYAK_HORN, Attribute::Type::NONE, 0, false);
    auto PYRAMIDAL_KEY = Equipment::Base("PYRAMIDAL KEY", "PYRAMIDAL KEY", Equipment::Class::NORMAL, Equipment::Type::PYRAMIDAL_KEY, Attribute::Type::NONE, 0, false);
    auto RING_OF_THE_PATRIARCH = Equipment::Base("RING OF THE PATRIARCH", "RING OF THE PATRIARCH", Equipment::Class::NORMAL, Equipment::Type::RING_OF_THE_PATRIARCH, Attribute::Type::NONE, 0, false);
    auto RUNESWORD3 = Equipment::Base("RUNESWORD", "RUNESWORD", Equipment::Class::WEAPON, Equipment::Type::RUNESWORD3, Attribute::Type::FIGHTING, 3, false);
    auto SILVER_SKULL = Equipment::Base("SILVER SKULL", "SILVER SKULL", Equipment::Class::NORMAL, Equipment::Type::SILVER_SKULL, Attribute::Type::NONE, 0, false);
    auto POTION_OF_INVULNERABILITY = Equipment::Base("POTION OF INVULNERABILITY", "POTION OF INVULNERABILITY", Equipment::Class::NORMAL, Equipment::Type::POTION_OF_INVULNERABILITY, Attribute::Type::NONE, 0, false);
    auto IRON_SHORTSWORD1 = Equipment::Base("IRON SHORTSWORD", "IRON SHORTSWORD", Equipment::Class::WEAPON, Equipment::Type::IRON_SHORTSWORD1, Attribute::Type::FIGHTING, 1, false);
    auto IRON_GREATAXE2 = Equipment::Base("IRON GREATAXE", "IRON GREATAXE", Equipment::Class::WEAPON, Equipment::Type::IRON_GREATAXE2, Attribute::Type::FIGHTING, 2, true);
    auto STEEL_LONGSWORD2 = Equipment::Base("STEEL LONGSWORD", "STEEL LONGSWORD", Equipment::Class::WEAPON, Equipment::Type::STEEL_LONGSWORD2, Attribute::Type::FIGHTING, 2, false);
    auto STEEL_GREATSWORD3 = Equipment::Base("STEEL GREATSWORD", "STEEL GREATSWORD", Equipment::Class::WEAPON, Equipment::Type::STEEL_GREATSWORD3, Attribute::Type::FIGHTING, 3, true);
    auto MASTERWORK_BLADE3 = Equipment::Base("MASTERWORK BLADE", "MASTERWORK BLADE", Equipment::Class::WEAPON, Equipment::Type::MASTERWORK_BLADE3, Attribute::Type::FIGHTING, 3, false);
    auto MASTERWORK_GREATSWORD4 = Equipment::Base("MASTERWORK GREATSWORD", "MASTERWORK GREATSWORD", Equipment::Class::WEAPON, Equipment::Type::MASTERWORK_GREATSWORD4, Attribute::Type::FIGHTING, 4, true);
    auto HIDE_ARMOUR1 = Equipment::Base("HIDE ARMOUR", "HIDE ARMOUR", Equipment::Class::ARMOUR, Equipment::Type::HIDE_ARMOUR1, Attribute::Type::ARMOUR, 1, false);
    auto BONE_ARMOUR2 = Equipment::Base("BONE ARMOUR", "BONE ARMOUR", Equipment::Class::ARMOUR, Equipment::Type::BONE_ARMOUR2, Attribute::Type::ARMOUR, 2, false);
    auto BRONZE_ARMOUR4 = Equipment::Base("BRONZE ARMOUR", "BRONZE ARMOUR", Equipment::Class::ARMOUR, Equipment::Type::BRONZE_ARMOUR4, Attribute::Type::ARMOUR, 4, false);
    auto SOFT_BOOTS1 = Equipment::Base("SOFT BOOTS", "SOFT BOOTS", Equipment::Class::NORMAL, Equipment::Type::SOFT_BOOTS1, Attribute::Type::STEALTH, 1, false);
    auto REFERENCE_BOOK1 = Equipment::Base("REFERENCE BOOK", "REFERENCE BOOK", Equipment::Class::NORMAL, Equipment::Type::REFERENCE_BOOK1, Attribute::Type::LORE, 1, false);
    auto WARM_CLOAK1 = Equipment::Base("WARM CLOAK", "WARM CLOAK", Equipment::Class::NORMAL, Equipment::Type::WARM_CLOAK1, Attribute::Type::SURVIVAL, 1, false);
    auto HANDSOME_BROOCH1 = Equipment::Base("HANDSOME BROOCH", "HANDSOME BROOCH", Equipment::Class::NORMAL, Equipment::Type::HANDSOME_BROOCH1, Attribute::Type::CHARISMA, 1, false);
    auto GOLDWAX_CANDLE = Equipment::Base("GOLDWAX CANDLE", "GOLDWAX CANDLE", Equipment::Class::NORMAL, Equipment::Type::GOLDWAX_CANDLE, Attribute::Type::NONE, 0, false);
    auto LIZARD_HIDE = Equipment::Base("LIZARD HIDE", "LIZARD HIDE", Equipment::Class::NORMAL, Equipment::Type::LIZARD_HIDE, Attribute::Type::NONE, 0, false);
    auto IRON_SCEPTRE = Equipment::Base("IRON SCEPTRE", "IRON SCEPTRE", Equipment::Class::NORMAL, Equipment::Type::IRON_SCEPTRE, Attribute::Type::NONE, 0, false);
    auto CRIER_BIRD = Equipment::Base("CRIER BIRD", "CRIER BIRD", Equipment::Class::NORMAL, Equipment::Type::CRIER_BIRD, Attribute::Type::NONE, 0, false);
    auto RUSTY_KEY = Equipment::Base("RUSTY KEY", "RUSTY KEY", Equipment::Class::NORMAL, Equipment::Type::RUSTY_KEY, Attribute::Type::NONE, 0, false);
    auto CLOAK_OF_PROTECTION3 = Equipment::Base("CLOAK OF PROTECTION", "CLOAK OF PROTECTION", Equipment::Class::ROBE, Equipment::Type::CLOAK_OF_PROTECTION3, Attribute::Type::SURVIVAL, 3, false);
    auto MAGICAL_SHORTSWORD4 = Equipment::Base("MAGICAL SHORTSWORD", "MAGICAL SHORTSWORD", Equipment::Class::WEAPON, Equipment::Type::MAGICAL_SHORTSWORD4, Attribute::Type::FIGHTING, 4, false);
    auto STONECUTTER_SWORD2 = Equipment::Base("STONECUTTER SWORD", "STONECUTTER SWORD", Equipment::Class::WEAPON, Equipment::Type::STONECUTTER_SWORD2, Attribute::Type::FIGHTING, 2, false);
    auto SPEAR = Equipment::Base("SPEAR", "SPEAR", Equipment::Class::WEAPON, Equipment::Type::SPEAR, Attribute::Type::FIGHTING, 0, false);
    auto PRECIOUS_TOMES = Equipment::Base("PRECIOUS TOMES", "PRECIOUS TOMES", Equipment::Class::NORMAL, Equipment::Type::PRECIOUS_TOMES, Attribute::Type::NONE, 0, false);
    auto CLUB = Equipment::Base("CLUB", "CLUB", Equipment::Class::WEAPON, Equipment::Type::CLUB, Attribute::Type::FIGHTING, 0, false);
    auto RUGGED_CLOAK2 = Equipment::Base("RUGGED CLOAK", "RUGGED CLOAK", Equipment::Class::ROBE, Equipment::Type::RUGGED_CLOAK2, Attribute::Type::SURVIVAL, 2, false);
    auto AMULET_OF_HEALTH1 = Equipment::Base("RUGGED CLOAK", "RUGGED CLOAK", Equipment::Class::NORMAL, Equipment::Type::AMULET_OF_HEALTH1, Attribute::Type::HEALTH, 1, false);
    auto BEAUTIFUL_LETTER = Equipment::Base("BEAUTIFUL LETTER", "BEAUTIFUL LETTER", Equipment::Class::NORMAL, Equipment::Type::BEAUTIFUL_LETTER, Attribute::Type::NONE, 0, false);
    auto CHAIN_ARMOUR2 = Equipment::Base("CHAIN ARMOUR", "CHAIN ARMOUR", Equipment::Class::ARMOUR, Equipment::Type::CHAIN_ARMOUR2, Attribute::Type::ARMOUR, 2, false);
    auto GLITTERING_NECKLACE2 = Equipment::Base("GLITTERING NECKLACE", "GLITTERING NECKLACE", Equipment::Class::NORMAL, Equipment::Type::GLITTERING_NECKLACE2, Attribute::Type::CHARISMA, 2, false);
    auto MAGICAL_WEAVE = Equipment::Base("MAGICAL WEAVE", "MAGICAL WEAVE", Equipment::Class::NORMAL, Equipment::Type::MAGICAL_WEAVE, Attribute::Type::NONE, 0, false);
    auto MADSHARD = Equipment::Base("MADSHARD", "MADSHARD", Equipment::Class::NORMAL, Equipment::Type::MADSHARD, Attribute::Type::NONE, 0, false);
    auto AMULET_OF_DEFENSE1 = Equipment::Base("AMULET OF DEFENSE", "AMULET OF DEFENSE", Equipment::Class::NORMAL, Equipment::Type::AMULET_OF_DEFENSE1, Attribute::Type::ARMOUR, 1, false);
    auto FAIRBROTHER_FAMILY_CREST = Equipment::Base("FAIRBROTHER FAMILY CREST", "FAIRBROTHER FAMILY CREST", Equipment::Class::NORMAL, Equipment::Type::FAIRBROTHER_FAMILY_CREST, Attribute::Type::NONE, 0, false);
    auto CLOAK_OF_WONDER3 = Equipment::Base("CLOAK OF WONDER", "CLOAK OF WONDER", Equipment::Class::ROBE, Equipment::Type::CLOAK_OF_WONDER3, Attribute::Type::ALL_SKILLS, 3, false);
    auto BLACK_PRISM = Equipment::Base("BLACK PRISM", "BLACK PRISM", Equipment::Class::NORMAL, Equipment::Type::BLACK_PRISM, Attribute::Type::NONE, 0, false);
    auto WAYFINDER_ROD = Equipment::Base("WAYFINDER ROD", "WAYFINDER ROD", Equipment::Class::NORMAL, Equipment::Type::WAYFINDER_ROD, Attribute::Type::NONE, 0, false);
    auto TOME_OF_KNOWLEDGE2 = Equipment::Base("TOME OF KNOWLEDGE", "TOME OF KNOWLEDGE", Equipment::Class::NORMAL, Equipment::Type::TOME_OF_KNOWLEDGE2, Attribute::Type::LORE, 2, false);
    auto SCROLLS_OF_LORE3 = Equipment::Base("SCROLLS OF LORE", "SCROLLS OF LORE", Equipment::Class::NORMAL, Equipment::Type::SCROLLS_OF_LORE3, Attribute::Type::LORE, 3, false);
    auto SKALLOS_RUNEBLADE = Equipment::Base("SKALLOS RUNEBLADE", "SKALLOS RUNEBLADE", Equipment::Class::WEAPON, Equipment::Type::SKALLOS_RUNEBLADE, Attribute::Type::FIGHTING3_LORE2, 3, false);
    auto ROYAL_LEDGER = Equipment::Base("ROYAL LEDGER", "ROYAL LEDGER", Equipment::Class::NORMAL, Equipment::Type::ROYAL_LEDGER, Attribute::Type::NONE, 0, false);
    auto KENSAI_ROBES3 = Equipment::Base("KENSAI ROBES", "KENSAI ROBES", Equipment::Class::ARMOUR, Equipment::Type::KENSAI_ROBES3, Attribute::Type::ARMOUR, 3, false);
    auto GOLDEN_CANDLESTICK = Equipment::Base("GOLDEN CANDLESTICK", "GOLDEN CANDLESTICK", Equipment::Class::NORMAL, Equipment::Type::GOLDEN_CANDLESTICK, Attribute::Type::NONE, 0, false);
    auto IRON_KEY = Equipment::Base("IRON KEY", "IRON KEY", Equipment::Class::NORMAL, Equipment::Type::IRON_KEY, Attribute::Type::NONE, 0, false);
    auto IRON_SWORD1 = Equipment::Base("IRON SWORD", "IRON SWORD", Equipment::Class::WEAPON, Equipment::Type::IRON_SWORD1, Attribute::Type::FIGHTING, 1, false);
}

#endif