#ifndef __ENGINE__HPP__
#define __ENGINE__HPP__

#include "character.hpp"
#include "monster.hpp"
#include "spells.hpp"

#include "random.hpp"

namespace Engine
{
    enum class CombatResult
    {
        NONE,
        VICTORY,
        DOOM,
        FLEE
    };

    enum class AttackStage
    {
        START,
        ATTACK,
        DAMAGE,
        END
    };

    enum class SaveStage
    {
        START,
        SAVE,
        REDUCE,
        END
    };

    const char *CURRENCY = "silver";

    Party::Base Party = Party::Base();

    auto Random = Random::Base();

    void GAIN_MONEY(Party::Base &party, int money)
    {
        party.Money += money;

        if (party.Money < 0)
        {
            party.Money = 0;
        }
    }

    void GAIN_HEALTH(Character::Base &character, int health)
    {
        character.Health += health;

        if (character.Health < 0)
        {
            character.Health = 0;
        }

        if (character.Health > character.MaximumHealth)
        {
            character.Health = character.MaximumHealth;
        }
    }

    void GAIN_HEALTH(Monster::Base &monster, int health)
    {
        monster.Health += health;

        if (monster.Health < 0)
        {
            monster.Health = 0;
        }
    }

    bool RECHARGE_SPELL(Party::Base &party, Spells::Base &spell)
    {
        auto result = false;

        if (party.Money >= spell.Recharge)
        {
            Engine::GAIN_MONEY(party, spell.Recharge);

            spell.Charged = true;

            result = true;
        }

        return result;
    }

    int FIND_LIST(std::vector<int> selection, int item)
    {
        auto found = -1;

        for (auto i = 0; i < selection.size(); i++)
        {
            if (selection[i] == item)
            {
                found = i;

                break;
            }
        }

        return found;
    }

    std::vector<int> ROLL_DICE(int count)
    {
        Engine::Random.UniformIntDistribution(1, 6);

        auto results = std::vector<int>();

        for (auto i = 0; i < count; i++)
        {
            results.push_back(Engine::Random.NextInt());
        }

        return results;
    }

    int SCORE(Character::Base &character, Attribute::Type type)
    {
        auto score = 1;

        for (auto i = 0; i < character.Attributes.size(); i++)
        {
            if (character.Attributes[i].Type == type)
            {
                score = character.Attributes[i].Value;

                break;
            }
        }

        return score;
    }

    int FIGHTING_SCORE(Character::Base &character)
    {
        auto max = 0;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Type == Equipment::Type::WEAPON && character.Equipment[i].Attribute == Attribute::Type::FIGHTING)
            {
                if (character.Equipment[i].Modifier >= max)
                {
                    max = character.Equipment[i].Modifier;
                }
            }
        }

        return max > 0 ? Engine::SCORE(character, Attribute::Type::FIGHTING) + max : 1;
    }

    int MAX(Character::Base &character, Equipment::Type type, Attribute::Type attribute)
    {
        auto max = 0;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Type == type && character.Equipment[i].Attribute == attribute)
            {
                if (character.Equipment[i].Modifier > max)
                {
                    max = character.Equipment[i].Modifier;
                }
            }
        }

        return max;
    }

    int MODIFIER(Character::Base &character, Equipment::Type type, Attribute::Type attribute)
    {
        auto modifier = 0;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Type == type && character.Equipment[i].Attribute == attribute)
            {
                modifier += character.Equipment[i].Modifier;
            }
        }

        return modifier;
    }

    int ARMOUR(Character::Base &character)
    {
        auto armour = MAX(character, Equipment::Type::SHIELD, Attribute::Type::ARMOUR) + MAX(character, Equipment::Type::ARMOUR, Attribute::Type::ARMOUR) + MODIFIER(character, Equipment::Type::NORMAL, Attribute::Type::ARMOUR);

        return armour;
    }

    int COUNT(std::vector<Monster::Base> &monsters)
    {
        auto result = 0;

        for (auto i = 0; i < monsters.size(); i++)
        {
            if (monsters[i].Health > 0)
            {
                result++;
            }
        }

        return result;
    }

    int COUNT(std::vector<Character::Base> &adventurers)
    {
        auto result = 0;

        for (auto i = 0; i < adventurers.size(); i++)
        {
            if (adventurers[i].Health > 0)
            {
                result++;
            }
        }

        return result;
    }
}
#endif