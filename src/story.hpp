#ifndef __STORY__HPP__
#define __STORY__HPP__

#include <tuple>
#include <vector>

#include "constants.hpp"
#include "controls.hpp"
#include "codes.hpp"
#include "equipment.hpp"
#include "attribute.hpp"
#include "character.hpp"
#include "monster.hpp"
#include "engine.hpp"

namespace Choice
{
    enum class Type
    {
        NORMAL = 0, // No requirements
        EQUIPMENT,
        ANY_EQUIPMENT,
        GET_EQUIPMENT,
        LOSE_EQUIPMENT,
        MONEY,
        GAIN_MONEY,
        LOSE_MONEY,
        LIFE,
        LOSE_LIFE,
        ATTRIBUTES,
        TEAM_ATTRIBUTES,
        LOSE_ATTRIBUTES,
        LOSE_ALL,
        CODES,
        GET_CODES,
        LOSE_CODES,
        BRIBE,
        PAY_WITH,
        SELL,
        BARTER,
        SHIP,
        ADD_MAX_HEALTH,
        BRIBE_CODEWORD,
        GAIN_ATTRIBUTE_SCORE
    };

    class Base
    {
    public:
        const char *Text = NULL;

        Choice::Type Type = Choice::Type::NORMAL;

        Team::Type Team = Team::Type::NONE;

        std::vector<Attribute::Type> Attributes = std::vector<Attribute::Type>();

        std::vector<Equipment::Base> Equipment = std::vector<Equipment::Base>();

        std::vector<Codes::Base> Codes = std::vector<Codes::Base>();

        std::vector<Codes::Type> InvisibleCodes = std::vector<Codes::Type>();

        int Value = 0;

        int Difficulty = 0;

        int Success = 0;

        Engine::Destination Destination = {Book::Type::NONE, -1};

        Engine::Destination DestinationFailed = {Book::Type::NONE, -1};

        Base(const char *text, Engine::Destination destination)
        {
            Text = text;

            Type = Choice::Type::NORMAL;

            Destination = destination;
        }

        Base(const char *text, Engine::Destination destination, Choice::Type type)
        {
            Text = text;

            Type = type;

            Destination = destination;
        }

        Base(const char *text, Engine::Destination destination, std::vector<Equipment::Base> equipment)
        {
            Text = text;

            Type = Choice::Type::EQUIPMENT;

            Equipment = equipment;

            Destination = destination;
        }

        Base(const char *text, Engine::Destination destination, Choice::Type type, std::vector<Equipment::Base> equipment)
        {
            Text = text;

            Type = type;

            Equipment = equipment;

            Destination = destination;
        }

        Base(const char *text, Engine::Destination destination, Choice::Type type, std::vector<Equipment::Base> equipment, int value)
        {
            Text = text;

            Type = type;

            Equipment = equipment;

            Value = value;

            Destination = destination;
        }

        Base(const char *text, Engine::Destination destination, Engine::Destination destinationFailed, Team::Type team, std::vector<Attribute::Type> attributes, int difficulty, int success)
        {
            Type = Choice::Type::ATTRIBUTES;

            Text = text;

            Team = team;

            Attributes = attributes;

            Difficulty = difficulty;

            Success = success;

            Destination = destination;

            DestinationFailed = destinationFailed;
        }

        Base(const char *text, Engine::Destination destination, Engine::Destination destinationFailed, std::vector<Attribute::Type> attributes, int difficulty, int success)
        {
            Type = Choice::Type::ATTRIBUTES;

            Text = text;

            Attributes = attributes;

            Difficulty = difficulty;

            Success = success;

            Destination = destination;

            DestinationFailed = destinationFailed;
        }

        Base(const char *text, Engine::Destination destination, Engine::Destination destinationFailed, Choice::Type type, std::vector<Attribute::Type> attributes, int difficulty, int success)
        {
            Text = text;

            Type = type;

            Attributes = attributes;

            Difficulty = difficulty;

            Success = success;

            Destination = destination;

            DestinationFailed = destinationFailed;
        }

        Base(const char *text, Engine::Destination destination, Engine::Destination destinationFailed, Team::Type team, Choice::Type type, std::vector<Attribute::Type> attributes, int difficulty, int success)
        {
            Text = text;

            Type = type;

            Team = team;

            Attributes = attributes;

            Difficulty = difficulty;

            Success = success;

            Destination = destination;

            DestinationFailed = destinationFailed;
        }

        Base(const char *text, Engine::Destination destination, std::vector<Codes::Base> codes)
        {
            Text = text;

            Type = Choice::Type::CODES;

            Codes = codes;

            Destination = destination;
        }

