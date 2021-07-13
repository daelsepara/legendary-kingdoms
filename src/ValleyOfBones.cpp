// Standard IO
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

// Using SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#if defined(_WIN32)

#include <windows.h>
#include <shlobj.h>

#else

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#endif

// JSON library
#include "nlohmann/json.hpp"

#include "constants.hpp"
#include "controls.hpp"
#include "engine.hpp"
#include "location.hpp"
#include "input.hpp"
#include "ship.hpp"
#include "story.hpp"

#include "book1.hpp"

// Forward declarations
bool harbourScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *harbour);
bool introScreen(SDL_Window *window, SDL_Renderer *renderer);
bool inventoryScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, std::vector<Equipment::Base> &Items, Control::Type mode, int limit);
bool mainScreen(SDL_Window *window, SDL_Renderer *renderer, Book::Type bookID, int storyID);
bool partyDetails(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party);
bool processStory(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Book::Type book, Story::Base *story);
bool selectParty(SDL_Window *window, SDL_Renderer *renderer, Book::Type bookID, Party::Base &party);
bool skillCheck(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> &party, int team_size, Attribute::Type skill, int difficulty, int success, std::vector<int> &selection);
bool skillTestScreen(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> &party, std::vector<int> team, Attribute::Type Skill, int difficulty, int success, bool useEquipment);
bool takeScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Equipment::Base> equipment, int TakeLimit, bool back_button);
bool testScreen(SDL_Window *window, SDL_Renderer *renderer, Book::Type bookID, int storyID);
bool viewParty(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party);

Engine::Combat combatScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Monster::Base> &monsters, bool canFlee, int fleeRound, bool useEquipment);

int armourSave(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, int damage);
int assignDamage(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> &party);
int attackScreen(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> &party, std::vector<Monster::Base> &monsters, int combatant, int opponent, int direction, bool useEquipment);
int magicAttackScreen(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> &party, std::vector<Monster::Base> &monsters, Spells::Base &spell, int combatant, int opponent, int fighting_score);
int selectCaster(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> &party, std::vector<Monster::Base> &monsters, std::vector<int> hasAttacked, Control::Type mode);
int selectOpponent(SDL_Window *window, SDL_Renderer *renderer, std::vector<Monster::Base> &monsters, std::vector<int> previousTargets);
int selectPartyMember(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> &party, Control::Type mode);

std::vector<int> selectSpell(SDL_Window *window, SDL_Renderer *renderer, std::vector<Spells::Base> &spells, int select_limit, Spells::Select mode);

Story::Base *processChoices(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *story);
Story::Base *renderChoices(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *story);

SDL_Surface *createImage(const char *image)
{
    // Load splash image
    auto surface = IMG_Load(image);

    if (surface == NULL)
    {
        std::cerr << "Unable to load image " << image << "! SDL Error: " << SDL_GetError() << std::endl;
    }

    return surface;
}

void createWindow(Uint32 flags, SDL_Window **window, SDL_Renderer **renderer, const char *title, const char *icon)
{
    // The window we'll be rendering to
    *window = NULL;
    *renderer = NULL;

    // Initialize SDL
    if (SDL_Init(flags) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        SDL_DisplayMode mode;

        SDL_GetCurrentDisplayMode(0, &mode);

        SCREEN_WIDTH = (mode.w);
        SCREEN_HEIGHT = (mode.h);

        Recompute();

        // Create window and renderer
        SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, (SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), window, renderer);

        SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_NONE);

        if (window == NULL || renderer == NULL)
        {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        }
        else if (window)
        {
            SDL_SetWindowTitle(*window, title);
        }

        auto surface = createImage(icon);

        if (surface)
        {
            // The icon is attached to the window pointer
            SDL_SetWindowIcon(*window, surface);

            // ...and the surface containing the icon pixel data is no longer required.
            SDL_FreeSurface(surface);

            surface = NULL;
        }
    }
}

void renderImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y)
{
    if (image && renderer)
    {
        SDL_Rect position;

        position.w = image->w;
        position.h = image->h;
        position.x = x;
        position.y = y;

        auto texture = SDL_CreateTextureFromSurface(renderer, image);

        if (texture)
        {
            SDL_Rect src;

            src.w = image->w;
            src.h = image->h;
            src.x = 0;
            src.y = 0;

            SDL_RenderCopy(renderer, texture, &src, &position);

            SDL_DestroyTexture(texture);

            texture = NULL;
        }
    }
}

int fitImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y, int w, int h)
{
    auto splash_h = image->h;
    auto splash_w = w;

    if (image && renderer)
    {
        SDL_Rect position;

        if (w != image->w)
        {
            splash_h = (int)((double)w / image->w * image->h);
            splash_w = w;

            if (splash_h > h)
            {
                splash_h = h;
                splash_w = (int)((double)h / image->h * image->w);
            }
        }

        position.w = splash_w;
        position.h = splash_h;
        position.x = x;
        position.y = y;

        auto texture = SDL_CreateTextureFromSurface(renderer, image);

        if (texture)
        {
            SDL_Rect src;

            src.w = image->w;
            src.h = image->h;
            src.x = 0;
            src.y = 0;

            SDL_RenderCopy(renderer, texture, &src, &position);

            SDL_DestroyTexture(texture);

            texture = NULL;
        }
    }

    return splash_h;
}

int fadeImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y, int w, int h, Uint8 alpha)
{
    auto splash_h = image->h;
    auto splash_w = w;

    if (image && renderer)
    {
        SDL_Rect position;

        if (w != image->w)
        {
            splash_h = (int)((double)w / image->w * image->h);
            splash_w = w;

            if (splash_h > h)
            {
                splash_h = h;
                splash_w = (int)((double)h / image->h * image->w);
            }
        }

        position.w = splash_w;
        position.h = splash_h;
        position.x = x;
        position.y = y;

        auto texture = SDL_CreateTextureFromSurface(renderer, image);

        SDL_SetTextureAlphaMod(texture, alpha);

        if (texture)
        {
            SDL_Rect src;

            src.w = image->w;
            src.h = image->h;
            src.x = 0;
            src.y = 0;

            SDL_RenderCopy(renderer, texture, &src, &position);

            SDL_DestroyTexture(texture);

            texture = NULL;
        }
    }

    return splash_h;
}

void stretchImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y, int w, int h)
{
    auto splash_h = image->h;
    auto splash_w = w;

    if (image && renderer)
    {
        SDL_Rect position;

        position.w = w;
        position.h = h;
        position.x = x;
        position.y = y;

        auto texture = SDL_CreateTextureFromSurface(renderer, image);

        if (texture)
        {
            SDL_Rect src;

            src.w = image->w;
            src.h = image->h;
            src.x = 0;
            src.y = 0;

            SDL_RenderCopy(renderer, texture, &src, &position);

            SDL_DestroyTexture(texture);

            texture = NULL;
        }
    }
}

// Render a portion of the text (image) on bounded surface within the specified window
void renderText(SDL_Renderer *renderer, SDL_Surface *text, Uint32 bg, int x, int y, int bounds, int offset)
{
    if (renderer)
    {
        if (text && renderer)
        {
            SDL_Rect dst;
            SDL_Rect src;

            // select portion to render
            src.w = text->w;
            src.h = text->h < bounds ? text->h : bounds;
            src.y = offset;
            src.x = 0;

            // specify location within the window
            dst.w = text->w;
            dst.h = text->h < bounds ? text->h : bounds;
            dst.x = x;
            dst.y = y;

            if (bg != 0)
            {
                SDL_SetRenderDrawColor(renderer, R(bg), G(bg), B(bg), A(bg));
                SDL_RenderFillRect(renderer, &dst);
            }

            auto texture = SDL_CreateTextureFromSurface(renderer, text);

            if (texture)
            {
                SDL_RenderCopy(renderer, texture, &src, &dst);

                SDL_DestroyTexture(texture);

                texture = NULL;
            }
        }
    }
}

// create text image with line wrap limit
SDL_Surface *createText(const char *text, const char *ttf, int font_size, SDL_Color textColor, int wrap, int style = TTF_STYLE_NORMAL)
{
    SDL_Surface *surface = NULL;

    TTF_Init();

    auto font = TTF_OpenFont(ttf, font_size);

    if (font != NULL)
    {
        if (std::strcmp(ttf, FONT_DARK11) == 0)
        {
            TTF_SetFontKerning(font, 0);
        }
    }

    if (font)
    {
        TTF_SetFontStyle(font, style);

        surface = TTF_RenderText_Blended_Wrapped(font, text, textColor, wrap);

        TTF_CloseFont(font);

        font = NULL;
    }

    TTF_Quit();

    return surface;
}

void fillRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color)
{
    SDL_Rect rect;

    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;

    SDL_SetRenderDrawColor(renderer, R(color), G(color), B(color), A(color));
    SDL_RenderFillRect(renderer, &rect);
}

void drawRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color)
{
    SDL_Rect rect;

    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;

    SDL_SetRenderDrawColor(renderer, R(color), G(color), B(color), A(color));
    SDL_RenderDrawRect(renderer, &rect);
}

void thickRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color, int pts)
{
    SDL_Rect rect;

    auto space = 2 * pts;

    for (auto size = pts; size >= 0; size--)
    {
        rect.w = w + 2 * (space - size);
        rect.h = h + 2 * (space - size);
        rect.x = x - space + size;
        rect.y = y - space + size;

        SDL_SetRenderDrawColor(renderer, R(color), G(color), B(color), A(color));
        SDL_RenderDrawRect(renderer, &rect);
    }
}

void putHeader(SDL_Renderer *renderer, const char *text, TTF_Font *font, int space, SDL_Color fg, Uint32 bg, int style, int w, int h, int x, int y)
{
    if (renderer)
    {
        TTF_SetFontStyle(font, style);

        auto surface = TTF_RenderText_Blended_Wrapped(font, text, fg, w - 2 * space);

        if (surface)
        {
            auto height = (surface->h + 2 * space) < h ? h : (surface->h + 2 * space);

            if (x < 0)
            {
                fillRect(renderer, w, height, (SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - height) / 2, bg);
            }
            else
            {
                fillRect(renderer, w, height, x, y, bg);
            }

            if (space > 0)
            {
                if (x < 0)
                {
                    renderText(renderer, surface, 0, (SCREEN_WIDTH - surface->w) / 2 + space, y + (SCREEN_HEIGHT - surface->h) / 2, height - 2 * space, 0);
                }
                else
                {
                    renderText(renderer, surface, 0, x + space, y + (h - surface->h) / 2, height - 2 * space, 0);
                }
            }
            else
            {
                if (x < 0)
                {
                    renderText(renderer, surface, 0, (SCREEN_WIDTH - surface->w) / 2, y + (SCREEN_HEIGHT - surface->h) / 2, height - 2 * space, 0);
                }
                else
                {
                    renderText(renderer, surface, 0, x + (w - surface->w) / 2, y + (h - surface->h) / 2, height - 2 * space, 0);
                }
            }

            SDL_FreeSurface(surface);

            surface = NULL;
        }
    }
}

void putText(SDL_Renderer *renderer, const char *text, TTF_Font *font, int space, SDL_Color fg, Uint32 bg, int style, int w, int h, int x, int y)
{
    if (renderer)
    {
        TTF_SetFontStyle(font, style);

        auto surface = TTF_RenderText_Blended_Wrapped(font, text, fg, w - 2 * space);

        if (surface)
        {
            auto height = (surface->h + 2 * space) < h ? h : (surface->h + 2 * space);

            if (x < 0)
            {
                fillRect(renderer, w, height, (SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - height) / 2, bg);
            }
            else
            {
                fillRect(renderer, w, height, x, y, bg);
            }

            if (space > 0)
            {
                if (x < 0)
                {
                    renderText(renderer, surface, 0, (SCREEN_WIDTH - surface->w) / 2 + space, (SCREEN_HEIGHT - height) / 2 + space, height - 2 * space, 0);
                }
                else
                {
                    renderText(renderer, surface, 0, x + space, y + space, height - 2 * space, 0);
                }
            }
            else
            {
                if (x < 0)
                {
                    renderText(renderer, surface, 0, (SCREEN_WIDTH - surface->w) / 2 + (w - surface->w), (SCREEN_HEIGHT - surface->h) / 2 + (h - surface->h) / 2, height, 0);
                }
                else
                {
                    renderText(renderer, surface, 0, x + (w - surface->w) / 2, y + (h - surface->h) / 2, height, 0);
                }
            }

            SDL_FreeSurface(surface);

            surface = NULL;
        }
    }
}

void fillWindow(SDL_Renderer *renderer, Uint32 color)
{
    SDL_SetRenderDrawColor(renderer, R(color), G(color), B(color), A(color));
    SDL_RenderClear(renderer);
}

void renderTextButtons(SDL_Renderer *renderer, std::vector<TextButton> controls, const char *ttf, int selected, SDL_Color fg, Uint32 bg, Uint32 bgSelected, int fontsize, int style = TTF_STYLE_NORMAL)
{
    if (controls.size() > 0)
    {
        for (auto i = 0; i < controls.size(); i++)
        {
            auto text = createText(controls[i].Text, ttf, fontsize, fg, controls[i].W, style);

            auto x = controls[i].X + (controls[i].W - text->w) / 2;
            auto y = controls[i].Y + (controls[i].H - text->h) / 2;

            SDL_Rect rect;

            rect.w = controls[i].W;
            rect.h = controls[i].H;
            rect.x = controls[i].X;
            rect.y = controls[i].Y;

            if (i == selected)
            {
                SDL_SetRenderDrawColor(renderer, R(bgSelected), G(bgSelected), B(bgSelected), A(bgSelected));
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, R(bg), G(bg), B(bg), A(bg));
            }

            SDL_RenderFillRect(renderer, &rect);

            if (i == selected)
            {
                renderText(renderer, text, bgSelected, x, y, 2 * fontsize, 0);
            }
            else
            {
                renderText(renderer, text, bg, x, y, 2 * fontsize, 0);
            }

            SDL_FreeSurface(text);

            text = NULL;
        }
    }
}

void renderButtons(SDL_Renderer *renderer, std::vector<Button> controls, int current, int fg, int space, int pts)
{
    if (controls.size() > 0)
    {
        for (auto i = 0; i < controls.size(); i++)
        {
            SDL_Rect rect;

            for (auto size = pts; size >= 0; size--)
            {
                rect.w = controls[i].W + 2 * (space - size);
                rect.h = controls[i].H + 2 * (space - size);
                rect.x = controls[i].X - space + size;
                rect.y = controls[i].Y - space + size;

                if (i == current)
                {
                    SDL_SetRenderDrawColor(renderer, R(fg), G(fg), B(fg), A(fg));
                    SDL_RenderDrawRect(renderer, &rect);
                }
            }

            renderImage(renderer, controls[i].Surface, controls[i].X, controls[i].Y);
        }
    }
}

std::vector<TextButton> createHTextButtons(const char **choices, int num, int text_buttonh, int text_x, int text_y)
{
    auto controls = std::vector<TextButton>();

    if (num > 0)
    {
        auto margin2 = (2.0 * Margin);
        auto marginleft = (1.0 - margin2);

        auto pixels = (int)(SCREEN_WIDTH * Margin) / 2;
        auto width = (int)(SCREEN_WIDTH * marginleft);

        auto text_spacew = width / num;
        auto text_buttonw = text_spacew - pixels;
        auto text_space = pixels / 2;

        for (auto i = 0; i < num; i++)
        {
            auto left = i > 0 ? i - 1 : i;
            auto right = i < num - 1 ? i + 1 : i;
            auto up = i;
            auto down = i;

            auto x = text_x + i * (text_buttonw + text_space * 2) + text_space;

            auto button = TextButton(i, choices[i], left, right, up, down, x, text_y, text_buttonw, text_buttonh);

            controls.push_back(button);
        }
    }

    return controls;
}

std::vector<TextButton> createFixedTextButtons(const char **choices, int num, int text_buttonw, int text_buttonh, int button_space, int text_x, int text_y)
{
    auto controls = std::vector<TextButton>();

    if (num > 0)
    {
        for (auto i = 0; i < num; i++)
        {
            auto left = i > 0 ? i - 1 : i;
            auto right = i < num - 1 ? i + 1 : i;
            auto up = i;
            auto down = i;

            auto x = text_x + i * (text_buttonw + button_space);

            auto button = TextButton(i, choices[i], left, right, up, down, x, text_y, text_buttonw, text_buttonh);

            controls.push_back(button);
        }
    }

    return controls;
}

SDL_Surface *createHeaderButton(SDL_Window *window, const char *text, SDL_Color color, Uint32 bg, int w, int h, int x)
{
    auto button = SDL_CreateRGBSurface(0, w, h, arrow_size, 0, 0, 0, 0);
    auto text_surface = createText(text, FONT_GARAMOND, 24, color, w, TTF_STYLE_NORMAL);

    if (button && text_surface)
    {
        SDL_Rect src;

        src.w = text_surface->w;
        src.h = text_surface->h;
        src.x = 0;
        src.y = 0;

        SDL_Rect dst;

        dst.w = button->w;
        dst.h = button->h;
        dst.x = 0;
        dst.y = 0;

        SDL_FillRect(button, &dst, bg);

        dst.x = x < 0 ? (button->w - text_surface->w) / 2 : x;
        dst.y = (button->h - text_surface->h) / 2;

        SDL_BlitSurface(text_surface, &src, button, &dst);
    }

    if (text_surface)
    {
        SDL_FreeSurface(text_surface);

        text_surface = NULL;
    }

    return button;
}

void clipValue(int &val, int min, int max)
{
    if (val < min)
    {
        val = min;
    }
    if (val > max)
    {
        val = max;
    }
}

bool introScreen(SDL_Window *window, SDL_Renderer *renderer)
{
    auto splashLogo = "images/legendary-kingdoms.png";

    auto splashImage = createImage(splashLogo);

    auto quit = false;

    if (window && renderer && splashImage)
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        for (Uint8 i = 0; i < (Uint8)255; i += (Uint8)1)
        {
            fillWindow(renderer, intBK);

            fadeImage(renderer, splashImage, (SCREEN_WIDTH - 800) / 2, (SCREEN_HEIGHT - 350) / 2, 800, 350, i);

            SDL_RenderPresent(renderer);

            SDL_Delay(5);
        }

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

        fillWindow(renderer, intBK);

        fitImage(renderer, splashImage, (SCREEN_WIDTH - 800) / 2, (SCREEN_HEIGHT - 350) / 2, 800, 350);

        SDL_RenderPresent(renderer);

        SDL_Delay(3000);

        SDL_FreeSurface(splashImage);

        splashImage = NULL;
    }

    return false;
}

std::string characterText(Character::Base &character, bool compact)
{
    std::string character_text = "";

    if (!compact)
    {
        character_text = character.Background;
    }
    else
    {
        character_text = character.Name;
    }

    if (!compact)
    {
        character_text += "\n";
    }

    character_text += "\n";

    for (auto i = 0; i < character.Attributes.size(); i++)
    {
        if (i > 0)
        {
            character_text += ", ";
        }

        character_text += std::string(Attribute::Descriptions[character.Attributes[i].Type]) + ": " + std::to_string(character.Attributes[i].Value);
    }

    character_text += ", Health: " + std::to_string(character.Health);

    if (!compact)
    {
        if (character.SpellCaster)
        {
            character_text += "\n\nSpellcaster";
        }
    }
    else
    {
        if (character.SpellCaster)
        {
            character_text += ", Spellcaster";
        }
    }

    return character_text;
};

