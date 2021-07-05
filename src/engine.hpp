#ifndef __ENGINE__HPP__
#define __ENGINE__HPP__

#include "character.hpp";
#include "monster.hpp";
#include "spells.hpp";

namespace Engine
{
    const char *CURRENCY = "silver";

    Party::Base Party = Party::Base();

    void GAIN_MONEY(Party::Base &party, int money)
    {
        party.Money += money;

        if (party.Money < 0)
        {
            party.Money = 0;
        }
    }

    bool RECHARGE_SPELL(Party::Base &party, Spells::Base &spell)
    {
        auto result = false;

        if (party.Money >= spell.Recharge)
        {
            Engine::GAIN_MONEY(party, -spell.Recharge);

            result = true;
        }

        return result;
    }
}
#endif