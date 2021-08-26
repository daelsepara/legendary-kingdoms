#ifndef __CHARACTER__HPP__
#define __CHARACTER__HPP__

#include <vector>
#include <string>

#include "attribute.hpp"
#include "book.hpp"
#include "codes.hpp"
#include "equipment.hpp"
#include "location.hpp"
#include "spells.hpp"

#include "army.hpp"
#include "ship.hpp"

#include <map>

namespace Team
{
    const int TEAMS = 100;

    enum class Type
    {
        NONE = 0,
        SAR_JESSICA_DAYNE,
        LORD_TIQUON,
        TASHA,
        AMELIA_PASS_DAYNE,
        AKIHIRO_OF_CHALICE,
        BRASH,
        SKULLCRACKER,
        SOLO = Team::TEAMS,
        SPELLCASTERS,
        FIGHTERS,
        WEAPONS,
        DISTRACTION,
        MERCENARY,
        CHASE,
        MAGICAL_DOOR,
        WALL_CLIMBING,
        ZIGGURAT,
        ASSASSINATION_DESCANTOS,
        EVERCHILD_SECURITY,
        SHADOW_ROOM
    };

    std::map<Team::Type, const char *> Descriptions = {
        {Team::Type::NONE, "None"},
        {Team::Type::SAR_JESSICA_DAYNE, "Sar Jessica Dayne"},
        {Team::Type::LORD_TIQUON, "Lord Ti'quon"},
        {Team::Type::TASHA, "Tasha"},
        {Team::Type::AMELIA_PASS_DAYNE, "Amelia Pass-Dayne"},
        {Team::Type::AKIHIRO_OF_CHALICE, "Akihiro of Chalice"},
        {Team::Type::BRASH, "Brash"},
        {Team::Type::SKULLCRACKER, "Skullcracker"},
        {Team::Type::SOLO, "Solo"},
        {Team::Type::SPELLCASTERS, "Spellcasers"},
        {Team::Type::FIGHTERS, "Fighters"},
        {Team::Type::WEAPONS, "Weapons"},
        {Team::Type::DISTRACTION, "Distraction"},
        {Team::Type::MERCENARY, "Mercenary"},
        {Team::Type::CHASE, "Chase"},
        {Team::Type::MAGICAL_DOOR, "Magical door"},
        {Team::Type::WALL_CLIMBING, "Wall-climbing"},
        {Team::Type::ZIGGURAT, "Wall-climbing"},
        {Team::Type::ASSASSINATION_DESCANTOS, "Assassination of Descantos"},
        {Team::Type::EVERCHILD_SECURITY, "Everchild's Personal Security"},
        {Team::Type::SHADOW_ROOM, "Shadow Room"}};
}

namespace Follower
{
    enum class Type
    {
        NONE = -1,
        MORDAIN_SKELETONS
    };

    class Base
    {
    public:
        std::string Name = "";
        Follower::Type Type = Follower::Type::NONE;
        int Health = 0;

        Base()
        {
        }

        Base(const char *name, Follower::Type type, int health)
        {
            Name = name;

            Type = type;

            Health = health;
        }
    };
}

namespace Character
{
    // Handle unique characters
    enum class Type
    {
        NONE = 0,
        SAR_JESSICA_DAYNE,
        LORD_TIQUON,
        TASHA,
        AMELIA_PASS_DAYNE,
        AKIHIRO_OF_CHALICE,
        BRASH,
        SKULLCRACKER
    };

    enum class Status
    {
        ARMOUR3,
        RITUAL_SCARRING_CURSUS,
        ENRAGED,
        FRONT,
        BACK,
        POTION_OF_INVULNERABILITY,
        CAPTURED,
        ENLIGHTENED,
        EXTRA_MAGIC_ROUND0,
        UNLIMITED_MAGIC_ROUND0,
        FOUND_COGWHEEL,
        ATTACK2_ENEMY0_ROUND0,
        UNARMED_COMBAT,
        LOST_FINGERNAILS,
        STUNNED,
        STUNNED_NEXT_ROUND,
        LOST_TONGUE,
        USED_MAGIC_INARENA,
        ENCHANTED_CURSED,
        SINKING_IN_QUICKSAND,
        FALLEN
    };

    typedef std::pair<Character::Type, Character::Type> Romance;

