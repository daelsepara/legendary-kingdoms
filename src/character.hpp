#ifndef __CHARACTER__HPP__
#define __CHARACTER__HPP__

#include <vector>
#include <string>

#include "attribute.hpp"
#include "book.hpp"
#include "codes.hpp"
#include "equipment.hpp"
#include "spells.hpp"

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

        std::vector<Spells::Base> SpellBook = std::vector<Spells::Base>();

        int Health = 0;

        int MaximumHealth = 0;

        int MaximumEquipment = 8;

        int SpellBookLimit = 6;

        bool SpellCaster = false;

        Base()
        {
        }

        Base(const char *name, const char *background, std::vector<Attribute::Base> attributes, int health, bool spellCaster)
        {
            Name = name;

            Background = background;

            Attributes = attributes;

            Health = health;

            MaximumHealth = health;

            SpellCaster = spellCaster;
        }
    };

    auto SAR_JESSICA_DAYNE = Character::Base("Sar Jessica Dayne", "Sar Jessica Dayne is a first-born knight of Royce. Her father, Baron Baldwin Dayne, is an ambitious contender for the rulership of Longport Bay, an important county. She is a powerful warrior, of impeccable noble blood, but she has received little love or affection since the death of her mother when Jessica was a girl. She is due to inherit the barony in preference to her younger brother, Ridley, but has clashed with her father in recent years, putting her inheritance in doubt. Sar Jessica hopes to become a rich and important woman in her own right before confronting her father at Fosterly Castle.", {Attribute::Base(Attribute::Type::FIGTING, 5), Attribute::Base(Attribute::Type::STEALTH, 1), Attribute::Base(Attribute::Type::LORE, 3), Attribute::Base(Attribute::Type::SURVIVAL, 2), Attribute::Base(Attribute::Type::CHARISMA, 3)}, 8, false);
    auto LORD_TIQUON = Character::Base("Lord Ti'quon", "Lord Ti'quon is a classically trained sorcerer from Drakehallow. Thanks to his family's wealth he has been able to learn his craft in the halls of Animus Mast, the premier academy for mages in the world. His knowledge is vast, learnt from library stacks and obscure scrolls, and his ambition is as great as his knowledge. Though at the beginning of his journey in the arts of magic, Ti'Quon shows great promise. He is handicapped, somewhat, by a lack of empathy, although his rich and regal voice make up for his curtness. If he can find additional sums for more tutelage, he desires to return to Animus Mast to unlock yet greater secrets of sorcery.", {Attribute::Base(Attribute::Type::FIGTING, 1), Attribute::Base(Attribute::Type::STEALTH, 2), Attribute::Base(Attribute::Type::LORE, 5), Attribute::Base(Attribute::Type::SURVIVAL, 1), Attribute::Base(Attribute::Type::CHARISMA, 2)}, 6, true);
    auto TASHA = Character::Base("Tasha", "Tasha is a buccaneer from the Splintered Isles and is little more than an outright thief. She is born to the sails, and her family have lived on ships and in coastal taverns for generations. Tasha has stolen and squandered great fortunes in her time. Her extravagance and her love of a pretty face has meant she has seldom held onto money for long. Most recently she was ripped off by her lover, Katherine of Middenmarch, a country girl who was apprenticed to Tasha, before betraying her mistress and selling her off to slavers. Getting even with the bumptious pirate is foremost on Tasha's mind.", {Attribute::Base(Attribute::Type::FIGTING, 3), Attribute::Base(Attribute::Type::STEALTH, 5), Attribute::Base(Attribute::Type::LORE, 1), Attribute::Base(Attribute::Type::SURVIVAL, 3), Attribute::Base(Attribute::Type::CHARISMA, 3)}, 8, false);
    auto AMELIA_PASS_DAYNE = Character::Base("Amelia Pass-Dayne", "Amelia is the half-sister of Jessica and grew up with her in Fosterly Castle. Her mother was a forester who took Amelia away from the castle when it became obvious that Baron Baldwin, her father, would take no interest in the girl. Since maturity Amelia has lived in the wild forests of the Savage Lands, learning earth magic from the druids of that isolated place. Though slender, she has become a rugged survivor, who can live off the land and fight off wild beasts. Her magic is instinctive, and much mocked by more scholarly wizards, but it is no less potent for all that. Amelia dreams of becoming a full-circle druid, but knows that she must learn much before returning home to the Savage Lands.", {Attribute::Base(Attribute::Type::FIGTING, 3), Attribute::Base(Attribute::Type::STEALTH, 2), Attribute::Base(Attribute::Type::LORE, 2), Attribute::Base(Attribute::Type::SURVIVAL, 3), Attribute::Base(Attribute::Type::CHARISMA, 1)}, 6, true);
    auto AKIHIRO_OF_CHALICE = Character::Base("Akihiro of Chalice", "Akihiro is a brooding young man from the Valley of Bones, who was banished from his city when he failed in his training to become a Kensai, a sword-saint of Chalice. A superb swordsman, Akihiro is his own worst enemy. He tortures himself over his failure to balance his mind and body. Since that time Akihiro has become an adept survivor, able to find water in the driest desert, and food in an empty wilderness. Though he tells himself he has given up, he longs in his heart to return to Chalice to attempt the tests again. Perhaps it is time for Akihiro to emerge from the wilderness and restore his own and his family's honour?", {Attribute::Base(Attribute::Type::FIGTING, 4), Attribute::Base(Attribute::Type::STEALTH, 3), Attribute::Base(Attribute::Type::LORE, 2), Attribute::Base(Attribute::Type::SURVIVAL, 5), Attribute::Base(Attribute::Type::CHARISMA, 1)}, 8, false);
    auto BRASH = Character::Base("Brash", "Brash was born in Royce, in the duchy of Pendrilor. He was a pot washer in Vasthall, the stately home of Duchess Elenor Mauntell, but unwisely engaged in a love triangle with her and King Frances Goldwell. Fleeing before he could be hung, Brash has since lived on his wits, travelling the world to escape the King’s agents. He can talk his way out of almost anything, and his ability to escape danger is second-to-none. One day, Brash will need to sort out the mess he left behind in Vasthall, but not until he has gained a little worldly experience.", {Attribute::Base(Attribute::Type::FIGTING, 2), Attribute::Base(Attribute::Type::STEALTH, 4), Attribute::Base(Attribute::Type::LORE, 3), Attribute::Base(Attribute::Type::SURVIVAL, 1), Attribute::Base(Attribute::Type::CHARISMA, 5)}, 8, false);

    std::vector<Character::Base> BOOK1 = {Character::SAR_JESSICA_DAYNE, Character::LORD_TIQUON, Character::TASHA, Character::AMELIA_PASS_DAYNE, Character::AKIHIRO_OF_CHALICE, Character::BRASH};
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