bool partyDetails(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party)
{
    auto *title = "Party Details";

    auto font_size = 20;
    auto garamond_size = 24;

    TTF_Init();

    auto font_mason = TTF_OpenFont(FONT_MASON, 32);
    auto font_mason2 = TTF_OpenFont(FONT_MASON, 22);
    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, garamond_size);
    auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

    TTF_SetFontKerning(font_dark11, 0);

    auto box_space = 10;
    auto character_box = (int)(text_bounds * 2 / 3);

    // Render window
    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        const char *choices[4] = {"ARMY", "FLEET", "ROMANCE", "BACK"};

        auto current = 0;

        auto selected = false;

        auto main_buttonh = (int)(0.06 * SCREEN_HEIGHT);

        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;

        auto controls = createHTextButtons(choices, 4, main_buttonh, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);

        controls[0].Type = Control::Type::ARMY;
        controls[1].Type = Control::Type::FLEET;
        controls[2].Type = Control::Type::ROMANCE;
        controls[3].Type = Control::Type::BACK;

        auto done = false;

        auto text_space = 8;

        auto Party = Party::Base();

        while (!done)
        {
            // Fill the surface with background
            fillWindow(renderer, intWH);

            auto adventurerh = splashw;

            putHeader(renderer, "Location", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty);
            putText(renderer, Location::Description[party.Location], font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + infoh);

            putHeader(renderer, "Money", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + 2 * infoh + box_space - 1));
            putText(renderer, (std::to_string(party.Money) + std::string(" silver coins")).c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (3 * boxh + infoh + box_space));

            putHeader(renderer, "Party", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

            if (Engine::COUNT(party.Party) > 0)
            {
                std::string party_string = "";

                auto count = 0;

                for (auto i = 0; i < party.Party.size(); i++)
                {
                    if (count > 0)
                    {
                        party_string += "\n";
                    }

                    party_string += party.Party[i].Name;

                    if (party.Party[i].Health <= 0)
                    {
                        party_string += " (D)";
                    }

                    count++;
                }

                putText(renderer, party_string.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
            }

            fillRect(renderer, textwidth, (text_bounds - infoh), textx, (texty + infoh), intBE);

            if (current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::ARMY)
                {
                    putHeader(renderer, "Army", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::FLEET)
                {
                    putHeader(renderer, "Fleet", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::ROMANCE)
                {
                    putHeader(renderer, "Romance", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else
                {
                    fillRect(renderer, textwidth, infoh, textx, texty, intBR);
                }
            }
            else
            {
                fillRect(renderer, textwidth, infoh, textx, texty, intBR);
            }

            renderTextButtons(renderer, controls, FONT_DARK11, current, clrWH, intDB, intLB, font_size + 2, TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::BACK)
                {
                    done = true;

                    break;
                }
            }
        }
    }

    if (font_mason)
    {
        TTF_CloseFont(font_mason);

        font_mason = NULL;
    }

    if (font_mason2)
    {
        TTF_CloseFont(font_mason2);

        font_mason2 = NULL;
    }

    if (font_dark11)
    {
        TTF_CloseFont(font_dark11);

        font_dark11 = NULL;
    }

    if (font_garamond)
    {
        TTF_CloseFont(font_garamond);

        font_garamond = NULL;
    }

    TTF_Quit();

    return false;
}

bool viewParty(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party)
{
    SDL_Surface *adventurer = NULL;
    SDL_Surface *text = NULL;

    auto *title = "View Party";

    auto font_size = 20;
    auto garamond_size = 24;

    TTF_Init();

    auto font_mason = TTF_OpenFont(FONT_MASON, 32);
    auto font_mason2 = TTF_OpenFont(FONT_MASON, 22);
    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, garamond_size);
    auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

    TTF_SetFontKerning(font_dark11, 0);

    auto box_space = 10;
    auto character_box = (int)(text_bounds * 2 / 3);

    // Render window
    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        const char *choices[6] = {"PARTY", "PREVIOUS", "NEXT", "EQUIPMENT", "SPELLBOOK", "BACK"};

        auto current = 0;

        auto selected = false;

        auto main_buttonh = (int)(0.06 * SCREEN_HEIGHT);

        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;

        auto controls = createHTextButtons(choices, 6, main_buttonh, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);

        controls[0].Type = Control::Type::PARTY;
        controls[1].Type = Control::Type::MINUS;
        controls[2].Type = Control::Type::PLUS;
        controls[3].Type = Control::Type::EQUIPMENT;
        controls[4].Type = Control::Type::SPELLBOOK;
        controls[5].Type = Control::Type::BACK;

        auto done = false;

        auto text_space = 8;

        auto Party = Party::Base();

        auto summary_view = true;

        auto character = 0;

        if (character >= 0 && character < party.Party.size())
        {
            if (party.Party[character].Image != NULL)
            {
                if (adventurer != NULL)
                {
                    SDL_FreeSurface(adventurer);

                    adventurer = NULL;
                }

                adventurer = createImage(party.Party[character].Image);
            }

            if (text != NULL)
            {
                SDL_FreeSurface(text);

                text = NULL;
            }

            text = createText(characterText(party.Party[character], false).c_str(), FONT_GARAMOND, garamond_size, clrDB, textwidth - 2 * text_space, TTF_STYLE_NORMAL);
        }

        while (!done)
        {
            // Fill the surface with background
            fillWindow(renderer, intWH);

            auto adventurerh = splashw;

            if (current != 0 && adventurer != NULL && !summary_view)
            {
                adventurerh = fitImage(renderer, adventurer, startx, starty, splashw, text_bounds);
            }
            else
            {
                putHeader(renderer, "Location", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty);
                putText(renderer, Location::Description[party.Location], font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + infoh);

                putHeader(renderer, "Money", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + 2 * infoh + box_space - 1));
                putText(renderer, (std::to_string(party.Money) + std::string(" silver coins")).c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (3 * boxh + infoh + box_space));

                putHeader(renderer, "Party", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (Engine::COUNT(party.Party) > 0)
                {
                    std::string party_string = "";

                    auto count = 0;

                    for (auto i = 0; i < party.Party.size(); i++)
                    {
                        if (count > 0)
                        {
                            party_string += "\n";
                        }

                        party_string += party.Party[i].Name;

                        if (party.Party[i].Health <= 0)
                        {
                            party_string += " (D)";
                        }

                        count++;
                    }

                    putText(renderer, party_string.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                }

                putHeader(renderer, "Codes", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                fillRect(renderer, textwidth, (character_box - infoh), textx, (texty + infoh), intBE);

                std::string code_string = "";

                for (auto i = 0; i < party.Codes.size(); i++)
                {
                    if (i > 0)
                    {
                        code_string += ", ";
                    }

                    code_string += std::string(Codes::Prefix[party.Codes[i].Type]) + std::to_string(party.Codes[i].Code);
                }

                putText(renderer, code_string.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth - 2 * text_space, (character_box - infoh - 2 * text_space), textx, (texty + infoh));
            }

            if (current != 0 && text != NULL && !summary_view)
            {
                fillRect(renderer, textwidth, character_box, textx, texty, intBE);

                renderText(renderer, text, intBE, textx + text_space, texty + text_space, character_box, 0);
            }

            if (current != 0 && character >= 0 && character < party.Party.size() && adventurer && !summary_view)
            {
                putText(renderer, party.Party[character].Name, font_mason, -1, clrDB, intWH, TTF_STYLE_NORMAL, splashw, infoh, startx, adventurerh + infoh - text_space);
            }

            if (current != 0 && character >= 0 && character < party.Party.size() && !summary_view)
            {
                if (party.Party[character].Status.size() > 0)
                {
                    putHeader(renderer, "Status", font_mason2, 8, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, starty + character_box + 10);

                    std::string status_string = "";

                    for (auto i = 0; i < party.Party[character].Status.size(); i++)
                    {
                        if (i > 0)
                        {
                            status_string += ", ";
                        }

                        status_string += Character::StatusDescriptions[party.Party[character].Status[i]];
                    }

                    putText(renderer, status_string.c_str(), font_garamond, 8, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, (text_bounds - character_box) - infoh - box_space, textx, starty + text_bounds + infoh - (text_bounds - character_box) + box_space);
                }
            }

            if (current >= 0 && current < controls.size() && !summary_view)
            {
                if (controls[current].Type == Control::Type::SPELLBOOK && party.Party[character].SpellCaster && party.Party[character].SpellBook.size() > 0)
                {
                    putHeader(renderer, "SPELLBOOK", font_mason2, 8, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, starty + character_box + 10);

                    std::string spellbook_string = "";

                    for (auto i = 0; i < party.Party[character].SpellBook.size(); i++)
                    {
                        if (i > 0)
                        {
                            spellbook_string += ", ";
                        }

                        spellbook_string += party.Party[character].SpellBook[i].Name;
                    }

                    putText(renderer, spellbook_string.c_str(), font_garamond, 8, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, (text_bounds - character_box) - infoh - box_space, textx, starty + text_bounds + infoh - (text_bounds - character_box) + box_space);
                }
                else if (controls[current].Type == Control::Type::EQUIPMENT && party.Party[character].Equipment.size() > 0)
                {
                    putHeader(renderer, "EQUIPMENT", font_mason2, 8, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, starty + character_box + 10);

                    std::string equipment_string = "";

                    for (auto i = 0; i < party.Party[character].Equipment.size(); i++)
                    {
                        if (i > 0)
                        {
                            equipment_string += ", ";
                        }

                        equipment_string += party.Party[character].Equipment[i].Name;

                        if (party.Party[character].Equipment[i].Attribute != Attribute::Type::NONE)
                        {
                            equipment_string += " (+ " + std::to_string(party.Party[character].Equipment[i].Modifier) + " " + std::string(Attribute::Descriptions[party.Party[character].Equipment[i].Attribute]) + ")";
                        }
                    }

                    putText(renderer, equipment_string.c_str(), font_garamond, 8, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, (text_bounds - character_box) - infoh - box_space, textx, starty + text_bounds + infoh - (text_bounds - character_box) + box_space);
                }
            }

            renderTextButtons(renderer, controls, FONT_DARK11, current, clrWH, intDB, intLB, font_size + 2, TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (current == 0)
            {
                summary_view = true;
            }
            else if (current > 0)
            {
                if (current < controls.size() && controls[current].Type != Control::Type::BACK)
                {
                    summary_view = false;
                }
            }

            if (selected && current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::PARTY)
                {
                    partyDetails(window, renderer, party);

                    done = false;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::MINUS)
                {
                    if (character > 0)
                    {
                        character--;
                    }

                    if (character >= 0 && character < party.Party.size())
                    {
                        if (party.Party[character].Image != NULL)
                        {
                            if (adventurer != NULL)
                            {
                                SDL_FreeSurface(adventurer);

                                adventurer = NULL;
                            }

                            adventurer = createImage(party.Party[character].Image);
                        }

                        if (text != NULL)
                        {
                            SDL_FreeSurface(text);

                            text = NULL;
                        }

                        text = createText(characterText(party.Party[character], false).c_str(), FONT_GARAMOND, garamond_size, clrDB, textwidth - 2 * text_space, TTF_STYLE_NORMAL);
                    }
                }
                else if (controls[current].Type == Control::Type::PLUS)
                {
                    if (character < party.Party.size() - 1)
                    {
                        character++;
                    }

                    if (character >= 0 && character < party.Party.size())
                    {
                        if (party.Party[character].Image != NULL)
                        {
                            if (adventurer != NULL)
                            {
                                SDL_FreeSurface(adventurer);

                                adventurer = NULL;
                            }

                            adventurer = createImage(party.Party[character].Image);
                        }

                        if (text != NULL)
                        {
                            SDL_FreeSurface(text);

                            text = NULL;
                        }

                        text = createText(characterText(party.Party[character], false).c_str(), FONT_GARAMOND, garamond_size, clrDB, textwidth - 2 * text_space, TTF_STYLE_NORMAL);
                    }
                }
                else if (controls[current].Type == Control::Type::EQUIPMENT)
                {
                    if (character >= 0 && character < party.Party.size())
                    {
                        if (party.Party[character].Equipment.size() > 0)
                        {
                            inventoryScreen(window, renderer, party.Party[character], party.Party[character].Equipment, Control::Type::USE, 0);
                        }
                    }

                    done = false;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::SPELLBOOK)
                {
                    done = false;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK)
                {
                    done = true;

                    break;
                }
            }
        }
    }

    if (adventurer != NULL)
    {
        SDL_FreeSurface(adventurer);

        adventurer = NULL;
    }

    if (text != NULL)
    {
        SDL_FreeSurface(text);

        text = NULL;
    }

    if (font_mason)
    {
        TTF_CloseFont(font_mason);

        font_mason = NULL;
    }

    if (font_mason2)
    {
        TTF_CloseFont(font_mason2);

        font_mason2 = NULL;
    }

    if (font_dark11)
    {
        TTF_CloseFont(font_dark11);

        font_dark11 = NULL;
    }

    if (font_garamond)
    {
        TTF_CloseFont(font_garamond);

        font_garamond = NULL;
    }

    TTF_Quit();

    return false;
}

bool selectParty(SDL_Window *window, SDL_Renderer *renderer, Book::Type bookID, Party::Base &party)
{
    party.Party.clear();

    SDL_Surface *adventurer = NULL;
    SDL_Surface *text = NULL;

    auto *title = "Select adventurers for your party";

    auto font_size = 20;
    auto garamond_size = 24;

    TTF_Init();

    auto font_mason = TTF_OpenFont(FONT_MASON, 32);
    auto font_mason2 = TTF_OpenFont(FONT_MASON, 22);
    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, garamond_size);

    auto box_space = 10;
    auto character_box = (int)(text_bounds * 2 / 3);

    // Render window
    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        const char *choices_add[5] = {"PREVIOUS", "NEXT", "ADD TO PARTY", "START", "BACK"};
        const char *choices_del[5] = {"PREVIOUS", "NEXT", "REMOVE", "START", "BACK"};

        auto current = -1;

        auto selected = false;

        auto main_buttonh = (int)(0.06 * SCREEN_HEIGHT);

        auto infoh = (int)(0.07 * SCREEN_HEIGHT);

        auto controls_add = createHTextButtons(choices_add, 5, main_buttonh, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);
        auto controls_del = createHTextButtons(choices_del, 5, main_buttonh, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);

        controls_add[0].Type = Control::Type::BACK;
        controls_add[1].Type = Control::Type::NEXT;
        controls_add[2].Type = Control::Type::PLUS;
        controls_add[3].Type = Control::Type::NEW;
        controls_add[4].Type = Control::Type::QUIT;

        controls_del[0].Type = Control::Type::BACK;
        controls_del[1].Type = Control::Type::NEXT;
        controls_del[2].Type = Control::Type::MINUS;
        controls_del[3].Type = Control::Type::NEW;
        controls_del[4].Type = Control::Type::QUIT;

        std::vector<TextButton> *controls;

        auto done = false;

        auto text_space = 8;

        auto Party = Party::Base();

        std::vector<Character::Base> characters = {};

        if (bookID == Book::Type::BOOK1)
        {
            characters = Character::BOOK1;
        }

        auto selection = std::vector<int>();

        auto character = 0;

        if (character >= 0 && character < characters.size())
        {
            if (characters[character].Image != NULL)
            {
                if (adventurer != NULL)
                {
                    SDL_FreeSurface(adventurer);

                    adventurer = NULL;
                }

                adventurer = createImage(characters[character].Image);
            }

            if (text != NULL)
            {
                SDL_FreeSurface(text);

                text = NULL;
            }

            text = createText(characterText(characters[character], false).c_str(), FONT_GARAMOND, garamond_size, clrDB, textwidth - 2 * text_space, TTF_STYLE_NORMAL);
        }

        while (!done)
        {
            // Fill the surface with background
            fillWindow(renderer, intWH);

            auto adventurerh = splashw;

            if (adventurer != NULL)
            {
                adventurerh = fitImage(renderer, adventurer, startx, starty, splashw, text_bounds);
            }

            fillRect(renderer, textwidth, character_box, textx, texty, intBE);

            if (text != NULL)
            {
                renderText(renderer, text, intBE, textx + text_space, texty + text_space, character_box, 0);
            }

            if (character >= 0 && character < characters.size() && adventurer)
            {
                putText(renderer, characters[character].Name, font_mason, -1, clrDB, intWH, TTF_STYLE_NORMAL, splashw, infoh, startx, adventurerh + infoh - text_space);
            }

            if (selection.size() > 0)
            {
                putHeader(renderer, std::string("PARTY (Limit: " + std::to_string(party.Limit) + ")").c_str(), font_mason2, 8, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, starty + character_box + 10);

                std::string party_string = "";

                for (auto i = 0; i < selection.size(); i++)
                {
                    if (selection[i] >= 0 && selection[i] < characters.size())
                    {
                        if (i > 0)
                        {
                            party_string += ", ";
                        }

                        party_string += characters[selection[i]].Name;
                    }
                }

                putText(renderer, party_string.c_str(), font_garamond, 8, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, (text_bounds - character_box) - infoh - box_space, textx, starty + text_bounds + infoh - (text_bounds - character_box) + box_space);
            }

            if (Engine::FIND_LIST(selection, character) >= 0)
            {
                renderTextButtons(renderer, controls_del, FONT_DARK11, current, clrWH, intDB, intLB, font_size + 2, TTF_STYLE_NORMAL);

                controls = &controls_del;
            }
            else
            {
                renderTextButtons(renderer, controls_add, FONT_DARK11, current, clrWH, intDB, intLB, font_size + 2, TTF_STYLE_NORMAL);

                controls = &controls_add;
            }

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            if (Engine::FIND_LIST(selection, character) >= 0)
            {
                done = Input::GetInput(renderer, controls_del, current, selected, scrollUp, scrollDown, hold);
            }
            else
            {
                done = Input::GetInput(renderer, controls_add, current, selected, scrollUp, scrollDown, hold);
            }

            if (selected && current >= 0 && current < controls->size())
            {
                switch (controls->at(current).Type)
                {
                case Control::Type::BACK:

                    if (character > 0)
                    {
                        character--;
                    }

                    if (character >= 0 && character < characters.size())
                    {
                        if (characters[character].Image != NULL)
                        {
                            if (adventurer != NULL)
                            {
                                SDL_FreeSurface(adventurer);

                                adventurer = NULL;
                            }

                            adventurer = createImage(characters[character].Image);
                        }

                        if (text != NULL)
                        {
                            SDL_FreeSurface(text);

                            text = NULL;
                        }

                        text = createText(characterText(characters[character], false).c_str(), FONT_GARAMOND, garamond_size, clrDB, textwidth - 2 * text_space, TTF_STYLE_NORMAL);
                    }

                    break;

                case Control::Type::PLUS:

                    if (selection.size() < party.Limit)
                    {
                        selection.push_back(character);
                    }

                    break;

                case Control::Type::MINUS:

                    if (selection.size() > 0)
                    {
                        auto result = Engine::FIND_LIST(selection, character);

                        if (result >= 0)
                        {
                            selection.erase(selection.begin() + result);
                        }
                    }

                    break;

                case Control::Type::NEXT:

                    if (character < characters.size() - 1)
                    {
                        character++;
                    }

                    if (character >= 0 && character < characters.size())
                    {
                        if (characters[character].Image != NULL)
                        {
                            if (adventurer != NULL)
                            {
                                SDL_FreeSurface(adventurer);

                                adventurer = NULL;
                            }

                            adventurer = createImage(characters[character].Image);
                        }

                        if (text != NULL)
                        {
                            SDL_FreeSurface(text);

                            text = NULL;
                        }

                        text = createText(characterText(characters[character], false).c_str(), FONT_GARAMOND, garamond_size, clrDB, textwidth - 2 * text_space, TTF_STYLE_NORMAL);
                    }

                    break;

                case Control::Type::NEW:

                    if (selection.size() == party.Limit)
                    {
                        for (auto i = 0; i < selection.size(); i++)
                        {
                            if (selection[i] >= 0 && selection[i] < characters.size())
                            {
                                auto character = characters[selection[i]];

                                if (character.SpellCaster)
                                {
                                    // TODO: Update this for other books
                                    if (bookID == Book::Type::BOOK1)
                                    {
                                        auto spells_selected = 0;

                                        auto spell_selection = std::vector<int>();

                                        auto spellBook = Spells::BOOK1_SPELLS;

                                        while (spells_selected < 3)
                                        {
                                            spell_selection = selectSpell(window, renderer, spellBook, 3, Spells::Select::SPELLBOOK);

                                            spells_selected = spell_selection.size();
                                        }

                                        for (auto j = 0; j < spell_selection.size(); j++)
                                        {
                                            character.SpellBook.push_back(spellBook[spell_selection[j]]);
                                        }
                                    }
                                }

                                party.Party.push_back(character);
                            }
                        }

                        // TODO: Select spells for spellcasters

                        done = true;

                        current = -1;

                        selected = false;
                    }

                    break;

                case Control::Type::ABOUT:

                    current = -1;

                    selected = false;

                    break;

                case Control::Type::QUIT:

                    done = true;

                    break;

                default:

                    selected = false;

                    done = false;

                    break;
                }
            }

            SDL_SetWindowTitle(window, title);
        }
    }

    if (adventurer != NULL)
    {
        SDL_FreeSurface(adventurer);

        adventurer = NULL;
    }

    if (text != NULL)
    {
        SDL_FreeSurface(text);

        text = NULL;
    }

    if (font_mason)
    {
        TTF_CloseFont(font_mason);

        font_mason = NULL;
    }

    if (font_mason2)
    {
        TTF_CloseFont(font_mason2);

        font_mason2 = NULL;
    }

    if (font_garamond)
    {
        TTF_CloseFont(font_garamond);

        font_garamond = NULL;
    }

    TTF_Quit();

    return false;
}

