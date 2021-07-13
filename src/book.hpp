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
        {Book::Type::NONE, "LEGENDARY KINGDOMS"},
        {Book::Type::BOOK1, "THE VALLEY OF BONES"},
        {Book::Type::BOOK2, "CROWN AND TOWER"},
        {Book::Type::BOOK3, "PIRATES OF THE SPLINTERED ISLES"},
        {Book::Type::BOOK4, "THE GILDED THRONE"},
        {Book::Type::BOOK5, "THE SAVAGE LANDS"},
        {Book::Type::BOOK6, "DRAKEHALLOW"}};
}

#endif