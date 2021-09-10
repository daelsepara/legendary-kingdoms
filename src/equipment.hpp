#ifndef __EQUIPMENT__HPP__
#define __EQUIPMENT__HPP__

#include <string>
#include <vector>

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

    std::vector<Equipment::Class> Classes = {
        Equipment::Class::NORMAL,
        Equipment::Class::WEAPON,
        Equipment::Class::ARMOUR,
        Equipment::Class::ROBE,
        Equipment::Class::SHIELD};

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
        IRON_KEY,
        POTION_OF_CHARISMA,
        HEALING_POTION,
        SCROLLS_OF_CURSUS,
        STEEL_SCIMITAR2,
        BLACK_PLATE_ARMOUR3,
        WOODEN_SHIELD2,
        MASTERWORK_LONGSWORD3,
        IRON_BATTLEAXE2,
        RING_OF_HEALTH2
    };

    class Base
    {
    public:
        std::string Name = "";

        std::string Description = "";

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
    auto LEATHER_ARMOUR1 = Equipment::Base("leather armour", "leather armour", Equipment::Class::ARMOUR, Equipment::Type::LEATHER_ARMOUR1, Attribute::Type::ARMOUR, 1, false);
    auto SHIELD2 = Equipment::Base("shield", "shield", Equipment::Class::SHIELD, Equipment::Type::SHIELD2, Attribute::Type::ARMOUR, 2, false);
    auto BLUESTONE = Equipment::Base("bluestone", "bluestone", Equipment::Class::NORMAL, Equipment::Type::BLUESTONE, Attribute::Type::NONE, 0, false);
    auto TITHE_REPORT = Equipment::Base("tithe report", "tithe report", Equipment::Class::NORMAL, Equipment::Type::TITHE_REPORT, Attribute::Type::NONE, 0, false);
    auto PRYBAR = Equipment::Base("prybar", "prybar", Equipment::Class::NORMAL, Equipment::Type::PRYBAR, Attribute::Type::NONE, 0, false);
    auto HYGLIPH_FLOWER = Equipment::Base("hygliph flower", "hygliph flower", Equipment::Class::NORMAL, Equipment::Type::HYGLIPH_FLOWER, Attribute::Type::NONE, 0, false);
    auto JEWELLERY_BOX = Equipment::Base("jewellery box", "jewellery box", Equipment::Class::NORMAL, Equipment::Type::JEWELLERY_BOX, Attribute::Type::NONE, 0, false);
    auto CALLIGRAPHY_INK = Equipment::Base("calligraphy ink", "calligraphy ink", Equipment::Class::NORMAL, Equipment::Type::CALLIGRAPHY_INK, Attribute::Type::NONE, 0, false);
    auto SILVER_IDOL = Equipment::Base("silver idol", "silver idol", Equipment::Class::NORMAL, Equipment::Type::SILVER_IDOL, Attribute::Type::NONE, 0, false);
    auto GOLD_PORTRAIT = Equipment::Base("gold portrait", "gold portrait", Equipment::Class::NORMAL, Equipment::Type::GOLD_PORTRAIT, Attribute::Type::NONE, 0, false);
    auto BRONZE_SCORPION = Equipment::Base("bronze scorpion", "bronze scorpion", Equipment::Class::NORMAL, Equipment::Type::BRONZE_SCORPION, Attribute::Type::NONE, 0, false);
    auto TROGLODYTE_HEAD = Equipment::Base("troglodyte head", "troglodyte head", Equipment::Class::NORMAL, Equipment::Type::TROGLODYTE_HEAD, Attribute::Type::NONE, 0, false);
    auto QUICKSILVER = Equipment::Base("quicksilver", "quicksilver", Equipment::Class::NORMAL, Equipment::Type::QUICKSILVER, Attribute::Type::NONE, 0, false);
    auto GOLD_BULLION = Equipment::Base("gold bullion", "gold bullion", Equipment::Class::NORMAL, Equipment::Type::GOLD_BULLION, Attribute::Type::NONE, 0, false);
    auto DRAGON_HEAD = Equipment::Base("dragon head", "dragon head", Equipment::Class::NORMAL, Equipment::Type::DRAGON_HEAD, Attribute::Type::NONE, 0, false);
    auto SCROLL_OF_RAGE = Equipment::Base("scroll of rage", "scroll of rage", Equipment::Class::NORMAL, Equipment::Type::SCROLL_OF_RAGE, Attribute::Type::NONE, 0, false);
    auto BRONZE_LOCKET = Equipment::Base("bronze locket", "bronze locket", Equipment::Class::NORMAL, Equipment::Type::BRONZE_LOCKET, Attribute::Type::NONE, 0, false);
    auto SEAL_OF_HOUSE_ROSS = Equipment::Base("seal of House Ross", "seal of House Ross", Equipment::Class::NORMAL, Equipment::Type::SEAL_OF_HOUSE_ROSS, Attribute::Type::NONE, 0, false);
    auto VIAL_OF_POISON = Equipment::Base("vial of poison", "vial of poison", Equipment::Class::NORMAL, Equipment::Type::VIAL_OF_POISON, Attribute::Type::NONE, 0, false);
    auto FINE_BOOTS2 = Equipment::Base("fine boots", "fine boots", Equipment::Class::NORMAL, Equipment::Type::FINE_BOOTS2, Attribute::Type::STEALTH, 2, false);
    auto REGISTRY_PAPERS = Equipment::Base("registry papers", "registry papers", Equipment::Class::NORMAL, Equipment::Type::REGISTRY_PAPERS, Attribute::Type::NONE, 0, false);
    auto GREY_TALISMAN = Equipment::Base("grey talisman", "grey talisman", Equipment::Class::NORMAL, Equipment::Type::GREY_TALISMAN, Attribute::Type::NONE, 0, false);
    auto BARBARIAN_BODY = Equipment::Base("barbarian body", "barbarian body", Equipment::Class::NORMAL, Equipment::Type::BARBARIAN_BODY, Attribute::Type::NONE, 0, 4, false);
    auto CRUDE_BLADE = Equipment::Base("crude blade", "crude blade", Equipment::Class::WEAPON, Equipment::Type::CRUDE_BLADE, Attribute::Type::FIGHTING, 0, false);
    auto MAUL = Equipment::Base("maul", "maul", Equipment::Class::WEAPON, Equipment::Type::MAUL, Attribute::Type::FIGHTING, 1, true);
    auto INCENSE = Equipment::Base("incense", "incense", Equipment::Class::NORMAL, Equipment::Type::INCENSE, Attribute::Type::NONE, 0, false);
    auto TALISMAN_OF_ST_ELIAS = Equipment::Base("talisman of St. Elias", "talisman of St. Elias", Equipment::Class::NORMAL, Equipment::Type::TALISMAN_OF_ST_ELIAS, Attribute::Type::NONE, 0, false);
    auto ENGAGEMENT_RING = Equipment::Base("engagement ring", "engagement ring", Equipment::Class::NORMAL, Equipment::Type::ENGAGEMENT_RING, Attribute::Type::NONE, 0, false);
    auto DRAGONYAK_HORN = Equipment::Base("dragonyak horn", "dragonyak horn", Equipment::Class::NORMAL, Equipment::Type::DRAGONYAK_HORN, Attribute::Type::NONE, 0, false);
    auto PYRAMIDAL_KEY = Equipment::Base("pyramidal key", "pyramidal key", Equipment::Class::NORMAL, Equipment::Type::PYRAMIDAL_KEY, Attribute::Type::NONE, 0, false);
    auto RING_OF_THE_PATRIARCH = Equipment::Base("ring of the patriarch", "ring of the patriarch", Equipment::Class::NORMAL, Equipment::Type::RING_OF_THE_PATRIARCH, Attribute::Type::NONE, 0, false);
    auto RUNESWORD3 = Equipment::Base("runesword", "runesword", Equipment::Class::WEAPON, Equipment::Type::RUNESWORD3, Attribute::Type::FIGHTING, 3, false);
    auto SILVER_SKULL = Equipment::Base("silver skull", "silver skull", Equipment::Class::NORMAL, Equipment::Type::SILVER_SKULL, Attribute::Type::NONE, 0, false);
    auto POTION_OF_INVULNERABILITY = Equipment::Base("potion of invulnerability", "potion of invulnerability", Equipment::Class::NORMAL, Equipment::Type::POTION_OF_INVULNERABILITY, Attribute::Type::NONE, 0, false);
    auto IRON_SHORTSWORD1 = Equipment::Base("iron shortsword", "iron shortsword", Equipment::Class::WEAPON, Equipment::Type::IRON_SHORTSWORD1, Attribute::Type::FIGHTING, 1, false);
    auto IRON_GREATAXE2 = Equipment::Base("iron greataxe", "iron greataxe", Equipment::Class::WEAPON, Equipment::Type::IRON_GREATAXE2, Attribute::Type::FIGHTING, 2, true);
    auto STEEL_LONGSWORD2 = Equipment::Base("steel longsword", "steel longsword", Equipment::Class::WEAPON, Equipment::Type::STEEL_LONGSWORD2, Attribute::Type::FIGHTING, 2, false);
    auto STEEL_GREATSWORD3 = Equipment::Base("steel greatsword", "steel greatsword", Equipment::Class::WEAPON, Equipment::Type::STEEL_GREATSWORD3, Attribute::Type::FIGHTING, 3, true);
    auto MASTERWORK_BLADE3 = Equipment::Base("masterwork blade", "masterwork blade", Equipment::Class::WEAPON, Equipment::Type::MASTERWORK_BLADE3, Attribute::Type::FIGHTING, 3, false);
    auto MASTERWORK_GREATSWORD4 = Equipment::Base("masterwork greatsword", "masterwork greatsword", Equipment::Class::WEAPON, Equipment::Type::MASTERWORK_GREATSWORD4, Attribute::Type::FIGHTING, 4, true);
    auto HIDE_ARMOUR1 = Equipment::Base("hide armour", "hide armour", Equipment::Class::ARMOUR, Equipment::Type::HIDE_ARMOUR1, Attribute::Type::ARMOUR, 1, false);
    auto BONE_ARMOUR2 = Equipment::Base("bone armour", "bone armour", Equipment::Class::ARMOUR, Equipment::Type::BONE_ARMOUR2, Attribute::Type::ARMOUR, 2, false);
    auto BRONZE_ARMOUR4 = Equipment::Base("bronze armour", "bronze armour", Equipment::Class::ARMOUR, Equipment::Type::BRONZE_ARMOUR4, Attribute::Type::ARMOUR, 4, false);
    auto SOFT_BOOTS1 = Equipment::Base("soft boots", "soft boots", Equipment::Class::NORMAL, Equipment::Type::SOFT_BOOTS1, Attribute::Type::STEALTH, 1, false);
    auto REFERENCE_BOOK1 = Equipment::Base("reference book", "reference book", Equipment::Class::NORMAL, Equipment::Type::REFERENCE_BOOK1, Attribute::Type::LORE, 1, false);
    auto WARM_CLOAK1 = Equipment::Base("warm cloak", "warm cloak", Equipment::Class::NORMAL, Equipment::Type::WARM_CLOAK1, Attribute::Type::SURVIVAL, 1, false);
    auto HANDSOME_BROOCH1 = Equipment::Base("handsome brooch", "handsome brooch", Equipment::Class::NORMAL, Equipment::Type::HANDSOME_BROOCH1, Attribute::Type::CHARISMA, 1, false);
    auto GOLDWAX_CANDLE = Equipment::Base("goldwax candle", "goldwax candle", Equipment::Class::NORMAL, Equipment::Type::GOLDWAX_CANDLE, Attribute::Type::NONE, 0, false);
    auto LIZARD_HIDE = Equipment::Base("lizard hide", "lizard hide", Equipment::Class::NORMAL, Equipment::Type::LIZARD_HIDE, Attribute::Type::NONE, 0, false);
    auto IRON_SCEPTRE = Equipment::Base("iron sceptre", "iron sceptre", Equipment::Class::NORMAL, Equipment::Type::IRON_SCEPTRE, Attribute::Type::NONE, 0, false);
    auto CRIER_BIRD = Equipment::Base("crier bird", "crier bird", Equipment::Class::NORMAL, Equipment::Type::CRIER_BIRD, Attribute::Type::NONE, 0, false);
    auto RUSTY_KEY = Equipment::Base("rusty key", "rusty key", Equipment::Class::NORMAL, Equipment::Type::RUSTY_KEY, Attribute::Type::NONE, 0, false);
    auto CLOAK_OF_PROTECTION3 = Equipment::Base("cloak of protection", "cloak of protection", Equipment::Class::ROBE, Equipment::Type::CLOAK_OF_PROTECTION3, Attribute::Type::SURVIVAL, 3, false);
    auto MAGICAL_SHORTSWORD4 = Equipment::Base("magical shortsword", "magical shortsword", Equipment::Class::WEAPON, Equipment::Type::MAGICAL_SHORTSWORD4, Attribute::Type::FIGHTING, 4, false);
    auto STONECUTTER_SWORD2 = Equipment::Base("stonecutter sword", "stonecutter sword", Equipment::Class::WEAPON, Equipment::Type::STONECUTTER_SWORD2, Attribute::Type::FIGHTING, 2, false);
    auto SPEAR = Equipment::Base("spear", "spear", Equipment::Class::WEAPON, Equipment::Type::SPEAR, Attribute::Type::FIGHTING, 0, false);
    auto PRECIOUS_TOMES = Equipment::Base("precious tomes", "precious tomes", Equipment::Class::NORMAL, Equipment::Type::PRECIOUS_TOMES, Attribute::Type::NONE, 0, false);
    auto CLUB = Equipment::Base("club", "club", Equipment::Class::WEAPON, Equipment::Type::CLUB, Attribute::Type::FIGHTING, 0, false);
    auto RUGGED_CLOAK2 = Equipment::Base("rugged cloak", "rugged cloak", Equipment::Class::ROBE, Equipment::Type::RUGGED_CLOAK2, Attribute::Type::SURVIVAL, 2, false);
    auto AMULET_OF_HEALTH1 = Equipment::Base("amulet of health", "amulet of health", Equipment::Class::NORMAL, Equipment::Type::AMULET_OF_HEALTH1, Attribute::Type::HEALTH, 1, false);
    auto BEAUTIFUL_LETTER = Equipment::Base("beautiful letter", "beautiful letter", Equipment::Class::NORMAL, Equipment::Type::BEAUTIFUL_LETTER, Attribute::Type::NONE, 0, false);
    auto CHAIN_ARMOUR2 = Equipment::Base("chain armour", "chain armour", Equipment::Class::ARMOUR, Equipment::Type::CHAIN_ARMOUR2, Attribute::Type::ARMOUR, 2, false);
    auto GLITTERING_NECKLACE2 = Equipment::Base("glittering necklace", "glittering necklace", Equipment::Class::NORMAL, Equipment::Type::GLITTERING_NECKLACE2, Attribute::Type::CHARISMA, 2, false);
    auto MAGICAL_WEAVE = Equipment::Base("magical weave", "magical weave", Equipment::Class::NORMAL, Equipment::Type::MAGICAL_WEAVE, Attribute::Type::NONE, 0, false);
    auto MADSHARD = Equipment::Base("madshard", "madshard", Equipment::Class::NORMAL, Equipment::Type::MADSHARD, Attribute::Type::NONE, 0, false);
    auto AMULET_OF_DEFENSE1 = Equipment::Base("amulet of defense", "amulet of defense", Equipment::Class::NORMAL, Equipment::Type::AMULET_OF_DEFENSE1, Attribute::Type::ARMOUR, 1, false);
    auto FAIRBROTHER_FAMILY_CREST = Equipment::Base("Fairbrother Family Crest", "Fairbrother Family Crest", Equipment::Class::NORMAL, Equipment::Type::FAIRBROTHER_FAMILY_CREST, Attribute::Type::NONE, 0, false);
    auto CLOAK_OF_WONDER3 = Equipment::Base("cloak of wonder", "cloak of wonder", Equipment::Class::ROBE, Equipment::Type::CLOAK_OF_WONDER3, Attribute::Type::ALL_SKILLS, 3, false);
    auto BLACK_PRISM = Equipment::Base("black prism", "black prism", Equipment::Class::NORMAL, Equipment::Type::BLACK_PRISM, Attribute::Type::NONE, 0, false);
    auto WAYFINDER_ROD = Equipment::Base("wayfinder rod", "wayfinder rod", Equipment::Class::NORMAL, Equipment::Type::WAYFINDER_ROD, Attribute::Type::NONE, 0, false);
    auto TOME_OF_KNOWLEDGE2 = Equipment::Base("tome of knowledge", "tome of knowledge", Equipment::Class::NORMAL, Equipment::Type::TOME_OF_KNOWLEDGE2, Attribute::Type::LORE, 2, false);
    auto SCROLLS_OF_LORE3 = Equipment::Base("scrolls of lore", "scrolls of lore", Equipment::Class::NORMAL, Equipment::Type::SCROLLS_OF_LORE3, Attribute::Type::LORE, 3, false);
    auto SKALLOS_RUNEBLADE = Equipment::Base("skallos runeblade", "skallos runeblade", Equipment::Class::WEAPON, Equipment::Type::SKALLOS_RUNEBLADE, Attribute::Type::FIGHTING3_LORE2, 3, false);
    auto ROYAL_LEDGER = Equipment::Base("royal ledger", "royal ledger", Equipment::Class::NORMAL, Equipment::Type::ROYAL_LEDGER, Attribute::Type::NONE, 0, false);
    auto KENSAI_ROBES3 = Equipment::Base("kensai robes", "kensai robes", Equipment::Class::ARMOUR, Equipment::Type::KENSAI_ROBES3, Attribute::Type::ARMOUR, 3, false);
    auto GOLDEN_CANDLESTICK = Equipment::Base("golden candlestick", "golden candlestick", Equipment::Class::NORMAL, Equipment::Type::GOLDEN_CANDLESTICK, Attribute::Type::NONE, 0, false);
    auto IRON_KEY = Equipment::Base("iron key", "iron key", Equipment::Class::NORMAL, Equipment::Type::IRON_KEY, Attribute::Type::NONE, 0, false);
    auto IRON_SWORD1 = Equipment::Base("iron sword", "iron sword", Equipment::Class::WEAPON, Equipment::Type::IRON_SWORD1, Attribute::Type::FIGHTING, 1, false);
    auto POTION_OF_CHARISMA = Equipment::Base("potion of fluency (+3 Charisma successes)", "potion of fluency (+3 Charisma successes)", Equipment::Class::NORMAL, Equipment::Type::POTION_OF_CHARISMA, Attribute::Type::NONE, 0, false);
    auto HEALING_POTION = Equipment::Base("healing potion", "healing potion", Equipment::Class::NORMAL, Equipment::Type::HEALING_POTION, Attribute::Type::NONE, 0, false);
    auto SCROLLS_OF_CURSUS = Equipment::Base("scrolls of cursus", "scrolls of cursus", Equipment::Class::NORMAL, Equipment::Type::SCROLLS_OF_CURSUS, Attribute::Type::NONE, 0, false);
    auto STEEL_SCIMITAR2 = Equipment::Base("steel scimitar", "steel scimitar", Equipment::Class::WEAPON, Equipment::Type::STEEL_SCIMITAR2, Attribute::Type::FIGHTING, 2, false);
    auto BLACK_PLATE_ARMOUR3 = Equipment::Base("black plate armour", "black plate armour", Equipment::Class::ARMOUR, Equipment::Type::BLACK_PLATE_ARMOUR3, Attribute::Type::ARMOUR, 3, false);
    auto WOODEN_SHIELD2 = Equipment::Base("wooden shield", "wooden shield", Equipment::Class::SHIELD, Equipment::Type::WOODEN_SHIELD2, Attribute::Type::ARMOUR, 2, false);
    auto MASTERWORK_LONGSWORD3 = Equipment::Base("masterwork longsword", "masterwork longsword", Equipment::Class::WEAPON, Equipment::Type::MASTERWORK_LONGSWORD3, Attribute::Type::FIGHTING, 3, false);
    auto IRON_BATTLEAXE2 = Equipment::Base("iron battleaxe", "iron battleaxe", Equipment::Class::WEAPON, Equipment::Type::IRON_BATTLEAXE2, Attribute::Type::FIGHTING, 2, false);
    auto RING_OF_HEALTH2 = Equipment::Base("ring of health", "ring of health", Equipment::Class::NORMAL, Equipment::Type::RING_OF_HEALTH2, Attribute::Type::HEALTH, 2, false);
}

#endif
