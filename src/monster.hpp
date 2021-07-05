#ifndef __MONSTER__HPP__
#define __MONSTER__HPP__

#include <vector>
#include <string>

namespace Monster
{
    class Base
    {
    public:
        const char *Name = NULL;

        int Attack = 0;

        int Difficulty = 0;

        int Defence = 0;

        int Health = 0;

        int Auto = 0;

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

            Auto = damage;
        }
    };
}

#endif