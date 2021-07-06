#ifndef __CODES__HPP__
#define __CODES__HPP__

#include "book.hpp"

namespace Codes
{
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