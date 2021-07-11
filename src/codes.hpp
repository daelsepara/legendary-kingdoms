#ifndef __CODES__HPP__
#define __CODES__HPP__

#include <map>

#include "book.hpp"

namespace Codes
{
    // Invisible (Party level) codes to handle situations not explicitly stated in the rules section. Can also be used to handle temporary effects
    enum class Type
    {
        FIRST_TIME_SALTAD,
        FOUGHT_IN_ARENA,
        SILVER_IDOL,
        BRIBE_GOLD_BULLION1,
        BRIBE_GOLD_BULLION2,
        BRIBE_GOLD_BULLION3
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
}

#endif