#ifndef __CODES__HPP__
#define __CODES__HPP__

#include <map>

#include "book.hpp"

namespace Codes
{
    // Invisible (Party level) codes to handle situations not explicitly stated in the rules section. Can also be used to handle temporary effects
    enum class Type
    {
        MAGIC_VAULT,
        NO_VAULT_ACCESS,
        STARTED_IN_BOOK1,
        STARTED_IN_BOOK2,
        STARTED_IN_BOOK3,
        STARTED_IN_BOOK4,
        STARTED_IN_BOOK5,
        STARTED_IN_BOOK6,
        FIRSTTIME_SOLD_SALTDADARENA,
        FOUGHT_IN_ARENA,
        BRIBE_GOLD_BULLION1,
        BRIBE_GOLD_BULLION2,
        BRIBE_GOLD_BULLION3,
        LAST_IN_COMBAT,
        WEAPON_AND_DISTRACTION,
        QUEEN_IN_SALTDAD,
        FIRST_TIME_SCROLL_OF_RAGE,
        SPEAK_BANDO,
        SPEAK_ORCISH,
        SPEAK_DREAR,
        WALL_CLIMBING_AND_DOOR,
        REPAIR_CALENDAR_KALU,
        NO_COMBAT_SPELLS,
        EVERCHILD_POISONED,
        DAZING_LIGHTS,
        BANDITS_GET3,
        LOST_SCHOLAR1,
        LOST_SCHOLAR2,
        LOST_SCHOLAR3,
        NO_BLESSINGS_FOREVER,
        TAUGHT_KOPU_BANDO,
        TAUGHT_KOPU_DREAR,
        TAUGHT_KOPU_ORCISH,
        ENEMY1_FREEATTACK_ROUND0,
        CHARISMA_SUCCESS_CHALICE,
        SLAVES_DAMAGE_FOR2,
        CAST_ORB,
        CAST_MAELSTROM,
        RECHARGE_COSTS_HALF,
        ENEMY_DAZING_LIGHTS,
        CANNOT_USE_SHIPWEAPONS
    };

    std::map<Book::Type, const char *> Prefix = {
        {Book::Type::BOOK1, "A"},
        {Book::Type::BOOK2, "B"},
        {Book::Type::BOOK3, "C"},
        {Book::Type::BOOK4, "D"},
        {Book::Type::BOOK5, "E"},
        {Book::Type::BOOK6, "F"}};

    class Base
    {
    public:
        Book::Type Type = Book::Type::BOOK1;

        int Code = 0;

        Base(Book::Type type, int code)
        {
            Type = type;
            Code = code;
        }
    };

    Codes::Base A(int code)
    {
        return Codes::Base(Book::Type::BOOK1, code);
    }

    Codes::Base B(int code)
    {
        return Codes::Base(Book::Type::BOOK2, code);
    }

    Codes::Base C(int code)
    {
        return Codes::Base(Book::Type::BOOK3, code);
    }

    Codes::Base D(int code)
    {
        return Codes::Base(Book::Type::BOOK4, code);
    }

    Codes::Base E(int code)
    {
        return Codes::Base(Book::Type::BOOK5, code);
    }

    Codes::Base F(int code)
    {
        return Codes::Base(Book::Type::BOOK6, code);
    }
}

#endif