    std::map<Character::Status, const char *> StatusDescriptions = {
        {Character::Status::ARMOUR3, "+3 Armour"},
        {Character::Status::RITUAL_SCARRING_CURSUS, "Ritual Scarring (Cursus)"},
        {Character::Status::ENRAGED, "Enraged"},
        {Character::Status::FRONT, "in Front"},
        {Character::Status::BACK, "at the Back"},
        {Character::Status::POTION_OF_INVULNERABILITY, "Invulnerable"},
        {Character::Status::CAPTURED, "Captured"},
        {Character::Status::ENLIGHTENED, "Enlightened"},
        {Character::Status::EXTRA_MAGIC_ROUND0, "Extra Magic Attack on Round 1"},
        {Character::Status::UNLIMITED_MAGIC_ROUND0, "Unlimited Magic Attacks on Round 1"},
        {Character::Status::FOUND_COGWHEEL, "Found the cogwheel"},
        {Character::Status::ATTACK2_ENEMY0_ROUND0, "Can attack first enemy twice on Round 1"},
        {Character::Status::UNARMED_COMBAT, "Unarmed Combat"},
        {Character::Status::LOST_FINGERNAILS, "Lost fingernails"},
        {Character::Status::STUNNED, "Stunned"},
        {Character::Status::STUNNED_NEXT_ROUND, "Stunned next round"},
        {Character::Status::LOST_TONGUE, "Lost Tongue"},
        {Character::Status::USED_MAGIC_INARENA, "Used magic in Saltdad Arena"},
        {Character::Status::ENCHANTED_CURSED, "Enchanted (Cursed)"},
        {Character::Status::SINKING_IN_QUICKSAND, "Sinking in quicksand"},
        {Character::Status::FALLEN, "Fallen"}};

    class Base
    {
    public:
        std::string Name = "";

        const char *Background = NULL;

        const char *Image = NULL;

        Character::Type Type = Character::Type::NONE;

        std::vector<Attribute::Base> Attributes = {};

        std::vector<Equipment::Base> Equipment = {};

        std::vector<Spells::Base> SpellBook = {};

        // Temporary Status Codes
        std::vector<Character::Status> Status = {};

        // Followers
        std::vector<Follower::Base> Followers = {};

        Team::Type Team = Team::Type::NONE;

        int Health = 0;

        int MaximumHealth = 0;

        int MaximumEquipment = 10;

        int SpellBookLimit = 6;

        bool SpellCaster = false;

        bool IsCivilized = true;

        bool Damaged = false;

        Base()
        {
        }

        Base(const char *name, Character::Type type, const char *image, const char *background, std::vector<Attribute::Base> attributes, int health, bool spellCaster)
        {
            Name = name;

            Type = type;

            Image = image;

            Background = background;

            Attributes = attributes;

            Health = health;

            MaximumHealth = health;

            SpellCaster = spellCaster;
        }

        Base(const char *name, Character::Type type, const char *image, const char *background, std::vector<Attribute::Base> attributes, std::vector<Equipment::Base> equipment, int health, bool spellCaster)
        {
            Name = name;

            Type = type;

            Image = image;

            Background = background;

            Attributes = attributes;

            Equipment = equipment;

            Health = health;

            MaximumHealth = health;

            SpellCaster = spellCaster;
        }

        Base(const char *name, Character::Type type, const char *background, std::vector<Attribute::Base> attributes, std::vector<Equipment::Base> equipment, int health, bool spellCaster, bool isCivilized)
        {
            Name = name;

            Type = type;

            Background = background;

            Attributes = attributes;

            Equipment = equipment;

            Health = health;

            MaximumHealth = health;

            SpellCaster = spellCaster;

            IsCivilized = isCivilized;
        }

        Base(const char *name, Character::Type type, const char *image, const char *background, std::vector<Attribute::Base> attributes, std::vector<Equipment::Base> equipment, int health, bool spellCaster, bool isCivilized)
        {
            Name = name;

            Type = type;

            Image = image;

            Background = background;

            Attributes = attributes;

            Equipment = equipment;

            Health = health;

            MaximumHealth = health;

            SpellCaster = spellCaster;

            IsCivilized = isCivilized;
        }
    };

