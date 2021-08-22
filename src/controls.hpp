#ifndef __CONTROLS__HPP__
#define __CONTROLS__HPP__
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

namespace Control
{
    enum class Type
    {
        ANY = 0,
        ABOUT,
        SCROLL_UP,
        SCROLL_DOWN,
        MAP,
        CHARACTER,
        PARTY,
        GAME,
        NEW,
        CUSTOM,
        LOAD,
        SAVE,
        SHOP,
        INN,
        NEXT,
        ENCYCLOPEDIA,
        GLOSSARY,
        ACTION,
        BACK,
        EQUIPMENT,
        USE,
        DROP,
        CONFIRM,
        QUIT,
        PLUS,
        MINUS,
        BARTER,
        GIVE,
        STEAL,
        LOSE,
        YES,
        NO,
        BUY,
        SELL,
        COMBAT,
        ADVENTURE,
        SKILL,
        TEAM_SKILL,
        ATTACK,
        DEFEND,
        SPELL,
        SPELL_TARGET,
        SPELLBOOK,
        UNLEARN,
        FLEE,
        HARBOUR,
        ARMY,
        FLEET,
        ROMANCE,
        BUY_SELL_SHIP,
        BUY_SELL_CARGO,
        REPAIR_SHIP,
        LEARN_SPELL,
        ROLL_FOR_ATTRIBUTE_INCREASE,
        RAISE_ATTRIBUTE_SCORE,
        SELECT_LOWEST_ATTRIBUTE,
        HEALTH,
        GAIN_HEALTH,
        LOSE_HEALTH,
        RAISE_MAX_HEALTH,
        TRANSFER,
        VAULT,
        ENTER_SHOP,
        RECRUIT,
        MASS_COMBAT,
        SEA_COMBAT,
        HEAL1,
        FULL_RECOVERY,
        RECHARGE,
        FOLLOWERS,
        SELECT_ORDER,
        CARGO,
        SAIL,
        DRINK,
        PREVIEW
    };

    class Base
    {
    public:
        int ID = -1;

        int Left = -1;
        int Right = -1;
        int Up = -1;
        int Down = -1;

        int X = 0;
        int Y = 0;

        int W = 0;
        int H = 0;

        Control::Type Type = Control::Type::ANY;
    };
}

class TextButton : public Control::Base
{
private:
    void construct(int id, const char *text, int left, int right, int up, int down, int x, int y, int w, int h)
    {
        ID = id;
        Text = text;
        Left = left;
        Right = right;
        Up = up;
        Down = down;
        X = x;
        Y = y;
        W = w;
        H = h;
    }

public:
    const char *Text = NULL;

    TextButton(int id, const char *text, int left, int right, int up, int down, int x, int y, int w, int h)
    {
        construct(id, text, left, right, up, down, x, y, w, h);
    }

    TextButton(int id, const char *text, int left, int right, int up, int down, int x, int y, int w, int h, Control::Type type)
    {
        Type = type;

        construct(id, text, left, right, up, down, x, y, w, h);
    }
};

class Button : public Control::Base
{
private:
    SDL_Surface *createImage(const char *file)
    {
        auto surface = IMG_Load(File);

        if (surface == NULL)
        {
            std::cerr << "Unable to load image " << file << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            W = surface->w;
            H = surface->h;
        }

        return surface;
    }

    void construct(int id, const char *file, int left, int right, int up, int down, int x, int y)
    {
        ID = id;
        File = file;
        Left = left;
        Right = right;
        Up = up;
        Down = down;
        X = x;
        Y = y;

        Surface = createImage(file);
    }

    void construct(int id, int left, int right, int up, int down, int x, int y)
    {
        ID = id;
        Left = left;
        Right = right;
        Up = up;
        Down = down;
        X = x;
        Y = y;
    }

public:
    const char *File = NULL;

    SDL_Surface *Surface = NULL;

    Button()
    {
    }

    Button(int id, const char *file, int left, int right, int up, int down, int x, int y)
    {
        construct(id, file, left, right, up, down, x, y);
    }

    Button(int id, const char *file, int left, int right, int up, int down, int x, int y, Control::Type type)
    {
        Type = type;

        construct(id, file, left, right, up, down, x, y);
    }

    Button(int id, SDL_Surface *image, int left, int right, int up, int down, int x, int y, Control::Type type)
    {
        Type = type;

        Surface = image;

        W = Surface->w;
        H = Surface->h;

        construct(id, left, right, up, down, x, y);
    }
};
#endif
