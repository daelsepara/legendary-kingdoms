#ifndef __ENGINE__HPP__
#define __ENGINE__HPP__

#include "character.hpp"
#include "location.hpp"
#include "monster.hpp"
#include "spells.hpp"
#include "random.hpp"
#include "ship.hpp"

#include <map>

namespace Engine
{
    typedef std::pair<Book::Type, int> Destination;

    enum class Combat
    {
        NONE,
        VICTORY,
        DOOM,
        FLEE
    };

    enum class Attack
    {
        START,
        ATTACK,
        DAMAGE,
        END
    };

    enum class ArmourSave
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
        if (character.Health > 0)
        {
            character.Health += health;
        }

        if (character.Health < 0)
        {
            character.Health = 0;
        }

        if (character.Health > character.MaximumHealth)
        {
            character.Health = character.MaximumHealth;
        }
    }

    void GAIN_HEALTH(std::vector<Character::Base> &party, int health)
    {
        for (auto i = 0; i < party.size(); i++)
        {
            Engine::GAIN_HEALTH(party[i], health);
        }
    }

    void GAIN_HEALTH(Monster::Base &monster, int health)
    {
        if (monster.Health > 0)
        {
            monster.Health += health;
        }

        if (monster.Health > monster.MaximumHealth)
        {
            if (monster.Type == Monster::Type::ZEALOT)
            {
                monster.MaximumHealth = monster.Health;
            }
            else
            {
                monster.Health = monster.MaximumHealth;
            }
        }

        if (monster.Health < 0)
        {
            monster.Health = 0;
        }
    }

    void GAIN_HEALTH(std::vector<Monster::Base> &monsters, int health)
    {
        for (auto i = 0; i < monsters.size(); i++)
        {
            Engine::GAIN_HEALTH(monsters[i], health);
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
            if (character.Equipment[i].Class == Equipment::Class::WEAPON && character.Equipment[i].Attribute == Attribute::Type::FIGHTING)
            {
                if (character.Equipment[i].Modifier >= max)
                {
                    max = character.Equipment[i].Modifier;
                }
            }
        }

        return max > 0 ? Engine::SCORE(character, Attribute::Type::FIGHTING) + max : 1;
    }

    bool TWO_HANDED(Character::Base &character)
    {
        auto result = false;

        auto max = 0;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Class == Equipment::Class::WEAPON && character.Equipment[i].Attribute == Attribute::Type::FIGHTING)
            {
                if (character.Equipment[i].Modifier >= max)
                {
                    max = character.Equipment[i].Modifier;

                    result = character.Equipment[i].TwoHanded;
                }
            }
        }

        return result;
    }

    int MAX(Character::Base &character, Equipment::Class type, Attribute::Type attribute)
    {
        auto max = 0;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Class == type && character.Equipment[i].Attribute == attribute)
            {
                if (character.Equipment[i].Modifier > max)
                {
                    max = character.Equipment[i].Modifier;
                }
            }
        }

        return max;
    }

    int MODIFIER(Character::Base &character, Equipment::Class type, Attribute::Type attribute)
    {
        auto modifier = 0;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Class == type && character.Equipment[i].Attribute == attribute)
            {
                modifier += character.Equipment[i].Modifier;
            }
        }

        return modifier;
    }

    bool HAS_STATUS(Character::Base &character, Character::Status status)
    {
        auto result = false;

        for (auto i = 0; i < character.Status.size(); i++)
        {
            if (character.Status[i] == status)
            {
                result = true;

                break;
            }
        }

        return result;
    }

    bool HAS_STATUS(std::vector<Character::Base> &party, Character::Status status)
    {
        auto result = false;

        for (auto i = 0; i < party.size(); i++)
        {
            if (Engine::HAS_STATUS(party[i], status))
            {
                result = true;

                break;
            }
        }

        return result;
    }

    void GAIN_STATUS(Character::Base &character, Character::Status status)
    {
        if (!Engine::HAS_STATUS(character, status))
        {
            character.Status.push_back(status);
        }
    }

    void REMOVE_STATUS(Character::Base &character, Character::Status status)
    {
        for (auto i = 0; i < character.Status.size(); i++)
        {
            if (character.Status[i] == status)
            {
                character.Status.erase(character.Status.begin() + i);

                break;
            }
        }
    }

    void CLEAR_STATUS(Character::Base &character)
    {
        character.Status.clear();
    }

    int ARMOUR(Character::Base &character)
    {
        auto armour = MAX(character, Equipment::Class::ARMOUR, Attribute::Type::ARMOUR) + MAX(character, Equipment::Class::ROBE, Attribute::Type::ARMOUR) + MODIFIER(character, Equipment::Class::NORMAL, Attribute::Type::ARMOUR);

        if (!Engine::TWO_HANDED(character))
        {
            armour += MAX(character, Equipment::Class::SHIELD, Attribute::Type::ARMOUR);
        }

        if (Engine::HAS_STATUS(character, Character::Status::ARMOUR3))
        {
            armour += 3;
        }

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

    int COUNT(std::vector<Spells::Base> &spells, Spells::Scope scope)
    {
        auto result = 0;

        for (auto i = 0; i < spells.size(); i++)
        {
            if (spells[i].Charged && spells[i].Scope == scope)
            {
                result++;
            }
        }

        return result;
    }

    int COUNT(std::vector<Spells::Base> &spells, Spells::Type type)
    {
        auto result = 0;

        for (auto i = 0; i < spells.size(); i++)
        {
            if (spells[i].Charged && spells[i].Type == type)
            {
                result++;
            }
        }

        return result;
    }

    int SPELLCASTERS(std::vector<Character::Base> &adventurers)
    {
        auto result = 0;

        for (auto i = 0; i < adventurers.size(); i++)
        {
            if (adventurers[i].SpellCaster)
            {
                result++;
            }
        }

        return result;
    }

    void GET_EQUIPMENT(Character::Base &player, std::vector<Equipment::Base> equipment)
    {
        player.Equipment.insert(player.Equipment.end(), equipment.begin(), equipment.end());
    }

    int FIND_EQUIPMENT(Character::Base &character, Equipment::Type item)
    {
        auto found = -1;

        if (character.Equipment.size() > 0)
        {
            for (auto i = 0; i < character.Equipment.size(); i++)
            {
                if (character.Equipment[i].Type == item)
                {
                    found = i;

                    break;
                }
            }
        }

        return found;
    }

    void LOSE_EQUIPMENT(Character::Base &character, std::vector<Equipment::Type> items)
    {
        if (character.Equipment.size() > 0 && items.size() > 0)
        {
            for (auto i = 0; i < items.size(); i++)
            {
                auto result = Engine::FIND_EQUIPMENT(character, items[i]);

                if (result >= 0)
                {
                    character.Equipment.erase(character.Equipment.begin() + result);
                }
            }
        }
    }

    void LOSE_EQUIPMENT(std::vector<Character::Base> &party, std::vector<Equipment::Type> items)
    {
        for (auto i = 0; i < items.size(); i++)
        {
            for (auto j = 0; j < party.size(); j++)
            {
                auto result = Engine::FIND_EQUIPMENT(party[j], items[i]);

                if (result >= 0)
                {
                    party[j].Equipment.erase(party[j].Equipment.begin() + result);

                    // break out of party loop
                    break;
                }
            }
        }
    }

    void LOSE_EQUIPMENT(std::vector<Character::Base> &party, Equipment::Type item, int count)
    {
        for (auto i = 0; i < count; i++)
        {
            Engine::LOSE_EQUIPMENT(party, {item});
        }
    }

    bool VERIFY_EQUIPMENT_LIMIT(Character::Base &player)
    {
        return player.Equipment.size() <= player.MaximumEquipment;
    }

    int COUNT_EQUIPMENT(Character::Base &character, Equipment::Type item)
    {
        auto found = 0;

        if (character.Equipment.size() > 0)
        {
            for (auto i = 0; i < character.Equipment.size(); i++)
            {
                if (character.Equipment[i].Type == item)
                {
                    found++;
                }
            }
        }

        return found;
    }

    int COUNT_EQUIPMENT(Character::Base &player, std::vector<Equipment::Type> equipment)
    {
        auto found = 0;

        for (auto i = 0; i < equipment.size(); i++)
        {
            if (Engine::COUNT_EQUIPMENT(player, equipment[i]) >= 0)
            {
                found++;
            }
        }

        return found;
    }

    int COUNT_EQUIPMENT(std::vector<Character::Base> &party, std::vector<Equipment::Type> equipment)
    {
        auto found = 0;

        for (auto i = 0; i < party.size(); i++)
        {
            found += Engine::COUNT_EQUIPMENT(party[i], equipment);
        }

        return found;
    }

    bool VERIFY_EQUIPMENT(Character::Base &player, std::vector<Equipment::Type> equipment)
    {
        auto found = 0;

        if (equipment.size() > 0)
        {
            for (auto i = 0; i < equipment.size(); i++)
            {
                auto result = Engine::FIND_EQUIPMENT(player, equipment[i]);

                if (result >= 0)
                {
                    found++;
                }
            }
        }

        return found >= equipment.size();
    }

    bool VERIFY_ANY_EQUIPMENT(Character::Base &player, std::vector<Equipment::Type> equpment)
    {
        return Engine::COUNT_EQUIPMENT(player, equpment) > 0;
    }

    bool VERIFY_EQUIPMENT(std::vector<Character::Base> &party, std::vector<Equipment::Type> equipment)
    {
        auto found = false;

        for (auto i = 0; i < party.size(); i++)
        {
            if (Engine::VERIFY_EQUIPMENT(party[i], equipment))
            {
                found = true;

                break;
            }
        }

        return found;
    }

    bool VERIFY_ANY_EQUIPMENT(std::vector<Character::Base> &party, std::vector<Equipment::Type> equipment)
    {
        auto found = false;

        for (auto i = 0; i < party.size(); i++)
        {
            if (Engine::VERIFY_ANY_EQUIPMENT(party[i], equipment))
            {
                found = true;

                break;
            }
        }

        return found;
    }

    int FIND_CODE(Party::Base &party, Codes::Base code)
    {
        auto found = -1;

        if (party.Codes.size() > 0)
        {
            for (auto i = 0; i < party.Codes.size(); i++)
            {
                if (party.Codes[i].Type == code.Type && party.Codes[i].Code == code.Code)
                {
                    found = i;

                    break;
                }
            }
        }

        return found;
    }

    int FIND_CODES(Party::Base &party, std::vector<Codes::Base> codes)
    {
        auto found = 0;

        if (party.Codes.size() > 0 && codes.size() > 0)
        {
            for (auto i = 0; i < codes.size(); i++)
            {
                auto result = Engine::FIND_CODE(party, codes[i]);

                if (result >= 0)
                {
                    found++;
                }
            }
        }

        return found;
    }

    bool VERIFY_CODES_ANY(Party::Base &party, std::vector<Codes::Base> codes)
    {
        return Engine::FIND_CODES(party, codes) > 0;
    }

    bool VERIFY_CODES_ALL(Party::Base &party, std::vector<Codes::Base> codes)
    {
        return Engine::FIND_CODES(party, codes) == codes.size();
    }

    bool VERIFY_CODES(Party::Base &party, std::vector<Codes::Base> codes)
    {
        return Engine::VERIFY_CODES_ALL(party, codes);
    }

    void GET_CODES(Party::Base &party, std::vector<Codes::Base> codes)
    {
        for (auto i = 0; i < codes.size(); i++)
        {
            if (!Engine::VERIFY_CODES(party, {codes[i]}))
            {
                party.Codes.push_back(codes[i]);
            }
        }
    }

    int FIND_CODE(Party::Base &party, Codes::Type code)
    {
        auto found = -1;

        if (party.InvisibleCodes.size() > 0)
        {
            for (auto i = 0; i < party.InvisibleCodes.size(); i++)
            {
                if (party.InvisibleCodes[i] == code)
                {
                    found = i;

                    break;
                }
            }
        }

        return found;
    }

    int FIND_CODES(Party::Base &party, std::vector<Codes::Type> codes)
    {
        auto found = 0;

        if (party.InvisibleCodes.size() > 0 && codes.size() > 0)
        {
            for (auto i = 0; i < codes.size(); i++)
            {
                auto result = Engine::FIND_CODE(party, codes[i]);

                if (result >= 0)
                {
                    found++;
                }
            }
        }

        return found;
    }

    bool VERIFY_CODES_ANY(Party::Base &party, std::vector<Codes::Type> codes)
    {
        return Engine::FIND_CODES(party, codes) > 0;
    }

    bool VERIFY_CODES_ALL(Party::Base &party, std::vector<Codes::Type> codes)
    {
        return Engine::FIND_CODES(party, codes) == codes.size();
    }

    bool VERIFY_CODES(Party::Base &party, std::vector<Codes::Type> codes)
    {
        return Engine::VERIFY_CODES_ALL(party, codes);
    }

    void GET_CODES(Party::Base &party, std::vector<Codes::Type> codes)
    {
        for (auto i = 0; i < codes.size(); i++)
        {
            if (!Engine::VERIFY_CODES(party, {codes[i]}))
            {
                party.InvisibleCodes.push_back(codes[i]);
            }
        }
    }

    int FIND_CHARACTER(Party::Base &party, Character::Type type)
    {
        auto result = -1;

        for (auto i = 0; i < party.Party.size(); i++)
        {
            if (party.Party[i].Type == type)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    int FIND_CHARACTER(std::vector<Character::Base> &party, Character::Type type)
    {
        auto result = -1;

        for (auto i = 0; i < party.size(); i++)
        {
            if (party[i].Type == type)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    void GAIN_HEARTS(Party::Base &party, Character::Type from, Character::Type to, int heart)
    {
        std::pair<Character::Type, Character::Type> romance = {from, to};

        if (party.Hearts.count(romance) > 0)
        {
            party.Hearts[romance] += heart;
        }
    }

    void REJOIN(Party::Base &party)
    {
        party.IsParty = true;

        if (party.Current.Type != Character::Type::NONE)
        {
            party.Party.push_back(party.Current);
        }

        party.Current = Character::Nobody;
    }

    void CONSOLIDATE(Party::Base &party)
    {
        if (party.OtherParty.size() > 0)
        {
            party.Party.insert(party.Party.end(), party.OtherParty.begin(), party.OtherParty.end());

            party.OtherParty.clear();
        }
    }

    void GO_SOLO(Party::Base &party, Character::Type character)
    {
        auto result = Engine::FIND_CHARACTER(party.Party, character);

        if (result >= 0)
        {
            party.Current = party.Party[result];

            party.IsParty = false;

            party.Party.erase(party.Party.begin() + result);
        }
    }

    bool HAS_MONSTER(std::vector<Monster::Base> &monsters, Monster::Type type)
    {
        auto result = false;

        for (auto i = 0; i < monsters.size(); i++)
        {
            if (monsters[i].Type == type && monsters[i].Health > 0)
            {
                result = true;

                break;
            }
        }

        return result;
    }

    bool HAS_SHIP(Party::Base &party, Location::Type location)
    {
        auto result = false;

        if (party.CurrentShip >= 0 && party.CurrentShip < party.Fleet.size())
        {
            if (party.Fleet[party.CurrentShip].Type != Ship::Type::NONE && party.Fleet[party.CurrentShip].Location == location)
            {
                result = true;
            }
        }

        for (auto i = 0; i < party.Fleet.size(); i++)
        {
            if (party.Fleet[i].Type != Ship::Type::NONE && party.Fleet[i].Location == location)
            {
                result = true;

                break;
            }
        }

        return result;
    }
}
#endif