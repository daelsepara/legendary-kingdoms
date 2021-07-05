#ifndef __CHARACTER__HPP__
#define __CHARACTER__HPP__

#include <vector>
#include <string>

#include "attribute.hpp";
#include "book.hpp";
#include "codes.hpp";
#include "equipment.hpp";
#include "spells.hpp";

namespace Character
{
    class Base
    {
    public:
        const char *Name = NULL;

        const char *Background = NULL;

        const char *Image = NULL;

        std::vector<Attribute::Base> Attributes = std::vector<Attribute::Base>();

        std::vector<Equipment::Base> Equipment = std::vector<Equipment::Base>();

        std::vector<Spells::Base*> SpellBook = std::vector<Spells::Base*>();

        int Health = 0;

        int MaximumHealth = 0;

        int MaximumEquipment = 8;

        int SpellBookLimit = 6;

        bool SpellCaster = false;

        Base()
        {
            
        }

        Base(const char *name, const char *background)
        {
            Name = name;

            Background = background;
        }
    };
}

namespace Party
{
    class Base
    {
    public:
        std::vector<Character::Base> Party = std::vector<Character::Base>();

        std::vector<Codes::Base> Codes = std::vector<Codes::Base>();

        Book::Type Book = Book::Type::BOOK1;

        int Money = 0;

        int Limit = 4;

        int StoryID = 0;

        Base()
        {
        }

        Base(std::vector<Character::Base> party)
        {
            Party = party;
        }
    };
}
#endif