bool mapScreen(SDL_Window *window, SDL_Renderer *renderer)
{
    auto done = false;

    auto map_valley = createImage("images/book1/valley-of-bones-map.png");
    auto map_kingdom = createImage("images/legendary-kingdoms-map-small.png");

    auto texture_valley = SDL_CreateTextureFromSurface(renderer, map_valley);
    auto texture_kingdom = SDL_CreateTextureFromSurface(renderer, map_kingdom);

    auto current_map = 1;

    // Render the image
    if (window && renderer && map_valley && map_kingdom)
    {
        auto selected = false;
        auto current = -1;

        auto marginw = (1.0 - 2.0 * Margin) * SCREEN_WIDTH;

        auto controls = std::vector<Button>();
        controls.push_back(Button(0, "icons/map.png", 0, 1, 0, 0, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(1, "icons/back-button.png", 0, 1, 1, 1, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        auto offset_x = 0;
        auto offset_y = 0;

        while (!done)
        {
            if (current_map == 1)
            {
                SDL_SetWindowTitle(window, "Map: The Valley of Bones");
            }
            else
            {
                SDL_SetWindowTitle(window, "Map: Legendary Kingdoms");
            }

            if (current_map == 1)
            {
                offset_x = (marginw - (double)text_bounds / map_valley->h * map_valley->w) / 2;
                offset_y = ((SCREEN_HEIGHT) - (double)text_bounds / map_valley->h * map_valley->h) / 2;
            }
            else
            {
                offset_x = (marginw - (double)text_bounds / map_kingdom->h * map_kingdom->w) / 2;
                offset_y = ((SCREEN_HEIGHT) - (double)text_bounds / map_kingdom->h * map_kingdom->h) / 2;
            }

            // Fill the surface with background color
            fillWindow(renderer, intWH);

            if (current_map == 1)
            {
                fitImage(renderer, map_valley, startx + offset_x, offset_y, marginw, text_bounds);
            }
            else
            {
                fitImage(renderer, map_kingdom, startx + offset_x, offset_y, marginw, text_bounds);
            }

            renderButtons(renderer, controls, current, intLB, 8, 4);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            if (map_valley && map_kingdom)
            {
                auto mousex = 0;
                auto mousey = 0;

                auto state = SDL_GetMouseState(&mousex, &mousey);

                auto zoomw = (int)(0.40 * (double)(marginw - 2 * offset_x));
                auto zoomh = (int)(0.40 * (double)text_bounds);

                if (zoomw > zoomh)
                {
                    zoomw = zoomh;
                }
                else
                {
                    zoomh = zoomw;
                }

                if (current_map == 1)
                {
                    clipValue(zoomw, 0, map_valley->w);
                    clipValue(zoomh, 0, map_valley->h);
                }
                else
                {
                    clipValue(zoomw, 0, map_kingdom->w);
                    clipValue(zoomh, 0, map_kingdom->h);
                }

                auto boundx = startx + offset_x + marginw;

                if (current_map == 1)
                {
                    boundx = (int)((double)text_bounds / map_valley->h * (double)map_valley->w);
                }
                else
                {
                    boundx = (int)((double)text_bounds / map_kingdom->h * (double)map_kingdom->w);
                }

                if ((mousex >= startx + offset_x) && mousex <= (startx + offset_x + boundx) && mousey >= offset_y && mousey <= (offset_y + text_bounds))
                {
                    auto scalex = (double)(mousex - (startx + offset_x)) / boundx;
                    auto scaley = (double)(mousey - offset_y) / text_bounds;

                    auto centerx = (int)(scalex * (double)map_kingdom->w);
                    auto centery = (int)(scaley * (double)map_kingdom->h);

                    clipValue(centerx, zoomw / 2, map_kingdom->w - zoomw / 2);
                    clipValue(centery, zoomh / 2, map_kingdom->h - zoomh / 2);

                    if (current_map == 1)
                    {
                        centerx = (int)(scalex * (double)map_valley->w);
                        centery = (int)(scaley * (double)map_valley->h);

                        clipValue(centerx, zoomw / 2, map_valley->w - zoomw / 2);
                        clipValue(centery, zoomh / 2, map_valley->h - zoomh / 2);
                    }

                    if ((current_map == 1 && texture_valley) || (current_map == 0 && texture_kingdom))
                    {
                        SDL_Rect src;

                        src.w = zoomw;
                        src.h = zoomh;
                        src.x = centerx - zoomw / 2;
                        src.y = centery - zoomh / 2;

                        SDL_Rect dst;

                        dst.w = zoomw;
                        dst.h = zoomh;
                        dst.x = mousex + buttonw / 4;
                        dst.y = mousey - (buttonh / 4 + zoomh);

                        clipValue(dst.x, buttonw / 4, SCREEN_WIDTH - buttonw / 4 - zoomw);
                        clipValue(dst.y, buttonh / 4, SCREEN_HEIGHT - buttonh / 4 - zoomh);

                        if (current_map == 1)
                        {
                            SDL_RenderCopy(renderer, texture_valley, &src, &dst);
                        }
                        else
                        {
                            SDL_RenderCopy(renderer, texture_kingdom, &src, &dst);
                        }

                        drawRect(renderer, dst.w, dst.h, dst.x, dst.y, intBK);
                    }
                }
            }

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size() && controls[current].Type == Control::Type::BACK)
            {
                break;
            }
            else if (selected && current >= 0 && current < controls.size() && controls[current].Type == Control::Type::MAP)
            {
                current_map = 1 - current_map;
            }
        }

        SDL_FreeSurface(map_valley);
        SDL_FreeSurface(map_kingdom);

        SDL_DestroyTexture(texture_valley);
        SDL_DestroyTexture(texture_kingdom);

        map_valley = NULL;
        map_kingdom = NULL;

        texture_valley = NULL;
        texture_kingdom = NULL;
    }

    return done;
}

std::vector<Button> equipmentList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Equipment::Base> list, int start, int last, int limit, bool confirm_button, bool back_button)
{
    auto font_size = 28;
    auto text_space = 8;
    auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

    auto controls = std::vector<Button>();

    if (list.size() > 0)
    {
        for (int i = 0; i < last - start; i++)
        {
            auto index = start + i;

            std::string item_string = list[index].Name;

            if (list[index].Attribute != Attribute::Type::NONE)
            {
                item_string += " (+ " + std::to_string(list[index].Modifier) + " " + std::string(Attribute::Descriptions[list[index].Attribute]) + ")";
            }

            auto text = createText(item_string.c_str(), FONT_GARAMOND, font_size, clrBK, textwidth - 4 * text_space, TTF_STYLE_NORMAL);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : texty + 2 * text_space);

            controls.push_back(Button(i, text, i, i, (i > 0 ? i - 1 : i), (i < (last - start) ? i + 1 : i), textx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = textwidth - 4 * text_space;

            controls[i].H = text->h;
        }
    }

    auto idx = controls.size();

    if (list.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (list.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    if (confirm_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/yes.png", idx - 1, back_button ? idx + 1 : idx, idx - 1, idx, startx, buttony, Control::Type::CONFIRM));
    }

    if (back_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/back-button.png", idx - 1, idx, list.size() > 0 ? (last - start) : idx, idx, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));
    }

    return controls;
}

std::vector<Button> spellList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Spells::Base> &spells, int start, int last, int limit, int offsetx, int offsety, int scrolly, bool confirm_button, bool back_button)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (spells.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto spell = spells[index];

            std::string spell_string = "";

            spell_string += spell.Name;

            spell_string += "\nType: " + std::string(Spells::ScopeDescriptions[spell.Scope]) + ", Charged: " + std::string(spell.Charged ? "Yes" : "No") + ", Recharge: " + std::to_string(spell.Recharge);

            auto button = createHeaderButton(window, spell_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, 0.125 * SCREEN_HEIGHT, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, button, i, i, (i > 0 ? i - 1 : i), (i < (last - start) ? i + 1 : i), offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = button->w;

            controls[i].H = button->h;
        }
    }

    auto idx = controls.size();

    if (spells.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (spells.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, scrolly, Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    if (confirm_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/yes.png", idx - 1, (back_button ? idx + 1 : idx), spells.size() > 0 ? (last - start) - 1 : idx, idx, startx, buttony, Control::Type::CONFIRM));
    }

    if (back_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/back-button.png", idx - 1, idx, spells.size() > 0 ? (last - start) - 1 : idx, idx, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));
    }

    return controls;
}

std::vector<Button> monsterList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Monster::Base> monsters, int start, int last, int limit, int offsetx, int offsety, bool confirm_button, bool back_button)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (monsters.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto monster = monsters[index];

            std::string monster_string = "";

            monster_string += monster.Name;

            monster_string += "\nAttack: " + std::to_string(monster.Attack) + " (" + std::to_string(monster.Difficulty) + "+)";

            if (monster.Auto > 0)
            {
                monster_string += " +" + std::to_string(monster.Auto) + " Auto";
            }

            monster_string += ", Defense: " + std::to_string(monster.Defence) + "+, Health: " + std::to_string(monster.Health);

            auto button = createHeaderButton(window, monster_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, 0.125 * SCREEN_HEIGHT, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, button, i, i, (i > 0 ? i - 1 : i), (i < (last - start) ? i + 1 : i), offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = button->w;

            controls[i].H = button->h;
        }
    }

    auto idx = controls.size();

    if (monsters.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (monsters.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    if (confirm_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/yes.png", idx - 1, (back_button ? idx + 1 : idx), monsters.size() > 0 ? (last - start) - 1 : idx, idx, startx, buttony, Control::Type::CONFIRM));
    }

    if (back_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/back-button.png", idx - 1, idx, monsters.size() > 0 ? (last - start) - 1 : idx, idx, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));
    }

    return controls;
}

std::vector<Button> combatantList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> party, int start, int last, int limit, int offsetx, int offsety, bool confirm_button, bool back_button)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (party.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto adventurer = party[index];

            std::string adventurer_string = characterText(adventurer, true);

            auto button = createHeaderButton(window, adventurer_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, 0.125 * SCREEN_HEIGHT, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, button, i, i, (i > 0 ? i - 1 : i), ((i < (last - start) - 1) ? i + 1 : ((back_button || confirm_button) ? i + 1 : i)), offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = button->w;

            controls[i].H = button->h;
        }
    }

    auto idx = controls.size();

    if (party.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (party.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    if (confirm_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/yes.png", idx, back_button ? idx + 1 : idx, party.size() > 0 ? (last - start) - 1 : idx, idx, startx, buttony, Control::Type::CONFIRM));
    }

    if (back_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/back-button.png", confirm_button ? idx - 1 : idx, idx, party.size() > 0 ? (last - start) - 1 : idx, idx, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));
    }

    return controls;
}

int armourSave(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, int damage)
{
    auto combat_damage = damage;

    if (character.Health > 0 && Engine::ARMOUR(character) > 0)
    {
        if (window && renderer)
        {
            auto flash_message = false;

            auto flash_color = intRD;

            std::string message = "";

            Uint32 start_ticks = 0;

            Uint32 duration = 3000;

            auto marginx = (int)(Margin * SCREEN_WIDTH);

            auto fullwidth = SCREEN_WIDTH - 2 * marginx;

            auto boxwidth = (SCREEN_WIDTH - 3 * marginx) / 2;

            auto headerw = (int)(boxwidth * 0.75);

            auto infoh = (int)(0.07 * SCREEN_HEIGHT);

            auto boxh = (int)(0.125 * SCREEN_HEIGHT);

            auto box_space = 10;

            auto main_buttonh = 48;

            auto done = false;

            auto stage = Engine::ArmourSave::START;

            SDL_SetWindowTitle(window, "Legendary Kingdoms 1 - The Valley of Bones: Armour Save");

            TTF_Init();

            auto font_mason = TTF_OpenFont(FONT_MASON, 32);

            auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 32);

            auto text_space = 8;

            auto font_size = 24;

            const char *choices_save[1] = {"Armour Save"};
            const char *choices_reduce[1] = {"Reduce Damage"};
            const char *choices_end[1] = {"Done"};

            SDL_Surface *dice[6];

            dice[0] = createImage("images/dice/dice1.png");
            dice[1] = createImage("images/dice/dice2.png");
            dice[2] = createImage("images/dice/dice3.png");
            dice[3] = createImage("images/dice/dice4.png");
            dice[4] = createImage("images/dice/dice5.png");
            dice[5] = createImage("images/dice/dice6.png");

            auto main_buttonw = 220;

            auto controls_save = createFixedTextButtons(choices_save, 1, main_buttonw, main_buttonh, 10, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);
            controls_save[0].Type = Control::Type::CONFIRM;

            auto controls_reduce = createFixedTextButtons(choices_reduce, 1, main_buttonw, main_buttonh, 10, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);
            controls_reduce[0].Type = Control::Type::CONFIRM;

            auto controls_end = createFixedTextButtons(choices_end, 1, main_buttonw, main_buttonh, 10, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);
            controls_end[0].Type = Control::Type::BACK;

            auto current = -1;

            auto selected = false;

            auto scrollUp = false;

            auto scrollDown = false;

            auto hold = false;

            auto focus = 0;

            std::vector<int> results = {};

            auto save_score = Engine::ARMOUR(character);

            if (save_score > combat_damage)
            {
                save_score = combat_damage;
            }

            auto size_dice = 64;

            auto cols = (fullwidth - 2 * box_space) / (size_dice + box_space);
            auto rows = (boxh * 3 - box_space) / (size_dice + box_space);

            auto controls = std::vector<TextButton>();

            auto reduced = false;
            auto assigned = false;

            auto final_damage = combat_damage;
            auto reduced_damage = 0;

            while (!done)
            {
                fillWindow(renderer, intWH);

                putHeader(renderer, "Armour save results", font_mason, 8, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty + infoh + boxh + box_space);
                fillRect(renderer, fullwidth, boxh * 3, startx, starty + infoh + boxh + box_space + infoh, intBE);

                if (stage != Engine::ArmourSave::START)
                {
                    if (stage == Engine::ArmourSave::SAVE)
                    {
                        if (results.size() == 0)
                        {
                            results = Engine::ROLL_DICE(save_score);
                        }
                    }

                    auto row = 0;
                    auto col = 0;

                    auto offsety = starty + infoh + boxh + box_space + infoh + box_space;
                    auto offsetx = startx + box_space;

                    for (auto i = 0; i < results.size(); i++)
                    {
                        if (results[i] >= 1 && results[i] <= 6)
                        {
                            auto result = results[i] - 1;

                            fitImage(renderer, dice[result], offsetx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), size_dice, size_dice);

                            if (stage == Engine::ArmourSave::REDUCE)
                            {
                                if (results[i] >= 4)
                                {
                                    thickRect(renderer, size_dice, size_dice, offsetx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), intLB, 2);

                                    reduced_damage++;
                                }
                            }

                            if (col < cols)
                            {
                                col++;
                            }
                            else
                            {
                                col = 0;

                                row++;
                            }
                        }
                    }
                }

                if (stage == Engine::ArmourSave::REDUCE)
                {
                    if (!reduced)
                    {
                        final_damage = combat_damage - reduced_damage;

                        flash_message = true;

                        if (final_damage > 0)
                        {
                            Engine::GAIN_HEALTH(character, -final_damage);

                            message = std::string(character.Name) + " DEALT " + std::to_string(final_damage) + " damage!";

                            flash_color = intRD;
                        }
                        else
                        {
                            message = std::string(character.Name) + " deflects the attack!";

                            flash_color = intLB;
                        }

                        start_ticks = SDL_GetTicks();

                        reduced = true;
                    }
                }

                std::string defender_string = "";

                fillRect(renderer, boxwidth, boxh, startx, starty + infoh, intBE);

                putHeader(renderer, character.Name, font_mason, 8, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);

                defender_string = "Armour Save: +" + std::to_string(save_score);
                defender_string += "\nHealth: " + std::to_string(character.Health);

                putText(renderer, defender_string.c_str(), font_garamond, 8, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx, starty + infoh);

                if (stage == Engine::ArmourSave::START)
                {
                    controls = controls_save;
                }
                else if (stage == Engine::ArmourSave::SAVE)
                {
                    controls = controls_reduce;
                }
                else if (stage == Engine::ArmourSave::REDUCE)
                {
                    controls = controls_end;
                }

                if (flash_message)
                {
                    if ((SDL_GetTicks() - start_ticks) < duration)
                    {
                        putHeader(renderer, message.c_str(), font_garamond, 8, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
                    }
                    else
                    {
                        flash_message = false;
                    }
                }

                renderTextButtons(renderer, controls, FONT_MASON, current, clrWH, intDB, intLB, font_size, TTF_STYLE_NORMAL);

                done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (stage == Engine::ArmourSave::START && controls[current].Type == Control::Type::CONFIRM)
                    {
                        stage = Engine::ArmourSave::SAVE;
                    }
                    else if (stage == Engine::ArmourSave::SAVE && controls[current].Type == Control::Type::CONFIRM)
                    {
                        stage = Engine::ArmourSave::REDUCE;
                    }
                    else if (stage == Engine::ArmourSave::REDUCE && controls[current].Type == Control::Type::BACK)
                    {
                        stage = Engine::ArmourSave::END;

                        done = true;

                        current = -1;

                        selected = false;

                        break;
                    }
                }
            }

            if (font_mason)
            {
                TTF_CloseFont(font_mason);

                font_mason = NULL;
            }

            if (font_garamond)
            {
                TTF_CloseFont(font_garamond);

                font_garamond = NULL;
            }

            TTF_Quit();

            for (auto i = 0; i < 6; i++)
            {
                if (dice[i])
                {
                    SDL_FreeSurface(dice[i]);

                    dice[i] = NULL;
                }
            }
        }
    }

    return combat_damage;
}

int assignDamage(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> &party)
{
    auto result = -1;

    auto title = "Legendary Kingdoms 1 - The Valley of Bones: Assign Damage";

    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 28);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto main_buttonh = 48;

        auto text_space = 8;
        auto messageh = (int)(0.25 * SCREEN_HEIGHT);
        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;
        auto offset = 0;
        auto limit = (text_bounds - text_space) / ((boxh) + 3 * text_space);
        auto last = offset + limit;

        if (last > party.size())
        {
            last = party.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = combatantList(window, renderer, party, offset, last, limit, textx, texty + infoh + text_space, true, false);

        auto done = false;

        auto selection = -1;

        while (!done)
        {
            auto current = -1;

            auto selected = false;

            auto scrollUp = false;

            auto scrollDown = false;

            auto hold = false;

            auto scrollSpeed = 1;

            auto space = 8;

            while (!done)
            {
                fillWindow(renderer, intWH);

                if (splash)
                {
                    fitImage(renderer, splash, startx, starty, splashw, text_bounds);
                }

                fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

                for (auto i = offset; i < last; i++)
                {
                    auto index = i - offset;

                    if (index >= 0 && index < party.size())
                    {
                        if (selection == index)
                        {
                            for (auto size = 4; size >= 0; size--)
                            {
                                auto w = controls[index].W + 2 * (8 - size);
                                auto h = controls[index].H + 2 * (8 - size);
                                auto x = controls[index].X - 8 + size;
                                auto y = controls[index].Y - 8 + size;

                                drawRect(renderer, w, h, x, y, intLB);
                            }
                        }
                        else if (party[index].Health > 0)
                        {
                            drawRect(renderer, controls[index].W + 8, controls[index].H + 8, controls[index].X - 4, controls[index].Y - 4, intBK);
                        }
                        else
                        {
                            drawRect(renderer, controls[index].W + 8, controls[index].H + 8, controls[index].X - 4, controls[index].Y - 4, intRD);
                        }
                    }
                }

                renderButtons(renderer, controls, current, intLB, space, 4);

                putHeader(renderer, "Party", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                putHeader(renderer, "Assign damage to", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (selection >= 0 && selection < party.size())
                {
                    if (party[selection].Health > 0)
                    {
                        putText(renderer, party[selection].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                    }
                    else
                    {
                        fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
                    }
                }
                else
                {
                    fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
                }

                if (flash_message)
                {
                    if ((SDL_GetTicks() - start_ticks) < duration)
                    {
                        putHeader(renderer, message.c_str(), font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
                    }
                    else
                    {
                        flash_message = false;
                    }
                }

                done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (controls[current].Type == Control::Type::BACK)
                    {
                        done = true;

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::CONFIRM)
                    {
                        if (selection >= 0 && selection < party.size())
                        {
                            done = true;

                            result = selection;

                            current = -1;

                            selected = false;
                        }
                        else
                        {
                            flash_message = true;

                            message = "You must SELECT an adventurer DAMAGED by the attack.";

                            start_ticks = SDL_GetTicks();

                            flash_color = intRD;
                        }
                    }
                    else if (controls[current].Type == Control::Type::ACTION)
                    {
                        if (current + offset >= 0 && current + offset < party.size())
                        {
                            if (selection == current + offset)
                            {
                                selection = -1;
                            }
                            else
                            {
                                if (party[current + offset].Health > 0)
                                {
                                    selection = current + offset;
                                }
                                else
                                {
                                    flash_message = true;

                                    message = std::string(party[current + offset].Name) + std::string(" is DEAD!");

                                    start_ticks = SDL_GetTicks();

                                    flash_color = intRD;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (font_garamond)
        {
            TTF_CloseFont(font_garamond);

            font_garamond = NULL;
        }

        if (font_dark11)
        {
            TTF_CloseFont(font_dark11);

            font_dark11 = NULL;
        }

        if (font_mason)
        {
            TTF_CloseFont(font_mason);

            font_mason = NULL;
        }

        TTF_Quit();

        if (splash)
        {
            SDL_FreeSurface(splash);

            splash = NULL;
        }
    }

    return result;
}

int magicAttackScreen(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> &party, std::vector<Monster::Base> &monsters, Spells::Base &spell, int combatant, int opponent, int fighting_score)
{
    auto combat_damage = 0;

    if (Engine::COUNT(party) > 0 && Engine::COUNT(monsters) > 0)
    {
        if (window && renderer)
        {
            auto flash_message = false;

            auto flash_color = intRD;

            std::string message = "";

            Uint32 start_ticks = 0;

            Uint32 duration = 3000;

            auto marginx = (int)(Margin * SCREEN_WIDTH);

            auto fullwidth = SCREEN_WIDTH - 2 * marginx;

            auto boxwidth = (SCREEN_WIDTH - 3 * marginx) / 2;

            auto headerw = (int)(boxwidth * 0.75);

            auto infoh = (int)(0.07 * SCREEN_HEIGHT);

            auto boxh = (int)(0.125 * SCREEN_HEIGHT);

            auto box_space = 10;

            auto main_buttonh = 48;

            auto done = false;

            auto stage = Engine::Attack::START;

            SDL_SetWindowTitle(window, "Legendary Kingdoms 1 - The Valley of Bones: Attack");

            TTF_Init();

            auto font_mason = TTF_OpenFont(FONT_MASON, 32);

            auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 32);

            auto text_space = 8;

            auto font_size = 24;

            const char *choices_attack[2] = {"Attack", "Cancel"};
            const char *choices_damage[1] = {"Deal Damage"};
            const char *choices_end[1] = {"Done"};

            SDL_Surface *dice[6];

            dice[0] = createImage("images/dice/dice1.png");
            dice[1] = createImage("images/dice/dice2.png");
            dice[2] = createImage("images/dice/dice3.png");
            dice[3] = createImage("images/dice/dice4.png");
            dice[4] = createImage("images/dice/dice5.png");
            dice[5] = createImage("images/dice/dice6.png");

            auto main_buttonw = 220;

            auto controls_attack = createFixedTextButtons(choices_attack, 2, main_buttonw, main_buttonh, 10, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);
            controls_attack[0].Type = Control::Type::CONFIRM;
            controls_attack[1].Type = Control::Type::BACK;

            auto controls_damage = createFixedTextButtons(choices_damage, 1, main_buttonw, main_buttonh, 10, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);
            controls_damage[0].Type = Control::Type::CONFIRM;
            controls_damage[0].W = controls_attack[0].W;

            auto controls_end = createFixedTextButtons(choices_end, 1, main_buttonw, main_buttonh, 10, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);
            controls_end[0].Type = Control::Type::BACK;
            controls_end[0].W = controls_attack[0].W;

            auto current = -1;

            auto selected = false;

            auto scrollUp = false;

            auto scrollDown = false;

            auto hold = false;

            auto focus = 0;

            std::vector<int> results = {};

            auto size_dice = 64;

            auto cols = (fullwidth - 2 * box_space) / (size_dice + box_space);
            auto rows = (boxh * 3 - box_space) / (size_dice + box_space);

            auto controls = std::vector<TextButton>();

            auto damaged = false;
            auto assigned = false;

            while (!done)
            {
                fillWindow(renderer, intWH);

                putHeader(renderer, "Attack Results", font_mason, 8, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty + infoh + boxh + box_space);
                fillRect(renderer, fullwidth, boxh * 3, startx, starty + infoh + boxh + box_space + infoh, intBE);

                if (stage != Engine::Attack::START)
                {
                    if (stage == Engine::Attack::ATTACK)
                    {
                        if (results.size() == 0)
                        {
                            results = Engine::ROLL_DICE(fighting_score);
                        }
                    }

                    auto row = 0;
                    auto col = 0;

                    auto offsety = starty + infoh + boxh + box_space + infoh + box_space;
                    auto offsetx = startx + box_space;

                    auto damage = 0;

                    for (auto i = 0; i < results.size(); i++)
                    {
                        if (results[i] >= 1 && results[i] <= 6)
                        {
                            auto result = results[i] - 1;

                            fitImage(renderer, dice[result], offsetx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), size_dice, size_dice);

                            if (stage == Engine::Attack::DAMAGE)
                            {
                                if (results[i] >= monsters[opponent].Defence)
                                {
                                    thickRect(renderer, size_dice, size_dice, offsetx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), intLB, 2);

                                    damage++;
                                }
                            }

                            if (col < cols)
                            {
                                col++;
                            }
                            else
                            {
                                col = 0;

                                row++;
                            }
                        }
                    }

                    if (stage == Engine::Attack::DAMAGE)
                    {
                        if (!damaged)
                        {
                            Engine::GAIN_HEALTH(monsters[opponent], -damage);

                            combat_damage = damage;

                            flash_message = true;

                            if (damage > 0)
                            {
                                monsters[opponent].Damaged = true;

                                message = std::string(party[combatant].Name) + "'s" + std::string(spell.Name) + " DEALS " + std::to_string(damage) + " to the " + std::string(monsters[opponent].Name) + "!";

                                flash_color = intLB;
                            }
                            else
                            {
                                message = std::string(party[combatant].Name) + "'s " + std::string(spell.Name) + " was INEFFECTIVE!";

                                flash_color = intRD;
                            }

                            start_ticks = SDL_GetTicks();

                            damaged = true;
                        }
                    }
                }

                putHeader(renderer, party[combatant].Name, font_mason, 8, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);
                fillRect(renderer, boxwidth, boxh, startx, starty + infoh, intBE);
                std::string attacker_string = "Magic Fighting Score: " + std::to_string(fighting_score);
                putText(renderer, attacker_string.c_str(), font_garamond, 8, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx, starty + infoh);

                putHeader(renderer, monsters[opponent].Name, font_mason, 8, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx + boxwidth + marginx, starty);
                fillRect(renderer, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh, intBE);
                std::string defender_string = "Defence: " + std::to_string(monsters[opponent].Defence) + "+\nHealth: " + std::to_string(monsters[opponent].Health);
                putText(renderer, defender_string.c_str(), font_garamond, 8, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh);

                std::string spell_string = "SPELL: " + std::string(spell.Name);
                putHeader(renderer, spell_string.c_str(), font_mason, 8, clrWH, intBR, TTF_STYLE_NORMAL, boxwidth, infoh, startx, starty + 2 * infoh + 4 * boxh + 2 * box_space);

                if (stage == Engine::Attack::START)
                {
                    controls = controls_attack;
                }
                else if (stage == Engine::Attack::ATTACK)
                {
                    controls = controls_damage;
                }
                else if (stage == Engine::Attack::DAMAGE)
                {
                    controls = controls_end;
                }

                if (flash_message)
                {
                    if ((SDL_GetTicks() - start_ticks) < duration)
                    {
                        putHeader(renderer, message.c_str(), font_garamond, 8, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
                    }
                    else
                    {
                        flash_message = false;
                    }
                }

                renderTextButtons(renderer, controls, FONT_MASON, current, clrWH, intDB, intLB, font_size, TTF_STYLE_NORMAL);

                done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (stage == Engine::Attack::START && controls[current].Type == Control::Type::BACK)
                    {
                        combat_damage = -1;

                        done = true;

                        current = -1;

                        selected = false;

                        break;
                    }
                    else if (stage == Engine::Attack::START && controls[current].Type == Control::Type::CONFIRM)
                    {
                        stage = Engine::Attack::ATTACK;
                    }
                    else if (stage == Engine::Attack::ATTACK && controls[current].Type == Control::Type::CONFIRM)
                    {
                        stage = Engine::Attack::DAMAGE;
                    }
                    else if (stage == Engine::Attack::DAMAGE && controls[current].Type == Control::Type::BACK)
                    {
                        stage = Engine::Attack::END;

                        done = true;

                        current = -1;

                        selected = false;

                        break;
                    }
                }
            }

            if (font_mason)
            {
                TTF_CloseFont(font_mason);

                font_mason = NULL;
            }

            if (font_garamond)
            {
                TTF_CloseFont(font_garamond);

                font_garamond = NULL;
            }

            TTF_Quit();

            for (auto i = 0; i < 6; i++)
            {
                if (dice[i])
                {
                    SDL_FreeSurface(dice[i]);

                    dice[i] = NULL;
                }
            }
        }
    }

    return combat_damage;
}

int attackScreen(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> &party, std::vector<Monster::Base> &monsters, int combatant, int opponent, int direction, bool useEquipment)
{
    auto combat_damage = 0;

    if (Engine::COUNT(party) > 0 && Engine::COUNT(monsters) > 0)
    {
        if (window && renderer)
        {
            auto flash_message = false;

            auto flash_color = intRD;

            std::string message = "";

            Uint32 start_ticks = 0;

            Uint32 duration = 3000;

            auto marginx = (int)(Margin * SCREEN_WIDTH);

            auto fullwidth = SCREEN_WIDTH - 2 * marginx;

            auto boxwidth = (SCREEN_WIDTH - 3 * marginx) / 2;

            auto headerw = (int)(boxwidth * 0.75);

            auto infoh = (int)(0.07 * SCREEN_HEIGHT);

            auto boxh = (int)(0.125 * SCREEN_HEIGHT);

            auto box_space = 10;

            auto main_buttonh = 48;

            auto done = false;

            auto stage = Engine::Attack::START;

            SDL_SetWindowTitle(window, "Legendary Kingdoms 1 - The Valley of Bones: Attack");

            TTF_Init();

            auto font_mason = TTF_OpenFont(FONT_MASON, 32);

            auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 32);

            auto text_space = 8;

            auto font_size = 24;

            const char *choices_attack[4] = {"Attack", "Add Focus", "Remove Focus", "Cancel"};
            const char *choices_defend[1] = {"Attack"};
            const char *choices_damage[1] = {"Deal Damage"};
            const char *choices_end[1] = {"Done"};
            const char *choices_assign[1] = {"Assign"};

            SDL_Surface *dice[6];

            dice[0] = createImage("images/dice/dice1.png");
            dice[1] = createImage("images/dice/dice2.png");
            dice[2] = createImage("images/dice/dice3.png");
            dice[3] = createImage("images/dice/dice4.png");
            dice[4] = createImage("images/dice/dice5.png");
            dice[5] = createImage("images/dice/dice6.png");

            auto main_buttonw = 220;

            auto controls_attack = createFixedTextButtons(choices_attack, 4, main_buttonw, main_buttonh, 10, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);
            controls_attack[0].Type = Control::Type::CONFIRM;
            controls_attack[1].Type = Control::Type::PLUS;
            controls_attack[2].Type = Control::Type::MINUS;
            controls_attack[3].Type = Control::Type::BACK;

            auto controls_defend = createFixedTextButtons(choices_defend, 1, main_buttonw, main_buttonh, 10, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);
            controls_defend[0].Type = Control::Type::CONFIRM;

            auto controls_damage = createFixedTextButtons(choices_damage, 1, main_buttonw, main_buttonh, 10, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);
            controls_damage[0].Type = Control::Type::CONFIRM;

            auto controls_end = createFixedTextButtons(choices_end, 1, main_buttonw, main_buttonh, 10, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);
            controls_end[0].Type = Control::Type::BACK;

            auto controls_assign = createFixedTextButtons(choices_assign, 1, main_buttonw, main_buttonh, 10, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);
            controls_assign[0].Type = Control::Type::CONFIRM;

            auto current = -1;

            auto selected = false;

            auto scrollUp = false;

            auto scrollDown = false;

            auto hold = false;

            auto focus = 0;

            std::vector<int> results = {};

            auto attack_score = 1;

            auto size_dice = 64;

            auto cols = (fullwidth - 2 * box_space) / (size_dice + box_space);
            auto rows = (boxh * 3 - box_space) / (size_dice + box_space);

            auto controls = std::vector<TextButton>();

            auto damaged = false;
            auto assigned = false;

            auto special_event_trigger = true;

            while (!done)
            {
                fillWindow(renderer, intWH);

                putHeader(renderer, "Attack Results", font_mason, 8, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty + infoh + boxh + box_space);

                fillRect(renderer, fullwidth, boxh * 3, startx, starty + infoh + boxh + box_space + infoh, intBE);

                if (stage != Engine::Attack::START)
                {
                    if (stage == Engine::Attack::ATTACK)
                    {
                        if (results.size() == 0)
                        {
                            results = Engine::ROLL_DICE(attack_score);
                        }
                    }

                    auto row = 0;
                    auto col = 0;

                    auto offsety = starty + infoh + boxh + box_space + infoh + box_space;
                    auto offsetx = startx + box_space;

                    int damage = 0;

                    for (auto i = 0; i < results.size(); i++)
                    {
                        if (results[i] >= 1 && results[i] <= 6)
                        {
                            auto result = results[i] - 1;

                            fitImage(renderer, dice[result], offsetx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), size_dice, size_dice);

                            if (stage == Engine::Attack::DAMAGE)
                            {
                                if (direction == 0)
                                {
                                    if (results[i] >= monsters[opponent].Defence)
                                    {
                                        thickRect(renderer, size_dice, size_dice, offsetx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), intLB, 2);

                                        damage++;
                                    }
                                }
                                else
                                {
                                    if (results[i] >= monsters[opponent].Difficulty)
                                    {
                                        thickRect(renderer, size_dice, size_dice, offsetx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), intRD, 2);

                                        damage++;
                                    }
                                }
                            }

                            if (col < cols)
                            {
                                col++;
                            }
                            else
                            {
                                col = 0;

                                row++;
                            }
                        }
                    }

                    if (direction == 1)
                    {
                        damage += monsters[opponent].Auto;
                    }

                    if (stage == Engine::Attack::DAMAGE)
                    {
                        if (!damaged)
                        {
                            if (direction == 0)
                            {
                                Engine::GAIN_HEALTH(monsters[opponent], -damage);

                                combat_damage = damage;

                                flash_message = true;

                                if (damage > 0)
                                {
                                    monsters[opponent].Damaged = true;

                                    message = std::string(party[combatant].Name) + " DEALS " + std::to_string(damage) + " to the " + std::string(monsters[opponent].Name) + "!";

                                    flash_color = intLB;
                                }
                                else
                                {
                                    message = std::string(party[combatant].Name) + "'s ATTACK was INEFFECTIVE!";

                                    flash_color = intRD;
                                }

                                start_ticks = SDL_GetTicks();
                            }
                            else
                            {
                                combat_damage = damage;

                                if (combat_damage > 0)
                                {
                                    message = std::string(monsters[opponent].Name) + " DEALS " + std::to_string(damage) + " to the party!";

                                    start_ticks = SDL_GetTicks();

                                    flash_message = true;

                                    flash_color = intRD;
                                }
                                else
                                {
                                    assigned = true;
                                }
                            }

                            damaged = true;
                        }
                    }
                }
                else
                {
                    if (direction == 1 && special_event_trigger)
                    {
                        if (monsters[opponent].Type == Monster::Type::SKALLOS && !monsters[opponent].Damaged)
                        {
                            flash_message = true;

                            flash_color = intRD;

                            message = "Skallos unleashes a roar of black magic! Each party member LOSES 1 Health. Skallos RECOVERS 4 Health Points!";

                            start_ticks = SDL_GetTicks();

                            Engine::GAIN_HEALTH(party, -1);

                            Engine::GAIN_HEALTH(monsters[opponent], 4);
                        }
                        else if (monsters[opponent].Type == Monster::Type::SNAKEMAN_PRIEST)
                        {
                            if (Engine::VERIFY_EQUIPMENT(party, {Equipment::Type::HYGLIPH_FLOWER}))
                            {
                                flash_message = true;

                                flash_color = intRD;

                                message = "The priest is put off by the pungent odour of the HYGLIPH FLOWER and requires a 5+ to his attack rolls to inflict damage during this battle.";

                                start_ticks = SDL_GetTicks();

                                monsters[opponent].Difficulty = 5;
                            }
                        }

                        special_event_trigger = false;
                    }
                }

                std::string attacker_string = "";

                fillRect(renderer, boxwidth, boxh, startx, starty + infoh, intBE);

                if (direction == 0)
                {
                    putHeader(renderer, party[combatant].Name, font_mason, 8, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);

                    auto score = 1;

                    if (useEquipment)
                    {
                        score = Engine::FIGHTING_SCORE(party[combatant]);
                    }
                    else
                    {
                        score = Engine::SCORE(party[combatant], Attribute::Type::FIGHTING);
                    }

                    attacker_string = "Fighting: " + std::to_string(score);
                    attacker_string += "\nHealth: " + std::to_string(party[combatant].Health);

                    attack_score = score;
                }
                else
                {
                    putHeader(renderer, monsters[opponent].Name, font_mason, 8, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);

                    attacker_string = "Attack: " + std::to_string(monsters[opponent].Attack) + " (" + std::to_string(monsters[opponent].Difficulty) + "+)";

                    if (monsters[opponent].Auto > 0)
                    {
                        attacker_string += " +" + std::to_string(monsters[opponent].Auto) + " Auto";
                    }

                    attacker_string += "\nHealth: " + std::to_string(monsters[opponent].Health);

                    attack_score = monsters[opponent].Attack;
                }

                putText(renderer, attacker_string.c_str(), font_garamond, 8, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx, starty + infoh);

                std::string defender_string = "";

                if (direction == 0)
                {
                    putHeader(renderer, monsters[opponent].Name, font_mason, 8, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx + boxwidth + marginx, starty);
                    defender_string = "Defence: " + std::to_string(monsters[opponent].Defence) + "+";
                    defender_string += "\nHealth: " + std::to_string(monsters[opponent].Health);
                }
                else
                {
                    if (combatant == -1)
                    {
                        putHeader(renderer, "To be determined", font_mason, 8, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx + boxwidth + marginx, starty);
                    }
                    else
                    {
                        putHeader(renderer, party[combatant].Name, font_mason, 8, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx + boxwidth + marginx, starty);
                        defender_string = "Health: " + std::to_string(party[combatant].Health);
                    }
                }

                fillRect(renderer, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh, intBE);

                putText(renderer, defender_string.c_str(), font_garamond, 8, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh);

                if (stage == Engine::Attack::START)
                {
                    if (direction == 0)
                    {
                        controls = controls_attack;
                    }
                    else
                    {
                        controls = controls_defend;
                    }
                }
                else if (stage == Engine::Attack::ATTACK)
                {
                    controls = controls_damage;
                }
                else if (stage == Engine::Attack::DAMAGE)
                {
                    if (direction == 0)
                    {
                        controls = controls_end;
                    }
                    else
                    {
                        if (!assigned)
                        {
                            controls = controls_assign;
                        }
                        else
                        {
                            controls = controls_end;
                        }
                    }
                }

                if (flash_message)
                {
                    if ((SDL_GetTicks() - start_ticks) < duration)
                    {
                        putHeader(renderer, message.c_str(), font_garamond, 8, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
                    }
                    else
                    {
                        flash_message = false;
                    }
                }

                renderTextButtons(renderer, controls, FONT_MASON, current, clrWH, intDB, intLB, font_size, TTF_STYLE_NORMAL);

                done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (stage == Engine::Attack::START && controls[current].Type == Control::Type::BACK)
                    {
                        done = true;

                        current = -1;

                        selected = false;

                        break;
                    }
                    else if (stage == Engine::Attack::START && controls[current].Type == Control::Type::CONFIRM)
                    {
                        stage = Engine::Attack::ATTACK;
                    }
                    else if (stage == Engine::Attack::ATTACK && controls[current].Type == Control::Type::CONFIRM)
                    {
                        stage = Engine::Attack::DAMAGE;
                    }
                    else if (stage == Engine::Attack::DAMAGE && controls[current].Type == Control::Type::BACK)
                    {
                        stage = Engine::Attack::END;

                        done = true;

                        current = -1;

                        selected = false;

                        break;
                    }
                    else if (stage == Engine::Attack::DAMAGE && controls[current].Type == Control::Type::CONFIRM)
                    {
                        if (Engine::COUNT(party) > 0)
                        {
                            if (combat_damage > 0)
                            {
                                auto result = assignDamage(window, renderer, party);

                                if (result >= 0 && result < party.size())
                                {
                                    if (party[result].Health > 0)
                                    {
                                        assigned = true;

                                        if (Engine::ARMOUR(party[result]) > 0)
                                        {
                                            auto reduced_damage = armourSave(window, renderer, party[result], combat_damage);

                                            done = true;

                                            selected = false;

                                            current = -1;

                                            break;
                                        }
                                        else
                                        {
                                            message = std::string(party[result].Name) + " DEALT " + std::to_string(combat_damage) + " DAMAGE!";

                                            start_ticks = SDL_GetTicks();

                                            flash_message = true;

                                            flash_color = intRD;

                                            Engine::GAIN_HEALTH(party[result], -combat_damage);

                                            combatant = result;
                                        }
                                    }
                                    else
                                    {
                                        message = std::string(party[result].Name) + " is already DEAD!";

                                        start_ticks = SDL_GetTicks();

                                        flash_message = true;

                                        flash_color = intRD;
                                    }
                                }
                            }
                            else
                            {
                                assigned = true;

                                message = "The " + std::string(monsters[opponent].Name) + "'s attack was INEFFECTIVE!";

                                start_ticks = SDL_GetTicks();

                                flash_message = true;

                                flash_color = intRD;
                            }
                        }
                        else
                        {
                            done = true;

                            selected = false;

                            current = -1;

                            break;
                        }
                    }
                }
            }

            if (font_mason)
            {
                TTF_CloseFont(font_mason);

                font_mason = NULL;
            }

            if (font_garamond)
            {
                TTF_CloseFont(font_garamond);

                font_garamond = NULL;
            }

            TTF_Quit();

            for (auto i = 0; i < 6; i++)
            {
                if (dice[i])
                {
                    SDL_FreeSurface(dice[i]);

                    dice[i] = NULL;
                }
            }
        }
    }

    return combat_damage;
}

std::vector<int> selectSpell(SDL_Window *window, SDL_Renderer *renderer, std::vector<Spells::Base> &spells, int select_limit, Spells::Select mode)
{
    auto select_result = std::vector<int>();

    auto title = "Legendary Kingdoms 1 - The Valley of Bones: Select Opponent";

    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_garamond2 = TTF_OpenFont(FONT_GARAMOND, 28);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto main_buttonh = 48;

        auto font_size = 24;
        auto text_space = 8;
        auto messageh = (int)(0.25 * SCREEN_HEIGHT);
        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;
        auto offset = 0;
        auto booksize = (int)(2 * (text_bounds) / 3);
        auto limit = (int)((booksize - text_space) / ((boxh) + 3 * text_space));
        auto last = offset + limit;

        if (last > spells.size())
        {
            last = spells.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto scrolly = startx + infoh + booksize - buttonh - text_space + 1;

        auto controls = spellList(window, renderer, spells, offset, last, limit, textx, texty + infoh + text_space, scrolly, true, (mode == Spells::Select::CAST_SPELL ? true : false));

        auto done = false;

        auto selection = std::vector<int>();

        while (!done)
        {
            auto current = -1;

            auto selected = false;

            auto scrollUp = false;

            auto scrollDown = false;

            auto hold = false;

            auto scrollSpeed = 1;

            auto space = 8;

            while (!done)
            {
                fillWindow(renderer, intWH);

                if (splash)
                {
                    fitImage(renderer, splash, startx, starty, splashw, text_bounds);
                }

                fillRect(renderer, textwidth, booksize, textx, texty + infoh, intBE);

                if (last - offset > 0)
                {
                    for (auto i = 0; i < last - offset; i++)
                    {
                        if (Engine::FIND_LIST(selection, offset + i) >= 0)
                        {
                            thickRect(renderer, controls[i].W, controls[i].H, controls[i].X, controls[i].Y, intLB, 2);
                        }
                        else if (spells[offset + i].Charged)
                        {
                            drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X - 4, controls[i].Y - 4, intBK);
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X - 4, controls[i].Y - 4, intRD);
                        }
                    }
                }

                if (current >= 0 && current < limit)
                {
                    fillRect(renderer, textwidth, text_bounds / 3 - box_space - infoh, textx, texty + infoh + booksize + box_space, intLB);

                    auto text = createText(spells[current + offset].Description, FONT_GARAMOND, font_size, clrWH, textwidth - 2 * text_space, TTF_STYLE_NORMAL);

                    renderText(renderer, text, intLB, textx + text_space, texty + infoh + booksize + box_space + text_space, text_bounds / 3 - texty, 0);

                    SDL_FreeSurface(text);

                    text = NULL;
                }

                renderButtons(renderer, controls, current, intLB, space, 4);

                std::string list_header = "Choose " + (select_limit > 1 ? std::to_string(select_limit) : "a") + " spell" + (select_limit > 1 ? "s" : "") + " to ";

                if (mode == Spells::Select::SPELLBOOK)
                {
                    list_header += "add to spellbook";
                }
                else if (mode == Spells::Select::CAST_SPELL)
                {
                    list_header = "cast";
                }
                else if (mode == Spells::Select::UNLEARN)
                {
                    list_header = "unlearn";
                }

                putHeader(renderer, list_header.c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                putHeader(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (selection.size() > 0)
                {
                    std::string spells_string = "";

                    for (auto i = 0; i < selection.size(); i++)
                    {
                        if (spells[selection[i]].Charged)
                        {
                            if (spells_string.length() > 0)
                            {
                                spells_string += ",";
                            }

                            spells_string += spells[selection[i]].Name;
                        }
                    }

                    if (spells_string.length() > 0)
                    {
                        putText(renderer, spells_string.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                    }
                    else
                    {
                        fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
                    }
                }
                else
                {
                    fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
                }

                if (flash_message)
                {
                    if ((SDL_GetTicks() - start_ticks) < duration)
                    {
                        putHeader(renderer, message.c_str(), font_garamond2, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
                    }
                    else
                    {
                        flash_message = false;
                    }
                }

                done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
                {
                    if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                    {
                        if (offset > 0)
                        {
                            offset -= scrollSpeed;

                            if (offset < 0)
                            {
                                offset = 0;
                            }

                            last = offset + limit;

                            if (last > spells.size())
                            {
                                last = spells.size();
                            }

                            controls = spellList(window, renderer, spells, offset, last, limit, textx, texty + infoh + text_space, scrolly, true, false);

                            SDL_Delay(50);
                        }

                        if (offset <= 0)
                        {
                            current = -1;

                            selected = false;
                        }
                    }
                    else if (controls[current].Type == Control::Type::SCROLL_DOWN || ((controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown))
                    {
                        if (spells.size() - last > 0)
                        {
                            if (offset < spells.size() - limit)
                            {
                                offset += scrollSpeed;
                            }

                            if (offset > spells.size() - limit)
                            {
                                offset = spells.size() - limit;
                            }

                            last = offset + limit;

                            if (last > spells.size())
                            {
                                last = spells.size();
                            }

                            controls = spellList(window, renderer, spells, offset, last, limit, textx, texty + infoh + text_space, scrolly, true, false);

                            SDL_Delay(50);

                            if (offset > 0)
                            {
                                if (controls[current].Type != Control::Type::SCROLL_DOWN)
                                {
                                    current++;
                                }
                            }
                        }

                        if (spells.size() - last <= 0)
                        {
                            selected = false;

                            current = -1;
                        }
                    }
                    else if (controls[current].Type == Control::Type::BACK)
                    {
                        select_result.clear();

                        done = true;

                        current = -1;

                        selected = false;

                        break;
                    }
                    else if (controls[current].Type == Control::Type::CONFIRM)
                    {
                        if (selection.size() > 0 && selection.size() >= select_limit)
                        {
                            done = true;

                            for (auto i = 0; i < selection.size(); i++)
                            {
                                select_result.push_back(selection[i]);
                            }

                            current = -1;

                            selected = false;
                        }
                        else
                        {
                            flash_message = true;

                            if (mode == Spells::Select::CAST_SPELL)
                            {
                                message = "You must select a spell to CAST.";
                            }
                            else if (mode == Spells::Select::SPELLBOOK)
                            {
                                message = "You must select spells to add to your spellbook.";
                            }
                            else if (mode == Spells::Select::UNLEARN)
                            {
                                message = "You must select a spell to erase from your spellbook.";
                            }

                            start_ticks = SDL_GetTicks();

                            flash_color = intRD;
                        }
                    }
                    else if (controls[current].Type == Control::Type::ACTION)
                    {
                        if (current + offset >= 0 && current + offset < spells.size())
                        {
                            auto result = Engine::FIND_LIST(selection, current + offset);

                            if (result >= 0)
                            {
                                selection.erase(selection.begin() + result);
                            }
                            else
                            {
                                if (spells[current + offset].Charged)
                                {
                                    if (selection.size() < select_limit)
                                    {
                                        selection.push_back(current + offset);
                                    }
                                }
                                else
                                {
                                    flash_message = true;

                                    message = std::string(spells[current + offset].Name) + std::string(" is not USABLE!");

                                    start_ticks = SDL_GetTicks();

                                    flash_color = intRD;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (font_garamond)
        {
            TTF_CloseFont(font_garamond);

            font_garamond = NULL;
        }

        if (font_garamond2)
        {
            TTF_CloseFont(font_garamond2);

            font_garamond2 = NULL;
        }

        if (font_dark11)
        {
            TTF_CloseFont(font_dark11);

            font_dark11 = NULL;
        }

        if (font_mason)
        {
            TTF_CloseFont(font_mason);

            font_mason = NULL;
        }

        TTF_Quit();

        if (splash)
        {
            SDL_FreeSurface(splash);

            splash = NULL;
        }
    }

    return select_result;
}

int selectOpponent(SDL_Window *window, SDL_Renderer *renderer, std::vector<Monster::Base> &monsters, std::vector<int> previousTargets)
{
    auto result = -1;

    auto title = "Legendary Kingdoms 1 - The Valley of Bones: Select Opponent";

    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto main_buttonh = 48;

        auto font_size = 20;
        auto text_space = 8;
        auto messageh = (int)(0.25 * SCREEN_HEIGHT);
        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;
        auto offset = 0;
        auto limit = (text_bounds - text_space) / ((boxh) + 3 * text_space);
        auto last = offset + limit;

        if (last > monsters.size())
        {
            last = monsters.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = monsterList(window, renderer, monsters, offset, last, limit, textx, texty + infoh + text_space, true, false);

        auto done = false;

        auto selection = -1;

        while (!done)
        {
            auto current = -1;

            auto selected = false;

            auto scrollUp = false;

            auto scrollDown = false;

            auto hold = false;

            auto scrollSpeed = 1;

            auto space = 8;

            while (!done)
            {
                fillWindow(renderer, intWH);

                if (splash)
                {
                    fitImage(renderer, splash, startx, starty, splashw, text_bounds);
                }

                fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

                if (last - offset > 0)
                {
                    for (auto i = 0; i < last - offset; i++)
                    {
                        if (selection == offset + i)
                        {
                            thickRect(renderer, controls[i].W, controls[i].H, controls[i].X, controls[i].Y, intLB, 2);
                        }
                        else if (monsters[offset + i].Health > 0)
                        {
                            drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X - 4, controls[i].Y - 4, intBK);
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X - 4, controls[i].Y - 4, intRD);
                        }
                    }
                }

                renderButtons(renderer, controls, current, intLB, space, 4);

                putHeader(renderer, "Select Opponent", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                putHeader(renderer, "Opponent", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (selection >= 0 && selection < monsters.size())
                {
                    if (monsters[selection].Health > 0)
                    {
                        putText(renderer, monsters[selection].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                    }
                    else
                    {
                        fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
                    }
                }
                else
                {
                    fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
                }

                if (flash_message)
                {
                    if ((SDL_GetTicks() - start_ticks) < duration)
                    {
                        putHeader(renderer, message.c_str(), font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
                    }
                    else
                    {
                        flash_message = false;
                    }
                }

                done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (controls[current].Type == Control::Type::BACK)
                    {
                        done = true;

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::CONFIRM)
                    {
                        if (selection >= 0 && selection < monsters.size())
                        {
                            if (previousTargets.size() > 0)
                            {
                                if (Engine::FIND_LIST(previousTargets, selection) >= 0)
                                {
                                    flash_message = true;

                                    message = "That opponent has been ATTACKED before! Choose another target!";

                                    start_ticks = SDL_GetTicks();

                                    flash_color = intRD;
                                }
                                else
                                {
                                    done = true;

                                    result = selection;

                                    current = -1;

                                    selected = false;
                                }
                            }
                            else
                            {
                                done = true;

                                result = selection;

                                current = -1;

                                selected = false;
                            }
                        }
                        else
                        {
                            flash_message = true;

                            message = "You must SELECT an opponent to ATTACK this round.";

                            start_ticks = SDL_GetTicks();

                            flash_color = intRD;
                        }
                    }
                    else if (controls[current].Type == Control::Type::ACTION)
                    {
                        if (current + offset >= 0 && current + offset < monsters.size())
                        {
                            if (selection == current + offset)
                            {
                                selection = -1;
                            }
                            else
                            {
                                if (monsters[current + offset].Health > 0)
                                {
                                    selection = current + offset;
                                }
                                else
                                {
                                    flash_message = true;

                                    message = std::string(monsters[current + offset].Name) + std::string(" is DEAD!");

                                    start_ticks = SDL_GetTicks();

                                    flash_color = intRD;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (font_garamond)
        {
            TTF_CloseFont(font_garamond);

            font_garamond = NULL;
        }

        if (font_garamond)
        {
            TTF_CloseFont(font_garamond);

            font_garamond = NULL;
        }

        if (font_dark11)
        {
            TTF_CloseFont(font_dark11);

            font_dark11 = NULL;
        }

        if (font_mason)
        {
            TTF_CloseFont(font_mason);

            font_mason = NULL;
        }

        TTF_Quit();

        if (splash)
        {
            SDL_FreeSurface(splash);

            splash = NULL;
        }
    }

    return result;
}

bool skillTestScreen(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> &party, std::vector<int> team, Attribute::Type Skill, int difficulty, int success, bool useEquipment)
{
    bool test_result = false;

    if (Engine::COUNT(party) > 0 && Engine::COUNT(party) >= team.size())
    {
        if (window && renderer)
        {
            auto flash_message = false;

            auto flash_color = intRD;

            std::string message = "";

            Uint32 start_ticks = 0;

            Uint32 duration = 3000;

            auto marginx = (int)(Margin * SCREEN_WIDTH);

            auto fullwidth = SCREEN_WIDTH - 2 * marginx;

            auto boxwidth = (SCREEN_WIDTH - 3 * marginx) / 2;

            auto headerw = (int)(boxwidth * 0.75);

            auto infoh = (int)(0.07 * SCREEN_HEIGHT);

            auto boxh = (int)(0.125 * SCREEN_HEIGHT);

            auto box_space = 10;

            auto main_buttonh = 48;

            auto done = false;

            auto stage = Attribute::Test::START;

            SDL_SetWindowTitle(window, "Legendary Kingdoms 1 - The Valley of Bones: Skill Check");

            TTF_Init();

            auto font_mason = TTF_OpenFont(FONT_MASON, 32);

            auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 32);

            auto text_space = 8;

            auto font_size = 24;

            const char *choices_skill[3] = {"Skill Check", "Add Focus", "Remove Focus"};
            const char *choices_confirm[1] = {"Confirm"};
            const char *choices_end[1] = {"Done"};

            SDL_Surface *dice[6];

            dice[0] = createImage("images/dice/dice1.png");
            dice[1] = createImage("images/dice/dice2.png");
            dice[2] = createImage("images/dice/dice3.png");
            dice[3] = createImage("images/dice/dice4.png");
            dice[4] = createImage("images/dice/dice5.png");
            dice[5] = createImage("images/dice/dice6.png");

            auto main_buttonw = 220;

            auto controls_skill = createFixedTextButtons(choices_skill, 3, main_buttonw, main_buttonh, 10, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);
            controls_skill[0].Type = Control::Type::CONFIRM;
            controls_skill[1].Type = Control::Type::PLUS;
            controls_skill[2].Type = Control::Type::MINUS;

            auto controls_confirm = createFixedTextButtons(choices_confirm, 1, main_buttonw, main_buttonh, 10, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);
            controls_confirm[0].Type = Control::Type::CONFIRM;

            auto controls_end = createFixedTextButtons(choices_end, 1, main_buttonw, main_buttonh, 10, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);
            controls_end[0].Type = Control::Type::BACK;

            auto current = -1;

            auto selected = false;

            auto scrollUp = false;

            auto scrollDown = false;

            auto hold = false;

            auto focus = 0;

            std::vector<int> results = {};

            auto skill_score = 0;

            for (auto i = 0; i < team.size(); i++)
            {
                skill_score += Engine::SCORE(party[team[i]], Skill) + Engine::MAX(party[team[i]], Equipment::Class::NORMAL, Skill);
            }

            if (skill_score > 20)
            {
                skill_score = 20;
            }

            auto size_dice = 64;

            auto cols = (fullwidth - 2 * box_space) / (size_dice + box_space);
            auto rows = (boxh * 3 - box_space) / (size_dice + box_space);

            auto controls = std::vector<TextButton>();

            auto skill_checked = false;

            int success_counter = 0;

            while (!done)
            {
                fillWindow(renderer, intWH);

                putHeader(renderer, "Skill Check Results", font_mason, 8, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty + infoh + boxh + box_space);

                fillRect(renderer, fullwidth, boxh * 3, startx, starty + infoh + boxh + box_space + infoh, intBE);

                if (stage != Attribute::Test::START)
                {
                    if (stage == Attribute::Test::CONFIRM)
                    {
                        if (results.size() == 0)
                        {
                            results = Engine::ROLL_DICE(skill_score);
                        }
                    }

                    auto row = 0;
                    auto col = 0;

                    auto offsety = starty + infoh + boxh + box_space + infoh + box_space;
                    auto offsetx = startx + box_space;

                    for (auto i = 0; i < results.size(); i++)
                    {
                        if (results[i] >= 1 && results[i] <= 6)
                        {
                            auto result = results[i] - 1;

                            fitImage(renderer, dice[result], offsetx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), size_dice, size_dice);

                            if (stage == Attribute::Test::CHECK)
                            {
                                if (results[i] >= difficulty)
                                {
                                    thickRect(renderer, size_dice, size_dice, offsetx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), intLB, 2);

                                    success_counter++;
                                }
                            }

                            if (col < cols)
                            {
                                col++;
                            }
                            else
                            {
                                col = 0;

                                row++;
                            }
                        }
                    }

                    if (stage == Attribute::Test::CHECK)
                    {
                        if (!skill_checked)
                        {
                            flash_message = true;

                            if (success_counter >= success)
                            {
                                message = "Skill Check PASSED!";

                                flash_color = intLB;

                                test_result = true;
                            }
                            else
                            {
                                message = "Skill Check FAILED!";

                                flash_color = intRD;

                                test_result = false;
                            }

                            start_ticks = SDL_GetTicks();

                            skill_checked = true;
                        }
                    }
                }

                std::string test_string = std::string(Attribute::Descriptions[Skill]) + ": " + std::to_string(difficulty) + "+, Success: " + std::to_string(success);

                putHeader(renderer, test_string.c_str(), font_mason, 8, clrWH, intBR, TTF_STYLE_NORMAL, boxwidth, infoh, startx, starty + 2 * infoh + 4 * boxh + 2 * box_space);

                putHeader(renderer, party[team[0]].Name, font_mason, 8, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);

                fillRect(renderer, boxwidth, boxh, startx, starty + infoh, intBE);

                auto score1 = 0;

                if (useEquipment)
                {
                    score1 = Engine::SCORE(party[team[0]], Skill) + Engine::MAX(party[team[0]], Equipment::Class::NORMAL, Skill);
                }
                else
                {
                    score1 = Engine::SCORE(party[team[0]], Skill);
                }

                std::string adventurer1 = std::string(Attribute::Descriptions[Skill]) + ": " + std::to_string(score1);

                putText(renderer, adventurer1.c_str(), font_garamond, 8, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx, starty + infoh);

                if (team.size() > 1)
                {
                    putHeader(renderer, party[team[1]].Name, font_mason, 8, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx + boxwidth + marginx, starty);

                    fillRect(renderer, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh, intBE);

                    auto score2 = 0;

                    if (useEquipment)
                    {
                        score2 = Engine::SCORE(party[team[1]], Skill) + Engine::MAX(party[team[1]], Equipment::Class::NORMAL, Skill);
                    }
                    else
                    {
                        score2 = Engine::SCORE(party[team[1]], Skill);
                    }

                    std::string adventurer2 = std::string(Attribute::Descriptions[Skill]) + ": " + std::to_string(score2);

                    putText(renderer, adventurer2.c_str(), font_garamond, 8, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh);
                }

                if (stage == Attribute::Test::START)
                {
                    controls = controls_skill;
                }
                else if (stage == Attribute::Test::CONFIRM)
                {
                    controls = controls_confirm;
                }
                else if (stage == Attribute::Test::CHECK)
                {
                    controls = controls_end;
                }

                if (flash_message)
                {
                    if ((SDL_GetTicks() - start_ticks) < duration)
                    {
                        putHeader(renderer, message.c_str(), font_garamond, 8, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
                    }
                    else
                    {
                        flash_message = false;
                    }
                }

                renderTextButtons(renderer, controls, FONT_MASON, current, clrWH, intDB, intLB, font_size, TTF_STYLE_NORMAL);

                done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (stage == Attribute::Test::START && controls[current].Type == Control::Type::CONFIRM)
                    {
                        stage = Attribute::Test::CONFIRM;
                    }
                    else if (stage == Attribute::Test::CONFIRM && controls[current].Type == Control::Type::CONFIRM)
                    {
                        stage = Attribute::Test::CHECK;
                    }
                    else if (stage == Attribute::Test::CHECK && controls[current].Type == Control::Type::BACK)
                    {
                        stage = Attribute::Test::END;

                        done = true;

                        current = -1;

                        selected = false;

                        break;
                    }
                }
            }

            if (font_mason)
            {
                TTF_CloseFont(font_mason);

                font_mason = NULL;
            }

            if (font_garamond)
            {
                TTF_CloseFont(font_garamond);

                font_garamond = NULL;
            }

            TTF_Quit();

            for (auto i = 0; i < 6; i++)
            {
                if (dice[i])
                {
                    SDL_FreeSurface(dice[i]);

                    dice[i] = NULL;
                }
            }
        }
    }

    return test_result;
}

int selectCaster(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> &party, std::vector<Monster::Base> &monsters, std::vector<int> hasAttacked, Control::Type mode)
{
    auto result = -1;

    auto title = "Legendary Kingdoms 1 - The Valley of Bones: Cast Spell";

    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto main_buttonh = 48;

        auto font_size = 20;
        auto text_space = 8;
        auto messageh = (int)(0.25 * SCREEN_HEIGHT);
        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;
        auto offset = 0;
        auto limit = (text_bounds - text_space) / ((boxh) + 3 * text_space);
        auto last = offset + limit;

        if (last > party.size())
        {
            last = party.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = combatantList(window, renderer, party, offset, last, limit, textx, texty + infoh + text_space, true, true);

        auto done = false;

        auto selection = -1;

        while (!done)
        {
            auto current = -1;

            auto selected = false;

            auto scrollUp = false;

            auto scrollDown = false;

            auto hold = false;

            auto scrollSpeed = 1;

            auto space = 8;

            while (!done)
            {
                fillWindow(renderer, intWH);

                if (splash)
                {
                    fitImage(renderer, splash, startx, starty, splashw, text_bounds);
                }

                fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

                if (last - offset > 0)
                {
                    for (auto i = 0; i < last - offset; i++)
                    {
                        if (selection == offset + i)
                        {
                            thickRect(renderer, controls[i].W, controls[i].H, controls[i].X, controls[i].Y, intLB, 2);
                        }
                        else if (party[offset + i].Health > 0)
                        {
                            drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X - 4, controls[i].Y - 4, intBK);
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X - 4, controls[i].Y - 4, intRD);
                        }
                    }
                }

                renderButtons(renderer, controls, current, intLB, space, 4);

                putHeader(renderer, "Select Caster", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                putHeader(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (selection >= 0 && selection < party.size())
                {
                    putText(renderer, party[selection].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                }
                else
                {
                    fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
                }

                if (flash_message)
                {
                    if ((SDL_GetTicks() - start_ticks) < duration)
                    {
                        putHeader(renderer, message.c_str(), font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
                    }
                    else
                    {
                        flash_message = false;
                    }
                }

                done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (controls[current].Type == Control::Type::BACK)
                    {
                        done = true;

                        current = -1;

                        selected = false;

                        result = -1;

                        break;
                    }
                    else if (controls[current].Type == Control::Type::CONFIRM)
                    {
                        if (selection >= 0 && selection < party.size())
                        {
                            if (party[selection].SpellCaster)
                            {
                                if (hasAttacked.size() > 0 && Engine::FIND_LIST(hasAttacked, selection) >= 0)
                                {
                                    flash_message = true;

                                    message = std::string(party[selection].Name) + " already attacked this turn!";

                                    start_ticks = SDL_GetTicks();

                                    flash_color = intRD;
                                }
                                else
                                {
                                    auto spell = selectSpell(window, renderer, party[selection].SpellBook, 1, Spells::Select::CAST_SPELL);

                                    if (spell.size() > 0)
                                    {
                                        auto i = spell[0];

                                        if (mode == Control::Type::COMBAT)
                                        {
                                            if (party[selection].SpellBook[i].Scope == Spells::Scope::COMBAT || party[selection].SpellBook[i].Scope == Spells::Scope::ADVENTURE_COMBAT)
                                            {
                                                auto cast = false;

                                                if (party[selection].SpellBook[i].Type == Spells::Type::ARMOUR_OF_HEAVEN)
                                                {
                                                    auto target = selectPartyMember(window, renderer, party, Control::Type::SPELL_TARGET);

                                                    if (Engine::HAS_STATUS(party[target], Character::Status::ARMOUR3))
                                                    {
                                                        flash_message = true;

                                                        message = std::string(party[target].Name) + " already has the " + std::string(party[selection].SpellBook[i].Name) + "!";

                                                        start_ticks = SDL_GetTicks();

                                                        flash_color = intRD;
                                                    }
                                                    else
                                                    {
                                                        Engine::GAIN_STATUS(party[target], Character::Status::ARMOUR3);

                                                        cast = true;
                                                    }
                                                }
                                                else if (party[selection].SpellBook[i].Type == Spells::Type::ICE_BOLT)
                                                {
                                                    if (Engine::COUNT(monsters) > 0)
                                                    {
                                                        auto target = selectOpponent(window, renderer, monsters, {});

                                                        if (target >= 0)
                                                        {
                                                            auto damage = magicAttackScreen(window, renderer, party, monsters, party[selection].SpellBook[i], selection, target, 8);

                                                            if (damage >= 0)
                                                            {
                                                                cast = true;
                                                            }
                                                        }
                                                    }
                                                    else
                                                    {
                                                        flash_message = true;

                                                        message = "There are no targets for " + std::string(party[selection].SpellBook[i].Name) + "!";

                                                        start_ticks = SDL_GetTicks();

                                                        flash_color = intRD;
                                                    }
                                                }
                                                else if (party[selection].SpellBook[i].Type == Spells::Type::UNFAILING_STRIKE)
                                                {
                                                    if (Engine::COUNT(monsters) > 0)
                                                    {
                                                        auto target = selectOpponent(window, renderer, monsters, {});

                                                        if (target >= 0)
                                                        {
                                                            monsters[target].Damaged = true;

                                                            Engine::GAIN_HEALTH(monsters[target], -3);

                                                            cast = true;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        flash_message = true;

                                                        message = "There are no targets for " + std::string(party[selection].SpellBook[i].Name) + "!";

                                                        start_ticks = SDL_GetTicks();

                                                        flash_color = intRD;
                                                    }
                                                }
                                                else if (party[selection].SpellBook[i].Type == Spells::Type::POSION_STREAM)
                                                {
                                                    if (Engine::COUNT(monsters) > 0)
                                                    {
                                                        auto max_targets = std::min(2, Engine::COUNT(monsters));

                                                        auto targets = std::vector<int>();

                                                        while (targets.size() < max_targets)
                                                        {
                                                            auto target = selectOpponent(window, renderer, monsters, targets);

                                                            if (target >= 0)
                                                            {
                                                                auto damage = magicAttackScreen(window, renderer, party, monsters, party[selection].SpellBook[i], selection, target, 5);

                                                                if (damage >= 0)
                                                                {
                                                                    targets.push_back(target);

                                                                    cast = true;
                                                                }
                                                                else
                                                                {
                                                                    break;
                                                                }
                                                            }
                                                            else
                                                            {
                                                                break;
                                                            }
                                                        }
                                                    }
                                                    else
                                                    {
                                                        flash_message = true;

                                                        message = "There are no targets for " + std::string(party[selection].SpellBook[i].Name) + "!";

                                                        start_ticks = SDL_GetTicks();

                                                        flash_color = intRD;
                                                    }
                                                }

                                                if (cast)
                                                {
                                                    party[selection].SpellBook[i].Charged = false;

                                                    result = selection;

                                                    done = true;
                                                }
                                            }
                                            else
                                            {
                                                flash_message = true;

                                                message = std::string(party[selection].Name) + " cannot cast " + std::string(party[selection].SpellBook[spell[0]].Name) + " during COMBAT!";

                                                start_ticks = SDL_GetTicks();

                                                flash_color = intRD;
                                            }
                                        }
                                        else if (mode == Control::Type::ADVENTURE)
                                        {
                                            if (party[selection].SpellBook[spell[0]].Scope == Spells::Scope::ADVENTURE || party[selection].SpellBook[spell[0]].Scope == Spells::Scope::ADVENTURE_COMBAT)
                                            {
                                                // TODO: Cast Spell
                                                result = selection;

                                                done = true;
                                            }
                                            else
                                            {
                                                flash_message = true;

                                                message = std::string(party[selection].Name) + " cannot cast " + std::string(party[selection].SpellBook[spell[0]].Name) + " at this time!";

                                                start_ticks = SDL_GetTicks();

                                                flash_color = intRD;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        selected = false;

                                        current = -1;
                                    }
                                }
                            }
                            else
                            {
                                flash_message = true;

                                message = std::string(party[selection].Name) + " cannot cast spells!";

                                start_ticks = SDL_GetTicks();

                                flash_color = intRD;
                            }
                        }
                        else
                        {
                            flash_message = true;

                            message = "You must select the adventurer to cast a spell.";

                            start_ticks = SDL_GetTicks();

                            flash_color = intRD;
                        }
                    }
                    else if (controls[current].Type == Control::Type::ACTION)
                    {
                        if (current + offset >= 0 && current + offset < party.size())
                        {
                            if (selection == current + offset)
                            {
                                selection = -1;
                            }
                            else
                            {
                                if (party[current + offset].Health > 0)
                                {
                                    selection = current + offset;
                                }
                                else
                                {
                                    flash_message = true;

                                    message = std::string(party[current + offset].Name) + std::string(" is DEAD!");

                                    start_ticks = SDL_GetTicks();

                                    flash_color = intRD;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (font_garamond)
        {
            TTF_CloseFont(font_garamond);

            font_garamond = NULL;
        }

        if (font_dark11)
        {
            TTF_CloseFont(font_dark11);

            font_dark11 = NULL;
        }

        if (font_mason)
        {
            TTF_CloseFont(font_mason);

            font_mason = NULL;
        }

        TTF_Quit();

        if (splash)
        {
            SDL_FreeSurface(splash);

            splash = NULL;
        }
    }

    return result;
}

bool skillCheck(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> &party, int team_size, Attribute::Type skill, int difficulty, int success, std::vector<int> &selection)
{
    bool result = false;

    auto title = "Legendary Kingdoms 1 - The Valley of Bones: Skill Check";

    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto main_buttonh = 48;

        auto font_size = 20;
        auto text_space = 8;
        auto messageh = (int)(0.25 * SCREEN_HEIGHT);
        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;
        auto offset = 0;
        auto limit = (text_bounds - text_space) / ((boxh) + 3 * text_space);
        auto last = offset + limit;

        if (last > party.size())
        {
            last = party.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = combatantList(window, renderer, party, offset, last, limit, textx, texty + infoh + text_space, true, true);

        auto done = false;

        selection = std::vector<int>();

        while (!done)
        {
            auto current = -1;

            auto selected = false;

            auto scrollUp = false;

            auto scrollDown = false;

            auto hold = false;

            auto scrollSpeed = 1;

            auto space = 8;

            while (!done)
            {
                fillWindow(renderer, intWH);

                if (splash)
                {
                    fitImage(renderer, splash, startx, starty, splashw, text_bounds);
                }

                fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

                for (auto i = offset; i < last; i++)
                {
                    auto index = i - offset;

                    if (index >= 0 && index < party.size())
                    {
                        if (Engine::FIND_LIST(selection, index) >= 0)
                        {
                            thickRect(renderer, controls[index].W, controls[index].H, controls[index].X, controls[index].Y, intLB, 2);
                        }
                        else if (party[index].Health > 0)
                        {
                            drawRect(renderer, controls[index].W + 8, controls[index].H + 8, controls[index].X - 4, controls[index].Y - 4, intBK);
                        }
                        else
                        {
                            drawRect(renderer, controls[index].W + 8, controls[index].H + 8, controls[index].X - 4, controls[index].Y - 4, intRD);
                        }
                    }
                }

                renderButtons(renderer, controls, current, intLB, space, 4);

                if (team_size > 1)
                {
                    putHeader(renderer, "Select Adventurers", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, "Select Adventurer", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }

                putHeader(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (selection.size() >= 0)
                {
                    std::string adventurers = "";

                    for (auto i = 0; i < selection.size(); i++)
                    {
                        if (selection[i] >= 0 && selection[i] < party.size())
                        {
                            if (party[selection[i]].Health > 0)
                            {
                                if (adventurers.length() > 0)
                                {
                                    adventurers += "\n";
                                }

                                adventurers += party[selection[i]].Name;
                            }
                        }
                    }

                    if (adventurers.length() > 0)
                    {
                        putText(renderer, adventurers.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                    }
                    else
                    {
                        fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
                    }
                }
                else
                {
                    fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
                }

                if (flash_message)
                {
                    if ((SDL_GetTicks() - start_ticks) < duration)
                    {
                        putHeader(renderer, message.c_str(), font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
                    }
                    else
                    {
                        flash_message = false;
                    }
                }

                done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (controls[current].Type == Control::Type::BACK)
                    {
                        done = true;

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::CONFIRM)
                    {
                        if (selection.size() >= team_size && selection.size() <= party.size())
                        {
                            done = true;

                            result = skillTestScreen(window, renderer, party, selection, skill, difficulty, success, true);

                            current = -1;

                            selected = false;
                        }
                        else
                        {
                            flash_message = true;

                            message = "You must select the adventurer(s) who will perform the skill check.";

                            start_ticks = SDL_GetTicks();

                            flash_color = intRD;
                        }
                    }
                    else if (controls[current].Type == Control::Type::ACTION)
                    {
                        if (current + offset >= 0 && current + offset < party.size())
                        {
                            auto index = Engine::FIND_LIST(selection, current + offset);

                            if (index >= 0)
                            {
                                selection.erase(selection.begin() + index);
                            }
                            else
                            {
                                if (selection.size() < team_size)
                                {
                                    if (party[current + offset].Health > 0)
                                    {
                                        selection.push_back(current + offset);
                                    }
                                    else
                                    {
                                        flash_message = true;

                                        message = std::string(party[current + offset].Name) + std::string(" is DEAD!");

                                        start_ticks = SDL_GetTicks();

                                        flash_color = intRD;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (font_garamond)
        {
            TTF_CloseFont(font_garamond);

            font_garamond = NULL;
        }

        if (font_dark11)
        {
            TTF_CloseFont(font_dark11);

            font_dark11 = NULL;
        }

        if (font_mason)
        {
            TTF_CloseFont(font_mason);

            font_mason = NULL;
        }

        TTF_Quit();

        if (splash)
        {
            SDL_FreeSurface(splash);

            splash = NULL;
        }
    }

    return result;
}

int selectPartyMember(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> &party, Control::Type mode)
{
    auto result = -1;

    auto title = "Legendary Kingdoms 1 - The Valley of Bones: Select Party Member";

    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto main_buttonh = 48;

        auto font_size = 20;
        auto text_space = 8;
        auto messageh = (int)(0.25 * SCREEN_HEIGHT);
        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;
        auto offset = 0;
        auto limit = (text_bounds - text_space) / ((boxh) + 3 * text_space);
        auto last = offset + limit;

        if (last > party.size())
        {
            last = party.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = combatantList(window, renderer, party, offset, last, limit, textx, texty + infoh + text_space, true, true);

        auto done = false;

        auto selection = -1;

        while (!done)
        {
            auto current = -1;

            auto selected = false;

            auto scrollUp = false;

            auto scrollDown = false;

            auto hold = false;

            auto scrollSpeed = 1;

            auto space = 8;

            while (!done)
            {
                fillWindow(renderer, intWH);

                if (splash)
                {
                    fitImage(renderer, splash, startx, starty, splashw, text_bounds);
                }

                fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

                if (last - offset > 0)
                {
                    for (auto i = 0; i < last - offset; i++)
                    {
                        if (selection == offset + i)
                        {
                            thickRect(renderer, controls[i].W, controls[i].H, controls[i].X, controls[i].Y, intLB, 2);
                        }
                        else if (party[offset + i].Health > 0)
                        {
                            drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X - 4, controls[i].Y - 4, intBK);
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X - 4, controls[i].Y - 4, intRD);
                        }
                    }
                }

                renderButtons(renderer, controls, current, intLB, space, 4);

                if (mode == Control::Type::COMBAT)
                {
                    putHeader(renderer, "Choose the adventurer attacking this round", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (mode == Control::Type::SPELL_TARGET)
                {
                    putHeader(renderer, "Choose target for this spell", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (mode == Control::Type::EQUIPMENT)
                {
                    putHeader(renderer, "Give the item(s) to", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, "Party", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }

                putHeader(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (selection >= 0 && selection < party.size())
                {
                    if (party[selection].Health > 0)
                    {
                        putText(renderer, party[selection].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                    }
                    else
                    {
                        fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
                    }
                }
                else
                {
                    fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
                }

                if (flash_message)
                {
                    if ((SDL_GetTicks() - start_ticks) < duration)
                    {
                        putHeader(renderer, message.c_str(), font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
                    }
                    else
                    {
                        flash_message = false;
                    }
                }

                done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (controls[current].Type == Control::Type::BACK)
                    {
                        done = true;

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::CONFIRM)
                    {
                        if (selection >= 0 && selection < party.size())
                        {
                            done = true;

                            result = selection;

                            current = -1;

                            selected = false;
                        }
                        else
                        {
                            flash_message = true;

                            message = "You must SELECT a party member!";

                            start_ticks = SDL_GetTicks();

                            flash_color = intRD;
                        }
                    }
                    else if (controls[current].Type == Control::Type::ACTION)
                    {
                        if (current + offset >= 0 && current + offset < party.size())
                        {
                            if (selection == current + offset)
                            {
                                selection = -1;
                            }
                            else
                            {
                                if (party[current + offset].Health > 0)
                                {
                                    selection = current + offset;
                                }
                                else
                                {
                                    flash_message = true;

                                    message = std::string(party[current + offset].Name) + std::string(" is DEAD!");

                                    start_ticks = SDL_GetTicks();

                                    flash_color = intRD;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (font_garamond)
        {
            TTF_CloseFont(font_garamond);

            font_garamond = NULL;
        }

        if (font_dark11)
        {
            TTF_CloseFont(font_dark11);

            font_dark11 = NULL;
        }

        if (font_mason)
        {
            TTF_CloseFont(font_mason);

            font_mason = NULL;
        }

        TTF_Quit();

        if (splash)
        {
            SDL_FreeSurface(splash);

            splash = NULL;
        }
    }

    return result;
}

Engine::Combat combatScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Monster::Base> &monsters, bool canFlee, int fleeRound, bool useEquipment)
{
    auto combatResult = Engine::Combat::NONE;

    auto title = "Legendary Kindoms 1 - The Valley of Bones: Combat";

    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto main_buttonh = 48;

        const char *choices_attack[4] = {"VIEW PARTY", "ATTACK", "CAST SPELL", "FLEE"};
        const char *choices_defend[4] = {"VIEW PARTY", "DEFEND", "CAST SPELL", "FLEE"};

        std::vector<TextButton> controls;

        auto font_size = 20;
        auto text_space = 8;
        auto messageh = (int)(0.25 * SCREEN_HEIGHT);
        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;
        auto offset = 0;
        auto limit = (text_bounds - text_space) / ((boxh) + 3 * text_space);
        auto last = offset + limit;

        if (last > monsters.size())
        {
            last = monsters.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto monster_list = monsterList(window, renderer, monsters, offset, last, limit, textx, texty + infoh + text_space, false, false);

        auto hasAttacked = std::vector<int>();

        if (Engine::VERIFY_CODES(party, {Codes::Type::LAST_IN_COMBAT}))
        {
            for (auto i = 0; i < party.Party.size(); i++)
            {
                if (party.Party[i].Health > 0)
                {
                    hasAttacked.push_back(i);
                }
            }

            if (fleeRound == 0)
            {
                fleeRound++;
            }
        }

        int combatRound = 0;

        while (Engine::COUNT(monsters) > 0 && Engine::COUNT(party.Party) > 0)
        {
            auto done = false;

            auto current = -1;

            auto selected = false;

            auto scrollUp = false;

            auto scrollDown = false;

            auto hold = false;

            auto scrollSpeed = 1;

            auto space = 8;

            while (!done)
            {
                if (hasAttacked.size() < Engine::COUNT(party.Party))
                {
                    controls = createHTextButtons(choices_attack, 4, main_buttonh, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);
                }
                else
                {
                    controls = createHTextButtons(choices_defend, 4, main_buttonh, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);
                }

                controls[0].Type = Control::Type::PARTY;
                controls[1].Type = Control::Type::ATTACK;
                controls[2].Type = Control::Type::SPELL;
                controls[3].Type = Control::Type::FLEE;

                fillWindow(renderer, intWH);

                if (splash)
                {
                    fitImage(renderer, splash, startx, starty, splashw, text_bounds);
                }

                fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

                renderButtons(renderer, monster_list, -1, intBK, space, 4);

                for (auto i = 0; i < monster_list.size(); i++)
                {
                    if (monsters[offset + i].Health > 0)
                    {
                        drawRect(renderer, monster_list[i].W, monster_list[i].H, monster_list[i].X, monster_list[i].Y, intBK);
                    }
                    else
                    {
                        drawRect(renderer, monster_list[i].W, monster_list[i].H, monster_list[i].X, monster_list[i].Y, intRD);
                    }
                }

                putHeader(renderer, "Opponents", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                putHeader(renderer, "Party", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (Engine::COUNT(party.Party) > 0)
                {
                    std::string party_string = "";

                    auto count = 0;

                    for (auto i = 0; i < party.Party.size(); i++)
                    {
                        if (count > 0)
                        {
                            party_string += "\n";
                        }

                        party_string += party.Party[i].Name;

                        if (party.Party[i].Health <= 0)
                        {
                            party_string += " (D)";
                        }
                        else
                        {
                            if (Engine::FIND_LIST(hasAttacked, i) >= 0)
                            {
                                party_string += " (A)";
                            }
                        }

                        count++;
                    }

                    putText(renderer, party_string.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                }
                else
                {
                    fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
                }

                renderTextButtons(renderer, controls, FONT_MASON, current, clrWH, intDB, intLB, font_size + 2, TTF_STYLE_NORMAL);

                if (flash_message)
                {
                    if ((SDL_GetTicks() - start_ticks) < duration)
                    {
                        putHeader(renderer, message.c_str(), font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
                    }
                    else
                    {
                        flash_message = false;
                    }
                }

                done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (controls[current].Type == Control::Type::FLEE)
                    {
                        if (canFlee)
                        {
                            if (fleeRound == -1)
                            {
                                done = true;

                                combatResult = Engine::Combat::FLEE;
                            }
                            else if (combatRound == fleeRound)
                            {
                                done = true;

                                combatResult = Engine::Combat::FLEE;
                            }
                            else
                            {
                                flash_message = true;

                                if (combatRound < fleeRound)
                                {
                                    message = "You CANNOT FLEE at this time.";
                                }
                                else
                                {
                                    message = "You CAN NO LONGER FLEE from this combat.";
                                }

                                start_ticks = SDL_GetTicks();

                                flash_color = intRD;
                            }
                        }
                        else
                        {
                            flash_message = true;

                            message = "You CANNOT FLEE from this combat.";

                            start_ticks = SDL_GetTicks();

                            flash_color = intRD;
                        }
                    }
                    else if (controls[current].Type == Control::Type::PARTY)
                    {
                        viewParty(window, renderer, party);

                        selected = false;

                        current = -1;
                    }
                    else if (controls[current].Type == Control::Type::ATTACK)
                    {
                        if (Engine::COUNT(party.Party) > 0 && hasAttacked.size() < Engine::COUNT(party.Party))
                        {
                            auto result = selectPartyMember(window, renderer, party.Party, Control::Type::COMBAT);

                            if (Engine::FIND_LIST(hasAttacked, result) >= 0)
                            {
                                flash_message = true;

                                message = std::string(party.Party[result].Name) + " ALREADY ATTACKED this round.";

                                start_ticks = SDL_GetTicks();

                                flash_color = intRD;
                            }
                            else
                            {
                                if (result >= 0 && result < party.Party.size())
                                {
                                    auto adventurer = party.Party[result];

                                    if (adventurer.Health > 0)
                                    {
                                        if (Engine::COUNT(monsters) > 0)
                                        {
                                            auto attack = selectOpponent(window, renderer, monsters, {});

                                            if (attack >= 0)
                                            {
                                                auto damage = attackScreen(window, renderer, party.Party, monsters, result, attack, 0, useEquipment);

                                                hasAttacked.push_back(result);

                                                if (canFlee)
                                                {
                                                    canFlee = false;
                                                }
                                            }
                                            else
                                            {
                                                flash_message = true;

                                                message = "The attack has failed!";

                                                start_ticks = SDL_GetTicks();

                                                flash_color = intRD;
                                            }
                                        }
                                        else
                                        {
                                            flash_message = true;

                                            message = "Your party has prevailed!";

                                            start_ticks = SDL_GetTicks();

                                            flash_color = intLB;
                                        }
                                    }
                                    else
                                    {
                                        flash_message = true;

                                        message = std::string(party.Party[result].Name) + " is DEAD.";

                                        start_ticks = SDL_GetTicks();

                                        flash_color = intRD;
                                    }
                                }
                            }
                        }

                        if (hasAttacked.size() >= Engine::COUNT(party.Party))
                        {
                            if (Engine::COUNT(monsters) > 0)
                            {
                                for (auto i = 0; i < monsters.size(); i++)
                                {
                                    if (monsters[i].Health > 0 && Engine::COUNT(party.Party) > 0)
                                    {
                                        if (monsters[i].Type == Monster::Type::ZEALOT_HEALER && Engine::COUNT(monsters) == 1)
                                        {
                                            attackScreen(window, renderer, party.Party, monsters, -1, i, 1, useEquipment);
                                        }
                                        else
                                        {
                                            attackScreen(window, renderer, party.Party, monsters, -1, i, 1, useEquipment);
                                        }
                                    }
                                }
                            }

                            // After combat round trigger
                            if (Engine::HAS_MONSTER(monsters, Monster::Type::SNAKEMAN_PRIEST))
                            {
                                flash_message = true;

                                flash_color = intRD;

                                message = "Blocks of stone come raining down from the walls! The priest and each party member LOSES 1 Health!";

                                Engine::GAIN_HEALTH(party.Party, -1);

                                Engine::GAIN_HEALTH(monsters, -1);

                                start_ticks = SDL_GetTicks();
                            }
                            else if (Engine::HAS_MONSTER(monsters, Monster::Type::ZEALOT_HEALER) && Engine::COUNT(monsters) > 1)
                            {
                                flash_message = true;

                                flash_color = intRD;

                                message = "The zealot healer HEALS each ZEALOT for 2 Health Points!";

                                Engine::GAIN_HEALTH(monsters, 2);

                                start_ticks = SDL_GetTicks();
                            }

                            hasAttacked.clear();

                            combatRound++;

                            // clear damaged flag for next round
                            for (auto i = 0; i < monsters.size(); i++)
                            {
                                monsters[i].Damaged = false;
                            }
                        }

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::SPELL)
                    {
                        if (Engine::COUNT(party.Party) > 0 && hasAttacked.size() < Engine::COUNT(party.Party))
                        {
                            if (Engine::SPELLCASTERS(party.Party) > 0)
                            {
                                auto combat_spells = 0;

                                for (auto i = 0; i < party.Party.size(); i++)
                                {
                                    if (party.Party[i].Health > 0 && party.Party[i].SpellCaster)
                                    {
                                        auto spells = Engine::COUNT(party.Party[i].SpellBook, Spells::Scope::COMBAT) + Engine::COUNT(party.Party[i].SpellBook, Spells::Scope::ADVENTURE_COMBAT);

                                        combat_spells += spells;
                                    }
                                }

                                if (combat_spells <= 0)
                                {
                                    flash_message = true;

                                    message = "Your PARTY does not have any usable COMBAT spells.";

                                    start_ticks = SDL_GetTicks();

                                    flash_color = intRD;
                                }
                                else
                                {
                                    auto result = selectCaster(window, renderer, party.Party, monsters, hasAttacked, Control::Type::COMBAT);

                                    if (result >= 0 && result < party.Party.size())
                                    {
                                        flash_message = true;

                                        message = std::string(party.Party[result].Name) + " CASTS a spell!";

                                        start_ticks = SDL_GetTicks();

                                        flash_color = intLB;

                                        hasAttacked.push_back(result);
                                    }

                                    selected = false;

                                    current = -1;
                                }
                            }
                            else
                            {
                                flash_message = true;

                                message = "There are NO SPELLCASTERS in your PARTY!";

                                start_ticks = SDL_GetTicks();

                                flash_color = intRD;
                            }
                        }
                        else
                        {
                            flash_message = true;

                            if (Engine::VERIFY_CODES(party, {Codes::Type::LAST_IN_COMBAT}) && combatRound == 0)
                            {
                                message = "Your PARTY does not get to ATTACK First nor CAST SPELLs this round!";
                            }
                            else
                            {
                                message = "Your PARTY has already ATTACKED this round.";
                            }

                            start_ticks = SDL_GetTicks();

                            flash_color = intRD;
                        }
                    }

                    monster_list = monsterList(window, renderer, monsters, offset, last, limit, textx, texty + infoh + text_space, false, false);
                }

                if (Engine::COUNT(party.Party) == 0 || Engine::COUNT(monsters) == 0)
                {
                    done = true;
                }
            }

            if (combatResult == Engine::Combat::FLEE)
            {
                break;
            }
        }

        if (font_garamond)
        {
            TTF_CloseFont(font_garamond);

            font_garamond = NULL;
        }

        if (font_dark11)
        {
            TTF_CloseFont(font_dark11);

            font_dark11 = NULL;
        }

        if (font_mason)
        {
            TTF_CloseFont(font_mason);

            font_mason = NULL;
        }

        TTF_Quit();

        if (splash)
        {
            SDL_FreeSurface(splash);

            splash = NULL;
        }
    }

    if (combatResult != Engine::Combat::FLEE && combatResult != Engine::Combat::NONE)
    {
        combatResult = Engine::COUNT(party.Party) > 0 ? Engine::Combat::VICTORY : Engine::Combat::DOOM;
    }

    // Clear temporary status, e.g. magic effects
    if (Engine::VERIFY_CODES(party, {Codes::Type::LAST_IN_COMBAT}))
    {
        Engine::LOSE_CODES(party, {Codes::Type::LAST_IN_COMBAT});
    }

    for (auto i = 0; i < party.Party.size(); i++)
    {
        Engine::REMOVE_STATUS(party.Party[i], Character::Status::ARMOUR3);
    }

    return combatResult;
}

Story::Base *findStory(Engine::Destination destination)
{
    auto next = (Story::Base *)&Story::notImplemented;

    auto bookID = destination.first;

    auto storyID = destination.second;

    if (bookID == Book::Type::BOOK1)
    {
        next = (Story::Base *)Story::FIND_STORY(storyID, Book1::Stories);
    }

    next->BookID = bookID;

    next->ID = storyID;

    return next;
}

bool inventoryScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, std::vector<Equipment::Base> &Items, Control::Type mode, int limit)
{
    if (Items.size() > 0)
    {
        auto font_size = 28;
        auto text_space = 8;
        auto scrollSpeed = 1;
        auto display_limit = (text_bounds - text_space) / (font_size + 7 * text_space / 2);

        auto offset = 0;

        auto last = offset + display_limit;

        if (last > Items.size())
        {
            last = Items.size();
        }

        const char *message = NULL;

        std::string temp_message = "";

        auto flash_message = false;

        auto flash_color = intRD;

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        auto done = false;

        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

        auto controls = equipmentList(window, renderer, Items, offset, last, display_limit, false, true);

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto selected = false;
        auto current = -1;
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto boxh = (int)(0.150 * SCREEN_HEIGHT);
        auto box_space = 10;

        while (!done)
        {
            last = offset + display_limit;

            if (last > Items.size())
            {
                last = Items.size();
            }

            SDL_SetWindowTitle(window, "Legendary Kingdoms 1 - The Valley of Bones: Equipment");

            fillWindow(renderer, intWH);

            if (flash_message)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message, font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }
                else
                {
                    flash_message = false;
                }
            }

            if (!flash_message)
            {
                if (mode == Control::Type::DROP)
                {
                    putText(renderer, "You are carrying too many items. Select item(s) to DROP.", font_garamond, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }
                else if (mode == Control::Type::USE)
                {
                    putText(renderer, "Select an item to USE", font_garamond, text_space, clrWH, intLB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }
                else if (mode == Control::Type::LOSE)
                {
                    std::string stolen_message = "DROP item(s) until only " + std::to_string(limit) + " item" + std::string(limit > 1 ? "s" : "") + " remains.";

                    putText(renderer, stolen_message.c_str(), font_garamond, text_space, clrWH, intLB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }
                else
                {
                    putText(renderer, "You are carrying these items", font_garamond, text_space, clrWH, intLB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }
            }

            putText(renderer, "Health", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (boxh + infoh - 1));
            putText(renderer, (std::to_string(character.Health)).c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - boxh);

            fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

            for (auto idx = offset; idx < last; idx++)
            {
                auto i = idx - offset;

                drawRect(renderer, controls[i].W + 2 * text_space, controls[i].H + 2 * text_space, controls[i].X - text_space, controls[i].Y - text_space, intBK);
            }

            renderButtons(renderer, controls, current, intLB, text_space, text_space / 2);

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
            {
                if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                {
                    if (offset > 0)
                    {
                        offset -= scrollSpeed;

                        if (offset < 0)
                        {
                            offset = 0;
                        }

                        last = offset + display_limit;

                        if (last > Items.size())
                        {
                            last = Items.size();
                        }

                        controls.clear();

                        controls = equipmentList(window, renderer, Items, offset, last, display_limit, false, true);

                        SDL_Delay(50);
                    }

                    if (offset <= 0)
                    {
                        current = -1;

                        selected = false;
                    }
                }
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || ((controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown))
                {
                    if (Items.size() - last > 0)
                    {
                        if (offset < Items.size() - display_limit)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > Items.size() - display_limit)
                        {
                            offset = Items.size() - display_limit;
                        }

                        last = offset + display_limit;

                        if (last > Items.size())
                        {
                            last = Items.size();
                        }

                        controls.clear();

                        controls = equipmentList(window, renderer, Items, offset, last, display_limit, false, true);

                        SDL_Delay(50);

                        if (offset > 0)
                        {
                            if (controls[current].Type != Control::Type::SCROLL_DOWN)
                            {
                                current++;
                            }
                        }
                    }

                    if (Items.size() - last <= 0)
                    {
                        selected = false;

                        current = -1;
                    }
                }
                else if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if ((current + offset >= 0) && (current + offset) < Items.size())
                    {
                        auto item = Items[current + offset];

                        if (mode == Control::Type::DROP)
                        {
                            Items.erase(Items.begin() + (current + offset));

                            if (offset > 0)
                            {
                                offset--;
                            }

                            last = offset + display_limit;

                            if (last > Items.size())
                            {
                                last = Items.size();
                            }

                            controls.clear();

                            controls = equipmentList(window, renderer, Items, offset, last, display_limit, false, true);

                            std::string description = item.Name;

                            if (item.Attribute != Attribute::Type::NONE)
                            {
                                description += " (+ " + std::to_string(item.Modifier) + " " + std::string(Attribute::Descriptions[item.Attribute]) + ")";
                            }

                            temp_message = description + " DROPPED!";

                            message = temp_message.c_str();

                            flash_color = intRD;

                            start_ticks = SDL_GetTicks();

                            flash_message = true;

                            selected = false;

                            current = -1;
                        }
                        else if (mode == Control::Type::LOSE)
                        {
                            if (Items.size() > limit)
                            {
                                Items.erase(Items.begin() + (current + offset));

                                Engine::LOSE_EQUIPMENT(character, {item.Type});

                                if (offset > 0)
                                {
                                    offset--;
                                }

                                last = offset + display_limit;

                                if (last > Items.size())
                                {
                                    last = Items.size();
                                }

                                controls.clear();

                                controls = equipmentList(window, renderer, Items, offset, last, display_limit, false, true);

                                std::string description = item.Name;

                                if (item.Attribute != Attribute::Type::NONE)
                                {
                                    description += " (+ " + std::to_string(item.Modifier) + " " + std::string(Attribute::Descriptions[item.Attribute]) + ")";
                                }

                                temp_message = description + " DROPPED!";

                                message = temp_message.c_str();

                                flash_color = intRD;

                                start_ticks = SDL_GetTicks();

                                flash_message = true;

                                selected = false;

                                current = -1;
                            }
                        }
                        else if (mode == Control::Type::USE)
                        {
                            bool used_up = false;

                            if (used_up)
                            {
                                if (Items.size() > 0)
                                {
                                    Items.erase(Items.begin() + (current + offset));

                                    Engine::LOSE_EQUIPMENT(character, {item.Type});

                                    if (offset > 0)
                                    {
                                        offset--;
                                    }

                                    last = offset + display_limit;

                                    if (last > Items.size())
                                    {
                                        last = Items.size();
                                    }

                                    controls.clear();

                                    controls = equipmentList(window, renderer, Items, offset, last, display_limit, false, true);
                                }
                            }

                            selected = false;

                            current = -1;
                        }
                    }

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = true;

                    break;
                }
            }
        }

        if (font_garamond)
        {
            TTF_CloseFont(font_garamond);

            font_garamond = NULL;
        }

        if (font_dark11)
        {
            TTF_CloseFont(font_dark11);

            font_dark11 = NULL;
        }

        TTF_Quit();
    }

    return false;
}

bool takeScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Equipment::Base> equipment, int TakeLimit, bool back_button)
{
    auto done = false;

    if (TakeLimit > 0)
    {
        auto font_size = 28;
        auto text_space = 8;
        auto scrollSpeed = 1;
        auto limit = (text_bounds - text_space) / (font_size + 7 * text_space / 2);
        auto offset = 0;
        auto last = offset + limit;

        if (last > equipment.size())
        {
            last = equipment.size();
        }

        const char *message = NULL;

        auto error = false;

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

        auto controls = equipmentList(window, renderer, equipment, offset, last, limit, true, back_button);

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto selected = false;
        auto current = -1;
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);

        auto selection = std::vector<int>();

        while (!done)
        {
            last = offset + limit;

            if (last > equipment.size())
            {
                last = equipment.size();
            }

            SDL_SetWindowTitle(window, "Legendary Kingdoms 1 - The Valley of Bones: Take Items");

            fillWindow(renderer, intWH);

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message, font_garamond, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, boxh * 2, startx, starty);
                }
                else
                {
                    error = false;
                }
            }

            if (!error)
            {
                std::string take_message = "";

                if (equipment.size() > 1)
                {
                    if (TakeLimit > 1)
                    {
                        if (TakeLimit == equipment.size())
                        {
                            take_message = "You can TAKE any number of items.";
                        }
                        else
                        {
                            take_message = "You can TAKE up to " + std::to_string(TakeLimit) + " items.";
                        }
                    }
                    else
                    {
                        take_message = "Choose an item to KEEP.";
                    }
                }
                else
                {
                    take_message = "KEEP this item?";
                }

                putText(renderer, take_message.c_str(), font_garamond, text_space, clrWH, intLB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
            }

            std::string take = "";

            if (selection.size() > 0)
            {
                for (auto i = 0; i < selection.size(); i++)
                {
                    if (i > 0)
                    {
                        take += ", ";
                    }

                    std::string description = equipment[selection[i]].Name;

                    take += description;
                }
            }

            putText(renderer, "SELECTED", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + infoh - 1));
            putText(renderer, selection.size() > 0 ? take.c_str() : "(None)", font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 3 * boxh, startx, starty + text_bounds - 3 * boxh);

            fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

            if (last - offset > 0)
            {
                for (auto i = 0; i < last - offset; i++)
                {
                    if (Engine::FIND_LIST(selection, offset + i) >= 0)
                    {
                        drawRect(renderer, controls[i].W + 2 * text_space, controls[i].H + 2 * text_space, controls[i].X - text_space, controls[i].Y - text_space, intBK);
                    }
                }
            }

            renderButtons(renderer, controls, current, intLB, text_space, text_space / 2);

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
            {
                if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                {
                    if (offset > 0)
                    {
                        offset -= scrollSpeed;

                        if (offset < 0)
                        {
                            offset = 0;
                        }

                        last = offset + limit;

                        if (last > equipment.size())
                        {
                            last = equipment.size();
                        }

                        controls.clear();

                        controls = equipmentList(window, renderer, equipment, offset, last, limit, true, back_button);

                        SDL_Delay(50);
                    }

                    if (offset <= 0)
                    {
                        current = -1;

                        selected = false;
                    }
                }
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || ((controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown))
                {
                    if (equipment.size() - last > 0)
                    {
                        if (offset < equipment.size() - limit)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > equipment.size() - limit)
                        {
                            offset = equipment.size() - limit;
                        }

                        last = offset + limit;

                        if (last > equipment.size())
                        {
                            last = equipment.size();
                        }

                        controls.clear();

                        controls = equipmentList(window, renderer, equipment, offset, last, limit, true, back_button);

                        SDL_Delay(50);

                        if (offset > 0)
                        {
                            if (controls[current].Type != Control::Type::SCROLL_DOWN)
                            {
                                current++;
                            }
                        }
                    }

                    if (equipment.size() - last <= 0)
                    {
                        selected = false;

                        current = -1;
                    }
                }
                else if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (current >= 0 && current < controls.size())
                    {
                        auto result = Engine::FIND_LIST(selection, offset + current);

                        if (result >= 0)
                        {
                            selection.erase(selection.begin() + result);
                        }
                        else
                        {
                            if (selection.size() < TakeLimit)
                            {
                                selection.push_back(offset + current);
                            }
                        }
                    }

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::CONFIRM && !hold)
                {
                    auto take = std::vector<Equipment::Base>();

                    for (auto i = 0; i < selection.size(); i++)
                    {
                        take.push_back(equipment[selection[i]]);
                    }

                    auto character = selectPartyMember(window, renderer, party.Party, Control::Type::EQUIPMENT);

                    if (character >= 0 && character < party.Party.size())
                    {
                        Engine::GET_EQUIPMENT(party.Party[character], take);

                        while (!Engine::VERIFY_EQUIPMENT_LIMIT(party.Party[character]))
                        {
                            inventoryScreen(window, renderer, party.Party[character], party.Party[character].Equipment, Control::Type::DROP, 0);
                        }

                        done = true;
                    }
                    else
                    {
                        done = false;
                    }

                    current = -1;

                    selected = false;

                    break;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = false;

                    break;
                }
            }
        }

        if (font_garamond)
        {
            TTF_CloseFont(font_garamond);

            font_garamond = NULL;
        }

        if (font_dark11)
        {
            TTF_CloseFont(font_dark11);

            font_dark11 = NULL;
        }

        TTF_Quit();
    }

    return done;
}

bool harbourScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *harbour)
{
    auto *title = "Harbour";

    auto font_size = 20;
    auto garamond_size = 24;

    TTF_Init();

    auto font_mason = TTF_OpenFont(FONT_MASON, 32);
    auto font_mason2 = TTF_OpenFont(FONT_MASON, 22);
    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, garamond_size);
    auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

    TTF_SetFontKerning(font_dark11, 0);

    auto box_space = 10;
    auto character_box = (int)(text_bounds * 2 / 3);

    // Render window
    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        const char *choices[4] = {"BUY/SELL SHIPS", "REPAIR SHIP", "BUY/SELL CARGO", "BACK"};

        auto current = 0;

        auto selected = false;

        auto main_buttonh = (int)(0.06 * SCREEN_HEIGHT);

        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;

        auto controls = createHTextButtons(choices, 4, main_buttonh, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);

        controls[0].Type = Control::Type::BUY_SELL_SHIP;
        controls[1].Type = Control::Type::REPAIR_SHIP;
        controls[2].Type = Control::Type::BUY_SELL_CARGO;
        controls[3].Type = Control::Type::BACK;

        auto done = false;

        auto text_space = 8;

        auto Party = Party::Base();

        while (!done)
        {
            // Fill the surface with background
            fillWindow(renderer, intWH);

            auto adventurerh = splashw;

            putHeader(renderer, "Location", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty);
            putText(renderer, Location::Description[party.Location], font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + infoh);

            putHeader(renderer, "Money", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + 2 * infoh + box_space - 1));
            putText(renderer, (std::to_string(party.Money) + std::string(" silver coins")).c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (3 * boxh + infoh + box_space));

            putHeader(renderer, "Party", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

            if (Engine::COUNT(party.Party) > 0)
            {
                std::string party_string = "";

                auto count = 0;

                for (auto i = 0; i < party.Party.size(); i++)
                {
                    if (count > 0)
                    {
                        party_string += "\n";
                    }

                    party_string += party.Party[i].Name;

                    if (party.Party[i].Health <= 0)
                    {
                        party_string += " (D)";
                    }

                    count++;
                }

                putText(renderer, party_string.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
            }

            fillRect(renderer, textwidth, (text_bounds - infoh), textx, (texty + infoh), intBE);

            if (current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::BUY_SELL_SHIP)
                {
                    putHeader(renderer, "Ship Prices", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::REPAIR_SHIP)
                {
                    putHeader(renderer, "Repair Costs", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::BUY_SELL_CARGO)
                {
                    putHeader(renderer, "Cargo Prices", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else
                {
                    fillRect(renderer, textwidth, infoh, textx, texty, intBR);
                }
            }
            else
            {
                fillRect(renderer, textwidth, infoh, textx, texty, intBR);
            }

            renderTextButtons(renderer, controls, FONT_DARK11, current, clrWH, intDB, intLB, font_size + 2, TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::BACK)
                {
                    done = true;

                    break;
                }
            }
        }
    }

    if (font_mason)
    {
        TTF_CloseFont(font_mason);

        font_mason = NULL;
    }

    if (font_mason2)
    {
        TTF_CloseFont(font_mason2);

        font_mason2 = NULL;
    }

    if (font_dark11)
    {
        TTF_CloseFont(font_dark11);

        font_dark11 = NULL;
    }

    if (font_garamond)
    {
        TTF_CloseFont(font_garamond);

        font_garamond = NULL;
    }

    TTF_Quit();

    return false;
}

std::vector<Button> createChoices(std::vector<Choice::Base> choices, int start, int last, int limit)
{
    auto controls = std::vector<Button>();

    return controls;
}

Story::Base *processChoices(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *story)
{
    Story::Base *next = &Story::notImplemented;

    auto error = false;
    const char *message = NULL;

    Uint32 start_ticks = 0;
    Uint32 duration = 5000;

    SDL_Surface *background = NULL;

    if (renderer && story->Choices.size() > 0)
    {
        SDL_Surface *splash = NULL;

        if (story->Image)
        {
            splash = createImage(story->Image);
        }

        auto choices = story->Choices;

        auto selected = false;
        auto current = -1;
        auto done = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto controls = std::vector<Button>();

        auto font_size = 28;
        auto text_space = 8;
        auto textwidth = (int)((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto box_space = 10;

        for (int i = 0; i < choices.size(); i++)
        {
            auto text = createText(choices[i].Text, FONT_GARAMOND, font_size, clrBK, textwidth - (4 * text_space), TTF_STYLE_NORMAL);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : texty + 2 * text_space);

            controls.push_back(Button(i, text, i, i, (i > 0 ? i - 1 : i), (i < choices.size() ? i + 1 : i), textx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = textwidth - (4 * text_space);

            controls[i].H = text->h;
        }

        auto idx = choices.size();

        controls.push_back(Button(idx, "icons/map.png", idx - 1, idx + 1, idx - 1, idx, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(idx + 1, "icons/user.png", idx, idx + 2, idx - 1, idx + 1, startx + gridsize, buttony, Control::Type::PARTY));
        controls.push_back(Button(idx + 2, "icons/back-button.png", idx + 1, idx + 2, idx - 1, idx + 2, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_mason2 = TTF_OpenFont(FONT_MASON, 28);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        int splash_h = splashw;

        if (splash)
        {
            if (splash->w != splashw)
            {
                splash_h = (int)((double)splashw / splash->w * splash->h);
            }
        }

        while (!done)
        {
            if (story->Title)
            {
                SDL_SetWindowTitle(window, story->Title);
            }
            else
            {
                SDL_SetWindowTitle(window, (std::string("Legendary Kingdoms 1 - The Valley of Bones: ") + std::string(3 - std::to_string(std::abs(story->ID)).length(), '0') + std::to_string(std::abs(story->ID))).c_str());
            }

            fillWindow(renderer, intWH);

            if (background)
            {
                stretchImage(renderer, background, 0, 0, SCREEN_WIDTH, buttony - button_space);
            }

            if (splash)
            {
                splash_h = fitImage(renderer, splash, startx, starty, splashw, text_bounds);
            }

            if (!splash || (splash && splash_h < (text_bounds - 2 * boxh - infoh)))
            {
                putHeader(renderer, "Party", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (Engine::COUNT(party.Party) > 0)
                {
                    std::string party_string = "";

                    auto count = 0;

                    for (auto i = 0; i < party.Party.size(); i++)
                    {
                        if (count > 0)
                        {
                            party_string += "\n";
                        }

                        party_string += party.Party[i].Name;

                        if (party.Party[i].Health <= 0)
                        {
                            party_string += " (D)";
                        }

                        count++;
                    }

                    putText(renderer, party_string.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                }
                else
                {
                    fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
                }
            }

            if (!splash)
            {
                std::string title_string = std::string(Book::Title[story->BookID]) + "\n";

                if (story->ID != -1)
                {
                    title_string += std::string(3 - std::to_string(std::abs(story->ID)).length(), '0') + std::to_string(std::abs(story->ID));

                    putText(renderer, title_string.c_str(), font_mason2, text_space, clrBK, intWH, TTF_STYLE_NORMAL, splashw, infoh, startx, starty);
                }
                else
                {
                    title_string += "Not Implemented";

                    putText(renderer, title_string.c_str(), font_mason2, text_space, clrBK, intWH, TTF_STYLE_NORMAL, splashw, infoh, startx, starty);
                }
            }

            if ((!splash || (splash && splash_h < (text_bounds - 4 * boxh - 2 * infoh - box_space))) && Engine::COUNT(party.OtherParty) > 0)
            {
                putHeader(renderer, "Other Party", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (4 * boxh + 2 * infoh + box_space));

                if (Engine::COUNT(party.OtherParty) > 0)
                {
                    std::string party_string = "";

                    auto count = 0;

                    for (auto i = 0; i < party.OtherParty.size(); i++)
                    {
                        if (count > 0)
                        {
                            party_string += "\n";
                        }

                        party_string += party.OtherParty[i].Name;

                        if (party.OtherParty[i].Health <= 0)
                        {
                            party_string += " (D)";
                        }

                        count++;
                    }

                    putText(renderer, party_string.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 4 * boxh - infoh - box_space);
                }
                else
                {
                    fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 4 * boxh - infoh - box_space, intBE);
                }
            }
            else if (!splash || (splash && splash_h < (text_bounds - 3 * boxh - 2 * infoh - box_space)))
            {
                putHeader(renderer, "Money", font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + 2 * infoh + box_space));

                putText(renderer, (std::to_string(party.Money) + " silver coins").c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - 3 * boxh - infoh - box_space);
            }

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

            fillRect(renderer, textwidth, text_bounds, textx, texty, BE_80);

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

            renderButtons(renderer, controls, current, intLB, text_space, text_space / 2);

            for (auto i = 0; i < story->Choices.size(); i++)
            {
                if (i != current)
                {
                    drawRect(renderer, controls[i].W + 2 * text_space, controls[i].H + 2 * text_space, controls[i].X - text_space, controls[i].Y - text_space, intBK);
                }
            }

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putHeader(renderer, message, font_garamond, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw * 2, boxh * 2, -1, -1);
                }
                else
                {
                    error = false;
                }
            }

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (current >= 0 && current < story->Choices.size())
                    {
                        if (story->Choices[current].Type == Choice::Type::NORMAL)
                        {
                            next = findStory(story->Choices[current].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[current].Type == Choice::Type::ATTRIBUTES)
                        {
                            if (story->Choices[current].Characters.size() > 0)
                            {
                                auto selection = std::vector<int>();

                                auto success = skillCheck(window, renderer, story->Choices[current].Characters, 1, story->Choices[current].Attributes[0], story->Choices[current].Difficulty, story->Choices[current].Success, selection);

                                if (selection.size() == 1)
                                {
                                    story->SkillCheck(story->Choices[current].Characters, success, selection);

                                    if (success)
                                    {
                                        next = findStory(story->Choices[current].Destination);
                                    }
                                    else
                                    {
                                        next = findStory(story->Choices[current].DestinationFailed);
                                    }

                                    done = true;

                                    break;
                                }
                            }
                            else
                            {
                                auto selection = std::vector<int>();

                                auto success = skillCheck(window, renderer, party.Party, 1, story->Choices[current].Attributes[0], story->Choices[current].Difficulty, story->Choices[current].Success, selection);

                                if (selection.size() == 1)
                                {
                                    story->SkillCheck(party, success, selection);

                                    if (success)
                                    {
                                        next = findStory(story->Choices[current].Destination);
                                    }
                                    else
                                    {
                                        next = findStory(story->Choices[current].DestinationFailed);
                                    }

                                    done = true;

                                    break;
                                }
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::TEAM_ATTRIBUTES)
                        {
                            if (story->Choices[current].Characters.size() > 0)
                            {
                                auto selection = std::vector<int>();

                                auto success = skillCheck(window, renderer, story->Choices[current].Characters, 2, story->Choices[current].Attributes[0], story->Choices[current].Difficulty, story->Choices[current].Success, selection);

                                if (selection.size() == 2)
                                {
                                    story->SkillCheck(story->Choices[current].Characters, success, selection);

                                    if (success)
                                    {
                                        next = findStory(story->Choices[current].Destination);
                                    }
                                    else
                                    {
                                        next = findStory(story->Choices[current].DestinationFailed);
                                    }

                                    done = true;

                                    break;
                                }
                            }
                            else
                            {
                                auto selection = std::vector<int>();

                                auto success = skillCheck(window, renderer, party.Party, 2, story->Choices[current].Attributes[0], story->Choices[current].Difficulty, story->Choices[current].Success, selection);

                                if (selection.size() == 2)
                                {
                                    story->SkillCheck(party, success, selection);

                                    if (success)
                                    {
                                        next = findStory(story->Choices[current].Destination);
                                    }
                                    else
                                    {
                                        next = findStory(story->Choices[current].DestinationFailed);
                                    }

                                    done = true;

                                    break;
                                }
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::EQUIPMENT)
                        {
                            auto equipment = std::vector<Equipment::Type>();

                            for (auto i = 0; i < story->Choices[current].Equipment.size(); i++)
                            {
                                equipment.push_back(story->Choices[current].Equipment[i].Type);
                            }

                            if (Engine::VERIFY_EQUIPMENT(party.Party, equipment))
                            {
                                next = findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                error = true;

                                if (story->Choices[current].Equipment.size() > 1)
                                {
                                    message = "You do not have the REQUIRED ITEMS!";
                                }
                                else
                                {
                                    message = "You do not have the REQUIRED ITEM!";
                                }

                                start_ticks = SDL_GetTicks();
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::ANY_EQUIPMENT)
                        {
                            auto equipment = std::vector<Equipment::Type>();

                            for (auto i = 0; i < story->Choices[current].Equipment.size(); i++)
                            {
                                equipment.push_back(story->Choices[current].Equipment[i].Type);
                            }

                            if (Engine::VERIFY_ANY_EQUIPMENT(party.Party, equipment))
                            {
                                next = findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                error = true;

                                message = "You do not have any of the REQUIRED ITEMS!";

                                start_ticks = SDL_GetTicks();
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::SHIP)
                        {
                            if (Engine::HAS_SHIP(party, party.Location))
                            {
                                next = findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                error = true;

                                message = "You do not a ship at this location!";

                                start_ticks = SDL_GetTicks();
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::ADD_MAX_HEALTH)
                        {
                            auto target = selectPartyMember(window, renderer, party.Party, Control::Type::SPELL_TARGET);

                            party.Party[target].MaximumHealth += story->Choices[current].Value;

                            Engine::GAIN_HEALTH(party.Party[target], story->Choices[current].Value);

                            next = findStory(story->Choices[current].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[current].Type == Choice::Type::BRIBE_CODEWORD)
                        {
                            auto equipment = std::vector<Equipment::Type>();

                            for (auto i = 0; i < story->Choices[current].Equipment.size(); i++)
                            {
                                equipment.push_back(story->Choices[current].Equipment[i].Type);
                            }

                            auto count = Engine::COUNT_EQUIPMENT(party.Party, equipment);

                            if (count >= story->Choices[current].Value)
                            {
                                Engine::LOSE_EQUIPMENT(party.Party, equipment[0], story->Choices[current].Value);

                                Engine::GET_CODES(party, story->Choices[current].InvisibleCodes);

                                next = findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                if (story->Choices[current].Value > 1)
                                {
                                    message = "You do not have the REQUIRED ITEMS!";
                                }
                                else
                                {
                                    message = "You do not have the REQUIRED ITEM!";
                                }

                                error = true;

                                start_ticks = SDL_GetTicks();
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::CODES)
                        {
                        }
                        else if (story->Choices[current].Type == Choice::Type::GET_EQUIPMENT)
                        {
                        }
                        else if (story->Choices[current].Type == Choice::Type::PAY_WITH)
                        {
                        }
                        else if (story->Choices[current].Type == Choice::Type::SELL)
                        {
                        }
                        else if (story->Choices[current].Type == Choice::Type::LOSE_EQUIPMENT)
                        {
                        }
                        else if (story->Choices[current].Type == Choice::Type::BRIBE)
                        {
                        }
                        else if (story->Choices[current].Type == Choice::Type::GET_CODES)
                        {
                        }
                        else if (story->Choices[current].Type == Choice::Type::LOSE_CODES)
                        {
                        }
                        else if (story->Choices[current].Type == Choice::Type::LOSE_ALL)
                        {
                        }
                        else if (story->Choices[current].Type == Choice::Type::LOSE_MONEY)
                        {
                        }
                        else if (story->Choices[current].Type == Choice::Type::GAIN_MONEY)
                        {
                        }
                        else if (story->Choices[current].Type == Choice::Type::MONEY)
                        {
                        }
                        else if (story->Choices[current].Type == Choice::Type::LIFE)
                        {
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::PARTY && !hold)
                {
                    viewParty(window, renderer, party);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::USE && !hold)
                {
                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::MAP && !hold)
                {
                    mapScreen(window, renderer);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    next = story;

                    done = true;

                    break;
                }
            }
        }

        if (font_garamond)
        {
            TTF_CloseFont(font_garamond);

            font_garamond = NULL;
        }

        if (font_mason)
        {
            TTF_CloseFont(font_mason);

            font_mason = NULL;
        }

        if (font_dark11)
        {
            TTF_CloseFont(font_dark11);

            font_mason = NULL;
        }

        if (font_mason2)
        {
            TTF_CloseFont(font_mason2);

            font_mason2 = NULL;
        }

        TTF_Quit();

        if (splash)
        {
            SDL_FreeSurface(splash);

            splash = NULL;
        }
    }

    if (background)
    {
        SDL_FreeSurface(background);

        background = NULL;
    }

    return next;
}

Story::Base *renderChoices(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *story)
{
    Story::Base *next = &Story::notImplemented;

    if (story->Choices.size() > 0)
    {
        next = processChoices(window, renderer, party, story);
    }
    else
    {
        next = findStory(story->Continue(party));
    }

    return next;
}

bool processStory(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Book::Type book, Story::Base *story)
{
    auto quit = false;

    auto space = 8;
    auto font_size = 28;
    auto text_space = 8;

    TTF_Init();

    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
    auto font_mason = TTF_OpenFont(FONT_MASON, 24);
    auto font_mason2 = TTF_OpenFont(FONT_MASON, 28);
    auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

    TTF_SetFontKerning(font_dark11, 0);

    auto infoh = (int)(0.07 * SCREEN_HEIGHT);
    auto boxh = (int)(0.125 * SCREEN_HEIGHT);
    auto box_space = 10;

    SDL_Surface *background = NULL;

    auto saveParty = party;

    std::vector<Button> controls = {};

    while (!quit)
    {
        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 5000;

        party.StoryID = story->ID;

        // capture party state before running the story
        saveParty = party;

        auto run_once = true;

        SDL_Surface *splash = NULL;
        SDL_Texture *splashTexture = NULL;
        SDL_Surface *text = NULL;

        if (run_once)
        {
            run_once = false;

            auto jump = story->Background(party);

            auto jumpBook = jump.first;

            auto jumpID = jump.second;

            if (jumpBook != Book::Type::NONE && jumpID >= 0)
            {
                story = findStory(jump);

                continue;
            }

            story->Event(party);
        }

        int splash_h = 250;

        if (story->Image)
        {
            splash = createImage(story->Image);
        }

        if (splash)
        {
            if (splash->w != splashw)
            {
                splash_h = (int)((double)splashw / splash->w * splash->h);
            }

            splashTexture = SDL_CreateTextureFromSurface(renderer, splash);
        }

        if (story->Text)
        {
            auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * space;

            text = createText(story->Text, FONT_GARAMOND, font_size, clrDB, textwidth, TTF_STYLE_NORMAL);
        }

        auto compact = (text && text->h <= text_bounds - 2 * text_space) || text == NULL;

        if (story->Controls == Story::Controls::STANDARD)
        {
            controls = Story::StandardControls(compact);
        }
        else if (story->Controls == Story::Controls::SHOP)
        {
            controls = Story::ShopControls(compact);
        }
        else if (story->Controls == Story::Controls::SELL)
        {
            controls = Story::SellControls(compact);
        }
        else if (story->Controls == Story::Controls::BUY_AND_SELL)
        {
            controls = Story::BuyAndSellControls(compact);
        }
        else if (story->Controls == Story::Controls::BARTER_AND_SHOP)
        {
            controls = Story::BarterAndShopControls(compact);
        }
        else if (story->Controls == Story::Controls::BARTER)
        {
            controls = Story::BarterControls(compact);
        }
        else if (story->Controls == Story::Controls::HARBOUR)
        {
            controls = Story::HarbourControls(compact);
        }
        else
        {
            controls = Story::ExitControls(compact);
        }

        if (story->Type != Story::Type::NORMAL || Engine::COUNT(party.Party) <= 0)
        {
            controls = Story::ExitControls(compact);
        }

        // Render the image
        if (window && renderer)
        {
            auto scrollSpeed = 20;
            auto hold = false;

            auto selected = false;
            auto current = -1;
            auto offset = 0;

            while (!quit)
            {
                if (story->Title)
                {
                    SDL_SetWindowTitle(window, story->Title);
                }
                else
                {
                    SDL_SetWindowTitle(window, (std::string("Legendary Kingdoms 1 - The Valley of Bones: ") + std::string(3 - std::to_string(std::abs(story->ID)).length(), '0') + std::to_string(std::abs(story->ID))).c_str());
                }

                fillWindow(renderer, intWH);

                // Fill the surface with background
                stretchImage(renderer, background, 0, 0, SCREEN_WIDTH, buttony - button_space);

                if (splash)
                {
                    splash_h = fitImage(renderer, splash, startx, texty, splashw, text_bounds);
                }

                if (!splash)
                {
                    std::string title_string = std::string(Book::Title[book]) + "\n";

                    if (story->ID != -1)
                    {
                        title_string += std::string(3 - std::to_string(std::abs(story->ID)).length(), '0') + std::to_string(std::abs(story->ID));

                        putText(renderer, title_string.c_str(), font_mason2, text_space, clrBK, intWH, TTF_STYLE_NORMAL, splashw, infoh, startx, starty);
                    }
                    else
                    {
                        title_string += "Not Implemented";

                        putText(renderer, title_string.c_str(), font_mason2, text_space, clrBK, intWH, TTF_STYLE_NORMAL, splashw, infoh, startx, starty);
                    }
                }

                if (!splash || (splash && splash_h < (text_bounds - 2 * boxh - infoh)))
                {
                    putHeader(renderer, "Party", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                    if (Engine::COUNT(party.Party) > 0)
                    {
                        std::string party_string = "";

                        auto count = 0;

                        for (auto i = 0; i < party.Party.size(); i++)
                        {
                            if (count > 0)
                            {
                                party_string += "\n";
                            }

                            party_string += party.Party[i].Name;

                            if (party.Party[i].Health <= 0)
                            {
                                party_string += " (D)";
                            }

                            count++;
                        }

                        putText(renderer, party_string.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                    }
                    else
                    {
                        fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
                    }
                }

                if ((!splash || (splash && splash_h < (text_bounds - 4 * boxh - 2 * infoh - box_space))) && Engine::COUNT(party.OtherParty) > 0)
                {
                    putHeader(renderer, "Other Party", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (4 * boxh + 2 * infoh + box_space));

                    if (Engine::COUNT(party.OtherParty) > 0)
                    {
                        std::string party_string = "";

                        auto count = 0;

                        for (auto i = 0; i < party.OtherParty.size(); i++)
                        {
                            if (count > 0)
                            {
                                party_string += "\n";
                            }

                            party_string += party.OtherParty[i].Name;

                            if (party.OtherParty[i].Health <= 0)
                            {
                                party_string += " (D)";
                            }

                            count++;
                        }

                        putText(renderer, party_string.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 4 * boxh - infoh - box_space);
                    }
                    else
                    {
                        fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 4 * boxh - infoh - box_space, intBE);
                    }
                }
                else if (!splash || (splash && splash_h < (text_bounds - 3 * boxh - 2 * infoh - box_space)))
                {
                    putHeader(renderer, "Money", font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + 2 * infoh + box_space));

                    putText(renderer, (std::to_string(party.Money) + " silver coins").c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - 3 * boxh - infoh - box_space);
                }

                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

                fillRect(renderer, textwidth, text_bounds, textx, texty, BE_80);

                if (story->Text && text)
                {
                    renderText(renderer, text, 0, textx + space, texty + space, text_bounds - 2 * space, offset);
                }

                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

                if (flash_message)
                {
                    if ((SDL_GetTicks() - start_ticks) < duration)
                    {
                        putText(renderer, message.c_str(), font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                    }
                    else
                    {
                        flash_message = false;
                    }
                }

                if (!compact)
                {
                    fillRect(renderer, controls[0].W + 2 * border_space, controls[0].H + 2 * border_space, controls[0].X - border_space, controls[0].Y - border_space, intWH);
                    fillRect(renderer, controls[1].W + 2 * border_space, controls[1].H + 2 * border_space, controls[1].X - border_space, controls[1].Y - border_space, intWH);
                }

                bool scrollUp = false;
                bool scrollDown = false;

                if (splash)
                {
                    auto mousex = 0;
                    auto mousey = 0;

                    auto state = SDL_GetMouseState(&mousex, &mousey);

                    auto zoomw = (int)(0.80 * (double)textwidth);
                    auto zoomh = (int)(0.80 * (double)text_bounds);

                    clipValue(zoomw, 0, splash->w);
                    clipValue(zoomh, 0, splash->h);

                    auto boundx = splashw;

                    if (splash_h == text_bounds)
                    {
                        boundx = (int)((double)splash_h / splash->h * (double)splash->w);
                    }

                    if (mousex >= startx && mousex <= (startx + boundx) && mousey >= starty && mousey <= (starty + splash_h))
                    {
                        auto scalex = (double)(mousex - startx) / boundx;
                        auto scaley = (double)(mousey - starty) / splash_h;

                        int centerx = (int)(scalex * (double)splash->w);
                        int centery = (int)(scaley * (double)splash->h);

                        clipValue(centerx, zoomw / 2, splash->w - zoomw / 2);
                        clipValue(centery, zoomh / 2, splash->h - zoomh / 2);

                        if (splashTexture)
                        {
                            SDL_Rect src;

                            src.w = zoomw;
                            src.h = zoomh;
                            src.x = centerx - zoomw / 2;
                            src.y = centery - zoomh / 2;

                            SDL_Rect dst;

                            dst.w = zoomw;
                            dst.h = zoomh;
                            dst.x = (textx + (textwidth - zoomw) / 2);
                            dst.y = (texty + (text_bounds - zoomh) / 2);

                            SDL_RenderCopy(renderer, splashTexture, &src, &dst);
                        }
                    }
                }

                if (story->Type == Story::Type::DOOM)
                {
                    putText(renderer, "This adventure is over.", font_garamond, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }
                else if (Engine::COUNT(party.Party) <= 0)
                {
                    putText(renderer, "Your party has died. This adventure is over.", font_garamond, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }

                renderButtons(renderer, controls, current, intLB, border_space, border_pts);

                quit = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold))
                {
                    if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                    {
                        if (text)
                        {
                            if (offset > 0)
                            {
                                offset -= scrollSpeed;
                            }

                            if (offset < 0)
                            {
                                offset = 0;
                            }
                        }
                    }
                    else if (controls[current].Type == Control::Type::SCROLL_DOWN || ((controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown))
                    {
                        if (text)
                        {
                            if (text->h >= text_bounds - 2 * space)
                            {
                                if (offset < text->h - text_bounds + 2 * space)
                                {
                                    offset += scrollSpeed;
                                }

                                if (offset > text->h - text_bounds + 2 * space)
                                {
                                    offset = text->h - text_bounds + 2 * space;
                                }
                            }
                        }
                    }
                    else if (controls[current].Type == Control::Type::PARTY && !hold)
                    {
                        viewParty(window, renderer, party);

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::MAP && !hold)
                    {
                        mapScreen(window, renderer);

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::HARBOUR && !hold)
                    {
                        harbourScreen(window, renderer, party, story);

                        current = -1;

                        selected = false;
                    }
                    else if ((controls[current].Type == Control::Type::SHOP || controls[current].Type == Control::Type::SELL) && !hold)
                    {
                        if (controls[current].Type == Control::Type::SHOP)
                        {
                        }
                        else
                        {
                        }

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::BARTER && !hold)
                    {
                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::GAME && !hold)
                    {
                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::NEXT && !hold)
                    {
                        if (Engine::COUNT(story->Monsters) > 0)
                        {
                            story->Combat = combatScreen(window, renderer, party, story->Monsters, story->CanFlee, story->FleeRound, true);
                        }

                        if (Engine::COUNT(party.Party) > 0)
                        {
                            if (story->Take.size() > 0 && story->Limit > 0)
                            {
                                auto done = takeScreen(window, renderer, party, story->Take, story->Limit, true);

                                if (!done)
                                {
                                    continue;
                                }
                                else
                                {
                                    story->Limit = 0;
                                }
                            }

                            current = -1;

                            selected = false;

                            auto next = renderChoices(window, renderer, party, story);

                            book = next->BookID;

                            if (next->ID != story->ID)
                            {
                                if (story->Bye)
                                {
                                    auto bye = createText(story->Bye, FONT_GARAMOND, font_size + 4, clrDB, (SCREEN_WIDTH * (1.0 - 2.0 * Margin)) - 2 * text_space, TTF_STYLE_NORMAL);
                                    auto forward = createImage("icons/next.png");

                                    if (bye && forward)
                                    {
                                        fillWindow(renderer, intWH);

                                        stretchImage(renderer, background, 0, 0, SCREEN_WIDTH, buttony - button_space);

                                        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

                                        fillRect(renderer, (1.0 - 2.0 * Margin) * SCREEN_WIDTH, bye->h + 2 * text_space, startx, ((buttony - button_space) - (bye->h + 2 * text_space)) / 2, BE_80);

                                        renderText(renderer, bye, 0, (SCREEN_WIDTH - bye->w) / 2, ((buttony - button_space) - bye->h) / 2, (buttony - button_space), 0);

                                        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

                                        renderImage(renderer, forward, SCREEN_WIDTH * (1.0 - Margin) - buttonw - button_space, buttony);

                                        SDL_RenderPresent(renderer);

                                        Input::WaitForNext(renderer);

                                        SDL_FreeSurface(bye);

                                        bye = NULL;

                                        SDL_FreeSurface(forward);

                                        forward = NULL;
                                    }
                                }

                                story = next;

                                break;
                            }
                            else if (Engine::COUNT(party.Party) <= 0)
                            {
                                controls = Story::ExitControls(compact);
                            }
                        }
                        else
                        {
                            controls = Story::ExitControls(compact);
                        }
                    }
                    else if (controls[current].Type == Control::Type::BACK && !hold)
                    {
                        quit = true;

                        break;
                    }
                }
            }
        }

        if (splash)
        {
            SDL_FreeSurface(splash);

            splash = NULL;
        }

        if (splashTexture)
        {
            SDL_DestroyTexture(splashTexture);

            splashTexture = NULL;
        }

        if (text)
        {
            SDL_FreeSurface(text);

            text = NULL;
        }
    }

    if (background)
    {
        SDL_FreeSurface(background);

        background = NULL;
    }

    if (font_garamond)
    {
        TTF_CloseFont(font_garamond);

        font_garamond = NULL;
    }

    if (font_mason)
    {
        TTF_CloseFont(font_mason);

        font_mason = NULL;
    }

    if (font_mason2)
    {
        TTF_CloseFont(font_mason2);

        font_mason2 = NULL;
    }

    if (font_dark11)
    {
        TTF_CloseFont(font_dark11);

        font_mason = NULL;
    }

    TTF_Quit();

    return quit;
}

bool storyScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Engine::Destination destination)
{
    auto story = findStory(destination);

    return processStory(window, renderer, party, story->BookID, story);
}

bool mainScreen(SDL_Window *window, SDL_Renderer *renderer, Book::Type bookID, int storyID)
{
    auto font_size = 20;

    auto *introduction = "Prepare for adventure in this huge open-world gamebook series. Legendary Kingdoms is a gamebook campaign, where you lead a party of adventurers in a world that adapts to your actions. Venture into ancient ruins, pick a side and lead an army into battle, sail the high seas on your own warship, defeat tyrants or bring them to power. Along the way your party will increase in skill, wealth and renown, allowing them to take on more challenging adventures. Reach the heights of power and you may uncover a dreadful threat to the world itself and go on a mission that spans all six gamebooks in the series.\n\nBook 1: The Valley of Bones takes place in a desert wilderness where tyrant kings oppress the teeming masses in a land strewn with ancient artefacts and ruins. But their grip on power is fragile... and the citizenry are ripe for revolution. It is a land of blood and sand, where civilisation is rare and terrible beasts roam freely.";

    auto splash = createImage("images/book1/valley-of-bones-cover.png");

    auto text = createText(introduction, FONT_GARAMOND, 28, clrDB, SCREEN_WIDTH * (1.0 - 3.0 * Margin) - splashw);

    auto title = "Legendary Kingdoms 1 - The Valley of Bones";

    Book1::InitializeStories();

    // Render window
    if (window && renderer && splash && text)
    {
        SDL_SetWindowTitle(window, title);

        const char *choices[4] = {"NEW GAME", "LOAD GAME", "ABOUT", "EXIT"};

        auto current = -1;

        auto selected = false;

        auto main_buttonh = 48;

        auto controls = createHTextButtons(choices, 4, main_buttonh, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);

        controls[0].Type = Control::Type::NEW;
        controls[1].Type = Control::Type::LOAD;
        controls[2].Type = Control::Type::ABOUT;
        controls[3].Type = Control::Type::QUIT;

        auto done = false;

        auto text_space = 8;

        while (!done)
        {
            auto Party = Party::Base();

            // Fill the surface with background
            fillWindow(renderer, intWH);

            fitImage(renderer, splash, startx, starty, splashw, text_bounds);

            fillRect(renderer, text->w + 2 * text_space, text->h + 2 * text_space, startx * 2 + splashw, texty, intWH);

            renderText(renderer, text, intBK, startx * 2 + splashw + text_space, starty + text_space, SCREEN_HEIGHT * (1.0 - 2 * Margin) - 2 * text_space, 0);

            renderTextButtons(renderer, controls, FONT_DARK11, current, clrWH, intDB, intLB, font_size + 2, TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size())
            {
                switch (controls[current].Type)
                {
                case Control::Type::NEW:

                    done = selectParty(window, renderer, bookID, Party);

                    if (Party.Party.size() == 4)
                    {
                        storyScreen(window, renderer, Party, {bookID, storyID});
                    }

                    current = -1;

                    selected = false;

                    storyID = 1;

                    break;

                case Control::Type::ABOUT:

                    current = -1;

                    selected = false;

                    break;

                case Control::Type::LOAD:

                    current = -1;

                    selected = false;

                    break;

                case Control::Type::QUIT:

                    done = true;

                    break;

                default:

                    selected = false;

                    done = false;

                    break;
                }
            }

            SDL_SetWindowTitle(window, title);
        }

        SDL_FreeSurface(splash);
        SDL_FreeSurface(text);

        splash = NULL;
        text = NULL;
    }

    return false;
}

bool testScreen(SDL_Window *window, SDL_Renderer *renderer, Book::Type bookID, int storyID)
{
    auto font_size = 20;
    auto text_space = 8;

    auto *introduction = "This is the DEBUG screen. Testing facilities for various gamebook functions such as COMBAT, SKILL CHECKS, MAGIC, etc, can be accessed here. While the game is still in the ALPHA stage, this is the default screen.\n\nTests:\n1 - Combat\n2 - Map\n3 - Team Skill check\n4 - Individual Skill check";

    auto text = createText(introduction, FONT_GARAMOND, 28, clrDB, textwidth - 2 * text_space);

    auto title = "Debug";

    auto splash = createImage("images/legendary-kingdoms-logo.png");

    // Render window
    if (window && renderer && text && splash)
    {
        SDL_SetWindowTitle(window, title);

        auto current = -1;

        auto selected = false;

        auto main_buttonh = 48;

        const char *choices[5] = {"1", "2", "3", "4", "Exit"};

        auto controls = createHTextButtons(choices, 5, main_buttonh, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);

        controls[0].Type = Control::Type::COMBAT;
        controls[1].Type = Control::Type::MAP;
        controls[2].Type = Control::Type::TEAM_SKILL;
        controls[3].Type = Control::Type::SKILL;
        controls[4].Type = Control::Type::QUIT;

        auto done = false;

        auto text_space = 8;

        auto Party = Party::Base();

        while (!done)
        {
            // Fill the surface with background
            fillWindow(renderer, intWH);

            fitImage(renderer, splash, startx, starty, splashw, text_bounds);

            fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

            renderText(renderer, text, intBK, startx * 2 + splashw + text_space, starty + text_space, SCREEN_HEIGHT * (1.0 - 2 * Margin) - 2 * text_space, 0);

            renderTextButtons(renderer, controls, FONT_MASON, current, clrWH, intDB, intLB, font_size + 2, TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            Control::Type result;

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            auto combat = Engine::Combat::NONE;

            if (selected && current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::COMBAT)
                {
                    selectParty(window, renderer, Book::Type::BOOK1, Party);

                    std::vector<Monster::Base> monsters = {
                        Monster::Base("Goblin", 4, 5, 4, 6, 0),
                        Monster::Base("Orc Bodyguard", 6, 4, 4, 10, 0)};

                    combat = combatScreen(window, renderer, Party, monsters, true, -1, false);

                    done = false;

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::MAP)
                {
                    mapScreen(window, renderer);

                    done = false;

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::TEAM_SKILL)
                {
                    selectParty(window, renderer, Book::Type::BOOK1, Party);

                    auto selection = std::vector<int>();

                    skillCheck(window, renderer, Party.Party, 2, Attribute::Type::STEALTH, 4, 4, selection);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::SKILL)
                {
                    selectParty(window, renderer, Book::Type::BOOK1, Party);

                    auto selection = std::vector<int>();

                    skillCheck(window, renderer, Party.Party, 1, Attribute::Type::LORE, 4, 3, selection);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::QUIT)
                {
                    done = true;

                    break;
                }
            }

            SDL_SetWindowTitle(window, title);
        }

        SDL_FreeSurface(splash);
        SDL_FreeSurface(text);

        splash = NULL;
        text = NULL;
    }

    return false;
}

int main(int argc, char **argv)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    auto title = "Legendary Kingdoms 1 - The Valley of Bones";

    createWindow(SDL_INIT_VIDEO, &window, &renderer, title, "icons/toxic.png");

    auto numGamePads = Input::InitializeGamePads();

    auto quit = false;

    auto storyID = 1;

    auto bookID = Book::Type::BOOK1;

    if (argc > 1)
    {
        storyID = std::atoi(argv[1]);
    }

    if (window && renderer)
    {
#if defined(DEBUG)
        testScreen(window, renderer, bookID, storyID);
#else
        //introScreen(window, renderer);

        mainScreen(window, renderer, bookID, storyID);
#endif

        // Destroy window and renderer

        if (renderer)
        {
            SDL_DestroyRenderer(renderer);

            renderer = NULL;
        }

        if (window)
        {
            SDL_DestroyWindow(window);

            window = NULL;
        }
    }

    // Quit SDL subsystems
    IMG_Quit();

    SDL_Quit();

    return 0;
}
