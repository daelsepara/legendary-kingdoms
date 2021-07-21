#ifndef __ENGINE__HPP__
#define __ENGINE__HPP__

#include "character.hpp"
#include "equipment.hpp"
#include "location.hpp"
#include "monster.hpp"
#include "spells.hpp"
#include "random.hpp"
#include "ship.hpp"

#include <map>

namespace Engine
{
    typedef std::pair<Book::Type, int> Destination;
    typedef std::tuple<Ship::Base, int, int> ShipPrices;
    typedef std::tuple<Cargo::Type, int, int> CargoPrices;
    typedef std::tuple<Equipment::Base, int, int> EquipmentPrice;
    typedef std::pair<Equipment::Base, std::vector<Equipment::Base>> BarterExchanges;

    enum class Combat
    {
        NONE,
        VICTORY,
        DEFEAT,
        FLEE
    };

    enum class Attack
    {
        START,
        ATTACK,
        DAMAGE,
        END
    };

    enum class RaiseAttribute
    {
        START,
        RAISE,
        CONFIRM,
        END
    };

    enum class Retreat
    {
        START,
        RETREAT,
        CONFIRM,
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

    void GAIN_HEALTH(std::vector<Character::Base> &party, Team::Type team, int health)
    {
        for (auto i = 0; i < party.size(); i++)
        {
            if (party[i].Team == team)
            {
                Engine::GAIN_HEALTH(party[i], health);
            }
        }
    }

