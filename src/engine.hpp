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

    // ship, price, sell price
    typedef std::tuple<Ship::Base, int, int> ShipPrices;

    // cargo, price, sell price
    typedef std::tuple<Cargo::Type, int, int> CargoPrices;

    // equipment, price, sell price, supply limit
    typedef std::tuple<Equipment::Base, int, int, int, std::vector<Codes::Base>> EquipmentPrice;

    // equipment needed, equipment that can be exchanged for it
    typedef std::tuple<Equipment::Base, std::vector<Equipment::Base>> BarterExchanges;

    // team, minimum, maximum
    typedef std::tuple<Team::Type, int, int> TeamAssignment;

    // spell type, combat round when cast
    typedef std::tuple<Spells::MassCombat, int> BattlefieldSpells;

    // status, unit number, start round, duration
    typedef std::tuple<Army::Status, int, int, int> ArmyStatus;

    // treshold, bye text, destination
    typedef std::tuple<int, const char *, Engine::Destination> RandomDestination;

    enum class Combat
    {
        NONE,
        VICTORY,
        DEFEAT,
        FLEE,
        EXCEED_LIMIT
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

    enum class MassCombat
    {
        START,
        COMBAT,
        MORALE,
        END
    };

    enum class MassCombatMode
    {
        NORMAL = 0,
        DEPLOY,
        VIEW,
        STATUS,
        SPELL
    };

    const char *CURRENCY = "silver";

    Party::Base Party = Party::Base();

    auto Random = Random::Base();

    int FIND_FOLLOWER(Character::Base &character, Follower::Type type)
    {
        auto result = -1;

        for (auto i = 0; i < character.Followers.size(); i++)
        {
            if (character.Followers[i].Type == type)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    bool HAS_FOLLOWER(Character::Base &character, Follower::Type follower)
    {
        auto result = Engine::FIND_FOLLOWER(character, follower);

        return (result >= 0 && result < character.Followers.size() && character.Followers[result].Health > 0);
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

    int FIND_SOLO(Party::Base &party)
    {
        auto result = -1;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (party.Members[i].Team == Team::Type::SOLO && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
            {
                result = i;

                break;
            }
        }

        return result;
    }

    void GET_EQUIPMENT(Character::Base &character, std::vector<Equipment::Base> equipment)
    {
        if (!Engine::HAS_STATUS(character, Character::Status::CAPTURED) && character.Type != Character::Type::SKULLCRACKER)
        {
            character.Equipment.insert(character.Equipment.end(), equipment.begin(), equipment.end());
        }
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

    int FIND_BEARER(Party::Base &party, Equipment::Type item)
    {
        auto found = -1;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            auto result = Engine::FIND_EQUIPMENT(party.Members[i], item);

            if (result >= 0 && result < party.Members[i].Equipment.size() && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
            {
                found = i;

                break;
            }
        }

        return found;
    }

    void LOSE_EQUIPMENT(Character::Base &character, std::vector<Equipment::Type> items)
    {
        if (character.Equipment.size() > 0 && items.size() > 0 && !Engine::HAS_STATUS(character, Character::Status::CAPTURED) && character.Type != Character::Type::SKULLCRACKER)
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
            for (auto j = 0; j < party.Members.size(); j++)
            {
                auto result = Engine::FIND_EQUIPMENT(party.Members[j], items[i]);

                if (result >= 0 && !Engine::HAS_STATUS(party.Members[j], Character::Status::CAPTURED) && (!party.InCity || (party.InCity && party.Members[j].IsCivilized)) && party.Members[i].Type != Character::Type::SKULLCRACKER)
                {
                    party.Members[j].Equipment.erase(party.Members[j].Equipment.begin() + result);

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

    int COUNT_INVENTORY(Character::Base &character)
    {
        auto size = character.Equipment.size();

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            size += character.Equipment[i].AdditionalSlots;
        }

        return size;
    }

    bool VERIFY_EQUIPMENT_LIMIT(Character::Base &character)
    {
        return Engine::COUNT_INVENTORY(character) <= character.MaximumEquipment;
    }

    bool VERIFY_EQUIPMENT_LIMIT(Party::Base &party)
    {
        auto result = true;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            result &= Engine::VERIFY_EQUIPMENT_LIMIT(party.Members[i]);
        }

        return result;
    }

    bool VERIFY_EQUIPMENT_LIMIT(Character::Base &character, int limit)
    {
        return Engine::COUNT_INVENTORY(character) <= limit;
    }

    bool VERIFY_EQUIPMENT_LIMIT(Party::Base &party, int limit)
    {
        auto result = true;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            result &= Engine::VERIFY_EQUIPMENT_LIMIT(party.Members[i], limit);
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
                    found += 1;
                }
            }
        }

        return found;
    }

    int COUNT_EQUIPMENT(Character::Base &character, std::vector<Equipment::Type> equipment)
    {
        auto found = 0;

        for (auto i = 0; i < equipment.size(); i++)
        {
            if (Engine::COUNT_EQUIPMENT(character, equipment[i]) > 0)
            {
                found += 1;
            }
        }

        return found;
    }

    int COUNT_EQUIPMENT(Party::Base &party, std::vector<Equipment::Type> equipment)
    {
        auto found = 0;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if ((!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
            {
                found += Engine::COUNT_EQUIPMENT(party.Members[i], equipment);
            }
        }

        return found;
    }

    bool VERIFY_EQUIPMENT(Character::Base &character, std::vector<Equipment::Type> equipment)
    {
        auto found = 0;

        if (equipment.size() > 0)
        {
            for (auto i = 0; i < equipment.size(); i++)
            {
                auto result = Engine::FIND_EQUIPMENT(character, equipment[i]);

                if (result >= 0)
                {
                    found += 1;
                }
            }
        }

        return found >= equipment.size();
    }

    bool VERIFY_ANY_EQUIPMENT(Character::Base &character, std::vector<Equipment::Type> equpment)
    {
        return Engine::COUNT_EQUIPMENT(character, equpment) > 0;
    }

    bool VERIFY_EQUIPMENT(Party::Base &party, std::vector<Equipment::Type> equipment)
    {
        auto found = false;

        auto result = Engine::FIND_SOLO(party);

        if (result >= 0 && result < party.Members.size() && (!party.InCity || (party.InCity && party.Members[result].IsCivilized)))
        {
            found = Engine::VERIFY_EQUIPMENT(party.Members[result], equipment);
        }
        else
        {
            for (auto i = 0; i < party.Members.size(); i++)
            {
                if (Engine::VERIFY_EQUIPMENT(party.Members[i], equipment) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
                {
                    found = true;

                    break;
                }
            }
        }

        return found;
    }

    bool VERIFY_ANY_EQUIPMENT(Party::Base &party, std::vector<Equipment::Type> equipment)
    {
        auto found = false;

        auto result = Engine::FIND_SOLO(party);

        if (result >= 0 && result < party.Members.size())
        {
            found = Engine::VERIFY_ANY_EQUIPMENT(party.Members[result], equipment);
        }
        else
        {
            for (auto i = 0; i < party.Members.size(); i++)
            {
                if (Engine::VERIFY_ANY_EQUIPMENT(party.Members[i], equipment) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
                {
                    found = true;

                    break;
                }
            }
        }

        return found;
    }

    int MAX(Character::Base &character, Equipment::Class type, Attribute::Type attribute)
    {
        auto max = 0;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Class == type && (character.Equipment[i].Attribute == attribute || character.Equipment[i].Attribute == Attribute::Type::ALL_SKILLS))
            {
                if (character.Equipment[i].Modifier > max)
                {
                    max = character.Equipment[i].Modifier;
                }
            }
        }

        return max;
    }

    int RAW_SCORE(Character::Base &character, Attribute::Type type, bool clip)
    {
        auto score = 0;

        if (type == Attribute::Type::HEALTH)
        {
            score = character.Health;
        }

        for (auto i = 0; i < character.Attributes.size(); i++)
        {
            if (character.Attributes[i].Type == type)
            {
                score = character.Attributes[i].Value;

                break;
            }
        }

        if (type == Attribute::Type::FIGHTING && Engine::HAS_STATUS(character, Character::Status::ENRAGED))
        {
            score += 1;
        }

        if (type == Attribute::Type::FIGHTING && Engine::HAS_STATUS(character, Character::Status::LOST_FINGERNAILS))
        {
            score -= 1;
        }

        if (type == Attribute::Type::LORE && Engine::HAS_STATUS(character, Character::Status::ENLIGHTENED))
        {
            score += 1;
        }

        if (clip)
        {
            if (score < 0)
            {
                score = 0;
            }
        }

        return score;
    }

    int SCORE(Character::Base &character, Attribute::Type type)
    {
        auto score = Engine::RAW_SCORE(character, type, false);

        if (type == Attribute::Type::FIGHTING && Engine::HAS_FOLLOWER(character, Follower::Type::MORDAIN_SKELETONS))
        {
            score += 2;
        }

        if (character.Health > 0)
        {
            for (auto i = 0; i < character.Equipment.size(); i++)
            {
                if (character.Equipment[i].Class == Equipment::Class::NORMAL && character.Equipment[i].Attribute == type)
                {
                    score += character.Equipment[i].Modifier;
                }
            }
        }

        if (type != Attribute::Type::HEALTH && type != Attribute::Type::ARMOUR)
        {
            score += Engine::MAX(character, Equipment::Class::ROBE, type);
            score += Engine::MAX(character, Equipment::Class::SHIELD, type);
        }

        if (type != Attribute::Type::FIGHTING)
        {
            if (score < 0)
            {
                score = 0;
            }
        }

        return score;
    }

    int FIGHTING_SCORE(Character::Base &character)
    {
        auto max = -1;

        auto score = Engine::SCORE(character, Attribute::Type::FIGHTING);

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Class == Equipment::Class::WEAPON && character.Equipment[i].Attribute == Attribute::Type::FIGHTING)
            {
                max = std::max(max, character.Equipment[i].Modifier);
            }
        }

        score = max >= 0 ? (score + max) : (Engine::HAS_STATUS(character, Character::Status::UNARMED_COMBAT) ? score : (score - 1));

        if (score < 0)
        {
            score = 0;
        }

        return score;
    }

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
        auto score = Engine::SCORE(character, Attribute::Type::HEALTH);

        if (score > 0)
        {
            score += health;
        }

        character.Health += (score - Engine::SCORE(character, Attribute::Type::HEALTH));

        if (character.Health < 0)
        {
            character.Health = 0;
        }

        if (character.Health > character.MaximumHealth)
        {
            character.Health = character.MaximumHealth;
        }
    }

    void GAIN_HEALTH(Party::Base &party, int health)
    {
        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (!party.InCity || (party.InCity && party.Members[i].IsCivilized))
            {
                Engine::GAIN_HEALTH(party.Members[i], health);
            }
        }
    }

    void GAIN_HEALTH(Party::Base &party, Team::Type team, int health)
    {
        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (party.Members[i].Team == team && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
            {
                Engine::GAIN_HEALTH(party.Members[i], health);
            }
        }
    }

    void RESTORE_HEALTH(Party::Base &party, int threshold)
    {
        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (party.Members[i].Health > 0 && party.Members[i].Health < threshold && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
            {
                Engine::GAIN_HEALTH(party.Members[i], threshold - party.Members[i].Health);
            }
        }
    }

    void RAISE_HEALTH(Party::Base &party, int value)
    {
        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0)
            {
                party.Members[i].MaximumHealth += value;

                if (party.Members[i].MaximumHealth < 0)
                {
                    party.Members[i].MaximumHealth = 0;
                }

                Engine::GAIN_HEALTH(party.Members[i], value);
            }
        }
    }

    void GAIN_HEALTH(Ship::Base &ship, int health)
    {
        if (ship.Health > 0)
        {
            ship.Health += health;
        }

        if (ship.Health < 0)
        {
            ship.Health = 0;
        }

        if (ship.Health > ship.MaximumHealth)
        {
            ship.Health = ship.MaximumHealth;
        }
    }

    void REST(Party::Base &party)
    {
        for (auto i = 0; i < party.Members.size(); i++)
        {
            party.Members[i].Health = party.Members[i].MaximumHealth;
        }
    }

    void GAIN_HEALTH(Monster::Base &monster, int health)
    {
        auto initial = monster.Health;

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

        if (monster.Health < initial)
        {
            monster.Damaged = true;
        }
    }

    void GAIN_HEALTH(std::vector<Monster::Base> &monsters, int health)
    {
        for (auto i = 0; i < monsters.size(); i++)
        {
            Engine::GAIN_HEALTH(monsters[i], health);
        }
    }

    void GAIN_HEALTH(std::vector<Monster::Base> &monsters, Monster::Type type, int health)
    {
        for (auto i = 0; i < monsters.size(); i++)
        {
            if (monsters[i].Type == type)
            {
                Engine::GAIN_HEALTH(monsters[i], health);
            }
        }
    }

    void GAIN_MORALE(Army::Base &unit, int morale)
    {
        if (unit.Morale > 0)
        {
            unit.Morale += morale;

            if (unit.Morale < 0)
            {
                unit.Morale = 0;
            }

            if (unit.Morale > unit.MaximumMorale)
            {
                unit.Morale = unit.MaximumMorale;
            }
        }
    }

    void GAIN_MORALE(Party::Base &party, int morale)
    {
        for (auto i = 0; i < party.Army.size(); i++)
        {
            Engine::GAIN_MORALE(party.Army[i], morale);
        }
    }

    void GAIN_MORALE(Party::Base &party, Location::Type garrison, int morale)
    {
        if (garrison == Location::Type::NONE)
        {
            Engine::GAIN_MORALE(party, morale);
        }
        else
        {
            for (auto i = 0; i < party.Army.size(); i++)
            {
                if (party.Army[i].Garrison == garrison)
                {
                    Engine::GAIN_MORALE(party.Army[i], morale);
                }
            }
        }
    }

    void RESTORE_MORALE(Party::Base &party)
    {
        for (auto i = 0; i < party.Army.size(); i++)
        {
            party.Army[i].Morale = party.Army[i].MaximumMorale;
        }
    }

    void RESTORE_MORALE(Party::Base &party, Location::Type garrison)
    {
        if (garrison == Location::Type::NONE)
        {
            Engine::RESTORE_MORALE(party);
        }
        else
        {
            for (auto i = 0; i < party.Army.size(); i++)
            {
                if (party.Army[i].Garrison == garrison)
                {
                    party.Army[i].Morale = party.Army[i].MaximumMorale;
                }
            }
        }
    }

    void RESTORE_STRENGTH(Party::Base &party)
    {
        for (auto i = 0; i < party.Army.size(); i++)
        {
            party.Army[i].Strength = party.Army[i].MaximumStrength;
        }
    }

    void RESTORE_STRENGTH(Party::Base &party, Location::Type garrison)
    {
        if (garrison == Location::Type::NONE)
        {
            Engine::RESTORE_STRENGTH(party);
        }
        else
        {
            for (auto i = 0; i < party.Army.size(); i++)
            {
                if (party.Army[i].Garrison == garrison)
                {
                    party.Army[i].Strength = party.Army[i].MaximumStrength;
                }
            }
        }
    }

    void TRANSFER(Party::Base &party, Location::Type from, Location::Type to)
    {
        for (auto i = 0; i < party.Army.size(); i++)
        {
            if (party.Army[i].Garrison == from)
            {
                party.Army[i].Garrison = to;
            }
        }
    }

    void GAIN_STRENGTH(Army::Base &unit, int strength)
    {
        if (unit.Strength > 0)
        {
            unit.Strength += strength;

            if (unit.Strength < 0)
            {
                unit.Strength = 0;
            }

            if (unit.Strength > unit.MaximumStrength)
            {
                unit.Strength = unit.MaximumStrength;
            }
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

    int COUNT(int rolls, int difficulty)
    {
        auto count = 0;

        auto results = Engine::ROLL_DICE(rolls);

        for (auto i = 0; i < results.size(); i++)
        {
            if (results[i] >= difficulty)
            {
                count += 1;
            }
        }

        return count;
    }

    bool HAS_STATUS(Party::Base &party, Character::Status status)
    {
        auto result = false;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::HAS_STATUS(party.Members[i], status) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
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

    void REMOVE_STATUS(Party::Base &party, Character::Status status)
    {
        for (auto i = 0; i < party.Members.size(); i++)
        {
            Engine::REMOVE_STATUS(party.Members[i], status);
        }
    }

    void CLEAR_STATUS(Character::Base &character)
    {
        character.Status.clear();
    }

    void GAIN_FOLLOWERS(Character::Base &character, std::vector<Follower::Base> followers)
    {
        if (Engine::SCORE(character, Attribute::Type::HEALTH) > 0 && followers.size() > 0)
        {
            for (auto i = 0; i < followers.size(); i++)
            {
                auto result = Engine::FIND_FOLLOWER(character, followers[i].Type);

                if (result < 0)
                {
                    character.Followers.push_back(followers[i]);
                }
            }
        }
    }

    void LOSE_FOLLOWERS(Character::Base &character, std::vector<Follower::Type> followers)
    {
        if (Engine::SCORE(character, Attribute::Type::HEALTH) > 0 && followers.size() > 0)
        {
            for (auto i = 0; i < followers.size(); i++)
            {
                auto result = Engine::FIND_FOLLOWER(character, followers[i]);

                if (result >= 0 && result < character.Followers.size())
                {
                    character.Followers.erase(character.Followers.begin() + result);
                }
            }
        }
    }

    void LOSE_FOLLOWERS(Party::Base &party, std::vector<Follower::Type> followers)
    {
        for (auto i = 0; i < party.Members.size(); i++)
        {
            Engine::LOSE_FOLLOWERS(party.Members[i], followers);
        }
    }

    bool HAS_FOLLOWER(Party::Base &party, Follower::Type follower)
    {
        auto result = false;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            result |= Engine::HAS_FOLLOWER(party.Members[i], follower);
        }

        return result;
    }

    void GAIN_SCORE(Character::Base &character, Attribute::Type type, int score)
    {
        for (auto i = 0; i < character.Attributes.size(); i++)
        {
            if (character.Attributes[i].Type == type)
            {
                character.Attributes[i].Value += score;

                if (character.Attributes[i].Value < 0)
                {
                    character.Attributes[i].Value = 0;
                }

                break;
            }
        }
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

    int FIND_EQUIPMENT(Character::Base &character, Equipment::Class type, Attribute::Type attribute)
    {
        auto result = -1;

        auto max = 0;

        for (auto i = 0; i < character.Equipment.size(); i++)
        {
            if (character.Equipment[i].Class == type && character.Equipment[i].Attribute == attribute)
            {
                if (character.Equipment[i].Modifier > max)
                {
                    max = character.Equipment[i].Modifier;

                    result = i;
                }
            }
        }

        return result;
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
                result += 1;
            }
        }

        return result;
    }

    int COUNT(std::vector<Monster::Base> &monsters, int round)
    {
        auto result = 0;

        for (auto i = 0; i < monsters.size(); i++)
        {
            if (monsters[i].Health > 0 && round >= monsters[i].Round)
            {
                result += 1;
            }
        }

        return result;
    }

    int COUNT(Party::Base &party)
    {
        auto result = 0;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0 && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
            {
                result += 1;
            }
        }

        return result;
    }

    int OUTSIDE(Party::Base &party)
    {
        auto result = 0;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (party.InCity && !party.Members[i].IsCivilized)
            {
                result += 1;
            }
        }

        return result;
    }

    int COUNT(Party::Base &party, Team::Type team)
    {
        auto result = 0;

        if (team == Team::Type::NONE)
        {
            return Engine::COUNT(party);
        }
        else
        {
            for (auto i = 0; i < party.Members.size(); i++)
            {
                if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0 && party.Members[i].Team == team && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
                {
                    result += 1;
                }
            }
        }

        return result;
    }

    int COUNT(Party::Base &party, std::vector<int> list)
    {
        auto result = 0;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0 && Engine::FIND_LIST(list, i) < 0 && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
            {
                result += 1;
            }
        }

        return result;
    }

    int COUNT(Party::Base &party, Team::Type team, std::vector<int> list)
    {
        auto result = 0;

        if (team == Team::Type::NONE)
        {
            return Engine::COUNT(party, list);
        }
        else
        {
            for (auto i = 0; i < party.Members.size(); i++)
            {
                if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0 && party.Members[i].Team == team && Engine::FIND_LIST(list, i) < 0 && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
                {
                    result += 1;
                }
            }
        }

        return result;
    }

    int TEAM_SIZE(Party::Base &party, Team::Type team)
    {
        auto result = 0;

        if (team == Team::Type::NONE)
        {
            return Engine::COUNT(party);
        }
        else
        {
            for (auto i = 0; i < party.Members.size(); i++)
            {
                if (party.Members[i].Team == team && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
                {
                    result += 1;
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
                result += 1;
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
                result += 1;
            }
        }

        return result;
    }

    int SPELLCASTERS(Party::Base &party)
    {
        auto result = 0;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (party.Members[i].SpellCaster && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
            {
                result += 1;
            }
        }

        return result;
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
                    found += 1;
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
                    found += 1;
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

            if (result >= 0 && result < party.InvisibleCodes.size())
            {
                party.InvisibleCodes.erase(party.InvisibleCodes.begin() + result);
            }
        }
    }

    void LOSE_CODES(Party::Base &party, std::vector<Codes::Base> codes)
    {
        for (auto i = 0; i < codes.size(); i++)
        {
            auto result = Engine::FIND_CODE(party, codes[i]);

            if (result >= 0 && result < party.Codes.size())
            {
                party.Codes.erase(party.Codes.begin() + result);
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

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (party.Members[i].Type == type)
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

    int FIND_CHARACTER(std::vector<Character::Type> &party, Character::Type type)
    {
        auto result = -1;

        for (auto i = 0; i < party.size(); i++)
        {
            if (party[i] == type)
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

    int HEARTS(Party::Base &party, Character::Type from, Character::Type to)
    {
        auto result = 0;

        Character::Romance romance = {from, to};

        if (party.Hearts.count(romance) > 0)
        {
            result = party.Hearts[romance];
        }

        return result;
    }

    void REJOIN(Party::Base &party)
    {
        if (party.CurrentCharacter >= 0 && party.Members.size())
        {
            party.Members[party.CurrentCharacter].Team = Team::Type::NONE;
        }

        party.CurrentCharacter = -1;
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

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::FIND_LIST(teams, party.Members[i].Team) < 0 && party.Members[i].Team != Team::Type::NONE)
            {
                teams.push_back(party.Members[i].Team);
            }
        }

        return teams.size();
    }

    std::vector<Team::Type> GET_TEAMS(Party::Base &party)
    {
        auto teams = std::vector<Team::Type>();

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::FIND_LIST(teams, party.Members[i].Team) < 0 && party.Members[i].Team != Team::Type::NONE)
            {
                teams.push_back(party.Members[i].Team);
            }
        }

        return teams;
    }

    void CONSOLIDATE(Party::Base &party)
    {
        if (Engine::COUNT_TEAMS(party) > 0)
        {
            for (auto i = 0; i < party.Members.size(); i++)
            {
                party.Members[i].Team = Team::Type::NONE;
            }
        }

        party.CurrentCharacter = -1;
    }

    void GO_SOLO(Party::Base &party, Character::Type character)
    {
        Engine::CONSOLIDATE(party);

        auto result = Engine::FIND_CHARACTER(party.Members, character);

        if (result >= 0 && result < party.Members.size())
        {
            party.CurrentCharacter = result;

            party.Members[result].Team = Team::Type::SOLO;
        }
    }

    int FIND_MONSTER(std::vector<Monster::Base> &monsters, Monster::Type type)
    {
        auto result = -1;

        for (auto i = 0; i < monsters.size(); i++)
        {
            if (monsters[i].Type == type && monsters[i].Health > 0)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    bool HAS_MONSTER(std::vector<Monster::Base> &monsters, Monster::Type type)
    {
        auto result = Engine::FIND_MONSTER(monsters, type);

        return (result >= 0 && result < monsters.size());
    }

    bool HAS_ALLY(std::vector<Allies::Type> allies, Allies::Type type)
    {
        auto result = false;

        for (auto i = 0; i < allies.size(); i++)
        {
            if (allies[i] == type)
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
                found += 1;
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
                found += 1;
            }
        }

        return found > 0;
    }

    bool HAS_SPELL(Party::Base &party, std::vector<Spells::Type> spells)
    {
        auto result = false;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0 && party.Members[i].SpellCaster && party.Members[i].SpellBook.size() > 0)
            {
                result = Engine::VERIFY_SPELL(party.Members[i], spells);

                if (result)
                {
                    break;
                }
            }
        }

        return result;
    }

    bool HAS_SPELL(Character::Base &character, std::vector<Spells::Type> spells)
    {
        auto result = false;

        if (Engine::SCORE(character, Attribute::Type::HEALTH) > 0 && character.SpellCaster && character.SpellBook.size() > 0)
        {
            result = Engine::VERIFY_SPELL(character, spells);
        }

        return result;
    }

    bool CAN_CAST(Party::Base &party, Team::Type team, Spells::Type spell)
    {
        auto result = false;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0 && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED) && party.Members[i].SpellCaster && party.Members[i].SpellBook.size() > 0 && (team == Team::Type::NONE || party.Members[i].Team == team))
            {
                auto found = Engine::FIND_SPELL(party.Members[i], spell);

                if (found >= 0 && found < party.Members[i].SpellBook.size() && party.Members[i].SpellBook[found].Charged)
                {
                    result = true;

                    break;
                }
            }
        }

        return result;
    }

    void CAST_SPELL(Party::Base &party, Team::Type team, Spells::Type spell)
    {
        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0 && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED) && party.Members[i].SpellCaster && party.Members[i].SpellBook.size() > 0 && (team == Team::Type::NONE || party.Members[i].Team == team))
            {
                auto found = Engine::FIND_SPELL(party.Members[i], spell);

                if (found >= 0 && found < party.Members[i].SpellBook.size() && party.Members[i].SpellBook[found].Charged)
                {
                    party.Members[i].SpellBook[found].Charged = false;

                    break;
                }
            }
        }
    }

    bool HAS_SPELL_ANY(Party::Base &party, std::vector<Spells::Type> spells)
    {
        auto result = false;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0 && party.Members[i].SpellCaster && party.Members[i].SpellBook.size() > 0)
            {
                result = Engine::VERIFY_SPELL_ANY(party.Members[i], spells);

                if (result)
                {
                    break;
                }
            }
        }

        return result;
    }

    bool VERIFY_SPELL_LIMIT(Character::Base &character)
    {
        if (character.SpellCaster)
        {
            return character.SpellBook.size() <= character.SpellBookLimit;
        }
        else
        {
            return true;
        }
    }

    bool VERIFY_SPELL_LIMIT(Character::Base &character, int spell_limit)
    {
        if (character.SpellCaster)
        {
            return character.SpellBook.size() <= spell_limit;
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

    void SET_LOCATION(Party::Base &party, Location::Type location, bool inCity)
    {
        if (party.CurrentShip >= 0 && party.CurrentShip < party.Fleet.size())
        {
            if (party.Fleet[party.CurrentShip].Location == party.Location)
            {
                party.Fleet[party.CurrentShip].Location = location;
            }
        }

        party.Location = location;
        party.InCity = inCity;
    }

    void SET_LOCATION(Party::Base &party, Location::Type location)
    {
        Engine::SET_LOCATION(party, location, false);
    }

    bool IN_TEAM(Party::Base &party, Character::Type character, Team::Type team)
    {
        auto found = false;

        auto result = Engine::FIND_CHARACTER(party.Members, character);

        if (result >= 0 && result < party.Members.size())
        {
            if (party.Members[result].Team == team || team == Team::Type::NONE)
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

    int FIND_UNIT(std::vector<Army::Base> &army, Location::Zone zone)
    {
        auto result = -1;

        if (zone == Location::Zone::LEFT_FLANK)
        {
            result = Engine::FIND_UNIT(army, Location::BattleField::LEFT_FLANK_FRONT);
        }
        else if (zone == Location::Zone::CENTER)
        {
            result = Engine::FIND_UNIT(army, Location::BattleField::CENTER_FRONT);
        }
        else if (zone == Location::Zone::RIGHT_FLANK)
        {
            result = Engine::FIND_UNIT(army, Location::BattleField::RIGHT_FLANK_FRONT);
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
        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (party.Members[i].Type != Character::Type::SKULLCRACKER)
            {
                party.Members[i].Equipment.clear();
            }
        }
    }

    void LOSE_ALL(Party::Base &party, Equipment::Class Class)
    {
        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (party.Members[i].Type != Character::Type::SKULLCRACKER)
            {
                auto items = std::vector<Equipment::Base>();

                for (auto j = 0; j < party.Members[i].Equipment.size(); j++)
                {
                    if (party.Members[i].Equipment[j].Class != Class)
                    {
                        items.push_back(party.Members[i].Equipment[j]);
                    }
                }

                if (items.size() > 0)
                {
                    party.Members[i].Equipment = items;
                }
                else
                {
                    party.Members[i].Equipment.clear();
                }
            }
        }
    }

    bool IS_CHARACTER(Team::Type team)
    {
        auto result = false;

        if (team != Team::Type::NONE && (int)(team) < Team::TEAMS)
        {
            result = true;
        }

        return result;
    }

    void SET_TEAM(Character::Base &character, Team::Type team)
    {
        character.Team = team;
    }

    void SET_TEAM(Character::Base &character)
    {
        if (character.Type == Character::Type::SAR_JESSICA_DAYNE)
        {
            Engine::SET_TEAM(character, Team::Type::SAR_JESSICA_DAYNE);
        }
        else if (character.Type == Character::Type::LORD_TIQUON)
        {
            Engine::SET_TEAM(character, Team::Type::LORD_TIQUON);
        }
        else if (character.Type == Character::Type::TASHA)
        {
            Engine::SET_TEAM(character, Team::Type::TASHA);
        }
        else if (character.Type == Character::Type::AMELIA_PASS_DAYNE)
        {
            Engine::SET_TEAM(character, Team::Type::AMELIA_PASS_DAYNE);
        }
        else if (character.Type == Character::Type::AKIHIRO_OF_CHALICE)
        {
            Engine::SET_TEAM(character, Team::Type::AKIHIRO_OF_CHALICE);
        }
        else if (character.Type == Character::Type::BRASH)
        {
            Engine::SET_TEAM(character, Team::Type::BRASH);
        }
    }

    int FIRST(Party::Base &party, std::vector<int> list)
    {
        auto result = -1;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0 && Engine::FIND_LIST(list, i) < 0 && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
            {
                result = i;

                break;
            }
        }

        if (result >= 0 && result < party.Members.size())
        {
            party.LastSelected = result;
        }

        return result;
    }

    int FIRST(Party::Base &party)
    {
        auto result = -1;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0 && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED))
            {
                result = i;

                break;
            }
        }

        if (result >= 0 && result < party.Members.size())
        {
            party.LastSelected = result;
        }

        return result;
    }

    int FIRST(Party::Base &party, Team::Type team, std::vector<int> list)
    {
        auto result = -1;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0 && party.Members[i].Team == team && Engine::FIND_LIST(list, i) < 0 && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
            {
                result = i;

                break;
            }
        }

        if (result >= 0 && result < party.Members.size())
        {
            party.LastSelected = result;
        }

        return result;
    }

    int FIRST(Party::Base &party, Team::Type team)
    {
        auto result = -1;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0 && party.Members[i].Team == team && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
            {
                result = i;

                break;
            }
        }

        if (result >= 0 && result < party.Members.size())
        {
            party.LastSelected = result;
        }

        return result;
    }

    int FIRST_CASTER(Party::Base &party, Team::Type team, std::vector<int> list)
    {
        auto result = -1;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0 && party.Members[i].Team == team && party.Members[i].SpellCaster && Engine::FIND_LIST(list, i) < 0 && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
            {
                result = i;

                break;
            }
        }

        if (result >= 0 && result < party.Members.size())
        {
            party.LastSelected = result;
        }

        return result;
    }

    int FIRST_CASTER(Party::Base &party, Team::Type team)
    {
        auto result = -1;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0 && party.Members[i].Team == team && party.Members[i].SpellCaster && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
            {
                result = i;

                break;
            }
        }

        if (result >= 0 && result < party.Members.size())
        {
            party.LastSelected = result;
        }

        return result;
    }

    int FIRST_CASTER(Party::Base &party)
    {
        auto result = -1;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0 && party.Members[i].SpellCaster && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
            {
                result = i;

                break;
            }
        }

        if (result >= 0 && result < party.Members.size())
        {
            party.LastSelected = result;
        }

        return result;
    }

    int FIRST(std::vector<Monster::Base> &monsters)
    {
        auto result = -1;

        for (auto i = 0; i < monsters.size(); i++)
        {
            if (monsters[i].Health > 0)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    int FIRST(std::vector<Monster::Base> &monsters, int round)
    {
        auto result = -1;

        for (auto i = 0; i < monsters.size(); i++)
        {
            if (monsters[i].Health > 0 && round >= monsters[i].Round)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    int MIN(Party::Base &party, Attribute::Type type)
    {
        auto score = 100;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (!Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED))
            {
                score = std::min(score, Engine::SCORE(party.Members[i], type));
            }
        }

        return score;
    }

    int MAX(Party::Base &party, Attribute::Type type)
    {
        auto score = 1;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (!Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED))
            {
                score = std::max(score, Engine::SCORE(party.Members[i], type));
            }
        }

        return score;
    }

    int COUNT(Party::Base &party, Attribute::Type type, int score)
    {
        auto result = 0;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (score == Engine::SCORE(party.Members[i], type) && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
            {
                result += 1;
            }
        }

        return result;
    }

    int FIRST(Party::Base &party, Attribute::Type type, int score)
    {
        auto result = -1;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (score == Engine::SCORE(party.Members[i], type) && !Engine::HAS_STATUS(party.Members[i], Character::Status::CAPTURED) && (!party.InCity || (party.InCity && party.Members[i].IsCivilized)))
            {
                result = i;

                break;
            }
        }

        return result;
    }

    int FIRST(std::vector<Ship::Base> ships)
    {
        auto result = -1;

        for (auto i = 0; i < ships.size(); i++)
        {
            if (ships[i].Health > 0)
            {
                result = i;

                break;
            }
        }

        return result;
    }

    int FIRST(std::vector<Ship::Base> ships, Location::Type location)
    {
        auto result = -1;

        if (location == Location::Type::NONE)
        {
            result = Engine::FIRST(ships);
        }
        else
        {
            for (auto i = 0; i < ships.size(); i++)
            {
                if (ships[i].Health > 0)
                {
                    result = i;

                    break;
                }
            }
        }

        return result;
    }

    bool HAS_UNIT(Party::Base &party, Army::Type unit)
    {
        auto result = Engine::FIND_UNIT(party.Army, unit);

        return (result >= 0 && result < party.Army.size());
    }

    void SET_POSITION(std::vector<Army::Base> &army, Location::BattleField from, Location::BattleField to)
    {
        auto result = Engine::FIND_UNIT(army, from);

        if (result >= 0 && result < army.size())
        {
            army[result].Position = to;
        }
    }

    void CLEAR_POSITIONS(std::vector<Army::Base> &army)
    {
        for (auto i = 0; i < army.size(); i++)
        {
            army[i].Position = Location::BattleField::NONE;
        }
    }

    void REMOVE_ROUTED(Party::Base &party)
    {
        auto remaining_army = std::vector<Army::Base>();

        for (auto i = 0; i < party.Army.size(); i++)
        {
            if (party.Army[i].Morale > 0)
            {
                remaining_army.push_back(party.Army[i]);
            }
        }

        party.Army.clear();

        if (remaining_army.size() > 0)
        {
            party.Army.insert(party.Army.end(), remaining_army.begin(), remaining_army.end());
        }
    }

    int ZONES(std::vector<Army::Base> &army, std::vector<Army::Base> &opposingArmy)
    {
        auto zones = 3;

        auto left_flank1a = Engine::FIND_UNIT(army, Location::BattleField::LEFT_FLANK_FRONT);
        auto left_flank1b = Engine::FIND_UNIT(army, Location::BattleField::LEFT_FLANK_SUPPORT);
        auto left_flank2a = Engine::FIND_UNIT(opposingArmy, Location::BattleField::LEFT_FLANK_FRONT);
        auto left_flank2b = Engine::FIND_UNIT(opposingArmy, Location::BattleField::LEFT_FLANK_SUPPORT);

        auto centre1a = Engine::FIND_UNIT(army, Location::BattleField::CENTER_FRONT);
        auto centre1b = Engine::FIND_UNIT(army, Location::BattleField::CENTER_SUPPORT);
        auto centre2a = Engine::FIND_UNIT(opposingArmy, Location::BattleField::CENTER_FRONT);
        auto centre2b = Engine::FIND_UNIT(opposingArmy, Location::BattleField::CENTER_SUPPORT);

        auto right_flank1a = Engine::FIND_UNIT(army, Location::BattleField::RIGHT_FLANK_FRONT);
        auto right_flank1b = Engine::FIND_UNIT(army, Location::BattleField::RIGHT_FLANK_SUPPORT);
        auto right_flank2a = Engine::FIND_UNIT(opposingArmy, Location::BattleField::RIGHT_FLANK_FRONT);
        auto right_flank2b = Engine::FIND_UNIT(opposingArmy, Location::BattleField::RIGHT_FLANK_SUPPORT);

        if (((left_flank1a >= 0 && left_flank1a < army.size()) || (left_flank1b >= 0 && left_flank1b < army.size())))
        {
            if (((left_flank2a >= 0 && left_flank2a < army.size()) || (left_flank2b >= 0 && left_flank2b < army.size())))
            {
                zones -= 1;
            }
        }
        else
        {
            zones -= 1;
        }

        if (((centre1a >= 0 && centre1a < army.size()) || (centre1b >= 0 && centre1b < army.size())))
        {
            if (((centre2a >= 0 && centre2a < army.size()) || (centre2b >= 0 && centre2b < army.size())))
            {
                zones -= 1;
            }
        }
        else
        {
            zones -= 1;
        }

        if (((right_flank1a >= 0 && right_flank1a < army.size()) || (right_flank1b >= 0 && right_flank1b < army.size())))
        {
            if (((right_flank2a >= 0 && right_flank2a < army.size()) || (right_flank2b >= 0 && right_flank2b < army.size())))
            {
                zones -= 1;
            }
        }
        else
        {
            zones -= 1;
        }

        if (zones < 0)
        {
            zones = 0;
        }

        return zones;
    }

    bool ACTIVE(std::vector<Army::Base> &army, std::vector<Army::Base> &opposingArmy, Location::Zone zone)
    {
        auto army1_active = false;
        auto army2_active = false;

        if (zone == Location::Zone::LEFT_FLANK)
        {
            auto left_flank1a = Engine::FIND_UNIT(army, Location::BattleField::LEFT_FLANK_FRONT);
            auto left_flank1b = Engine::FIND_UNIT(army, Location::BattleField::LEFT_FLANK_SUPPORT);
            auto left_flank2a = Engine::FIND_UNIT(opposingArmy, Location::BattleField::LEFT_FLANK_FRONT);
            auto left_flank2b = Engine::FIND_UNIT(opposingArmy, Location::BattleField::LEFT_FLANK_SUPPORT);

            if (((left_flank1a >= 0 && left_flank1a < army.size()) || (left_flank1b >= 0 && left_flank1b < army.size())))
            {
                army1_active = true;
            }

            if (((left_flank2a >= 0 && left_flank2a < army.size()) || (left_flank2b >= 0 && left_flank2b < army.size())))
            {
                army2_active = true;
            }
        }
        else if (zone == Location::Zone::CENTER)
        {
            auto centre1a = Engine::FIND_UNIT(army, Location::BattleField::CENTER_FRONT);
            auto centre1b = Engine::FIND_UNIT(army, Location::BattleField::CENTER_SUPPORT);
            auto centre2a = Engine::FIND_UNIT(opposingArmy, Location::BattleField::CENTER_FRONT);
            auto centre2b = Engine::FIND_UNIT(opposingArmy, Location::BattleField::CENTER_SUPPORT);

            if (((centre1a >= 0 && centre1a < army.size()) || (centre1b >= 0 && centre1b < army.size())))
            {
                army1_active = true;
            }

            if (((centre2a >= 0 && centre2a < army.size()) || (centre2b >= 0 && centre2b < army.size())))
            {
                army2_active = true;
            }
        }
        else if (zone == Location::Zone::RIGHT_FLANK)
        {
            auto right_flank1a = Engine::FIND_UNIT(army, Location::BattleField::RIGHT_FLANK_FRONT);
            auto right_flank1b = Engine::FIND_UNIT(army, Location::BattleField::RIGHT_FLANK_SUPPORT);
            auto right_flank2a = Engine::FIND_UNIT(opposingArmy, Location::BattleField::RIGHT_FLANK_FRONT);
            auto right_flank2b = Engine::FIND_UNIT(opposingArmy, Location::BattleField::RIGHT_FLANK_SUPPORT);

            if (((right_flank1a >= 0 && right_flank1a < army.size()) || (right_flank1b >= 0 && right_flank1b < army.size())))
            {
                army1_active = true;
            }

            if (((right_flank2a >= 0 && right_flank2a < army.size()) || (right_flank2b >= 0 && right_flank2b < army.size())))
            {
                army2_active = true;
            }
        }

        return army1_active && army2_active;
    }

    int GET_STRENGTH(std::vector<Army::Base> &army, Location::Zone zone)
    {
        auto result = 0;

        if (zone == Location::Zone::LEFT_FLANK)
        {
            auto unit = Engine::FIND_UNIT(army, Location::BattleField::LEFT_FLANK_FRONT);

            if (unit >= 0 && unit < army.size())
            {
                result = army[unit].Strength;
            }
        }
        else if (zone == Location::Zone::CENTER)
        {
            auto unit = Engine::FIND_UNIT(army, Location::BattleField::CENTER_FRONT);

            if (unit >= 0 && unit < army.size())
            {
                result = army[unit].Strength;
            }
        }
        else if (zone == Location::Zone::RIGHT_FLANK)
        {
            auto unit = Engine::FIND_UNIT(army, Location::BattleField::RIGHT_FLANK_FRONT);

            if (unit >= 0 && unit < army.size())
            {
                result = army[unit].Strength;
            }
        }

        return result;
    }

    Spells::MassCombat GET_SPELL(std::vector<Engine::BattlefieldSpells> spells, int round)
    {
        auto result = Spells::MassCombat::NONE;

        for (auto i = 0; i < spells.size(); i++)
        {
            if (std::get<1>(spells[i]) == round)
            {
                result = std::get<0>(spells[i]);

                break;
            }
        }

        return result;
    }

    void GAIN_HEALTH(Follower::Base &follower, int health)
    {
        follower.Health += health;

        if (follower.Health < 0)
        {
            follower.Health = 0;
        }
    }

    int COUNT(std::vector<Army::Base> &army)
    {
        auto result = 0;

        for (auto i = 0; i < army.size(); i++)
        {
            if (army[i].Morale > 0)
            {
                result += 1;
            }
        }

        return result;
    }

    int COUNT(std::vector<Army::Base> &army, Location::Type garrison)
    {
        auto result = 0;

        if (garrison == Location::Type::NONE)
        {
            result = Engine::COUNT(army);
        }
        else
        {
            for (auto i = 0; i < army.size(); i++)
            {
                if (army[i].Morale > 0 && army[i].Garrison == garrison)
                {
                    result += 1;
                }
            }
        }

        return result;
    }

    int COUNT(std::vector<Ship::Base> ships)
    {
        auto result = 0;

        for (auto i = 0; i < ships.size(); i++)
        {
            if (ships[i].Health > 0)
            {
                result += 1;
            }
        }

        return result;
    }

    int COUNT(std::vector<Ship::Base> ships, Location::Type location)
    {
        auto result = 0;

        if (location == Location::Type::NONE)
        {
            result = Engine::COUNT(ships);
        }
        else
        {
            for (auto i = 0; i < ships.size(); i++)
            {
                if (ships[i].Health > 0 && ships[i].Location == location)
                {
                    result += 1;
                }
            }
        }

        return result;
    }

    bool SHIP_INTACT(Party::Base &party)
    {
        return (party.CurrentShip >= 0 && party.CurrentShip < party.Fleet.size() && party.Fleet[party.CurrentShip].Health > 0);
    }

    void KILL(Party::Base &party, Character::Type character)
    {
        auto result = Engine::FIND_CHARACTER(party, character);

        if (result >= 0 && result < party.Members.size())
        {
            party.Members[result].Health = 0;
        }
    }

    int FIND_CHARACTER(Party::Base &party, Character::Status status)
    {
        auto result = -1;

        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (Engine::HAS_STATUS(party.Members[i], status))
            {
                result = i;

                break;
            }
        }

        return result;
    }
}
#endif