    auto SAR_JESSICA_DAYNE = Character::Base("Sar Jessica Dayne", Character::Type::SAR_JESSICA_DAYNE, "images/characters/sar-jessica-dayne.png", "Sar Jessica Dayne is a first-born knight of Royce. Her father, Baron Baldwin Dayne, is an ambitious contender for the rulership of Longport Bay, an important county. She is a powerful warrior, of impeccable noble blood, but she has received little love or affection since the death of her mother when Jessica was a girl. She is due to inherit the barony in preference to her younger brother, Ridley, but has clashed with her father in recent years, putting her inheritance in doubt. Sar Jessica hopes to become a rich and important woman in her own right before confronting her father at Fosterly Castle.", {Attribute::Base(Attribute::Type::FIGHTING, 5), Attribute::Base(Attribute::Type::STEALTH, 1), Attribute::Base(Attribute::Type::LORE, 3), Attribute::Base(Attribute::Type::SURVIVAL, 2), Attribute::Base(Attribute::Type::CHARISMA, 3)}, {}, 8, false);
    auto LORD_TIQUON = Character::Base("Lord Ti'quon", Character::Type::LORD_TIQUON, "images/characters/lord-tiquon.png", "Lord Ti'quon is a classically trained sorcerer from Drakehallow. Thanks to his family's wealth he has been able to learn his craft in the halls of Animus Mast, the premier academy for mages in the world. His knowledge is vast, learnt from library stacks and obscure scrolls, and his ambition is as great as his knowledge. Though at the beginning of his journey in the arts of magic, Ti'Quon shows great promise. He is handicapped, somewhat, by a lack of empathy, although his rich and regal voice make up for his curtness. If he can find additional sums for more tutelage, he desires to return to Animus Mast to unlock yet greater secrets of sorcery.", {Attribute::Base(Attribute::Type::FIGHTING, 1), Attribute::Base(Attribute::Type::STEALTH, 2), Attribute::Base(Attribute::Type::LORE, 5), Attribute::Base(Attribute::Type::SURVIVAL, 1), Attribute::Base(Attribute::Type::CHARISMA, 2)}, {}, 6, true);
    auto TASHA = Character::Base("Tasha", Character::Type::TASHA, "images/characters/tasha.png", "Tasha is a buccaneer from the Splintered Isles and is little more than an outright thief. She is born to the sails, and her family have lived on ships and in coastal taverns for generations. Tasha has stolen and squandered great fortunes in her time. Her extravagance and her love of a pretty face has meant she has seldom held onto money for long. Most recently she was ripped off by her lover, Katherine of Middenmarch, a country girl who was apprenticed to Tasha, before betraying her mistress and selling her off to slavers. Getting even with the bumptious pirate is foremost on Tasha's mind.", {Attribute::Base(Attribute::Type::FIGHTING, 3), Attribute::Base(Attribute::Type::STEALTH, 5), Attribute::Base(Attribute::Type::LORE, 1), Attribute::Base(Attribute::Type::SURVIVAL, 3), Attribute::Base(Attribute::Type::CHARISMA, 3)}, {}, 8, false);
    auto AMELIA_PASS_DAYNE = Character::Base("Amelia Pass-Dayne", Character::Type::AMELIA_PASS_DAYNE, "images/characters/amelia-pass-dayne.png", "Amelia is the half-sister of Jessica and grew up with her in Fosterly Castle. Her mother was a forester who took Amelia away from the castle when it became obvious that Baron Baldwin, her father, would take no interest in the girl. Since maturity Amelia has lived in the wild forests of the Savage Lands, learning earth magic from the druids of that isolated place. Though slender, she has become a rugged survivor, who can live off the land and fight off wild beasts. Her magic is instinctive, and much mocked by more scholarly wizards, but it is no less potent for all that. Amelia dreams of becoming a full-circle druid, but knows that she must learn much before returning home to the Savage Lands.", {Attribute::Base(Attribute::Type::FIGHTING, 3), Attribute::Base(Attribute::Type::STEALTH, 2), Attribute::Base(Attribute::Type::LORE, 2), Attribute::Base(Attribute::Type::SURVIVAL, 3), Attribute::Base(Attribute::Type::CHARISMA, 1)}, {}, 6, true);
    auto AKIHIRO_OF_CHALICE = Character::Base("Akihiro of Chalice", Character::Type::AKIHIRO_OF_CHALICE, "images/characters/akihiro-of-chalice.png", "Akihiro is a brooding young man from the Valley of Bones, who was banished from his city when he failed in his training to become a Kensai, a sword-saint of Chalice. A superb swordsman, Akihiro is his own worst enemy. He tortures himself over his failure to balance his mind and body. Since that time Akihiro has become an adept survivor, able to find water in the driest desert, and food in an empty wilderness. Though he tells himself he has given up, he longs in his heart to return to Chalice to attempt the tests again. Perhaps it is time for Akihiro to emerge from the wilderness and restore his own and his family's honour?", {Attribute::Base(Attribute::Type::FIGHTING, 4), Attribute::Base(Attribute::Type::STEALTH, 3), Attribute::Base(Attribute::Type::LORE, 2), Attribute::Base(Attribute::Type::SURVIVAL, 5), Attribute::Base(Attribute::Type::CHARISMA, 1)}, {}, 8, false);
    auto BRASH = Character::Base("Brash", Character::Type::BRASH, "images/characters/brash.png", "Brash was born in Royce, in the duchy of Pendrilor. He was a pot washer in Vasthall, the stately home of Duchess Elenor Mauntell, but unwisely engaged in a love triangle with her and King Frances Goldwell. Fleeing before he could be hung, Brash has since lived on his wits, travelling the world to escape the King's agents. He can talk his way out of almost anything, and his ability to escape danger is second-to-none. One day, Brash will need to sort out the mess he left behind in Vasthall, but not until he has gained a little worldly experience.", {Attribute::Base(Attribute::Type::FIGHTING, 2), Attribute::Base(Attribute::Type::STEALTH, 4), Attribute::Base(Attribute::Type::LORE, 3), Attribute::Base(Attribute::Type::SURVIVAL, 1), Attribute::Base(Attribute::Type::CHARISMA, 5)}, {}, 8, false);
    auto SKULLCRACKER = Character::Base("Skullcracker", Character::Type::SKULLCRACKER, "images/characters/skullcracker.png", "He is lazy and will not carry any equipment except his CLUB. Skullcracker is never disarmed. He won't enter a city: Skullcracker will wait outside. Ogres aren't very popular in civilised places. Skullcracker will recover Health at the same rate as other party members when you pay to stay at an inn. He won't board a ship. If you insist, he will leave the party. His contract with you ends when you travel to another land. At this point, Skullcracker will leave and wander into the desert.", {Attribute::Base(Attribute::Type::FIGHTING, 8), Attribute::Base(Attribute::Type::STEALTH, 1), Attribute::Base(Attribute::Type::LORE, 1), Attribute::Base(Attribute::Type::SURVIVAL, 3), Attribute::Base(Attribute::Type::CHARISMA, 2)}, {Equipment::CLUB}, 12, false, false);

