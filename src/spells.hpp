#ifndef __SPELLS__HPP__
#define __SPELLS__HPP__

namespace Spells
{
    enum class Type
    {
        NONE = -1,
        ANIMAL_SPEECH,
        ARMOUR_OF_HEAVEN,
        ICE_BOLT,
        POSION_STREAM,
        UNFAILING_STRIKE,
        WOLF_SPIRIT,
        MAGIC_CABINET
    };

    enum class Scope
    {
        NONE = 0,
        ADVENTURE,
        COMBAT,
        MASS_COMBAT,
        SEA_COMBAT,
        ADVENTURE_COMBAT
    };

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
    auto WOLF_SPIRIT = Spells::Base("Wolf Spirit", "You summon the spirit of the wolf to help guide you from danger in the natural world. Cast this spell when you fail a Survival check. You gain an additional 3 automatic successes for this check only.", Spells::Scope::ADVENTURE, Spells::Type::WOLF_SPIRIT, 75);
    auto MAGIC_CABINET = Spells::Base("Magic Cabinet", "You summon a magic cabinet made of purple wood. You can use this cabinet to access The Vault, storing and/or taking out as many items as you wish. When you have done this, the cabinet will vanish, until summoned again.", Spells::Scope::ADVENTURE, Spells::Type::MAGIC_CABINET, 100);

    std::vector<Spells::Base> LIST = {Spells::ANIMAL_SPEECH, Spells::ARMOUR_OF_HEAVEN, Spells::ICE_BOLT, Spells::POISON_STREAM, Spells::UNFAILING_STRIKE, Spells::WOLF_SPIRIT, Spells::MAGIC_CABINET};
}

#endif