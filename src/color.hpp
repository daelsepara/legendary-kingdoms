#ifndef __COLOR__HPP__
#define __COLOR__HPP__

#include <SDL.h>
#include "book.hpp"
#include "constants.hpp"

namespace Color
{
    Uint32 HEADER(Book::Type book)
    {
        auto fg = intBR;

        if (book == Book::Type::BOOK0 || book == Book::Type::BOOK1)
        {
            fg = intBR;
        }
        else if (book == Book::Type::BOOK2)
        {
            fg = intGR;
        }
        else
        {
            fg = intBR;
        }

        return fg;
    }
}

#endif
