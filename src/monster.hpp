#ifndef __MONSTER__HPP__
#define __MONSTER__HPP__

#include <vector>
#include <string>

#include "equipment.hpp"

namespace Allies
{
    enum class Type
    {
        NONE = -1,
        SLAVES,
        YU_YUAN
    };
}

namespace Monster
{
    enum class Type
    {
        NORMAL = 0,
        BEETLE_SWARM,
        BRONZEGUARD,
        CURSITE_ASSASSIN,
        EVERCHILD_ASSASSIN,
        FASTILON,
        GOBLIN_ARCHERS,
        IMOPPOSH_THE_MAD,
        JUNGLE,
        MONKEY_WITH_SPELLS,
        NAGA,
        ORC,
        ORC_LEADER,
        PAPER,
        ROCK,
        SALT_DRAGON,
        SKALLOS,
        SKELETON_ARCHERS,
        SNAKEMAN,
        SNAKEMAN_PRIEST,
        SPIDER_WITH_SWARM,
        STONE,
        TOMMUL,
        UNBRAAKI,
        ZEALOT,
        ZEALOT_HEALER,
        ZEALOT_SORCERER
    };

    class Base
    {
    public:
        std::string Name = "";

        int Attack = 0;

        int Difficulty = 0;

        int Defence = 0;

        int Health = 0;

        int MaximumHealth = 0;

        int Auto = 0;

        int Round = 0;

        int Attacks = 1; // default number of attacks

        Monster::Type Type = Monster::Type::NORMAL;

        bool Damaged = false;

        std::vector<Equipment::Base> Loot = {};

        Base()
        {
        }

        Base(const char *name, int attack, int difficulty, int defence, int health, int damage)
        {
            Name = name;

            Attack = attack;

            Difficulty = difficulty;

            Defence = defence;

            Health = health;

            MaximumHealth = health;

            Auto = damage;
        }

        Base(const char *name, int attack, int difficulty, int defence, int health, int damage, std::vector<Equipment::Base> loot)
        {
            Name = name;

            Attack = attack;

            Difficulty = difficulty;

            Defence = defence;

            Health = health;

            MaximumHealth = health;

            Auto = damage;

            Loot = loot;
        }

        Base(const char *name, int attack, int difficulty, int defence, int health, int damage, int round)
        {
            Name = name;

            Attack = attack;

            Difficulty = difficulty;

            Defence = defence;

            Health = health;

            MaximumHealth = health;

            Auto = damage;

            Round = round;
        }

        Base(const char *name, int attack, int difficulty, int defence, int health, int damage, int round, int attacks)
        {
            Name = name;

            Attack = attack;

            Difficulty = difficulty;

            Defence = defence;

            Health = health;

            MaximumHealth = health;

            Auto = damage;

            Round = round;

            Attacks = attacks;
        }

        Base(const char *name, Monster::Type type, int attack, int difficulty, int defence, int health, int damage)
        {
            Name = name;

            Type = type;

            Attack = attack;

            Difficulty = difficulty;

            Defence = defence;

            Health = health;

            MaximumHealth = health;

            Auto = damage;
        }

        Base(const char *name, Monster::Type type, int attack, int difficulty, int defence, int health, int damage, int attacks)
        {
            Name = name;

            Type = type;

            Attack = attack;

            Difficulty = difficulty;

            Defence = defence;

            Health = health;

            MaximumHealth = health;

            Auto = damage;

            Attacks = attacks;
        }
    };
}

#endif
