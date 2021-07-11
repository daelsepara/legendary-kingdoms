#ifndef __BOOK__HPP__
#define __BOOK__HPP__

#include <map>

namespace Book
{
    enum class Type
    {
        NONE = 0,
        BOOK1,
        BOOK2,
        BOOK3,
        BOOK4,
        BOOK5,
        BOOK6
    };

    std::map<Book::Type, const char *> Title = {
        {Book::Type::NONE, "Legendary Kingdoms"},
        {Book::Type::BOOK1, "The Valley of Bones"},
        {Book::Type::BOOK2, "Crown and Tower"},
        {Book::Type::BOOK3, "Pirates of the Splintered Isles"},
        {Book::Type::BOOK4, "The Gilded Throne"},
        {Book::Type::BOOK5, "The Savage Lands"},
        {Book::Type::BOOK6, "Drakehallow"}};
}

#endif