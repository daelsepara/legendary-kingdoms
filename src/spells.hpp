#ifndef __SPELLS__HPP__
#define __SPELLS__HPP__

#include <map>
#include <vector>

namespace Spells
{
    enum class Select
    {
        SPELLBOOK,
        UNLEARN,
        CAST_SPELL
    };

    enum class Type
    {
        NONE = -1,
        ANIMAL_SPEECH,
        ARMOUR_OF_HEAVEN,
        ICE_BOLT,
        POSION_STREAM,
        UNFAILING_STRIKE,
        WOLF_SPIRIT,
        MAGIC_CABINET,
        WISDOM,
        ORB_OF_ANNIHILATION,
        MAELSTROM,
        NATURE_WARD,
        DAZING_LIGHTS,
        SOOTHING_TOUCH,
        SHADOW_DOOR,
        TELEPORT_CIRCLE,
        SILVER_TONGUE
    };

    enum class MassCombat
    {
        NONE = 0,
        ENFEEBLEMENT_CENTER_FRONT, // Mass combat spell
        CLINGING_DREAD_LEFT_FRONT, // Mass combat spell
        ROUT_LEFT_FRONT            // Mass combat spell
    };

    std::map<Spells::MassCombat, const char *> MassCombatNames = {
        {Spells::MassCombat::NONE, "None"},
        {Spells::MassCombat::ENFEEBLEMENT_CENTER_FRONT, "Enfeeblement"},
        {Spells::MassCombat::CLINGING_DREAD_LEFT_FRONT, "Clinging Dread"},
        {Spells::MassCombat::ROUT_LEFT_FRONT, "Rout"}};

    std::map<Spells::MassCombat, const char *> MassCombatDescriptions = {
        {Spells::MassCombat::NONE, "None"},
        {Spells::MassCombat::ENFEEBLEMENT_CENTER_FRONT, "Your front unit in the Centre zone loses 1 point of Strength"},
        {Spells::MassCombat::CLINGING_DREAD_LEFT_FRONT, "Your front unit in the Left Flank zone loses 1 point of Morale"},
        {Spells::MassCombat::ROUT_LEFT_FRONT, "Your front unit in the Left Flank must make an immediate Morale check or flee"}};

    enum class Scope
    {
        NONE = 0,
        ADVENTURE,
        COMBAT,
        MASS_COMBAT,
        SEA_COMBAT,
        ADVENTURE_COMBAT,
        EXPLORATION
    };

    std::map<Spells::Scope, const char *> ScopeDescriptions = {
        {Spells::Scope::ADVENTURE, "Adventure"},
        {Spells::Scope::COMBAT, "Combat"},
        {Spells::Scope::MASS_COMBAT, "Mass Combat"},
        {Spells::Scope::SEA_COMBAT, "Sea Combat"},
        {Spells::Scope::ADVENTURE_COMBAT, "Adventure/Combat"},
        {Spells::Scope::EXPLORATION, "Exploration"}};

    class Base
    {
    public:
        const char *Name = NULL;

        const char *Description = NULL;

        Spells::Scope Scope = Spells::Scope::NONE;

        Spells::Type Type = Spells::Type::NONE;

        bool Charged = false;

        int Recharge = 0;

        Base(const char *name, const char *description, Spells::Scope scope, Spells::Type type, int recharge)
        {
            Name = name;

            Description = description;

            Scope = scope;

            Type = type;

            Recharge = recharge;

            Charged = true;
        }
    };

    auto ANIMAL_SPEECH = Spells::Base("Animal Speech", "You are able to communicate using the secret speech of animals.", Spells::Scope::ADVENTURE, Spells::Type::ANIMAL_SPEECH, 50);
    auto ARMOUR_OF_HEAVEN = Spells::Base("Armour of Heaven", "Gain a +3 bonus to one party member's Armour which lasts until the end of this battle.", Spells::Scope::COMBAT, Spells::Type::ARMOUR_OF_HEAVEN, 50);
    auto ICE_BOLT = Spells::Base("Ice Bolt", "You conjure a great block of ice and hurl it at your foe with magical strength. Make an immediate attack with a Fighting score of 8 against any opponent.", Spells::Scope::COMBAT, Spells::Type::ICE_BOLT, 50);
    auto POISON_STREAM = Spells::Base("Poison Stream", "A sickly green poison jets from your hands. Make two immediate attacks with a Fighting score of 5, against two different opponents.", Spells::Scope::COMBAT, Spells::Type::POSION_STREAM, 50);
    auto UNFAILING_STRIKE = Spells::Base("Unfailing Strike", "A pillar of energy sizzles into a single foe. Choose an opponent to lose 3 Health.", Spells::Scope::COMBAT, Spells::Type::UNFAILING_STRIKE, 50);
    auto WOLF_SPIRIT = Spells::Base("Wolf Spirit", "You summon the spirit of the wolf to help guide you from danger in the natural world. Cast this spell when you fail a Survival check. You gain an additional 3 automatic successes for this check only.", Spells::Scope::EXPLORATION, Spells::Type::WOLF_SPIRIT, 75);
    auto MAGIC_CABINET = Spells::Base("Magic Cabinet", "You summon a magic cabinet made of purple wood. You can use this cabinet to access The Vault, storing and/or taking out as many items as you wish. When you have done this, the cabinet will vanish, until summoned again.", Spells::Scope::ADVENTURE, Spells::Type::MAGIC_CABINET, 100);
    auto WISDOM = Spells::Base("Wisdom", "Your mind awakens with ancient knowledge. Cast this spell when you have failed a Lore check. You gain 3 automatic successes for this check only.", Spells::Scope::EXPLORATION, Spells::Type::WISDOM, 75);
    auto DAZING_LIGHTS = Spells::Base("Dazing Lights", "Swirling lights dazzle your foes' eyes. When your opponent's attack this round, they must reduce the score of each of their dice by 1 point.", Spells::Scope::COMBAT, Spells::Type::DAZING_LIGHTS, 50);
    auto SOOTHING_TOUCH = Spells::Base("Soothing Touch", "Your hands glow with divine energy. Choose one of your party members; they can restore 5 Health points.", Spells::Scope::ADVENTURE_COMBAT, Spells::Type::SOOTHING_TOUCH, 50);
    auto SHADOW_DOOR = Spells::Base("Shadow Door", "You can detect the secret doors installed by the penumbral lords when they ruled the land.", Spells::Scope::ADVENTURE, Spells::Type::SHADOW_DOOR, 50);
    auto SILVER_TONGUE = Spells::Base("Silver Tongue", "Your confidence grows as you summon this spell. Cast this spell when you fail a Charisma check. You gain an additional 3 automatic successes for this check only.", Spells::Scope::ADVENTURE, Spells::Type::SILVER_TONGUE, 75);

    std::vector<Spells::Base> BOOK1_SPELLS = {Spells::ANIMAL_SPEECH, Spells::ARMOUR_OF_HEAVEN, Spells::ICE_BOLT, Spells::POISON_STREAM, Spells::UNFAILING_STRIKE, Spells::WOLF_SPIRIT, Spells::MAGIC_CABINET};
}

#endif