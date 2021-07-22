#ifndef __MONSTER__HPP__
#define __MONSTER__HPP__

#include <vector>
#include <string>

namespace Monster
{
    enum class Type
    {
        NORMAL = 0,
        SKALLOS,
        SNAKEMAN_PRIEST,
        ZEALOT,
        ZEALOT_HEALER,
        PAPER
    };

    class Base
    {
    public:
        const char *Name = NULL;

        int Attack = 0;

        int Difficulty = 0;

        int Defence = 0;

        int Health = 0;

        int MaximumHealth = 0;

        int Auto = 0;

        int Round = 0;

        Monster::Type Type = Monster::Type::NORMAL;

        bool Damaged = false;

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
    };
}

#endif