        Base(const char *text, Engine::Destination destination, Choice::Type type, std::vector<Codes::Base> codes)
        {
            Text = text;

            Type = type;

            Codes = codes;

            Destination = destination;
        }

        Base(const char *text, Engine::Destination destination, Choice::Type type, int value)
        {
            Text = text;

            Type = type;

            Value = value;

            Destination = destination;
        }

        Base(const char *text, Engine::Destination destination, Choice::Type type, std::vector<Equipment::Base> equipment, std::vector<Codes::Type> codes, int value)
        {
            Text = text;

            Type = type;

            Equipment = equipment;

            InvisibleCodes = codes;

            Value = value;

            Destination = destination;
        }

        Base(const char *text, Engine::Destination destination, Choice::Type type, std::vector<Attribute::Type> attributes, int value)
        {
            Text = text;

            Type = type;

            Attributes = attributes;

            Value = value;

            Destination = destination;
        }
    };
} // namespace Choice

namespace Story
{
    enum class Type
    {
        NORMAL = 0,
        DOOM
    };

    enum class Controls
    {
        NONE = 0,
        STANDARD,
        SHOP,
        SELL,
        BUY_AND_SELL,
        BARTER,
        BARTER_AND_SHOP,
        HARBOUR
    };

    class Base
    {
    public:
        Book::Type BookID = Book::Type::NONE;

        int ID = -1;

        const char *Text = NULL;

        const char *Title = NULL;

        const char *Bye = NULL;

        const char *Image = NULL;

        Story::Type Type = Story::Type::NORMAL;

        Story::Controls Controls = Story::Controls::NONE;

        std::vector<Choice::Base> Choices = {};
        std::vector<Engine::EquipmentPrice> Shop = {};
        std::vector<Engine::EquipmentPrice> Sell = {};
        std::vector<Engine::BarterExchanges> Barter = {};

        // Player selects items to take up to a certain limit
        std::vector<Equipment::Base> Take = {};

        // Player selects spells to learn
        std::vector<Spells::Base> Spells = {};

        // Player selects units to add to army
        std::vector<Army::Base> Army = {};

        // Player selects items to lose
        std::vector<Equipment::Base> ToLose = {};

        // Combat encounters
        std::vector<Monster::Base> Monsters = {};

        int FleeRound = -1;

        bool CanFlee = false;

        // Limits
        int Limit = 0;

        int LimitSkills = 0;

        // Harbour
        std::vector<Engine::ShipPrices> Ships = {};
        std::vector<Engine::CargoPrices> Cargo = {};
        int ShipRepairPrice = -1;

        // Handle background events
        virtual Engine::Destination Background(Party::Base &party) { return {Book::Type::NONE, -1}; };

        // Handle events before story branches
        virtual void Event(Party::Base &party){};

        // Jump to next book/section
        virtual Engine::Destination Continue(Party::Base &party) { return {Book::Type::NONE, -1}; };

        // Callbacks
        virtual void SkillCheck(Party::Base &party, bool outcome, std::vector<int> selection){};
        virtual void AfterCombat(Party::Base &party, Engine::Combat result){};

        Base()
        {
        }

        Base(int id)
        {
            ID = id;
        }

        Base(int id, const char *text, Story::Type type)
        {
            ID = id;
            Text = text;
            type = type;
        }

        Base(const char *text, Story::Type type)
        {
            Text = text;
            type = type;
        }
    };