    void REST(Party::Base &party)
    {
        for (auto i = 0; i < party.Party.size(); i++)
        {
            party.Party[i].Health = party.Party[i].MaximumHealth;
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

    void GAIN_SCORE(Character::Base &character, Attribute::Type type, int score)
    {
        for (auto i = 0; i < character.Attributes.size(); i++)
        {
            if (character.Attributes[i].Type == type)
            {
                character.Attributes[i].Value += score;

                if (character.Attributes[i].Value < 1)
                {
                    character.Attributes[i].Value = 1;
                }

                break;
            }
        }
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

        if (Engine::HAS_STATUS(character, Character::Status::ENRAGED))
        {
            max += 1;
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

    int COUNT(std::vector<Character::Base> &adventurers, Team::Type team)
    {
        auto result = 0;

        if (team == Team::Type::NONE)
        {
            return Engine::COUNT(adventurers);
        }
        else
        {
            for (auto i = 0; i < adventurers.size(); i++)
            {
                if (adventurers[i].Health > 0 && (adventurers[i].Team == team))
                {
                    result++;
                }
            }
        }

        return result;
    }

    int TEAM_SIZE(std::vector<Character::Base> &adventurers, Team::Type team)
    {
        auto result = 0;

        if (team == Team::Type::NONE)
        {
            return Engine::COUNT(adventurers);
        }
        else
        {
            for (auto i = 0; i < adventurers.size(); i++)
            {
                if (adventurers[i].Team == team)
                {
                    result++;
                }
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

    void LOSE_EQUIPMENT(Party::Base &party, std::vector<Equipment::Type> items)
    {
        for (auto i = 0; i < items.size(); i++)
        {
            for (auto j = 0; j < party.Party.size(); j++)
            {
                auto result = Engine::FIND_EQUIPMENT(party.Party[j], items[i]);

                if (result >= 0)
                {
                    party.Party[j].Equipment.erase(party.Party[j].Equipment.begin() + result);

                    // break out of party loop
                    break;
                }
            }
        }
    }

    void LOSE_EQUIPMENT(Party::Base &party, Equipment::Type item, int count)
    {
        for (auto i = 0; i < count; i++)
        {
            Engine::LOSE_EQUIPMENT(party, {item});
        }
    }

    int COUNT_INVENTORY(Character::Base &player)
    {
        auto size = player.Equipment.size();

        for (auto i = 0; i < player.Equipment.size(); i++)
        {
            size += player.Equipment[i].AdditionalSlots;
        }

        return size;
    }

    bool VERIFY_EQUIPMENT_LIMIT(Character::Base &player)
    {
        return Engine::COUNT_INVENTORY(player) <= player.MaximumEquipment;
    }

    bool VERIFY_EQUIPMENT_LIMIT(Party::Base &party)
    {
        auto result = true;

        for (auto i = 0; i < party.Party.size(); i++)
        {
            result &= Engine::VERIFY_EQUIPMENT_LIMIT(party.Party[i]);
        }

        return result;
    }

    bool VERIFY_EQUIPMENT_LIMIT(Character::Base &player, int limit)
    {
        return Engine::COUNT_INVENTORY(player) <= limit;
    }

    bool VERIFY_EQUIPMENT_LIMIT(Party::Base &party, int limit)
    {
        auto result = true;

        for (auto i = 0; i < party.Party.size(); i++)
        {
            result &= Engine::VERIFY_EQUIPMENT_LIMIT(party.Party[i], limit);
        }

        return result;
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

    void LOSE_CODES(Party::Base &party, std::vector<Codes::Type> codes)
    {
        for (auto i = 0; i < codes.size(); i++)
        {
            auto result = Engine::FIND_CODE(party, codes[i]);

            if (result >= 0)
            {
                party.InvisibleCodes.erase(party.InvisibleCodes.begin() + result);
            }
        }
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
        Character::Romance romance = {from, to};

        if (party.Hearts.count(romance) > 0)
        {
            party.Hearts[romance] += heart;
        }
        else
        {
            party.Hearts[romance] = heart;
        }
    }

    void REJOIN(Party::Base &party)
    {
        if (party.Current >= 0 && party.Party.size())
        {
            party.Party[party.Current].Team = Team::Type::NONE;
        }

        party.Current = -1;
    }

    int FIND_LIST(std::vector<Team::Type> selection, Team::Type team)
    {
        auto found = -1;

        for (auto i = 0; i < selection.size(); i++)
        {
            if (selection[i] == team)
            {
                found = i;

                break;
            }
        }

        return found;
    }

    int COUNT_TEAMS(Party::Base &party)
    {
        auto teams = std::vector<Team::Type>();

        for (auto i = 0; i < party.Party.size(); i++)
        {
            if (Engine::FIND_LIST(teams, party.Party[i].Team) < 0 && party.Party[i].Team != Team::Type::NONE)
            {
                teams.push_back(party.Party[i].Team);
            }
        }

        return teams.size();
    }

    std::vector<Team::Type> GET_TEAMS(Party::Base &party)
    {
        auto teams = std::vector<Team::Type>();

        for (auto i = 0; i < party.Party.size(); i++)
        {
            if (Engine::FIND_LIST(teams, party.Party[i].Team) < 0 && party.Party[i].Team != Team::Type::NONE)
            {
                teams.push_back(party.Party[i].Team);
            }
        }

        return teams;
    }

    void CONSOLIDATE(Party::Base &party)
    {
        if (Engine::COUNT_TEAMS(party) > 0)
        {
            for (auto i = 0; i < party.Party.size(); i++)
            {
                party.Party[i].Team = Team::Type::NONE;
            }
        }
    }

    void GO_SOLO(Party::Base &party, Character::Type character)
    {
        auto result = Engine::FIND_CHARACTER(party.Party, character);

        if (result >= 0)
        {
            party.Current = result;

            party.Party[result].Team = Team::Type::SOLO;
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

    int FIND_SPELL(Character::Base &character, Spells::Type spell)
    {
        auto found = -1;

        if (character.SpellCaster)
        {
            for (auto i = 0; i < character.SpellBook.size(); i++)
            {
                if (character.SpellBook[i].Type == spell)
                {
                    found = i;

                    break;
                }
            }
        }

        return found;
    }

    bool VERIFY_SPELL(Character::Base &character, std::vector<Spells::Type> spells)
    {
        auto found = 0;

        for (auto i = 0; i < spells.size(); i++)
        {
            auto result = Engine::FIND_SPELL(character, spells[i]);

            if (result >= 0)
            {
                found++;
            }
        }

        return found >= spells.size();
    }

    bool VERIFY_SPELL_ANY(Character::Base &character, std::vector<Spells::Type> spells)
    {
        auto found = 0;

        for (auto i = 0; i < spells.size(); i++)
        {
            auto result = Engine::FIND_SPELL(character, spells[i]);

            if (result >= 0)
            {
                found++;
            }
        }

        return found > 0;
    }

    bool HAS_SPELL(Party::Base &party, std::vector<Spells::Type> spells)
    {
        auto result = false;

        for (auto i = 0; i < party.Party.size(); i++)
        {
            if (party.Party[i].Health > 0 && party.Party[i].SpellCaster && party.Party[i].SpellBook.size() > 0)
            {
                result = Engine::VERIFY_SPELL(party.Party[i], spells);

                if (result)
                {
                    break;
                }
            }
        }

        return result;
    }

    bool HAS_SPELL_ANY(Party::Base &party, std::vector<Spells::Type> spells)
    {
        auto result = false;

        for (auto i = 0; i < party.Party.size(); i++)
        {
            if (party.Party[i].Health > 0 && party.Party[i].SpellCaster && party.Party[i].SpellBook.size() > 0)
            {
                result = Engine::VERIFY_SPELL_ANY(party.Party[i], spells);

                if (result)
                {
                    break;
                }
            }
        }

        return result;
    }

    bool VERIFY_SPELL_LIMIT(Character::Base &player)
    {
        if (player.SpellCaster)
        {
            return player.SpellBook.size() <= player.SpellBookLimit;
        }
        else
        {
            return true;
        }
    }

    int FIND_SHIP(Party::Base &party, Location::Type location, int needed_space)
    {
        auto result = -1;

        if (party.Fleet.size() > 0)
        {
            for (auto i = 0; i < party.Fleet.size(); i++)
            {
                auto available_space = party.Fleet[i].MaximumCargo - party.Fleet[i].Cargo.size();

                if (party.Fleet[i].Type != Ship::Type::NONE && party.Fleet[i].Location == location && available_space >= needed_space)
                {
                    result = i;

                    break;
                }
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

    void SET_LOCATION(Party::Base &party, Location::Type location)
    {
        if (party.CurrentShip >= 0 && party.CurrentShip < party.Fleet.size())
        {
            if (party.Fleet[party.CurrentShip].Location == party.Location)
            {
                party.Fleet[party.CurrentShip].Location = location;
            }
        }

        party.Location = location;
    }

    bool IN_TEAM(Party::Base &party, Character::Type character, Team::Type team)
    {
        auto found = false;

        auto result = Engine::FIND_CHARACTER(party.Party, character);

        if (result >= 0 && result < party.Party.size())
        {
            if (party.Party[result].Team == team || team == Team::Type::NONE)
            {
                found = true;
            }
        }

        return found;
    }

    bool IN_PARTY(Party::Base &party, Character::Type character)
    {
        return Engine::IN_TEAM(party, character, Team::Type::NONE);
    }

    int FIND_UNIT(std::vector<Army::Base> &army, Army::Type unit)
    {
        int result = -1;

        for (auto i = 0; i < army.size(); i++)
        {
            if (army[i].Type == unit)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    int FIND_UNIT(std::vector<Army::Base> &army, Location::BattleField position)
    {
        int result = -1;

        for (auto i = 0; i < army.size(); i++)
        {
            if (army[i].Position == position)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    void SINK_SHIP(Party::Base &party)
    {
        if (party.CurrentShip >= 0 && party.CurrentShip < party.Fleet.size())
        {
            party.Fleet.erase(party.Fleet.begin() + party.CurrentShip);
        }

        party.CurrentShip = -1;
    }

    void LOSE_ALL(Party::Base &party)
    {
        for (auto i = 0; i < party.Party.size(); i++)
        {
            party.Party[i].Equipment.clear();
        }
    }

    void LOSE_ALL(Party::Base &party, Equipment::Class Class)
    {
        for (auto i = 0; i < party.Party.size(); i++)
        {
            auto items = std::vector<Equipment::Base>();

            for (auto j = 0; j < party.Party[i].Equipment.size(); j++)
            {
                if (party.Party[i].Equipment[j].Class != Class)
                {
                    items.push_back(party.Party[i].Equipment[j]);
                }
            }

            if (items.size() > 0)
            {
                party.Party[i].Equipment = items;
            }
            else
            {
                party.Party[i].Equipment.clear();
            }
        }
    }
}
#endif