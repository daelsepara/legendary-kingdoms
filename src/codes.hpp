#ifndef __CODES__HPP__
#define __CODES__HPP__

#include "book.hpp"

namespace Codes
{
    // Invisible codes to handle situations not explicitly stated in the rules section. Can also be used to handle temporary effects
    enum class Type
    {
        FIRST_TIME_SALTAD,
        FOUGHT_IN_ARENA
    };

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