    std::vector<Character::Base> BOOK1 = {Character::SAR_JESSICA_DAYNE, Character::LORD_TIQUON, Character::TASHA, Character::AMELIA_PASS_DAYNE, Character::AKIHIRO_OF_CHALICE, Character::BRASH};

    std::vector<Character::Base> ALL = {Character::SAR_JESSICA_DAYNE, Character::LORD_TIQUON, Character::TASHA, Character::AMELIA_PASS_DAYNE, Character::AKIHIRO_OF_CHALICE, Character::BRASH};

    auto Nobody = Character::Base();
}

namespace Party
{
    class Base
    {
    public:
        std::vector<Character::Base> Members = {};

        std::vector<Character::Type> Dead = {};

        std::vector<Codes::Base> Codes = {};

        std::vector<Codes::Type> InvisibleCodes = {};

        std::vector<Equipment::Base> Vault = {};

        int VaultMoney = 0;

        int Money = 0;

        int Limit = 4;

        Location::Type Location = Location::Type::NONE;

        bool InCity = false;

        std::vector<Ship::Base> Fleet = {};

        int CurrentShip = -1;

        std::vector<Army::Base> Army = {};

        // For tracking recent success (team checks, individual checks)
        int RecentSuccesses = 0;

        // Last Selected Party Member
        int LastSelected = -1;

        // Last Selected Party Members for skill checks
        std::vector<int> LastSelection = {};

        // Order
        std::vector<Character::Type> Order = {};

        // Romance histogram
        std::map<Character::Romance, int> Hearts = {};

        // Only useful for keeping track of characters gone solo.
        int CurrentCharacter = -1;

        // Book reference
        Book::Type Book = Book::Type::BOOK1;

        int StoryID = 0;

        Base()
        {
        }

        Base(std::vector<Character::Base> party)
        {
            Members = party;
        }
    };
}
#endif