    std::vector<Button> StandardControls(bool compact = false)
    {
        auto idx = 0;

        auto controls = std::vector<Button>();

        if (!compact)
        {
            controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
            controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx = 2;
        }

        controls.push_back(Button(idx, "icons/map.png", idx, idx + 1, compact ? idx : 1, idx, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(idx + 1, "icons/disk.png", idx, idx + 2, compact ? idx + 1 : 1, idx + 1, startx + gridsize, buttony, Control::Type::GAME));
        controls.push_back(Button(idx + 2, "icons/user.png", idx + 1, idx + 3, compact ? idx + 2 : 1, idx + 2, startx + 2 * gridsize, buttony, Control::Type::PARTY));
        controls.push_back(Button(idx + 3, "icons/next.png", idx + 2, idx + 4, compact ? idx + 3 : 1, idx + 3, startx + 3 * gridsize, buttony, Control::Type::NEXT));
        controls.push_back(Button(idx + 4, "icons/exit.png", idx + 3, idx + 4, compact ? idx + 4 : 1, idx + 4, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        return controls;
    }

    std::vector<Button> ShopControls(bool compact = false)
    {
        auto idx = 0;

        auto controls = std::vector<Button>();

        if (!compact)
        {
            controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
            controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx = 2;
        }

        controls.push_back(Button(idx, "icons/map.png", idx, idx + 1, compact ? idx : 1, idx, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(idx + 1, "icons/disk.png", idx, idx + 2, compact ? idx + 1 : 1, idx + 1, startx + gridsize, buttony, Control::Type::GAME));
        controls.push_back(Button(idx + 2, "icons/user.png", idx + 1, idx + 3, compact ? idx + 2 : 1, idx + 2, startx + 2 * gridsize, buttony, Control::Type::PARTY));
        controls.push_back(Button(idx + 3, "icons/shop.png", idx + 2, idx + 4, compact ? idx + 3 : 1, idx + 3, startx + 3 * gridsize, buttony, Control::Type::SHOP));
        controls.push_back(Button(idx + 4, "icons/next.png", idx + 3, idx + 5, compact ? idx + 4 : 1, idx + 4, startx + 4 * gridsize, buttony, Control::Type::NEXT));
        controls.push_back(Button(idx + 5, "icons/exit.png", idx + 4, idx + 5, compact ? idx + 5 : 1, idx + 5, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        return controls;
    }

    std::vector<Button> SellControls(bool compact = false)
    {
        auto idx = 0;

        auto controls = std::vector<Button>();

        if (!compact)
        {
            controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
            controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx = 2;
        }

        controls.push_back(Button(idx, "icons/map.png", idx, idx + 1, compact ? idx : 1, idx, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(idx + 1, "icons/disk.png", idx, idx + 2, compact ? idx + 1 : 1, idx + 1, startx + gridsize, buttony, Control::Type::GAME));
        controls.push_back(Button(idx + 2, "icons/user.png", idx + 1, idx + 3, compact ? idx + 2 : 1, idx + 2, startx + 2 * gridsize, buttony, Control::Type::PARTY));
        controls.push_back(Button(idx + 3, "icons/selling.png", idx + 2, idx + 4, compact ? idx + 3 : 1, idx + 3, startx + 3 * gridsize, buttony, Control::Type::SELL));
        controls.push_back(Button(idx + 4, "icons/next.png", idx + 3, idx + 5, compact ? idx + 4 : 1, idx + 4, startx + 4 * gridsize, buttony, Control::Type::NEXT));
        controls.push_back(Button(idx + 5, "icons/exit.png", idx + 4, idx + 5, compact ? idx + 5 : 1, idx + 5, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        return controls;
    }

    std::vector<Button> BuyAndSellControls(bool compact = false)
    {
        auto idx = 0;

        auto controls = std::vector<Button>();

        if (!compact)
        {
            controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
            controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx = 2;
        }

        controls.push_back(Button(idx, "icons/map.png", idx, idx + 1, compact ? idx : 1, idx, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(idx + 1, "icons/disk.png", idx, idx + 2, compact ? idx + 1 : 1, idx + 1, startx + gridsize, buttony, Control::Type::GAME));
        controls.push_back(Button(idx + 2, "icons/user.png", idx + 1, idx + 3, compact ? idx + 2 : 1, idx + 2, startx + 2 * gridsize, buttony, Control::Type::PARTY));
        controls.push_back(Button(idx + 3, "icons/shop.png", idx + 2, idx + 4, compact ? idx + 3 : 1, idx + 3, startx + 3 * gridsize, buttony, Control::Type::SHOP));
        controls.push_back(Button(idx + 4, "icons/selling.png", idx + 3, idx + 5, compact ? idx + 4 : 1, idx + 4, startx + 4 * gridsize, buttony, Control::Type::SELL));
        controls.push_back(Button(idx + 5, "icons/next.png", idx + 4, idx + 6, compact ? idx + 5 : 1, idx + 5, startx + 5 * gridsize, buttony, Control::Type::NEXT));
        controls.push_back(Button(idx + 6, "icons/exit.png", idx + 5, idx + 6, compact ? idx + 6 : 1, idx + 6, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        return controls;
    }

    std::vector<Button> BarterControls(bool compact = false)
    {
        auto idx = 0;

        auto controls = std::vector<Button>();

        if (!compact)
        {
            controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
            controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx = 2;
        }

        controls.push_back(Button(idx, "icons/map.png", idx, idx + 1, compact ? idx : 1, idx, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(idx + 1, "icons/disk.png", idx, idx + 2, compact ? idx + 1 : 1, idx + 1, startx + gridsize, buttony, Control::Type::GAME));
        controls.push_back(Button(idx + 2, "icons/user.png", idx + 1, idx + 3, compact ? idx + 2 : 1, idx + 2, startx + 2 * gridsize, buttony, Control::Type::PARTY));
        controls.push_back(Button(idx + 3, "icons/exchange.png", idx + 2, idx + 4, compact ? idx + 3 : 1, idx + 3, startx + 3 * gridsize, buttony, Control::Type::BARTER));
        controls.push_back(Button(idx + 4, "icons/next.png", idx + 3, idx + 5, compact ? idx + 4 : 1, idx + 4, startx + 4 * gridsize, buttony, Control::Type::NEXT));
        controls.push_back(Button(idx + 5, "icons/exit.png", idx + 4, idx + 5, compact ? idx + 5 : 1, idx + 5, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        return controls;
    }

    std::vector<Button> BarterAndShopControls(bool compact = false)
    {
        auto idx = 0;

        auto controls = std::vector<Button>();

        if (!compact)
        {
            controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
            controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx = 2;
        }

        controls.push_back(Button(idx, "icons/map.png", idx, idx + 1, compact ? idx : 1, idx, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(idx + 1, "icons/disk.png", idx, idx + 2, compact ? idx + 1 : 1, idx + 1, startx + gridsize, buttony, Control::Type::GAME));
        controls.push_back(Button(idx + 2, "icons/user.png", idx + 1, idx + 3, compact ? idx + 2 : 1, idx + 2, startx + 2 * gridsize, buttony, Control::Type::PARTY));
        controls.push_back(Button(idx + 3, "icons/shop.png", idx + 2, idx + 4, compact ? idx + 3 : 1, idx + 3, startx + 3 * gridsize, buttony, Control::Type::SHOP));
        controls.push_back(Button(idx + 4, "icons/exchange.png", idx + 3, idx + 5, compact ? idx + 4 : 1, idx + 4, startx + 4 * gridsize, buttony, Control::Type::BARTER));
        controls.push_back(Button(idx + 5, "icons/next.png", idx + 4, idx + 6, compact ? idx + 5 : 1, idx + 5, startx + 5 * gridsize, buttony, Control::Type::NEXT));
        controls.push_back(Button(idx + 6, "icons/exit.png", idx + 5, idx + 6, compact ? idx + 6 : 1, idx + 6, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        return controls;
    }

    std::vector<Button> HarbourControls(bool compact = false)
    {
        auto idx = 0;

        auto controls = std::vector<Button>();

        if (!compact)
        {
            controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
            controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx = 2;
        }

        controls.push_back(Button(idx, "icons/map.png", idx, idx + 1, compact ? idx : 1, idx, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(idx + 1, "icons/disk.png", idx, idx + 2, compact ? idx + 1 : 1, idx + 1, startx + gridsize, buttony, Control::Type::GAME));
        controls.push_back(Button(idx + 2, "icons/user.png", idx + 1, idx + 3, compact ? idx + 2 : 1, idx + 2, startx + 2 * gridsize, buttony, Control::Type::PARTY));
        controls.push_back(Button(idx + 3, "icons/anchors.png", idx + 2, idx + 4, compact ? idx + 3 : 1, idx + 3, startx + 3 * gridsize, buttony, Control::Type::HARBOUR));
        controls.push_back(Button(idx + 4, "icons/next.png", idx + 3, idx + 5, compact ? idx + 4 : 1, idx + 4, startx + 4 * gridsize, buttony, Control::Type::NEXT));
        controls.push_back(Button(idx + 5, "icons/exit.png", idx + 4, idx + 5, compact ? idx + 5 : 1, idx + 5, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        return controls;
    }

    std::vector<Button> ExitControls(bool compact = false)
    {
        auto idx = 0;

        auto controls = std::vector<Button>();

        if (!compact)
        {
            controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
            controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx = 2;
        }

        controls.push_back(Button(idx, "icons/exit.png", compact ? idx : idx - 1, idx, compact ? idx : idx - 1, idx, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        return controls;
    }

    class NotImplemented : public Story::Base
    {
    public:
        NotImplemented()
        {
            BookID = Book::Type::NONE;

            ID = -1;

            Title = "Not implemented yet";

            Controls = Story::Controls::NONE;
        }
    };

    auto notImplemented = NotImplemented();

    void *FIND_STORY(int id, std::vector<Story::Base *> &Stories)
    {
        Story::Base *story = &notImplemented;

        if (Stories.size() > 0)
        {
            for (auto i = 0; i < Stories.size(); i++)
            {
                if (((Story::Base *)Stories[i])->ID == id)
                {
                    story = (Story::Base *)Stories[i];

                    break;
                }
            }
        }

        return story;
    }
}
#endif