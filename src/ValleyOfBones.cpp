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
bool armyScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Army::Base> army);
bool assignTeams(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Engine::TeamAssignment> teams, int min_teams);
bool inventoryScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Character::Base &character, int equipment_limit, bool InCombat);
bool harbourScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *harbour);
bool introScreen(SDL_Window *window, SDL_Renderer *renderer);
bool mainScreen(SDL_Window *window, SDL_Renderer *renderer, Book::Type bookID, int storyID);
bool partyDetails(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party);
bool processStory(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Book::Type book, Story::Base *story);
bool retreatArmy(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, int unit, Location::Type &location, int threshold, int rolls);
bool selectParty(SDL_Window *window, SDL_Renderer *renderer, Book::Type bookID, Party::Base &party);
bool shopScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Engine::EquipmentPrice> shop, Character::Base &character);
bool skillCheck(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, int team_size, Attribute::Type skill, int difficulty, int success, std::vector<int> &selection);
bool skillTestScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<int> team, Attribute::Type Skill, int difficulty, int success, bool useEquipment);
bool takeScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Equipment::Base> equipment, int TakeLimit, bool back_button);
bool testScreen(SDL_Window *window, SDL_Renderer *renderer, Book::Type bookID, int storyID);
bool viewParty(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, bool inCombat);

int armourSave(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, int damage);
int assignDamage(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> &party, Team::Type team);
int attackScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Monster::Base> &monsters, int combatant, int opponent, int direction, bool useEquipment);
int castSpell(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Monster::Base> &monsters, std::vector<int> hasAttacked, int combatRound, Control::Type mode);
int magicAttackScreen(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> &party, std::vector<Monster::Base> &monsters, Spells::Base &spell, int combatant, int opponent, int fighting_score);
int selectOpponent(SDL_Window *window, SDL_Renderer *renderer, std::vector<Monster::Base> &monsters, std::vector<int> previousTargets, int combatRound);
int selectPartyMember(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, Equipment::Base equipment, Control::Type mode);

Attribute::Type selectAttribute(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, int increase);

Engine::Combat combatScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Monster::Base> &monsters, bool canFlee, int fleeRound, bool useEquipment);

Story::Base *findStory(Engine::Destination destination);
Story::Base *processChoices(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *story);
Story::Base *renderChoices(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *story);

std::vector<int> selectSpell(SDL_Window *window, SDL_Renderer *renderer, Character::Base &caster, std::vector<Spells::Base> &spells, int select_limit, Spells::Select mode);

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

void renderTextButtons(SDL_Renderer *renderer, std::vector<TextButton> controls, const char *ttf, int selected, SDL_Color fg, Uint32 bg, Uint32 bgSelected, int fontsize, int offsetx, int scrolly, bool hide_scroll, int style = TTF_STYLE_NORMAL)
{
    if (controls.size() > 0)
    {
        for (auto i = 0; i < controls.size(); i++)
        {
            if (!hide_scroll && (controls[i].Type == Control::Type::SCROLL_UP || controls[i].Type == Control::Type::SCROLL_DOWN))
            {
                auto space = 8;
                auto pts = 4;

                SDL_Surface *button_surface = NULL;

                if (controls[i].Type == Control::Type::SCROLL_UP)
                {
                    button_surface = createImage("icons/up-arrow.png");

                    if (button_surface)
                    {
                        renderImage(renderer, button_surface, offsetx, texty + border_space);
                    }
                }
                else
                {
                    button_surface = createImage("icons/down-arrow.png");

                    if (button_surface)
                    {
                        renderImage(renderer, button_surface, offsetx, scrolly);
                    }
                }

                if (i == selected)
                {
                    for (auto size = pts; size >= 0; size--)
                    {
                        auto Y = controls[i].Type == Control::Type::SCROLL_UP ? texty + border_space : scrolly;

                        if (i == selected)
                        {
                            SDL_Rect rect;

                            rect.w = controls[i].W + 2 * (space - size);
                            rect.h = controls[i].H + 2 * (space - size);
                            rect.x = controls[i].X - space + size;
                            rect.y = controls[i].Y - space + size;

                            SDL_SetRenderDrawColor(renderer, R(bgSelected), G(bgSelected), B(bgSelected), A(bgSelected));
                            SDL_RenderDrawRect(renderer, &rect);
                        }
                    }
                }

                if (button_surface)
                {
                    SDL_FreeSurface(button_surface);

                    button_surface = NULL;
                }
            }
            else if (controls[i].Type != Control::Type::SCROLL_UP && controls[i].Type != Control::Type::SCROLL_DOWN)
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
}

void renderButtons(SDL_Renderer *renderer, std::vector<Button> controls, int current, int fg, int space, int pts)
{
    if (controls.size() > 0)
    {
        for (auto i = 0; i < controls.size(); i++)
        {
            SDL_Rect rect;

            if (i == current)
            {
                for (auto size = pts; size >= 0; size--)
                {
                    rect.w = controls[i].W + 2 * (space - size);
                    rect.h = controls[i].H + 2 * (space - size);
                    rect.x = controls[i].X - space + size;
                    rect.y = controls[i].Y - space + size;

                    SDL_SetRenderDrawColor(renderer, R(fg), G(fg), B(fg), A(fg));
                    SDL_RenderDrawRect(renderer, &rect);
                }
            }

            renderImage(renderer, controls[i].Surface, controls[i].X, controls[i].Y);
        }
    }
}

void renderButtons(SDL_Renderer *renderer, std::vector<Button> controls, int current, int fg, int space, int pts, bool hide_scroll)
{
    if (controls.size() > 0)
    {
        for (auto i = 0; i < controls.size(); i++)
        {
            if (!hide_scroll || (controls[i].Type != Control::Type::SCROLL_UP && controls[i].Type != Control::Type::SCROLL_DOWN))
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

std::vector<TextButton> createHTextButtons(const char **choices, int num, int text_buttonh, int text_x, int text_y, bool has_scrolls)
{
    if (!has_scrolls)
    {
        return createHTextButtons(choices, num, text_buttonh, text_x, text_y);
    }

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

        controls.push_back(TextButton(0, "", 0, 0, 0, 1, -1, -1, 32, 32, Control::Type::SCROLL_UP));
        controls.push_back(TextButton(1, "", 1, 1, 0, 2, -1, -1, 32, 32, Control::Type::SCROLL_DOWN));

        for (auto i = 0; i < num; i++)
        {
            auto left = i > 0 ? i + 1 : i + 2;
            auto right = i < num - 1 ? i + 3 : i + 2;
            auto up = 1;
            auto down = i + 2;

            auto x = text_x + i * (text_buttonw + text_space * 2) + text_space;

            auto button = TextButton(i + 2, choices[i], left, right, up, down, x, text_y, text_buttonw, text_buttonh);

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

SDL_Surface *createHeaderButton(SDL_Window *window, const char *font, int font_size, const char *text, SDL_Color color, Uint32 bg, int w, int h, int x)
{
    auto button = SDL_CreateRGBSurface(0, w, h, arrow_size, 0, 0, 0, 0);
    auto text_surface = createText(text, font, font_size, color, w, TTF_STYLE_NORMAL);

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

        if (character.Team != Team::Type::NONE)
        {
            if (!Engine::IS_CHARACTER(character.Team))
            {
                character_text += ", Team: " + std::string(Team::Descriptions[character.Team]);
            }
        }
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

std::vector<Button> attributeList(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, std::vector<Attribute::Type> &attributes, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (attributes.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            std::string attributes_string = std::string(Attribute::Descriptions[attributes[index]]);

            if (character.Type != Character::Type::NONE)
            {
                attributes_string += ": " + std::to_string(Engine::SCORE(character, attributes[index]));
            }

            auto button = createHeaderButton(window, FONT_GARAMOND, 28, attributes_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, button, i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = button->w;

            controls[i].H = button->h;
        }
    }

    auto idx = controls.size();

    if (attributes.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (attributes.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), (texty + text_bounds - arrow_size - border_space), Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/yes.png", idx, idx, attributes.size() > 0 ? idx - 1 : idx, idx, startx, buttony, Control::Type::CONFIRM));

    return controls;
}

std::vector<Button> armyList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Army::Base> &army, int start, int last, int limit, int offsetx, int offsety, bool party_controls)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (army.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto unit = army[index];

            std::string ship_string = "[" + std::string(unit.Name) + "] Strength: " + std::to_string(unit.Strength) + ", Morale: " + std::to_string(unit.Morale);

            ship_string += "\nPosition: " + std::string(Location::BattleFieldDescription[unit.Position]) + " Garrison: " + std::string(Location::Description[unit.Garrison]);

            auto button = createHeaderButton(window, FONT_GARAMOND, 24, ship_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, button, i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = button->w;

            controls[i].H = button->h;
        }
    }

    auto idx = controls.size();

    if (army.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (army.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), (texty + text_bounds - arrow_size - border_space), Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    idx = controls.size();

    if (party_controls)
    {
        auto text_y = (int)(SCREEN_HEIGHT * (1.0 - Margin)) - 48;

        controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "ARMY", clrWH, intDB, 220, 48, -1), idx, idx + 1, army.size() > 0 ? idx - 1 : idx, idx, startx, text_y, Control::Type::ARMY));
        controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "FLEET", clrWH, intDB, 220, 48, -1), idx, idx + 2, army.size() > 0 ? idx - 1 : idx + 1, idx + 1, startx + (220 + button_space), text_y, Control::Type::FLEET));
        controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "ROMANCE", clrWH, intDB, 220, 48, -1), idx + 1, idx + 3, army.size() > 0 ? idx - 1 : idx + 2, idx + 2, startx + 2 * (220 + button_space), text_y, Control::Type::ROMANCE));
        controls.push_back(Button(idx + 3, createHeaderButton(window, FONT_DARK11, 22, "BACK", clrWH, intDB, 220, 48, -1), idx + 2, idx + 3, army.size() > 0 ? idx - 1 : idx + 3, idx + 3, startx + 3 * (220 + button_space), text_y, Control::Type::BACK));
    }
    else
    {
        controls.push_back(Button(idx, "icons/yes.png", army.size() > 0 ? idx - 1 : idx, idx + 1, army.size() > 0 ? idx - 1 : idx, idx, startx, buttony, Control::Type::CONFIRM));
        controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, army.size() > 0 ? idx - 1 : idx + 1, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH) - buttonw), buttony, Control::Type::BACK));
    }

    return controls;
}

std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (ships.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto ship = ships[index];

            std::string ship_string = "[" + std::string(ship.Name) + "] Fighting: " + std::to_string(ship.Fighting) + ", Health: " + std::to_string(ship.Health) + ", Cargo Units: " + std::to_string(ship.MaximumCargo);

            ship_string += "\nLocation: " + std::string(Location::Description[ship.Location]);

            if (ship.Cargo.size() > 0)
            {
                ship_string += " Cargo: ";

                for (auto j = 0; j < ship.Cargo.size(); j++)
                {
                    if (j > 0)
                    {
                        ship_string += ", ";
                    }

                    ship_string += Cargo::Description[ship.Cargo[j]];
                }
            }

            auto button = createHeaderButton(window, FONT_GARAMOND, 24, ship_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, button, i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = button->w;

            controls[i].H = button->h;
        }
    }

    auto idx = controls.size();

    if (ships.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (ships.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), (texty + text_bounds - arrow_size - border_space), Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    idx = controls.size();

    auto text_y = (int)(SCREEN_HEIGHT * (1.0 - Margin)) - 48;

    controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "ARMY", clrWH, intDB, 220, 48, -1), idx, idx + 1, ships.size() > 0 ? idx - 1 : idx, idx, startx, text_y, Control::Type::ARMY));
    controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "FLEET", clrWH, intDB, 220, 48, -1), idx, idx + 2, ships.size() > 0 ? idx - 1 : idx + 1, idx + 1, startx + (220 + button_space), text_y, Control::Type::FLEET));
    controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "ROMANCE", clrWH, intDB, 220, 48, -1), idx + 1, idx + 3, ships.size() > 0 ? idx - 1 : idx + 2, idx + 2, startx + 2 * (220 + button_space), text_y, Control::Type::ROMANCE));
    controls.push_back(Button(idx + 3, createHeaderButton(window, FONT_DARK11, 22, "BACK", clrWH, intDB, 220, 48, -1), idx + 2, idx + 3, ships.size() > 0 ? idx - 1 : idx + 3, idx + 3, startx + 3 * (220 + button_space), text_y, Control::Type::BACK));

    return controls;
}

std::vector<Button> romanceList(SDL_Window *window, SDL_Renderer *renderer, std::map<Character::Romance, int> &hearts, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (hearts.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto ship = hearts.begin();

            std::advance(ship, index);

            Character::Romance romance = ship->first;

            auto from = Character::ALL[Engine::FIND_CHARACTER(Character::ALL, romance.first)];
            auto to = Character::ALL[Engine::FIND_CHARACTER(Character::ALL, romance.second)];

            auto count = ship->second;

            std::string ship_string = std::string(from.Name) + " has " + std::to_string(count) + " heart";

            if (count > 1)
            {
                ship_string += "s";
            }

            ship_string += " for " + std::string(to.Name) + ".";

            auto button = createHeaderButton(window, FONT_GARAMOND, 24, ship_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, button, i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = button->w;

            controls[i].H = button->h;
        }
    }

    auto idx = controls.size();

    if (hearts.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (hearts.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), (texty + text_bounds - arrow_size - border_space), Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    idx = controls.size();

    auto text_y = (int)(SCREEN_HEIGHT * (1.0 - Margin)) - 48;

    controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "ARMY", clrWH, intDB, 220, 48, -1), idx, idx + 1, hearts.size() > 0 ? idx - 1 : idx, idx, startx, text_y, Control::Type::ARMY));
    controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "FLEET", clrWH, intDB, 220, 48, -1), idx, idx + 2, hearts.size() > 0 ? idx - 1 : idx + 1, idx + 1, startx + (220 + button_space), text_y, Control::Type::FLEET));
    controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "ROMANCE", clrWH, intDB, 220, 48, -1), idx + 1, idx + 3, hearts.size() > 0 ? idx - 1 : idx + 2, idx + 2, startx + 2 * (220 + button_space), text_y, Control::Type::ROMANCE));
    controls.push_back(Button(idx + 3, createHeaderButton(window, FONT_DARK11, 22, "BACK", clrWH, intDB, 220, 48, -1), idx + 2, idx + 3, hearts.size() > 0 ? idx - 1 : idx + 3, idx + 3, startx + 3 * (220 + button_space), text_y, Control::Type::BACK));

    return controls;
}

std::vector<Button> teamsList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Team::Type> &teams, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (teams.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            std::string teams_string = std::string(Team::Descriptions[teams[index]]);

            auto button = createHeaderButton(window, FONT_GARAMOND, 28, teams_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, button, i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = button->w;

            controls[i].H = button->h;
        }
    }

    auto idx = controls.size();

    if (teams.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (teams.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), (texty + text_bounds - arrow_size - border_space), Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/yes.png", idx, idx, teams.size() > 0 ? idx - 1 : idx, idx, startx, buttony, Control::Type::CONFIRM));
    controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, teams.size() > 0 ? idx - 1 : idx + 1, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH) - buttonw), buttony, Control::Type::BACK));

    return controls;
}

bool partyDetails(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party)
{
    auto *title = "Legendary Kingdoms: Party";

    auto font_size = 28;

    TTF_Init();

    auto font_mason = TTF_OpenFont(FONT_MASON, 32);
    auto font_mason2 = TTF_OpenFont(FONT_MASON, 22);
    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
    auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

    TTF_SetFontKerning(font_dark11, 0);

    auto box_space = 10;
    auto character_box = (int)(text_bounds * 2 / 3);

    // Render window
    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        auto current = 0;

        auto selected = false;

        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;
        auto text_space = 8;

        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space - infoh) / (font_size * 2 + text_space * 4);
        auto last = offset + limit;

        if (last > party.Army.size())
        {
            last = party.Army.size();
        }

        auto controls = std::vector<Button>();

        controls = armyList(window, renderer, party.Army, offset, last, limit, textx, (texty + infoh), true);

        auto current_mode = Control::Type::ARMY;

        bool scrollUp = false;
        bool scrollDown = false;
        auto scrollSpeed = 1;
        bool hold = false;

        auto done = false;

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

            if (current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::ARMY)
                {
                    if (current_mode != Control::Type::ARMY)
                    {
                        offset = 0;

                        last = offset + limit;

                        if (last > party.Army.size())
                        {
                            last = party.Army.size();
                        }

                        controls = armyList(window, renderer, party.Army, offset, last, limit, textx, texty + infoh, true);

                        if (party.Army.size() - last > 0)
                        {
                            current = last - offset + 1;
                        }
                        else
                        {
                            current = last - offset;
                        }
                    }

                    current_mode = Control::Type::ARMY;
                }
                else if (controls[current].Type == Control::Type::FLEET)
                {
                    if (current_mode != Control::Type::FLEET)
                    {
                        offset = 0;

                        last = offset + limit;

                        if (last > party.Fleet.size())
                        {
                            last = party.Fleet.size();
                        }

                        controls = shipList(window, renderer, party.Fleet, offset, last, limit, textx, texty + infoh);

                        if (party.Fleet.size() - last > 0)
                        {
                            current = last - offset + 2;
                        }
                        else
                        {
                            current = last - offset + 1;
                        }
                    }

                    current_mode = Control::Type::FLEET;
                }
                else if (controls[current].Type == Control::Type::ROMANCE)
                {
                    if (current_mode != Control::Type::ROMANCE)
                    {
                        offset = 0;

                        last = offset + limit;

                        if (last > party.Hearts.size())
                        {
                            last = party.Hearts.size();
                        }

                        controls = romanceList(window, renderer, party.Hearts, offset, last, limit, textx, texty + infoh);

                        if (party.Hearts.size() - last > 0)
                        {
                            current = last - offset + 3;
                        }
                        else
                        {
                            current = last - offset + 2;
                        }
                    }

                    current_mode = Control::Type::ROMANCE;
                }
            }

            fillRect(renderer, textwidth, (text_bounds - infoh), textx, (texty + infoh), intBE);

            if (current_mode == Control::Type::ARMY)
            {
                putHeader(renderer, "Army", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
            else if (current_mode == Control::Type::FLEET)
            {
                putHeader(renderer, "Fleet", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
            else if (current_mode == Control::Type::ROMANCE)
            {
                putHeader(renderer, "Romance", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
            else
            {
                fillRect(renderer, textwidth, infoh, textx, texty, intBR);
            }

            for (auto i = offset; i < last; i++)
            {
                auto index = i - offset;

                if (current != index)
                {
                    if (index >= 0 && index < controls.size())
                    {
                        drawRect(renderer, controls[index].W + 16, controls[index].H + 16, controls[index].X - 8, controls[index].Y - 8, intBK);
                    }
                }
            }

            renderButtons(renderer, controls, current, intLB, 8, 4);

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

                        if (current_mode == Control::Type::ARMY)
                        {
                            if (last > party.Army.size())
                            {
                                last = party.Army.size();
                            }

                            controls = armyList(window, renderer, party.Army, offset, last, limit, textx, texty + infoh, true);
                        }
                        else if (current_mode == Control::Type::FLEET)
                        {
                            if (last > party.Fleet.size())
                            {
                                last = party.Fleet.size();
                            }

                            controls = shipList(window, renderer, party.Fleet, offset, last, limit, textx, texty + infoh);
                        }
                        else if (current_mode == Control::Type::ROMANCE)
                        {
                            if (last > party.Hearts.size())
                            {
                                last = party.Hearts.size();
                            }

                            controls = romanceList(window, renderer, party.Hearts, offset, last, limit, textx, texty + infoh);
                        }

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
                    if (current_mode == Control::Type::ARMY)
                    {
                        if (party.Army.size() - last > 0)
                        {
                            if (offset < party.Army.size() - limit)
                            {
                                offset += scrollSpeed;
                            }

                            if (offset > party.Army.size() - limit)
                            {
                                offset = party.Army.size() - limit;
                            }

                            last = offset + limit;

                            if (last > party.Army.size())
                            {
                                last = party.Army.size();
                            }

                            controls = armyList(window, renderer, party.Army, offset, last, limit, textx, texty + infoh, true);

                            SDL_Delay(50);

                            if (offset > 0)
                            {
                                if (controls[current].Type != Control::Type::SCROLL_DOWN)
                                {
                                    current++;
                                }
                            }
                        }

                        if (party.Army.size() - last <= 0)
                        {
                            selected = false;

                            current = -1;
                        }
                    }
                    else if (current_mode == Control::Type::FLEET)
                    {
                        if (party.Fleet.size() - last > 0)
                        {
                            if (offset < party.Fleet.size() - limit)
                            {
                                offset += scrollSpeed;
                            }

                            if (offset > party.Fleet.size() - limit)
                            {
                                offset = party.Fleet.size() - limit;
                            }

                            last = offset + limit;

                            if (last > party.Fleet.size())
                            {
                                last = party.Fleet.size();
                            }

                            controls = shipList(window, renderer, party.Fleet, offset, last, limit, textx, texty + infoh);

                            SDL_Delay(50);

                            if (offset > 0)
                            {
                                if (controls[current].Type != Control::Type::SCROLL_DOWN)
                                {
                                    current++;
                                }
                            }
                        }

                        if (party.Fleet.size() - last <= 0)
                        {
                            selected = false;

                            current = -1;
                        }
                    }
                    else if (current_mode == Control::Type::ROMANCE)
                    {
                        if (party.Hearts.size() - last > 0)
                        {
                            if (offset < party.Hearts.size() - limit)
                            {
                                offset += scrollSpeed;
                            }

                            if (offset > party.Hearts.size() - limit)
                            {
                                offset = party.Hearts.size() - limit;
                            }

                            last = offset + limit;

                            if (last > party.Hearts.size())
                            {
                                last = party.Hearts.size();
                            }

                            controls = romanceList(window, renderer, party.Hearts, offset, last, limit, textx, texty + infoh);

                            SDL_Delay(50);

                            if (offset > 0)
                            {
                                if (controls[current].Type != Control::Type::SCROLL_DOWN)
                                {
                                    current++;
                                }
                            }
                        }

                        if (party.Hearts.size() - last <= 0)
                        {
                            selected = false;

                            current = -1;
                        }
                    }

                    SDL_Delay(50);

                    if (offset > 0)
                    {
                        if (controls[current].Type != Control::Type::SCROLL_DOWN)
                        {
                            current++;
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::BACK)
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

bool viewParty(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, bool inCombat)
{
    SDL_Surface *adventurer = NULL;
    SDL_Surface *text = NULL;
    SDL_Surface *code_text = NULL;

    auto *title = "Legendary Kingdoms: View Party";

    auto font_size = 20;
    auto garamond_size = 24;

    TTF_Init();

    auto font_mason = TTF_OpenFont(FONT_MASON, 24);
    auto font_mason2 = TTF_OpenFont(FONT_MASON, 22);
    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, garamond_size);
    auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

    TTF_SetFontKerning(font_dark11, 0);

    auto box_space = 10;
    auto character_box = (int)(text_bounds * 2 / 3);
    auto offset = 0;

    // Render window
    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        const char *choices[6] = {"PARTY", "PREVIOUS", "NEXT", "EQUIPMENT", "SPELLBOOK", "BACK"};

        auto current = 2;

        auto selected = false;

        auto main_buttonh = (int)(0.06 * SCREEN_HEIGHT);

        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;

        auto controls = createHTextButtons(choices, 6, main_buttonh, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh), true);

        auto scrolly = texty + text_bounds - arrow_size - border_space;
        auto offsetx = (int)((1.0 - Margin) * SCREEN_WIDTH) - arrow_size;

        controls[0].X = offsetx;
        controls[0].Y = texty + border_space;
        controls[1].X = offsetx;
        controls[1].Y = scrolly;
        controls[2].Type = Control::Type::PARTY;
        controls[3].Type = Control::Type::MINUS;
        controls[4].Type = Control::Type::PLUS;
        controls[5].Type = Control::Type::EQUIPMENT;
        controls[6].Type = Control::Type::SPELLBOOK;
        controls[7].Type = Control::Type::BACK;

        auto done = false;

        auto text_space = 8;

        auto Party = Party::Base();

        auto character = 0;

        auto current_mode = Control::Type::PARTY;

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

        if (party.Codes.size() > 0)
        {
            std::string code_string = "";

            for (auto i = 0; i < party.Codes.size(); i++)
            {
                if (i > 0)
                {
                    code_string += ", ";
                }

                code_string += std::string(Codes::Prefix[party.Codes[i].Type]) + std::to_string(party.Codes[i].Code);
            }

            code_text = createText(code_string.c_str(), FONT_MASON, 24, clrBK, textwidth - 2 * text_space, TTF_STYLE_NORMAL);
        }

        auto compact = (code_text && code_text->h <= text_bounds - infoh - 2 * text_space) || code_text == NULL;

        while (!done)
        {
            // Fill the surface with background
            fillWindow(renderer, intWH);

            auto adventurerh = splashw;

            if (current_mode != Control::Type::PARTY && adventurer != NULL)
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

                fillRect(renderer, textwidth, (text_bounds - infoh), textx, (texty + infoh), intBE);

                if (code_text)
                {
                    renderText(renderer, code_text, intBE, textx + text_space, texty + infoh + text_space, text_bounds - infoh - 2 * text_space, offset);
                }
            }

            if (current_mode != Control::Type::PARTY && text != NULL)
            {
                fillRect(renderer, textwidth, character_box, textx, texty, intBE);

                renderText(renderer, text, intBE, textx + text_space, texty + text_space, character_box - 2 * text_space, 0);
            }

            if (current_mode != Control::Type::PARTY && character >= 0 && character < party.Party.size() && adventurer)
            {
                putText(renderer, party.Party[character].Name, font_mason, -1, clrDB, intWH, TTF_STYLE_NORMAL, splashw, infoh, startx, adventurerh + infoh - text_space);
            }

            if (current_mode != Control::Type::PARTY && character >= 0 && character < party.Party.size())
            {
                if ((party.Party[character].Status.size() > 0) || (party.Party[character].Team != Team::Type::NONE && !Engine::IS_CHARACTER(party.Party[character].Team)))
                {
                    putHeader(renderer, "Status", font_mason2, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, starty + character_box + 10);

                    std::string status_string = "";

                    for (auto i = 0; i < party.Party[character].Status.size(); i++)
                    {
                        if (i > 0)
                        {
                            status_string += ", ";
                        }

                        status_string += Character::StatusDescriptions[party.Party[character].Status[i]];
                    }

                    if (party.Party[character].Team != Team::Type::NONE)
                    {
                        if (!Engine::IS_CHARACTER(party.Party[character].Team))
                        {
                            if (status_string.length() > 0)
                            {
                                status_string += ", ";
                            }

                            status_string += std::string(Team::Descriptions[party.Party[character].Team]);

                            if (party.Party[character].Team != Team::Type::SOLO)
                            {
                                status_string += " team";
                            }
                        }
                    }

                    putText(renderer, status_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, (text_bounds - character_box) - infoh - box_space, textx, starty + text_bounds + infoh - (text_bounds - character_box) + box_space);
                }
            }

            if (current_mode != Control::Type::PARTY)
            {
                if (current_mode == Control::Type::SPELLBOOK && party.Party[character].SpellCaster && party.Party[character].SpellBook.size() > 0)
                {
                    putHeader(renderer, "SPELLBOOK", font_mason2, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, starty + character_box + 10);

                    std::string spellbook_string = "";

                    for (auto i = 0; i < party.Party[character].SpellBook.size(); i++)
                    {
                        if (i > 0)
                        {
                            spellbook_string += ", ";
                        }

                        spellbook_string += party.Party[character].SpellBook[i].Name;
                    }

                    putText(renderer, spellbook_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, (text_bounds - character_box) - infoh - box_space, textx, starty + text_bounds + infoh - (text_bounds - character_box) + box_space);
                }
                else if (current_mode == Control::Type::EQUIPMENT && party.Party[character].Equipment.size() > 0)
                {
                    putHeader(renderer, "EQUIPMENT", font_mason2, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, starty + character_box + 10);

                    std::string equipment_string = "";

                    for (auto i = 0; i < party.Party[character].Equipment.size(); i++)
                    {
                        if (i > 0)
                        {
                            equipment_string += ", ";
                        }

                        equipment_string += party.Party[character].Equipment[i].Name;

                        if (party.Party[character].Equipment[i].TwoHanded)
                        {
                            equipment_string += "*";
                        }

                        if (party.Party[character].Equipment[i].Attribute != Attribute::Type::NONE || party.Party[character].Equipment[i].AdditionalSlots > 0)
                        {
                            equipment_string += " (";

                            if (party.Party[character].Equipment[i].Attribute != Attribute::Type::NONE)
                            {
                                equipment_string += "+" + std::to_string(party.Party[character].Equipment[i].Modifier) + " " + std::string(Attribute::Descriptions[party.Party[character].Equipment[i].Attribute]);
                            }

                            if (party.Party[character].Equipment[i].AdditionalSlots > 0)
                            {
                                if (party.Party[character].Equipment[i].Attribute != Attribute::Type::NONE)
                                {
                                    equipment_string += ", ";
                                }

                                equipment_string += std::to_string(party.Party[character].Equipment[i].AdditionalSlots + 1) + " slots";
                            }

                            equipment_string += +")";
                        }
                    }

                    putText(renderer, equipment_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, (text_bounds - character_box) - infoh - box_space, textx, starty + text_bounds + infoh - (text_bounds - character_box) + box_space);
                }
            }

            renderTextButtons(renderer, controls, FONT_DARK11, current, clrWH, intDB, intLB, font_size + 2, offsetx, scrolly, ((current_mode != Control::Type::PARTY || compact) ? true : false), TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;
            int scrollSpeed = 20;

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (current == 2)
            {
                current_mode = Control::Type::PARTY;
            }
            else if (current > 2)
            {
                if (current > 2 && current < controls.size() && controls[current].Type != Control::Type::BACK)
                {
                    current_mode = controls[current].Type;
                }
            }

            if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
            {
                if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                {
                    if (code_text)
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
                    if (code_text)
                    {
                        if (code_text->h >= text_bounds - 2 * text_space - infoh)
                        {
                            if (offset < code_text->h - text_bounds + 2 * text_space + infoh)
                            {
                                offset += scrollSpeed;
                            }

                            if (offset > code_text->h - text_bounds + 2 * text_space + infoh)
                            {
                                offset = code_text->h - text_bounds + 2 * text_space + infoh;
                            }
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::PARTY && !hold)
                {
                    partyDetails(window, renderer, party);

                    done = false;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::MINUS && !hold)
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
                else if (controls[current].Type == Control::Type::PLUS && !hold)
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
                else if (controls[current].Type == Control::Type::EQUIPMENT && !hold)
                {
                    if (character >= 0 && character < party.Party.size())
                    {
                        inventoryScreen(window, renderer, party, party.Party[character], -1, inCombat);
                    }

                    done = false;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::SPELLBOOK && !hold)
                {
                    done = false;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
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

    if (code_text != NULL)
    {
        SDL_FreeSurface(code_text);

        code_text = NULL;
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

    auto *title = "Legendary Kingdoms: Select adventurers for your party";

    auto font_size = 20;
    auto garamond_size = 24;

    TTF_Init();

    auto font_mason = TTF_OpenFont(FONT_MASON, 32);
    auto font_mason2 = TTF_OpenFont(FONT_MASON, 22);
    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, garamond_size);

    auto box_space = 10;
    auto character_box = (int)(text_bounds * 2 / 3);

    auto flash_message = false;

    auto flash_color = intRD;

    std::string message = "";

    Uint32 start_ticks = 0;

    Uint32 duration = 3000;

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

        auto controls_add = createHTextButtons(choices_add, 5, main_buttonh, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
        auto controls_del = createHTextButtons(choices_del, 5, main_buttonh, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));

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
                putHeader(renderer, std::string("PARTY (Limit: " + std::to_string(party.Limit) + ")").c_str(), font_mason2, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, starty + character_box + 10);

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

                putText(renderer, party_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, (text_bounds - character_box) - infoh - box_space, textx, starty + text_bounds + infoh - (text_bounds - character_box) + box_space);
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
                                            spell_selection = selectSpell(window, renderer, character, spellBook, 3, Spells::Select::SPELLBOOK);

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

                        done = true;

                        current = -1;

                        selected = false;
                    }
                    else
                    {
                        flash_message = true;

                        message = "Please select up to 4 party members!";

                        start_ticks = SDL_GetTicks();

                        flash_color = intRD;
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

        auto marginw = ((int)((1.0 - 2.0 * Margin) * SCREEN_WIDTH));

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

            if (list[index].TwoHanded)
            {
                item_string += "*";
            }

            if (list[index].Attribute != Attribute::Type::NONE || list[index].AdditionalSlots > 0)
            {
                item_string += " (";

                if (list[index].Attribute != Attribute::Type::NONE)
                {
                    item_string += "+" + std::to_string(list[index].Modifier) + " " + std::string(Attribute::Descriptions[list[index].Attribute]);
                }

                if (list[index].AdditionalSlots > 0)
                {
                    if (list[index].Attribute != Attribute::Type::NONE)
                    {
                        item_string += ", ";
                    }

                    item_string += std::to_string(list[index].AdditionalSlots + 1) + " slots";
                }

                item_string += ")";
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
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (list.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), (texty + text_bounds - arrow_size - border_space), Control::Type::SCROLL_DOWN));

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

        controls.push_back(Button(idx, "icons/back-button.png", idx - 1, idx, list.size() > 0 ? (last - start) : idx, idx, ((int)((1.0 - Margin) * SCREEN_WIDTH) - buttonw), buttony, Control::Type::BACK));
    }

    return controls;
}

std::vector<Button> equipmentList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Equipment::Base> list, int start, int last, int limit, int offsety, int scrolly)
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

            if (list[index].TwoHanded)
            {
                item_string += "*";
            }

            if (list[index].Attribute != Attribute::Type::NONE || list[index].AdditionalSlots > 0)
            {
                item_string += " (";

                if (list[index].Attribute != Attribute::Type::NONE)
                {
                    item_string += "+" + std::to_string(list[index].Modifier) + " " + std::string(Attribute::Descriptions[list[index].Attribute]);
                }

                if (list[index].AdditionalSlots > 0)
                {
                    if (list[index].Attribute != Attribute::Type::NONE)
                    {
                        item_string += ", ";
                    }

                    item_string += std::to_string(list[index].AdditionalSlots + 1) + " slots";
                }

                item_string += ")";
            }

            auto text = createText(item_string.c_str(), FONT_GARAMOND, font_size, clrBK, textwidth - 4 * text_space, TTF_STYLE_NORMAL);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

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
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (list.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), scrolly, Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/yes.png", idx, idx + 1, (list.size() > 0 ? idx - 1 : idx), idx, startx, buttony, Control::Type::USE));
    controls.push_back(Button(idx + 1, "icons/no.png", idx, idx + 2, (list.size() > 0 ? (last - start) : idx + 1), idx + 1, startx + gridsize, buttony, Control::Type::DROP));
    controls.push_back(Button(idx + 2, "icons/interaction.png", idx + 1, idx + 3, (list.size() > 0 ? (last - start) : idx + 2), idx + 2, startx + 2 * gridsize, buttony, Control::Type::TRANSFER));
    controls.push_back(Button(idx + 3, "icons/vault.png", idx + 2, idx + 4, (list.size() > 0 ? (last - start) : idx + 3), idx + 3, startx + 3 * gridsize, buttony, Control::Type::VAULT));
    controls.push_back(Button(idx + 4, "icons/back-button.png", idx + 3, idx + 4, (list.size() > 0 ? (last - start) : idx + 4), idx + 4, ((int)((1.0 - Margin) * SCREEN_WIDTH) - buttonw), buttony, Control::Type::BACK));

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

            auto button = createHeaderButton(window, FONT_GARAMOND, 24, spell_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, ((int)(0.125 * SCREEN_HEIGHT)), text_space);

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
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (spells.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), scrolly, Control::Type::SCROLL_DOWN));

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

        controls.push_back(Button(idx, "icons/back-button.png", idx - 1, idx, spells.size() > 0 ? (last - start) - 1 : idx, idx, ((int)((1.0 - Margin) * SCREEN_WIDTH) - buttonw), buttony, Control::Type::BACK));
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

            auto button = createHeaderButton(window, FONT_GARAMOND, 24, monster_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, ((int)(0.125 * SCREEN_HEIGHT)), text_space);

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
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (monsters.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), (texty + text_bounds - arrow_size - border_space), Control::Type::SCROLL_DOWN));

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

        controls.push_back(Button(idx, "icons/back-button.png", idx - 1, idx, monsters.size() > 0 ? (last - start) - 1 : idx, idx, ((int)((1.0 - Margin) * SCREEN_WIDTH) - buttonw), buttony, Control::Type::BACK));
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

            auto button = createHeaderButton(window, FONT_GARAMOND, 22, adventurer_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, ((int)(0.125 * SCREEN_HEIGHT)), text_space);

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
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (party.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), (texty + text_bounds - arrow_size - border_space), Control::Type::SCROLL_DOWN));

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

        controls.push_back(Button(idx, "icons/back-button.png", confirm_button ? idx - 1 : idx, idx, party.size() > 0 ? (last - start) - 1 : idx, idx, ((int)((1.0 - Margin) * SCREEN_WIDTH) - buttonw), buttony, Control::Type::BACK));
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

            SDL_SetWindowTitle(window, "Legendary Kingdoms: Armour Save");

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

            auto controls_save = createFixedTextButtons(choices_save, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_save[0].Type = Control::Type::CONFIRM;

            auto controls_reduce = createFixedTextButtons(choices_reduce, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_reduce[0].Type = Control::Type::CONFIRM;

            auto controls_end = createFixedTextButtons(choices_end, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
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

                putHeader(renderer, "Armour save results", font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty + infoh + boxh + box_space);
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

                putHeader(renderer, character.Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);

                defender_string = "Armour Save: +" + std::to_string(save_score);
                defender_string += "\nHealth: " + std::to_string(character.Health);

                putText(renderer, defender_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx, starty + infoh);

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
                        putHeader(renderer, message.c_str(), font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
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

int assignDamage(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team)
{
    auto result = -1;

    auto title = "Legendary Kingdoms: Assign Damage";

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

        if (last > party.Party.size())
        {
            last = party.Party.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = combatantList(window, renderer, party.Party, offset, last, limit, textx, texty + infoh + text_space, true, false);

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

                    if (index >= 0 && index < party.Party.size())
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
                        else if (party.Party[index].Health > 0)
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

                if (selection >= 0 && selection < party.Party.size())
                {
                    if (party.Party[selection].Health > 0)
                    {
                        putText(renderer, party.Party[selection].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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
                        if (selection >= 0 && selection < party.Party.size())
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
                        if (current + offset >= 0 && current + offset < party.Party.size())
                        {
                            if (selection == current + offset)
                            {
                                selection = -1;
                            }
                            else
                            {
                                if (party.Party[current + offset].Health > 0)
                                {
                                    if (team == Team::Type::NONE || party.Party[current + offset].Team == team)
                                    {
                                        selection = current + offset;
                                    }
                                    else
                                    {
                                        flash_message = true;

                                        if (!Engine::IS_CHARACTER(party.Party[current + offset].Team))
                                        {
                                            message = std::string(party.Party[current + offset].Name) + " is not part of the " + std::string(Team::Descriptions[team]) + " team!";
                                        }
                                        else
                                        {
                                            message = "You can only assign damage to " + std::string(Team::Descriptions[team]) + "!";
                                        }

                                        start_ticks = SDL_GetTicks();

                                        flash_color = intRD;
                                    }
                                }
                                else
                                {
                                    flash_message = true;

                                    message = std::string(party.Party[current + offset].Name) + std::string(" is DEAD!");

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

            SDL_SetWindowTitle(window, "Legendary Kingdoms: Attack");

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

            auto controls_attack = createFixedTextButtons(choices_attack, 2, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_attack[0].Type = Control::Type::CONFIRM;
            controls_attack[1].Type = Control::Type::BACK;

            auto controls_damage = createFixedTextButtons(choices_damage, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_damage[0].Type = Control::Type::CONFIRM;
            controls_damage[0].W = controls_attack[0].W;

            auto controls_end = createFixedTextButtons(choices_end, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
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

                putHeader(renderer, "Attack Results", font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty + infoh + boxh + box_space);
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

                                message = std::string(party[combatant].Name) + "'s " + std::string(spell.Name) + " DEALS " + std::to_string(damage) + " to the " + std::string(monsters[opponent].Name) + "!";

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

                putHeader(renderer, party[combatant].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);
                fillRect(renderer, boxwidth, boxh, startx, starty + infoh, intBE);
                std::string attacker_string = "Magic Fighting Score: " + std::to_string(fighting_score);
                putText(renderer, attacker_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx, starty + infoh);

                putHeader(renderer, monsters[opponent].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx + boxwidth + marginx, starty);
                fillRect(renderer, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh, intBE);
                std::string defender_string = "Defence: " + std::to_string(monsters[opponent].Defence) + "+\nHealth: " + std::to_string(monsters[opponent].Health);
                putText(renderer, defender_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh);

                std::string spell_string = "SPELL: " + std::string(spell.Name);
                putHeader(renderer, spell_string.c_str(), font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, boxwidth, infoh, startx, starty + 2 * infoh + 4 * boxh + 2 * box_space);

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
                        putHeader(renderer, message.c_str(), font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
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

int attackScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Monster::Base> &monsters, int combatant, int opponent, int direction, bool useEquipment)
{
    auto combat_damage = 0;

    if (Engine::COUNT(party.Party) > 0 && Engine::COUNT(monsters) > 0)
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

            SDL_SetWindowTitle(window, "Legendary Kingdoms: Attack");

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

            auto controls_attack = createFixedTextButtons(choices_attack, 4, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_attack[0].Type = Control::Type::CONFIRM;
            controls_attack[1].Type = Control::Type::PLUS;
            controls_attack[2].Type = Control::Type::MINUS;
            controls_attack[3].Type = Control::Type::BACK;

            auto controls_defend = createFixedTextButtons(choices_defend, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_defend[0].Type = Control::Type::CONFIRM;

            auto controls_damage = createFixedTextButtons(choices_damage, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_damage[0].Type = Control::Type::CONFIRM;

            auto controls_end = createFixedTextButtons(choices_end, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_end[0].Type = Control::Type::BACK;

            auto controls_assign = createFixedTextButtons(choices_assign, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
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

                putHeader(renderer, "Attack Results", font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty + infoh + boxh + box_space);

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

                                    message = std::string(party.Party[combatant].Name) + " DEALS " + std::to_string(damage) + " to the " + std::string(monsters[opponent].Name) + "!";

                                    flash_color = intLB;
                                }
                                else
                                {
                                    message = std::string(party.Party[combatant].Name) + "'s ATTACK was INEFFECTIVE!";

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
                                    message = "The " + std::string(monsters[opponent].Name) + "'s attack was INEFFECTIVE!";

                                    start_ticks = SDL_GetTicks();

                                    flash_message = true;

                                    flash_color = intRD;

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

                            Engine::GAIN_HEALTH(party.Party, -1);

                            Engine::GAIN_HEALTH(monsters[opponent], 4);
                        }
                        else if (monsters[opponent].Type == Monster::Type::SNAKEMAN_PRIEST)
                        {
                            if (Engine::VERIFY_EQUIPMENT(party.Party, {Equipment::Type::HYGLIPH_FLOWER}))
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
                    putHeader(renderer, party.Party[combatant].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);

                    auto score = 1;

                    if (useEquipment)
                    {
                        score = Engine::FIGHTING_SCORE(party.Party[combatant]);
                    }
                    else
                    {
                        score = Engine::SCORE(party.Party[combatant], Attribute::Type::FIGHTING);

                        if (Engine::HAS_STATUS(party.Party[combatant], Character::Status::ENRAGED))
                        {
                            score++;
                        }
                    }

                    attacker_string = "Fighting: " + std::to_string(score);
                    attacker_string += "\nHealth: " + std::to_string(party.Party[combatant].Health);

                    attack_score = score;
                }
                else
                {
                    putHeader(renderer, monsters[opponent].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);

                    attacker_string = "Attack: " + std::to_string(monsters[opponent].Attack) + " (" + std::to_string(monsters[opponent].Difficulty) + "+)";

                    if (monsters[opponent].Auto > 0)
                    {
                        attacker_string += " +" + std::to_string(monsters[opponent].Auto) + " Auto";
                    }

                    attacker_string += "\nHealth: " + std::to_string(monsters[opponent].Health);

                    attack_score = monsters[opponent].Attack;
                }

                putText(renderer, attacker_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx, starty + infoh);

                std::string defender_string = "";

                if (direction == 0)
                {
                    putHeader(renderer, monsters[opponent].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx + boxwidth + marginx, starty);
                    defender_string = "Defence: " + std::to_string(monsters[opponent].Defence) + "+";
                    defender_string += "\nHealth: " + std::to_string(monsters[opponent].Health);
                }
                else
                {
                    if (combatant == -1)
                    {
                        putHeader(renderer, "To be determined", font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx + boxwidth + marginx, starty);
                    }
                    else
                    {
                        putHeader(renderer, party.Party[combatant].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx + boxwidth + marginx, starty);
                        defender_string = "Health: " + std::to_string(party.Party[combatant].Health);
                    }
                }

                fillRect(renderer, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh, intBE);

                putText(renderer, defender_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh);

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
                        putHeader(renderer, message.c_str(), font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
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

                        combat_damage = -1;

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
                        if (Engine::COUNT(party.Party, team) > 0)
                        {
                            if (combat_damage > 0)
                            {
                                auto result = -1;

                                if (Engine::COUNT(party.Party, team) == 1)
                                {
                                    if (team != Team::Type::NONE)
                                    {
                                        result = Engine::FIRST(party, team);
                                    }
                                    else
                                    {
                                        result = Engine::FIRST(party);
                                    }
                                }
                                else
                                {
                                    result = assignDamage(window, renderer, party, team);
                                }

                                if (result >= 0 && result < party.Party.size())
                                {
                                    if (party.Party[result].Health > 0)
                                    {
                                        assigned = true;

                                        if (Engine::ARMOUR(party.Party[result]) > 0 && monsters[opponent].Type != Monster::Type::PAPER)
                                        {
                                            auto reduced_damage = armourSave(window, renderer, party.Party[result], combat_damage);

                                            done = true;

                                            selected = false;

                                            current = -1;

                                            break;
                                        }
                                        else
                                        {
                                            message = std::string(party.Party[result].Name) + " DEALT " + std::to_string(combat_damage) + " DAMAGE!";

                                            start_ticks = SDL_GetTicks();

                                            flash_message = true;

                                            flash_color = intRD;

                                            Engine::GAIN_HEALTH(party.Party[result], -combat_damage);

                                            combat_damage = result;
                                        }
                                    }
                                    else
                                    {
                                        message = std::string(party.Party[result].Name) + " is already DEAD!";

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

bool retreatArmy(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, int unit, Location::Type &location, int threshold, int rolls)
{
    auto retreats = false;

    if (party.Army.size() > 0)
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

            auto stage = Engine::Retreat::START;

            SDL_SetWindowTitle(window, "Legendary Kingdoms: Retreat Army");

            TTF_Init();

            auto font_mason = TTF_OpenFont(FONT_MASON, 32);

            auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 32);

            auto text_space = 8;

            auto font_size = 24;

            const char *choices_retreat[1] = {"Retreat"};
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

            auto controls_retreat = createFixedTextButtons(choices_retreat, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_retreat[0].Type = Control::Type::CONFIRM;

            auto controls_confirm = createFixedTextButtons(choices_confirm, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_confirm[0].Type = Control::Type::CONFIRM;

            auto controls_end = createFixedTextButtons(choices_end, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_end[0].Type = Control::Type::BACK;

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

            auto confirmed = false;

            while (!done)
            {
                fillWindow(renderer, intWH);

                putHeader(renderer, "Results", font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty + infoh + boxh + box_space);
                fillRect(renderer, fullwidth, boxh * 3, startx, starty + infoh + boxh + box_space + infoh, intBE);

                if (stage != Engine::Retreat::START)
                {
                    auto success = 0;

                    if (rolls > 0)
                    {
                        if (stage == Engine::Retreat::RETREAT)
                        {
                            if (results.size() == 0)
                            {
                                results = Engine::ROLL_DICE(rolls);
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

                                success += results[i];

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

                    if (stage == Engine::Retreat::CONFIRM)
                    {
                        if (!confirmed)
                        {
                            if (rolls > 0)
                            {
                                if (success > threshold)
                                {
                                    retreats = true;

                                    message = std::string(party.Army[unit].Name) + " retreats safely to " + std::string(Location::Description[location]) + ".";

                                    flash_color = intLB;

                                    flash_message = true;

                                    start_ticks = SDL_GetTicks();

                                    party.Army[unit].Morale = party.Army[unit].MaximumMorale;

                                    party.Army[unit].Garrison = location;
                                }
                                else
                                {
                                    retreats = false;

                                    message = std::string(party.Army[unit].Name) + " destroyed.";

                                    flash_color = intRD;

                                    flash_message = true;

                                    start_ticks = SDL_GetTicks();
                                }
                            }
                            else
                            {
                                retreats = true;

                                message = std::string(party.Army[unit].Name) + " retreats safely to " + std::string(Location::Description[location]) + ".";

                                flash_color = intLB;

                                flash_message = true;

                                start_ticks = SDL_GetTicks();

                                party.Army[unit].Morale = party.Army[unit].MaximumMorale;

                                party.Army[unit].Garrison = location;
                            }

                            confirmed = true;
                        }
                    }
                }

                putHeader(renderer, party.Army[unit].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);
                fillRect(renderer, boxwidth, boxh, startx, starty + infoh, intBE);
                std::string army_string = "Strength: " + std::to_string(party.Army[unit].Strength) + " Morale: " + std::to_string(party.Army[unit].Morale) + "\nPosition: " + Location::BattleFieldDescription[party.Army[unit].Position];
                putText(renderer, army_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx, starty + infoh);

                std::string attribute_string = "Retreat: Difficulty " + std::to_string(threshold);
                putHeader(renderer, attribute_string.c_str(), font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, boxwidth, infoh, startx, starty + 2 * infoh + 4 * boxh + 2 * box_space);

                if (stage == Engine::Retreat::START)
                {
                    controls = controls_retreat;
                }
                else if (stage == Engine::Retreat::RETREAT)
                {
                    controls = controls_confirm;
                }
                else if (stage == Engine::Retreat::CONFIRM)
                {
                    controls = controls_end;
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

                renderTextButtons(renderer, controls, FONT_MASON, current, clrWH, intDB, intLB, font_size, TTF_STYLE_NORMAL);

                done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (stage == Engine::Retreat::START && controls[current].Type == Control::Type::CONFIRM)
                    {
                        if (rolls > 0)
                        {
                            stage = Engine::Retreat::RETREAT;
                        }
                        else
                        {
                            stage = Engine::Retreat::CONFIRM;
                        }
                    }
                    else if (stage == Engine::Retreat::RETREAT && controls[current].Type == Control::Type::CONFIRM)
                    {
                        stage = Engine::Retreat::CONFIRM;
                    }
                    else if (stage == Engine::Retreat::CONFIRM && controls[current].Type == Control::Type::BACK)
                    {
                        stage = Engine::Retreat::END;

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

    return retreats;
}

int gainAttributeScore(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, Attribute::Type &attribute, int score, int rolls)
{
    auto increase = 0;

    if (character.Health > 0)
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

            auto stage = Engine::RaiseAttribute::START;

            SDL_SetWindowTitle(window, "Legendary Kingdoms: Increase Attribute");

            TTF_Init();

            auto font_mason = TTF_OpenFont(FONT_MASON, 32);

            auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 32);

            auto text_space = 8;

            auto font_size = 24;

            const char *choices_raise[2] = {"Raise Attribute", "Cancel"};
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

            auto controls_raise = createFixedTextButtons(choices_raise, 2, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_raise[0].Type = Control::Type::CONFIRM;
            controls_raise[1].Type = Control::Type::BACK;

            auto controls_confirm = createFixedTextButtons(choices_confirm, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_confirm[0].Type = Control::Type::CONFIRM;

            auto controls_end = createFixedTextButtons(choices_end, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_end[0].Type = Control::Type::BACK;

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

            auto confirmed = false;

            while (!done)
            {
                fillWindow(renderer, intWH);

                putHeader(renderer, "Results", font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty + infoh + boxh + box_space);
                fillRect(renderer, fullwidth, boxh * 3, startx, starty + infoh + boxh + box_space + infoh, intBE);

                if (stage != Engine::RaiseAttribute::START)
                {
                    auto success = 0;

                    if (rolls > 0)
                    {
                        if (stage == Engine::RaiseAttribute::RAISE)
                        {
                            if (results.size() == 0)
                            {
                                results = Engine::ROLL_DICE(rolls);
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

                                success += results[i];

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

                    if (stage == Engine::RaiseAttribute::CONFIRM)
                    {
                        if (!confirmed)
                        {
                            if (rolls > 0)
                            {
                                if (success > Engine::SCORE(character, attribute))
                                {
                                    Engine::GAIN_SCORE(character, attribute, score);

                                    message = std::string(character.Name) + "'s " + std::string(Attribute::Descriptions[attribute]) + " increased by " + std::to_string(score) + "!";

                                    flash_color = intLB;

                                    flash_message = true;

                                    start_ticks = SDL_GetTicks();

                                    increase = score;
                                }
                                else
                                {
                                    message = std::string(character.Name) + "'s " + std::string(Attribute::Descriptions[attribute]) + " score did not increase.";

                                    flash_color = intRD;

                                    flash_message = true;

                                    start_ticks = SDL_GetTicks();

                                    increase = 0;
                                }
                            }
                            else
                            {
                                Engine::GAIN_SCORE(character, attribute, score);

                                message = std::string(character.Name) + "'s " + std::string(Attribute::Descriptions[attribute]) + " increased by " + std::to_string(score) + "!";

                                flash_color = intLB;

                                flash_message = true;

                                start_ticks = SDL_GetTicks();

                                increase = score;
                            }

                            confirmed = true;
                        }
                    }
                }

                putHeader(renderer, character.Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);
                fillRect(renderer, boxwidth, boxh, startx, starty + infoh, intBE);
                std::string attacker_string = std::string(Attribute::Descriptions[attribute]) + " Score: " + std::to_string(Engine::SCORE(character, attribute));
                putText(renderer, attacker_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx, starty + infoh);

                std::string attribute_string = "Raise: " + std::string(Attribute::Descriptions[attribute]);
                putHeader(renderer, attribute_string.c_str(), font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, boxwidth, infoh, startx, starty + 2 * infoh + 4 * boxh + 2 * box_space);

                if (stage == Engine::RaiseAttribute::START)
                {
                    controls = controls_raise;
                }
                else if (stage == Engine::RaiseAttribute::RAISE)
                {
                    controls = controls_confirm;
                }
                else if (stage == Engine::RaiseAttribute::CONFIRM)
                {
                    controls = controls_end;
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

                renderTextButtons(renderer, controls, FONT_MASON, current, clrWH, intDB, intLB, font_size, TTF_STYLE_NORMAL);

                done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (stage == Engine::RaiseAttribute::START && controls[current].Type == Control::Type::BACK)
                    {
                        increase = -1;

                        done = true;

                        current = -1;

                        selected = false;

                        break;
                    }
                    else if (stage == Engine::RaiseAttribute::START && controls[current].Type == Control::Type::CONFIRM)
                    {
                        if (rolls > 0)
                        {
                            stage = Engine::RaiseAttribute::RAISE;
                        }
                        else
                        {
                            stage = Engine::RaiseAttribute::CONFIRM;
                        }
                    }
                    else if (stage == Engine::RaiseAttribute::RAISE && controls[current].Type == Control::Type::CONFIRM)
                    {
                        stage = Engine::RaiseAttribute::CONFIRM;
                    }
                    else if (stage == Engine::RaiseAttribute::CONFIRM && controls[current].Type == Control::Type::BACK)
                    {
                        stage = Engine::RaiseAttribute::END;

                        done = true;

                        current = -1;

                        selected = false;

                        increase = score;

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

    return increase;
}

std::vector<int> selectSpell(SDL_Window *window, SDL_Renderer *renderer, Character::Base &caster, std::vector<Spells::Base> &spells, int select_limit, Spells::Select mode)
{
    auto select_result = std::vector<int>();

    auto title = "Legendary Kingdoms: Select Spell";

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
                            thickRect(renderer, controls[i].W + 4, controls[i].H + 4, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
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
                    list_header += "add to ";

                    if (caster.Type != Character::Type::NONE)
                    {
                        list_header += std::string(caster.Name) + "'s ";
                    }

                    list_header += "spellbook";
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
                            else if (mode == Spells::Select::SPELLBOOK || mode == Spells::Select::UNLEARN)
                            {
                                message = "You must select ";

                                if (select_limit > 1)
                                {
                                    message += std::to_string(select_limit);
                                }
                                else
                                {
                                    message += "a";
                                }

                                message += " spell";

                                if (select_limit > 1)
                                {
                                    message += "s";
                                }

                                message += " to ";

                                if (mode == Spells::Select::SPELLBOOK)
                                {
                                    message += "add to ";
                                }
                                else
                                {
                                    message += "erase from ";
                                }

                                if (caster.Type != Character::Type::NONE)
                                {
                                    message += std::string(caster.Name) + "'s";
                                }
                                else
                                {
                                    message += "your";
                                }

                                message += " spellbook.";
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

int selectOpponent(SDL_Window *window, SDL_Renderer *renderer, std::vector<Monster::Base> &monsters, std::vector<int> previousTargets, int combatRound)
{
    auto result = -1;

    auto title = "Legendary Kingdoms: Select Opponent";

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
                            thickRect(renderer, controls[i].W + 4, controls[i].H + 4, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                        }
                        else if (monsters[offset + i].Health > 0)
                        {
                            if (combatRound >= monsters[offset + i].Round)
                            {
                                drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X - 4, controls[i].Y - 4, intBK);
                            }
                            else
                            {
                                thickRect(renderer, controls[i].W + 4, controls[i].H + 4, controls[i].X - 2, controls[i].Y - 2, intGR, 2);
                            }
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

                            if (last > monsters.size())
                            {
                                last = monsters.size();
                            }

                            controls.clear();

                            controls = monsterList(window, renderer, monsters, offset, last, limit, textx, texty + infoh + text_space, true, false);

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
                        if (monsters.size() - last > 0)
                        {
                            if (offset < monsters.size() - limit)
                            {
                                offset += scrollSpeed;
                            }

                            if (offset > monsters.size() - limit)
                            {
                                offset = monsters.size() - limit;
                            }

                            last = offset + limit;

                            if (last > monsters.size())
                            {
                                last = monsters.size();
                            }

                            controls.clear();

                            controls = monsterList(window, renderer, monsters, offset, last, limit, textx, texty + infoh + text_space, true, false);

                            SDL_Delay(50);

                            if (offset > 0)
                            {
                                if (controls[current].Type != Control::Type::SCROLL_DOWN)
                                {
                                    current++;
                                }
                            }
                        }

                        if (monsters.size() - last <= 0)
                        {
                            selected = false;

                            current = -1;
                        }
                    }
                    else if (controls[current].Type == Control::Type::BACK)
                    {
                        result = -1;

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
                                if (monsters[current + offset].Health > 0 && combatRound >= monsters[current + offset].Round)
                                {
                                    selection = current + offset;
                                }
                                else if (monsters[current + offset].Health > 0 && combatRound < monsters[current + offset].Round)
                                {
                                    flash_message = true;

                                    message = std::string(monsters[current + offset].Name) + std::string(" has not joined the battle yet!");

                                    start_ticks = SDL_GetTicks();

                                    flash_color = intRD;
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

bool skillTestScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<int> team, Attribute::Type Skill, int difficulty, int success, bool useEquipment)
{
    bool test_result = false;

    if (Engine::COUNT(party.Party) > 0 && Engine::COUNT(party.Party) >= team.size())
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

            SDL_SetWindowTitle(window, "Legendary Kingdoms: Skill Check");

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

            auto controls_skill = createFixedTextButtons(choices_skill, 3, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_skill[0].Type = Control::Type::CONFIRM;
            controls_skill[1].Type = Control::Type::PLUS;
            controls_skill[2].Type = Control::Type::MINUS;

            auto controls_confirm = createFixedTextButtons(choices_confirm, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_confirm[0].Type = Control::Type::CONFIRM;

            auto controls_end = createFixedTextButtons(choices_end, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
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
                skill_score += Engine::SCORE(party.Party[team[i]], Skill) + Engine::MAX(party.Party[team[i]], Equipment::Class::NORMAL, Skill);
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

                putHeader(renderer, "Skill Check Results", font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty + infoh + boxh + box_space);

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

                            party.RecentSuccesses = success_counter;

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

                std::string test_string = std::string(Attribute::Descriptions[Skill]) + ": " + std::to_string(difficulty) + "+, Success: " + (success > 0 ? std::to_string(success) : std::string("Special"));

                putHeader(renderer, test_string.c_str(), font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, boxwidth, infoh, startx, starty + 2 * infoh + 4 * boxh + 2 * box_space);

                putHeader(renderer, party.Party[team[0]].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);

                fillRect(renderer, boxwidth, boxh, startx, starty + infoh, intBE);

                auto score1 = 0;

                if (useEquipment)
                {
                    score1 = Engine::SCORE(party.Party[team[0]], Skill) + Engine::MAX(party.Party[team[0]], Equipment::Class::NORMAL, Skill);
                }
                else
                {
                    score1 = Engine::SCORE(party.Party[team[0]], Skill);
                }

                std::string adventurer1 = std::string(Attribute::Descriptions[Skill]) + ": " + std::to_string(score1);

                putText(renderer, adventurer1.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx, starty + infoh);

                if (team.size() > 1)
                {
                    putHeader(renderer, party.Party[team[1]].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx + boxwidth + marginx, starty);

                    fillRect(renderer, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh, intBE);

                    auto score2 = 0;

                    if (useEquipment)
                    {
                        score2 = Engine::SCORE(party.Party[team[1]], Skill) + Engine::MAX(party.Party[team[1]], Equipment::Class::NORMAL, Skill);
                    }
                    else
                    {
                        score2 = Engine::SCORE(party.Party[team[1]], Skill);
                    }

                    std::string adventurer2 = std::string(Attribute::Descriptions[Skill]) + ": " + std::to_string(score2);

                    putText(renderer, adventurer2.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh);
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
                        putHeader(renderer, message.c_str(), font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
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

        if (team.size() > 0)
        {
            party.LastSelection = team;
        }
    }

    return test_result;
}

int castSpell(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Monster::Base> &monsters, std::vector<int> hasAttacked, int combatRound, Control::Type mode)
{
    auto result = -1;

    auto title = "Legendary Kingdoms: Select Caster";

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

        if (last > party.Party.size())
        {
            last = party.Party.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = combatantList(window, renderer, party.Party, offset, last, limit, textx, texty + infoh + text_space, true, true);

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
                            thickRect(renderer, controls[i].W + 4, controls[i].H + 4, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                        }
                        else if (party.Party[offset + i].Health > 0)
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

                if (selection >= 0 && selection < party.Party.size())
                {
                    putText(renderer, party.Party[selection].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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
                        if (selection >= 0 && selection < party.Party.size())
                        {
                            if (party.Party[selection].SpellCaster)
                            {
                                if (hasAttacked.size() > 0 && Engine::FIND_LIST(hasAttacked, selection) >= 0)
                                {
                                    flash_message = true;

                                    message = std::string(party.Party[selection].Name) + " already attacked this turn!";

                                    start_ticks = SDL_GetTicks();

                                    flash_color = intRD;
                                }
                                else
                                {
                                    auto spell = selectSpell(window, renderer, party.Party[selection], party.Party[selection].SpellBook, 1, Spells::Select::CAST_SPELL);

                                    if (spell.size() > 0)
                                    {
                                        auto i = spell[0];

                                        if (mode == Control::Type::COMBAT)
                                        {
                                            if (party.Party[selection].SpellBook[i].Scope == Spells::Scope::COMBAT || party.Party[selection].SpellBook[i].Scope == Spells::Scope::ADVENTURE_COMBAT)
                                            {
                                                auto cast = false;

                                                if (party.Party[selection].SpellBook[i].Type == Spells::Type::ARMOUR_OF_HEAVEN)
                                                {
                                                    auto target = -1;

                                                    if (Engine::COUNT(party.Party, team) == 1)
                                                    {
                                                        if (team == Team::Type::NONE)
                                                        {
                                                            target = Engine::FIRST(party);
                                                        }
                                                        else
                                                        {
                                                            target = Engine::FIRST(party, team);
                                                        }
                                                    }
                                                    else
                                                    {
                                                        target = selectPartyMember(window, renderer, party, team, Equipment::NONE, Control::Type::SPELL_TARGET);
                                                    }

                                                    if (target >= 0 && target < party.Party.size())
                                                    {
                                                        if (Engine::HAS_STATUS(party.Party[target], Character::Status::ARMOUR3))
                                                        {
                                                            flash_message = true;

                                                            message = std::string(party.Party[target].Name) + " already has the " + std::string(party.Party[selection].SpellBook[i].Name) + "!";

                                                            start_ticks = SDL_GetTicks();

                                                            flash_color = intRD;
                                                        }
                                                        else
                                                        {
                                                            Engine::GAIN_STATUS(party.Party[target], Character::Status::ARMOUR3);

                                                            cast = true;
                                                        }
                                                    }
                                                }
                                                else if (party.Party[selection].SpellBook[i].Type == Spells::Type::ICE_BOLT)
                                                {
                                                    if (Engine::COUNT(monsters, combatRound) > 0)
                                                    {
                                                        auto target = -1;

                                                        if (Engine::COUNT(monsters, combatRound) == 1)
                                                        {
                                                            target = Engine::FIRST(monsters, combatRound);
                                                        }
                                                        else
                                                        {
                                                            target = selectOpponent(window, renderer, monsters, {}, combatRound);
                                                        }

                                                        if (target >= 0)
                                                        {
                                                            auto damage = magicAttackScreen(window, renderer, party.Party, monsters, party.Party[selection].SpellBook[i], selection, target, 8);

                                                            if (damage >= 0)
                                                            {
                                                                cast = true;
                                                            }
                                                        }
                                                    }
                                                    else
                                                    {
                                                        flash_message = true;

                                                        message = "There are no targets for " + std::string(party.Party[selection].SpellBook[i].Name) + "!";

                                                        start_ticks = SDL_GetTicks();

                                                        flash_color = intRD;
                                                    }
                                                }
                                                else if (party.Party[selection].SpellBook[i].Type == Spells::Type::UNFAILING_STRIKE)
                                                {
                                                    if (Engine::COUNT(monsters, combatRound) > 0)
                                                    {
                                                        auto target = -1;

                                                        if (Engine::COUNT(monsters, combatRound) == 1)
                                                        {
                                                            target = Engine::FIRST(monsters, combatRound);
                                                        }
                                                        else
                                                        {
                                                            target = selectOpponent(window, renderer, monsters, {}, combatRound);
                                                        }

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

                                                        message = "There are no targets for " + std::string(party.Party[selection].SpellBook[i].Name) + "!";

                                                        start_ticks = SDL_GetTicks();

                                                        flash_color = intRD;
                                                    }
                                                }
                                                else if (party.Party[selection].SpellBook[i].Type == Spells::Type::POSION_STREAM)
                                                {
                                                    if (Engine::COUNT(monsters, combatRound) > 0)
                                                    {
                                                        auto max_targets = std::min(2, Engine::COUNT(monsters, combatRound));

                                                        auto targets = std::vector<int>();

                                                        while (targets.size() < max_targets)
                                                        {
                                                            auto target = -1;

                                                            if (Engine::COUNT(monsters, combatRound) == 1)
                                                            {
                                                                target = Engine::FIRST(monsters, combatRound);
                                                            }
                                                            else
                                                            {
                                                                target = selectOpponent(window, renderer, monsters, targets, combatRound);
                                                            }

                                                            if (target >= 0)
                                                            {
                                                                auto damage = magicAttackScreen(window, renderer, party.Party, monsters, party.Party[selection].SpellBook[i], selection, target, 5);

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

                                                        message = "There are no targets for " + std::string(party.Party[selection].SpellBook[i].Name) + "!";

                                                        start_ticks = SDL_GetTicks();

                                                        flash_color = intRD;
                                                    }
                                                }

                                                if (cast)
                                                {
                                                    party.Party[selection].SpellBook[i].Charged = false;

                                                    result = selection;

                                                    done = true;
                                                }
                                            }
                                            else
                                            {
                                                flash_message = true;

                                                message = std::string(party.Party[selection].Name) + " cannot cast " + std::string(party.Party[selection].SpellBook[spell[0]].Name) + " during COMBAT!";

                                                start_ticks = SDL_GetTicks();

                                                flash_color = intRD;
                                            }
                                        }
                                        else if (mode == Control::Type::ADVENTURE)
                                        {
                                            if (party.Party[selection].SpellBook[spell[0]].Scope == Spells::Scope::ADVENTURE || party.Party[selection].SpellBook[spell[0]].Scope == Spells::Scope::ADVENTURE_COMBAT)
                                            {
                                                // TODO: Cast Spell
                                                result = selection;

                                                done = true;
                                            }
                                            else
                                            {
                                                flash_message = true;

                                                message = std::string(party.Party[selection].Name) + " cannot cast " + std::string(party.Party[selection].SpellBook[spell[0]].Name) + " at this time!";

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

                                message = std::string(party.Party[selection].Name) + " cannot cast spells!";

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
                        if (current + offset >= 0 && current + offset < party.Party.size())
                        {
                            if (selection == current + offset)
                            {
                                selection = -1;
                            }
                            else
                            {
                                if (party.Party[current + offset].Health > 0)
                                {
                                    if (team == Team::Type::NONE || party.Party[current + offset].Team == team)
                                    {
                                        selection = current + offset;
                                    }
                                    else
                                    {
                                        flash_message = true;

                                        if (!Engine::IS_CHARACTER(team))
                                        {
                                            message = std::string(party.Party[current + offset].Name) + " is not part of the " + std::string(Team::Descriptions[team]) + " team!";
                                        }
                                        else
                                        {
                                            message = "You can only choose " + std::string(Team::Descriptions[team]) + "!";
                                        }

                                        start_ticks = SDL_GetTicks();

                                        flash_color = intRD;
                                    }
                                }
                                else
                                {
                                    flash_message = true;

                                    message = std::string(party.Party[current + offset].Name) + std::string(" is DEAD!");

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

bool skillCheck(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, int team_size, Attribute::Type skill, int difficulty, int success, std::vector<int> &selection)
{
    bool result = false;

    auto title = "Legendary Kingdoms: Skill Check";

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

        if (last > party.Party.size())
        {
            last = party.Party.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = combatantList(window, renderer, party.Party, offset, last, limit, textx, texty + infoh + text_space, true, true);

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

                    if (index >= 0 && index < party.Party.size())
                    {
                        if (Engine::FIND_LIST(selection, index) >= 0)
                        {
                            thickRect(renderer, controls[index].W + 4, controls[index].H + 4, controls[index].X - 2, controls[index].Y - 2, intLB, 2);
                        }
                        else if (party.Party[index].Health > 0)
                        {
                            if (team != Team::Type::NONE)
                            {
                                if (party.Party[index].Team == team)
                                {
                                    drawRect(renderer, controls[index].W + 8, controls[index].H + 8, controls[index].X - 4, controls[index].Y - 4, intBK);
                                }
                                else
                                {
                                    thickRect(renderer, controls[index].W + 4, controls[index].H + 4, controls[index].X - 2, controls[index].Y - 2, intGR, 2);
                                }
                            }
                            else
                            {
                                drawRect(renderer, controls[index].W + 8, controls[index].H + 8, controls[index].X - 4, controls[index].Y - 4, intBK);
                            }
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
                        if (selection[i] >= 0 && selection[i] < party.Party.size())
                        {
                            if (party.Party[selection[i]].Health > 0)
                            {
                                if (adventurers.length() > 0)
                                {
                                    adventurers += "\n";
                                }

                                adventurers += party.Party[selection[i]].Name;
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
                        if ((selection.size() >= team_size && selection.size() <= party.Party.size()) || (selection.size() == Engine::COUNT(party.Party) && selection.size() < team_size))
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
                        if (current + offset >= 0 && current + offset < party.Party.size())
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
                                    if (party.Party[current + offset].Health > 0)
                                    {
                                        if (team == Team::Type::NONE)
                                        {
                                            selection.push_back(current + offset);
                                        }
                                        else if (party.Party[current + offset].Team == team)
                                        {
                                            selection.push_back(current + offset);
                                        }
                                        else
                                        {
                                            flash_message = true;

                                            message = std::string(party.Party[current + offset].Name) + " is not part of the " + std::string(Team::Descriptions[team]) + " team!";

                                            start_ticks = SDL_GetTicks();

                                            flash_color = intRD;
                                        }
                                    }
                                    else
                                    {
                                        flash_message = true;

                                        message = std::string(party.Party[current + offset].Name) + std::string(" is DEAD!");

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

Attribute::Type selectAttribute(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, int increase)
{
    auto result = Attribute::Type::NONE;

    auto title = "Legendary Kingdoms: Select Attribute";

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

        std::vector<Attribute::Type> attributes = {Attribute::Type::FIGHTING, Attribute::Type::STEALTH, Attribute::Type::LORE, Attribute::Type::SURVIVAL, Attribute::Type::CHARISMA};

        if (last > attributes.size())
        {
            last = attributes.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = attributeList(window, renderer, character, attributes, offset, last, limit, textx, texty + infoh + text_space);

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
                            thickRect(renderer, controls[i].W + 4, controls[i].H + 4, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X - 4, controls[i].Y - 4, intBK);
                        }
                    }
                }

                renderButtons(renderer, controls, current, intLB, space, 4);

                std::string select_string = "Which of " + std::string(character.Name) + "'s attributes will be raised by " + std::to_string(increase) + "?";

                putHeader(renderer, select_string.c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                putHeader(renderer, "Attribute", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (selection >= 0 && selection < attributes.size())
                {
                    putText(renderer, Attribute::Descriptions[attributes[selection]], font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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

                if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
                {
                    if (controls[current].Type == Control::Type::BACK)
                    {
                        done = true;

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                    {
                        if (offset > 0)
                        {
                            offset -= scrollSpeed;

                            if (offset < 0)
                            {
                                offset = 0;
                            }

                            last = offset + limit;

                            if (last > attributes.size())
                            {
                                last = attributes.size();
                            }

                            controls.clear();

                            controls = attributeList(window, renderer, character, attributes, offset, last, limit, textx, texty + infoh + text_space);

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
                        if (attributes.size() - last > 0)
                        {
                            if (offset < attributes.size() - limit)
                            {
                                offset += scrollSpeed;
                            }

                            if (offset > attributes.size() - limit)
                            {
                                offset = attributes.size() - limit;
                            }

                            last = offset + limit;

                            if (last > attributes.size())
                            {
                                last = attributes.size();
                            }

                            controls.clear();

                            controls = attributeList(window, renderer, character, attributes, offset, last, limit, textx, texty + infoh + text_space);

                            SDL_Delay(50);

                            if (offset > 0)
                            {
                                if (controls[current].Type != Control::Type::SCROLL_DOWN)
                                {
                                    current++;
                                }
                            }
                        }

                        if (attributes.size() - last <= 0)
                        {
                            selected = false;

                            current = -1;
                        }
                    }
                    else if (controls[current].Type == Control::Type::CONFIRM)
                    {
                        if (selection >= 0 && selection < attributes.size())
                        {
                            done = true;

                            result = attributes[selection];

                            current = -1;

                            selected = false;
                        }
                        else
                        {
                            flash_message = true;

                            message = "You must SELECT an ATTRIBUTE!";

                            start_ticks = SDL_GetTicks();

                            flash_color = intRD;
                        }
                    }
                    else if (controls[current].Type == Control::Type::ACTION)
                    {
                        if (current + offset >= 0 && current + offset < attributes.size())
                        {
                            if (selection == current + offset)
                            {
                                selection = -1;
                            }
                            else
                            {
                                selection = current + offset;
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

bool selectTeam(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, std::vector<Engine::TeamAssignment> teams)
{
    auto result = false;

    auto title = "Legendary Kingdoms: Select Team";

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

        auto teams_list = std::vector<Team::Type>();

        for (auto i = 0; i < teams.size(); i++)
        {
            teams_list.push_back(std::get<0>(teams[i]));
        }

        teams_list.push_back(Team::Type::NONE);

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = teamsList(window, renderer, teams_list, offset, last, limit, textx, texty + infoh + text_space);

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
                            thickRect(renderer, controls[i].W + 4, controls[i].H + 4, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X - 4, controls[i].Y - 4, intBK);
                        }
                    }
                }

                renderButtons(renderer, controls, current, intLB, space, space / 2);

                putHeader(renderer, "Select Team", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                putHeader(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (selection >= 0 && selection < teams_list.size())
                {
                    putText(renderer, Team::Descriptions[teams_list[selection]], font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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

                            if (last > teams_list.size())
                            {
                                last = teams_list.size();
                            }

                            controls.clear();

                            controls = teamsList(window, renderer, teams_list, offset, last, limit, textx, texty + infoh + text_space);

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
                        if (teams_list.size() - last > 0)
                        {
                            if (offset < teams_list.size() - limit)
                            {
                                offset += scrollSpeed;
                            }

                            if (offset > teams_list.size() - limit)
                            {
                                offset = teams_list.size() - limit;
                            }

                            last = offset + limit;

                            if (last > teams_list.size())
                            {
                                last = teams_list.size();
                            }

                            controls.clear();

                            controls = teamsList(window, renderer, teams_list, offset, last, limit, textx, texty + infoh + text_space);

                            SDL_Delay(50);

                            if (offset > 0)
                            {
                                if (controls[current].Type != Control::Type::SCROLL_DOWN)
                                {
                                    current++;
                                }
                            }
                        }

                        if (teams_list.size() - last <= 0)
                        {
                            selected = false;

                            current = -1;
                        }
                    }
                    else if (controls[current].Type == Control::Type::CONFIRM && !hold)
                    {
                        if (selection >= 0 && selection < teams_list.size())
                        {
                            character.Team = teams_list[selection];

                            selected = false;

                            current = -1;

                            done = true;

                            break;
                        }
                    }
                    else if (controls[current].Type == Control::Type::ACTION && !hold)
                    {
                        if (current + offset >= 0 && current + offset < teams_list.size())
                        {
                            if (selection == current + offset)
                            {
                                selection = -1;
                            }
                            else
                            {
                                selection = current + offset;
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

bool assignTeams(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Engine::TeamAssignment> teams, int min_teams)
{
    auto result = false;

    auto title = "Legendary Kingdoms: Assign Party Members to Teams";

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
        auto limit = (text_bounds - 2 * text_space - infoh) / (font_size * 2 + text_space * 4);
        auto last = offset + limit;

        if (last > party.Party.size())
        {
            last = party.Party.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = combatantList(window, renderer, party.Party, offset, last, limit, textx, texty + infoh + text_space, true, false);

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
                        drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X - 4, controls[i].Y - 4, intBK);
                    }
                }

                renderButtons(renderer, controls, current, intLB, space, space / 2);

                putHeader(renderer, "Assign Party Member to a Team", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                putHeader(renderer, "Teams", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (teams.size() > 0)
                {
                    std::string teams_string = "";

                    for (auto i = 0; i < teams.size(); i++)
                    {
                        if (i > 0)
                        {
                            teams_string += ", ";
                        }

                        teams_string += std::string(Team::Descriptions[std::get<0>(teams[i])]);
                    }

                    putText(renderer, teams_string.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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

                if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
                {
                    if (controls[current].Type == Control::Type::BACK)
                    {
                        done = true;

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                    {
                        if (offset > 0)
                        {
                            offset -= scrollSpeed;

                            if (offset < 0)
                            {
                                offset = 0;
                            }

                            last = offset + limit;

                            if (last > party.Party.size())
                            {
                                last = party.Party.size();
                            }

                            controls.clear();

                            controls = combatantList(window, renderer, party.Party, offset, last, limit, textx, texty + infoh + text_space, true, false);

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
                        if (party.Party.size() - last > 0)
                        {
                            if (offset < party.Party.size() - limit)
                            {
                                offset += scrollSpeed;
                            }

                            if (offset > party.Party.size() - limit)
                            {
                                offset = party.Party.size() - limit;
                            }

                            last = offset + limit;

                            if (last > party.Party.size())
                            {
                                last = party.Party.size();
                            }

                            controls.clear();

                            controls = combatantList(window, renderer, party.Party, offset, last, limit, textx, texty + infoh + text_space, true, false);

                            SDL_Delay(50);

                            if (offset > 0)
                            {
                                if (controls[current].Type != Control::Type::SCROLL_DOWN)
                                {
                                    current++;
                                }
                            }
                        }

                        if (party.Party.size() - last <= 0)
                        {
                            selected = false;

                            current = -1;
                        }
                    }
                    else if (controls[current].Type == Control::Type::CONFIRM && !hold)
                    {
                        auto assignment_error = false;

                        for (auto i = 0; i < teams.size(); i++)
                        {
                            auto count = Engine::COUNT(party.Party, std::get<0>(teams[i]));

                            if (!(count >= std::get<1>(teams[i]) && count <= std::get<2>(teams[i])))
                            {
                                assignment_error = true;
                            }
                        }

                        selection = -1;

                        current = -1;

                        selected = false;

                        if (assignment_error || Engine::COUNT_TEAMS(party) < min_teams)
                        {
                            flash_message = true;

                            flash_color = intRD;

                            start_ticks = SDL_GetTicks();

                            message = "Please complete team selection";

                            done = false;
                        }
                        else
                        {
                            done = true;

                            break;
                        }
                    }
                    else if (controls[current].Type == Control::Type::ACTION && !hold)
                    {
                        if (current + offset >= 0 && current + offset < party.Party.size())
                        {
                            selection = current + offset;

                            if (selection >= 0 && selection < party.Party.size())
                            {
                                if (party.Party[selection].Health > 0)
                                {
                                    selectTeam(window, renderer, party.Party[selection], teams);

                                    controls.clear();

                                    controls = combatantList(window, renderer, party.Party, offset, last, limit, textx, texty + infoh + text_space, true, false);
                                }
                                else
                                {
                                    flash_message = true;

                                    flash_color = intRD;

                                    start_ticks = SDL_GetTicks();

                                    message = std::string(party.Party[selection].Name) + " is DEAD!";
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

int selectPartyMember(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, Equipment::Base equipment, Control::Type mode)
{
    auto result = -1;

    auto title = "Legendary Kingdoms: Select Party Member";

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

        if (last > party.Party.size())
        {
            last = party.Party.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = combatantList(window, renderer, party.Party, offset, last, limit, textx, texty + infoh + text_space, true, true);

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
                            thickRect(renderer, controls[i].W + 4, controls[i].H + 4, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                        }
                        else if (party.Party[offset + i].Health > 0)
                        {
                            if (team != Team::Type::NONE)
                            {
                                if (party.Party[offset + i].Team == team)
                                {
                                    drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X - 4, controls[i].Y - 4, intBK);
                                }
                                else
                                {
                                    thickRect(renderer, controls[i].W + 4, controls[i].H + 4, controls[i].X - 2, controls[i].Y - 2, intGR, 2);
                                }
                            }
                            else
                            {
                                drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X - 4, controls[i].Y - 4, intBK);
                            }
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X + 4, controls[i].Y + 4, intRD);
                        }
                    }
                }

                renderButtons(renderer, controls, current, intLB, space, 4);

                if (mode == Control::Type::COMBAT)
                {
                    putHeader(renderer, "Choose the party member attacking this round", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (mode == Control::Type::SPELL_TARGET)
                {
                    putHeader(renderer, "Choose target for this spell", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (mode == Control::Type::ROLL_FOR_ATTRIBUTE_INCREASE || mode == Control::Type::RAISE_ATTRIBUTE_SCORE)
                {
                    putHeader(renderer, "Choose party member to receive ATTRIBUTE increase", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (mode == Control::Type::LEARN_SPELL)
                {
                    putHeader(renderer, "Choose the spellcaster who learns this spell", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (mode == Control::Type::EQUIPMENT)
                {
                    if (equipment.Type != Equipment::Type::NONE)
                    {
                        std::string equipment_string = "Give the " + std::string(equipment.Name) + " to";

                        putHeader(renderer, equipment_string.c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                    }
                    else
                    {
                        putHeader(renderer, "Give the item(s) to", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                    }
                }
                else if (mode == Control::Type::HEALTH)
                {
                    putHeader(renderer, "Choose party member to receive Maximum Health increase", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (mode == Control::Type::GAIN_HEALTH)
                {
                    putHeader(renderer, "Choose party member to recover health point(s)", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (mode == Control::Type::LOSE_HEALTH)
                {
                    putHeader(renderer, "Choose party member to lose health point(s)", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (mode == Control::Type::ENTER_SHOP)
                {
                    putHeader(renderer, "Choose party member to enter shop", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (mode == Control::Type::PARTY)
                {
                    putHeader(renderer, "Choose party member", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, "Choose Party Member", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }

                putHeader(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (selection >= 0 && selection < party.Party.size())
                {
                    if (party.Party[selection].Health > 0)
                    {
                        putText(renderer, party.Party[selection].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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

                if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
                {
                    if (controls[current].Type == Control::Type::BACK)
                    {
                        done = true;

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                    {
                        if (offset > 0)
                        {
                            offset -= scrollSpeed;

                            if (offset < 0)
                            {
                                offset = 0;
                            }

                            last = offset + limit;

                            if (last > party.Party.size())
                            {
                                last = party.Party.size();
                            }

                            controls.clear();

                            controls = combatantList(window, renderer, party.Party, offset, last, limit, textx, texty + infoh + text_space, true, true);

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
                        if (party.Party.size() - last > 0)
                        {
                            if (offset < party.Party.size() - limit)
                            {
                                offset += scrollSpeed;
                            }

                            if (offset > party.Party.size() - limit)
                            {
                                offset = party.Party.size() - limit;
                            }

                            last = offset + limit;

                            if (last > party.Party.size())
                            {
                                last = party.Party.size();
                            }

                            controls.clear();

                            controls = combatantList(window, renderer, party.Party, offset, last, limit, textx, texty + infoh + text_space, true, true);

                            SDL_Delay(50);

                            if (offset > 0)
                            {
                                if (controls[current].Type != Control::Type::SCROLL_DOWN)
                                {
                                    current++;
                                }
                            }
                        }

                        if (party.Party.size() - last <= 0)
                        {
                            selected = false;

                            current = -1;
                        }
                    }
                    else if (controls[current].Type == Control::Type::CONFIRM)
                    {
                        if (selection >= 0 && selection < party.Party.size())
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
                        if (current + offset >= 0 && current + offset < party.Party.size())
                        {
                            if (selection == current + offset)
                            {
                                selection = -1;
                            }
                            else
                            {
                                if (party.Party[current + offset].Health > 0)
                                {
                                    if (team == Team::Type::NONE || party.Party[current + offset].Team == team)
                                    {
                                        selection = current + offset;
                                    }
                                    else
                                    {
                                        flash_message = true;

                                        if (!Engine::IS_CHARACTER(team))
                                        {
                                            message = std::string(party.Party[current + offset].Name) + " is not part of the " + std::string(Team::Descriptions[team]) + " team!";
                                        }
                                        else
                                        {
                                            message = "You can only choose " + std::string(Team::Descriptions[team]) + "!";
                                        }

                                        start_ticks = SDL_GetTicks();

                                        flash_color = intRD;
                                    }
                                }
                                else
                                {
                                    flash_message = true;

                                    message = std::string(party.Party[current + offset].Name) + std::string(" is DEAD!");

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

    if (result >= 0 && result < party.Party.size())
    {
        party.LastSelected = result;
    }

    return result;
}

Engine::Combat combatScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Monster::Base> &monsters, bool canFlee, int fleeRound, bool useEquipment)
{
    auto combatResult = Engine::Combat::NONE;

    auto title = "Legendary Kindoms: Combat";

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
        const char *choices_next[4] = {"VIEW PARTY", "NEXT ROUND", "CAST SPELL", "FLEE"};

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

        while (Engine::COUNT(monsters) > 0 && Engine::COUNT(party.Party, team) > 0)
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
                auto last = offset + limit;

                if (last > monsters.size())
                {
                    last = monsters.size();
                }

                if (Engine::COUNT(monsters, combatRound) == 0)
                {
                    controls = createHTextButtons(choices_next, 4, main_buttonh, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
                }
                else if (hasAttacked.size() < Engine::TEAM_SIZE(party.Party, team))
                {
                    controls = createHTextButtons(choices_attack, 4, main_buttonh, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
                }
                else
                {
                    controls = createHTextButtons(choices_defend, 4, main_buttonh, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
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
                        if (combatRound < monsters[offset + i].Round)
                        {
                            thickRect(renderer, monster_list[i].W + 4, monster_list[i].H + 4, monster_list[i].X - 2, monster_list[i].Y - 2, intGR, 2);
                        }
                        else
                        {
                            drawRect(renderer, monster_list[i].W + 8, monster_list[i].H + 8, monster_list[i].X - 4, monster_list[i].Y - 4, intBK);
                        }
                    }
                    else
                    {
                        drawRect(renderer, monster_list[i].W + 8, monster_list[i].H + 8, monster_list[i].X - 4, monster_list[i].Y - 4, intRD);
                    }
                }

                putHeader(renderer, "Opponents", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                putHeader(renderer, "Party", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (Engine::COUNT(party.Party, team) > 0)
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
                        viewParty(window, renderer, party, true);

                        selected = false;

                        current = -1;
                    }
                    else if (controls[current].Type == Control::Type::ATTACK)
                    {
                        if (Engine::COUNT(party.Party, team) > 0 && hasAttacked.size() < Engine::TEAM_SIZE(party.Party, team) && Engine::COUNT(monsters, combatRound) > 0)
                        {
                            auto result = -1;

                            if (Engine::COUNT(party.Party, team, hasAttacked) == 1)
                            {
                                if (team != Team::Type::NONE)
                                {
                                    result = Engine::FIRST(party, team, hasAttacked);
                                }
                                else
                                {
                                    result = Engine::FIRST(party, hasAttacked);
                                }
                            }
                            else
                            {
                                result = selectPartyMember(window, renderer, party, team, Equipment::NONE, Control::Type::COMBAT);
                            }

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
                                        if (Engine::COUNT(monsters, combatRound) > 0)
                                        {
                                            auto attack = -1;

                                            if (Engine::COUNT(monsters, combatRound) == 1)
                                            {
                                                attack = Engine::FIRST(monsters, combatRound);
                                            }
                                            else
                                            {
                                                attack = selectOpponent(window, renderer, monsters, {}, combatRound);
                                            }

                                            if (attack >= 0)
                                            {
                                                auto damage = attackScreen(window, renderer, party, team, monsters, result, attack, 0, useEquipment);

                                                if (damage >= 0)
                                                {
                                                    hasAttacked.push_back(result);

                                                    if (canFlee)
                                                    {
                                                        canFlee = false;
                                                    }
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

                        if (hasAttacked.size() >= Engine::COUNT(party.Party, team) || Engine::COUNT(monsters, combatRound) == 0)
                        {
                            if (Engine::COUNT(monsters, combatRound) > 0)
                            {
                                for (auto i = 0; i < monsters.size(); i++)
                                {
                                    if (monsters[i].Health > 0 && Engine::COUNT(party.Party, team) > 0 && combatRound >= monsters[i].Round)
                                    {
                                        if (monsters[i].Type != Monster::Type::ZEALOT_HEALER)
                                        {
                                            attackScreen(window, renderer, party, team, monsters, -1, i, 1, useEquipment);
                                        }
                                        else if (Engine::COUNT(monsters) == 1)
                                        {
                                            attackScreen(window, renderer, party, team, monsters, -1, i, 1, useEquipment);
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
                        if (Engine::COUNT(party.Party, team) > 0 && hasAttacked.size() < Engine::TEAM_SIZE(party.Party, team))
                        {
                            if (Engine::SPELLCASTERS(party.Party) > 0)
                            {
                                auto combat_spells = 0;

                                for (auto i = 0; i < party.Party.size(); i++)
                                {
                                    if (party.Party[i].Health > 0 && party.Party[i].SpellCaster && (team == Team::Type::NONE || party.Party[i].Team == team))
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
                                    auto result = castSpell(window, renderer, party, team, monsters, hasAttacked, combatRound, Control::Type::COMBAT);

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

                if (Engine::COUNT(party.Party, team) == 0 || Engine::COUNT(monsters) == 0)
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
        combatResult = Engine::COUNT(party.Party, team) > 0 ? Engine::Combat::VICTORY : Engine::Combat::DEFEAT;
    }

    // Clear temporary status, e.g. magic effects
    if (Engine::VERIFY_CODES(party, {Codes::Type::LAST_IN_COMBAT}))
    {
        Engine::LOSE_CODES(party, {Codes::Type::LAST_IN_COMBAT});
    }

    for (auto i = 0; i < party.Party.size(); i++)
    {
        if (team == Team::Type::NONE || party.Party[i].Team == team)
        {
            Engine::REMOVE_STATUS(party.Party[i], Character::Status::ARMOUR3);
            Engine::REMOVE_STATUS(party.Party[i], Character::Status::ENRAGED);
        }
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

std::vector<Button> shopList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Engine::EquipmentPrice> &shop, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (shop.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto item = std::get<0>(shop[index]);

            auto buy = std::get<1>(shop[index]);

            auto sell = std::get<2>(shop[index]);

            std::string item_string = std::string(item.Name);

            if (item.TwoHanded)
            {
                item_string += "*";
            }

            if (item.Attribute != Attribute::Type::NONE || item.AdditionalSlots > 0)
            {
                item_string += " (";

                if (item.Attribute != Attribute::Type::NONE)
                {
                    item_string += "+" + std::to_string(item.Modifier) + " " + std::string(Attribute::Descriptions[item.Attribute]);
                }

                if (item.AdditionalSlots > 0)
                {
                    if (item.Attribute != Attribute::Type::NONE)
                    {
                        item_string += ", ";
                    }

                    item_string += std::to_string(item.AdditionalSlots + 1) + " slots";
                }

                item_string += +")";
            }

            item_string += "\nPrice: " + std::string(buy > 0 ? std::to_string(buy) + " silver coins" : "Not available") + ", Sell: " + std::string(sell > 0 ? std::to_string(sell) + " silver coins" : "--");

            auto button = createHeaderButton(window, FONT_GARAMOND, 24, item_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, button, i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = button->w;

            controls[i].H = button->h;
        }
    }

    auto idx = controls.size();

    if (shop.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (shop.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), (texty + text_bounds - arrow_size - border_space), Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/shop.png", idx, idx + 1, (shop.size() > 0 ? idx - 1 : idx), idx, startx, buttony, Control::Type::BUY));
    controls.push_back(Button(idx + 1, "icons/selling.png", idx, idx + 2, (shop.size() > 0 ? (last - start) : idx + 1), idx + 1, startx + gridsize, buttony, Control::Type::SELL));
    controls.push_back(Button(idx + 2, "icons/items.png", idx + 1, idx + 3, (shop.size() > 0 ? (last - start) : idx + 2), idx + 2, startx + 2 * gridsize, buttony, Control::Type::EQUIPMENT));
    controls.push_back(Button(idx + 3, "icons/back-button.png", idx + 2, idx + 3, (shop.size() > 0 ? (last - start) : idx + 3), idx + 3, ((int)((1.0 - Margin) * SCREEN_WIDTH) - buttonw), buttony, Control::Type::BACK));

    return controls;
}

bool shopScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Engine::EquipmentPrice> shop, Character::Base &character)
{
    auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

    auto infoh = (int)(0.07 * SCREEN_HEIGHT);
    auto boxh = (int)(0.150 * SCREEN_HEIGHT);
    auto box_space = 10;

    auto font_size = 28;
    auto text_space = 8;
    auto scrollSpeed = 1;
    auto limit = (text_bounds - 2 * text_space - infoh) / (font_size * 2 + text_space * 4);

    auto offset = 0;

    auto last = offset + limit;

    if (last > shop.size())
    {
        last = shop.size();
    }

    std::string message = "";

    auto flash_message = false;

    auto flash_color = intRD;

    Uint32 start_ticks = 0;

    Uint32 duration = 3000;

    auto done = false;

    auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

    auto offsety = (texty + infoh);

    auto controls = shopList(window, renderer, shop, offset, last, limit, textx, offsety);

    TTF_Init();

    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
    auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);
    auto font_mason = TTF_OpenFont(FONT_MASON, 24);

    TTF_SetFontKerning(font_dark11, 0);

    auto selected = false;
    auto current = -1;
    auto quit = false;
    auto scrollUp = false;
    auto scrollDown = false;
    auto hold = false;

    auto selection = std::vector<int>();

    while (!done)
    {
        if (!Engine::VERIFY_EQUIPMENT_LIMIT(character))
        {
            message = "You are carrying too many items! Drop or transfer excess items.";

            flash_message = true;

            flash_color = intRD;

            start_ticks = SDL_GetTicks();
        }

        last = offset + limit;

        if (last > shop.size())
        {
            last = shop.size();
        }

        SDL_SetWindowTitle(window, "Legendary Kingdoms: Items");

        fillWindow(renderer, intWH);

        if (splash)
        {
            fitImage(renderer, splash, startx, starty, splashw, text_bounds);
        }

        putHeader(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

        if (selection.size() > 0)
        {
            std::string selection_string = "";

            for (auto i = 0; i < selection.size(); i++)
            {
                if (i > 0)
                {
                    selection_string += ", ";
                }

                auto item = std::get<0>(shop[selection[i]]);

                selection_string += std::string(item.Name);
            }

            putText(renderer, selection_string.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
        }
        else
        {
            fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
        }

        if (selection.size() > 0)
        {
            if (current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::BUY)
                {
                    std::string buy_string = "Buy ";

                    if (selection.size() > 1)
                    {
                        buy_string += "these";
                    }
                    else
                    {
                        buy_string += "this";
                    }

                    putHeader(renderer, buy_string.c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::SELL)
                {
                    std::string sell_string = "Sell ";

                    if (selection.size() > 1)
                    {
                        sell_string += "these";
                    }
                    else
                    {
                        sell_string += "this";
                    }

                    putHeader(renderer, sell_string.c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::EQUIPMENT)
                {
                    std::string view_string = "View " + std::string(character.Name) + "'s items";

                    putHeader(renderer, view_string.c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, "Items for Sale", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
            }
            else
            {
                putHeader(renderer, "Items for Sale", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
        }
        else
        {
            if (current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::EQUIPMENT)
                {
                    std::string view_string = "View " + std::string(character.Name) + "'s items";

                    putHeader(renderer, view_string.c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, "Items for Sale", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
            }
            else
            {
                putHeader(renderer, "Items for Sale", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
        }

        fillRect(renderer, textwidth, text_bounds - infoh, textx, texty + infoh, intBE);

        if (last - offset > 0)
        {
            for (auto i = 0; i < last - offset; i++)
            {
                if (Engine::FIND_LIST(selection, offset + i) >= 0)
                {
                    thickRect(renderer, controls[i].W + 4, controls[i].H + 4, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                }
                else
                {
                    drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X - 4, controls[i].Y - 4, intBK);
                }
            }
        }

        renderButtons(renderer, controls, current, intLB, text_space, text_space / 2);

        if (flash_message)
        {
            if ((SDL_GetTicks() - start_ticks) < duration)
            {
                putHeader(renderer, message.c_str(), font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, boxh * 2, -1, -1);
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

                    if (last > shop.size())
                    {
                        last = shop.size();
                    }

                    controls.clear();

                    controls = shopList(window, renderer, shop, offset, last, limit, textx, offsety);

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
                if (shop.size() - last > 0)
                {
                    if (offset < shop.size() - limit)
                    {
                        offset += scrollSpeed;
                    }

                    if (offset > shop.size() - limit)
                    {
                        offset = shop.size() - limit;
                    }

                    last = offset + limit;

                    if (last > shop.size())
                    {
                        last = shop.size();
                    }

                    controls.clear();

                    controls = shopList(window, renderer, shop, offset, last, limit, textx, offsety);

                    SDL_Delay(50);

                    if (offset > 0)
                    {
                        if (controls[current].Type != Control::Type::SCROLL_DOWN)
                        {
                            current++;
                        }
                    }
                }

                if (shop.size() - last <= 0)
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
                        if (selection.size() < shop.size())
                        {
                            selection.push_back(offset + current);
                        }
                    }
                }

                current = -1;

                selected = false;
            }
            else if (controls[current].Type == Control::Type::BUY && !hold)
            {
                if (selection.size() > 0)
                {
                    auto total = 0;

                    auto items = std::vector<Equipment::Base>();

                    for (auto i = 0; i < selection.size(); i++)
                    {
                        if (selection[i] >= 0 && selection[i] < shop.size())
                        {
                            auto item = std::get<0>(shop[selection[i]]);

                            auto price = std::get<1>(shop[selection[i]]);

                            if (price > 0)
                            {
                                items.push_back(item);

                                total += price;
                            }
                        }
                    }

                    if (items.size() > 0)
                    {
                        if (party.Money >= total)
                        {
                            Engine::GAIN_MONEY(party, -total);

                            character.Equipment.insert(character.Equipment.end(), items.begin(), items.end());

                            while (!Engine::VERIFY_EQUIPMENT_LIMIT(character))
                            {
                                inventoryScreen(window, renderer, party, character, -1, false);
                            }
                        }
                        else
                        {
                            message = "You do not have enough silver coins!";

                            flash_message = true;

                            flash_color = intRD;

                            start_ticks = SDL_GetTicks();
                        }
                    }
                }
            }
            else if (controls[current].Type == Control::Type::SELL && !hold)
            {
                if (selection.size() > 0)
                {
                    std::string sold_string = "";
                    std::string unsold_string = "";

                    auto sold = 0;
                    auto unsold = 0;

                    for (auto i = 0; i < selection.size(); i++)
                    {
                        if (selection[i] >= 0 && selection[i] < shop.size())
                        {
                            auto item = std::get<0>(shop[selection[i]]);

                            auto price = std::get<2>(shop[selection[i]]);

                            if (price > 0)
                            {
                                if (!Engine::VERIFY_EQUIPMENT(character, {item.Type}))
                                {
                                    unsold++;

                                    if (unsold_string.length() > 0)
                                    {
                                        unsold_string += ", ";
                                    }

                                    unsold_string += item.Name;
                                }
                                else
                                {
                                    Engine::LOSE_EQUIPMENT(character, {item.Type});

                                    Engine::GAIN_MONEY(party, price);

                                    sold++;

                                    if (sold_string.length() > 0)
                                    {
                                        sold_string += ", ";
                                    }

                                    sold_string += item.Name;
                                }
                            }
                            else
                            {
                                unsold++;

                                if (unsold_string.length() > 0)
                                {
                                    unsold_string += ", ";
                                }

                                unsold_string += item.Name;
                            }
                        }
                    }

                    message = "";

                    if (sold > 0)
                    {
                        message += "Items sold: " + sold_string;
                    }

                    if (unsold > 0)
                    {
                        if (sold > 0)
                        {
                            message += "\n";
                        }

                        message += "Items not sold: " + unsold_string;
                    }

                    flash_message = true;

                    if (sold > unsold)
                    {
                        flash_color = intLB;
                    }
                    else
                    {
                        flash_color = intRD;
                    }

                    start_ticks = SDL_GetTicks();
                }
            }
            else if (controls[current].Type == Control::Type::EQUIPMENT && !hold)
            {
                inventoryScreen(window, renderer, party, character, -1, false);
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

    return false;
}

bool inventoryScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Character::Base &character, int equipment_limit, bool InCombat)
{
    auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

    auto infoh = (int)(0.07 * SCREEN_HEIGHT);
    auto boxh = (int)(0.150 * SCREEN_HEIGHT);
    auto box_space = 10;

    auto font_size = 28;
    auto text_space = 8;
    auto scrollSpeed = 1;
    auto limit = (text_bounds - 2 * text_space - infoh) / (font_size + 7 * text_space / 2);

    auto offset = 0;

    auto last = offset + limit;

    if (last > character.Equipment.size())
    {
        last = character.Equipment.size();
    }

    std::string message = "";

    auto flash_message = false;

    auto flash_color = intRD;

    Uint32 start_ticks = 0;

    Uint32 duration = 3000;

    auto done = false;

    auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

    auto scrolly = (texty + text_bounds - arrow_size - border_space);
    auto offsety = (texty + infoh);

    auto controls = equipmentList(window, renderer, character.Equipment, offset, last, limit, offsety, scrolly);

    TTF_Init();

    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
    auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);
    auto font_mason = TTF_OpenFont(FONT_MASON, 24);

    TTF_SetFontKerning(font_dark11, 0);

    auto selected = false;
    auto current = -1;
    auto quit = false;
    auto scrollUp = false;
    auto scrollDown = false;
    auto hold = false;

    auto selection = -1;

    while (!done)
    {
        if (!Engine::VERIFY_EQUIPMENT_LIMIT(character) || (equipment_limit > -1 && !Engine::VERIFY_EQUIPMENT_LIMIT(character, equipment_limit)))
        {
            if (equipment_limit > -1)
            {
                if (equipment_limit > 0)
                {
                    message = "You are carrying more than " + std::to_string(equipment_limit) + " item";

                    if (equipment_limit > 1)
                    {
                        message += "s";
                    }

                    message += ". Drop or transfer excess items.";
                }
                else
                {
                    message = "Drop all your items.";
                }
            }
            else
            {
                message = "You are carrying too many items! Drop or transfer excess items.";
            }

            flash_message = true;

            flash_color = intRD;

            start_ticks = SDL_GetTicks();
        }

        last = offset + limit;

        if (last > character.Equipment.size())
        {
            last = character.Equipment.size();
        }

        SDL_SetWindowTitle(window, "Legendary Kingdoms: Items");

        fillWindow(renderer, intWH);

        if (splash)
        {
            fitImage(renderer, splash, startx, starty, splashw, text_bounds);
        }

        putHeader(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

        if (selection >= 0 && selection < character.Equipment.size())
        {
            putText(renderer, character.Equipment[selection].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
        }
        else
        {
            fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
        }

        if (selection >= 0 && selection < character.Equipment.size())
        {
            auto item = character.Equipment[selection];

            if (current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::USE)
                {
                    putHeader(renderer, (std::string("Use the ") + std::string(item.Name)).c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::DROP)
                {
                    putHeader(renderer, (std::string("Drop the ") + std::string(item.Name)).c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::TRANSFER)
                {
                    putHeader(renderer, (std::string("Transfer the ") + std::string(item.Name) + " to another").c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::VAULT)
                {
                    putHeader(renderer, (std::string("Send the ") + std::string(item.Name) + " to the magic vault").c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, (std::string(character.Name) + "'s items").c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
            }
            else
            {
                putHeader(renderer, (std::string(character.Name) + "'s items").c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
        }
        else
        {
            if (current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::USE)
                {
                    putHeader(renderer, "Use item", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::DROP)
                {
                    putHeader(renderer, "Drop item", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::TRANSFER)
                {
                    putHeader(renderer, "Transfer item to another party member", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::VAULT)
                {
                    putHeader(renderer, "Access magic vault", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, (std::string(character.Name) + "'s items").c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
            }
            else
            {
                putHeader(renderer, (std::string(character.Name) + "'s items").c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
        }

        fillRect(renderer, textwidth, text_bounds - infoh, textx, texty + infoh, intBE);

        if (last - offset > 0)
        {
            for (auto i = 0; i < last - offset; i++)
            {
                if (selection == offset + i)
                {
                    drawRect(renderer, controls[i].W + 16, controls[i].H + 16, controls[i].X - 8, controls[i].Y - 8, intBK);
                }
            }
        }

        renderButtons(renderer, controls, current, intLB, text_space, text_space / 2);

        if (flash_message)
        {
            if ((SDL_GetTicks() - start_ticks) < duration)
            {
                putHeader(renderer, message.c_str(), font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, boxh * 2, -1, -1);
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

                    if (last > character.Equipment.size())
                    {
                        last = character.Equipment.size();
                    }

                    controls.clear();

                    controls = equipmentList(window, renderer, character.Equipment, offset, last, limit, offsety, scrolly);

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
                if (character.Equipment.size() - last > 0)
                {
                    if (offset < character.Equipment.size() - limit)
                    {
                        offset += scrollSpeed;
                    }

                    if (offset > character.Equipment.size() - limit)
                    {
                        offset = character.Equipment.size() - limit;
                    }

                    last = offset + limit;

                    if (last > character.Equipment.size())
                    {
                        last = character.Equipment.size();
                    }

                    controls.clear();

                    controls = equipmentList(window, renderer, character.Equipment, offset, last, limit, offsety, scrolly);

                    SDL_Delay(50);

                    if (offset > 0)
                    {
                        if (controls[current].Type != Control::Type::SCROLL_DOWN)
                        {
                            current++;
                        }
                    }
                }

                if (character.Equipment.size() - last <= 0)
                {
                    selected = false;

                    current = -1;
                }
            }
            else if (controls[current].Type == Control::Type::ACTION && !hold)
            {
                if ((current + offset >= 0) && (current + offset) < character.Equipment.size())
                {
                    if (selection == current + offset)
                    {
                        selection = -1;
                    }
                    else
                    {
                        selection = current + offset;
                    }
                }

                selected = false;
            }
            else if (controls[current].Type == Control::Type::USE && !hold)
            {
                if (selection >= 0 && selection < character.Equipment.size())
                {
                    auto used_up = false;

                    auto item = character.Equipment[selection];

                    if (item.Type == Equipment::Type::SCROLL_OF_RAGE)
                    {
                        if (InCombat)
                        {
                            used_up = true;

                            for (auto i = 0; i < party.Party.size(); i++)
                            {
                                Engine::GAIN_STATUS(party.Party[i], Character::Status::ENRAGED);
                            }

                            message = "Your party gains +1 Fighting score until end of combat!";

                            flash_color = intLB;

                            start_ticks = SDL_GetTicks();

                            flash_message = true;
                        }
                        else
                        {
                            message = "You cannot use the " + std::string(item.Name) + " when not in combat!";

                            flash_color = intRD;

                            start_ticks = SDL_GetTicks();

                            flash_message = true;
                        }
                    }

                    if (used_up)
                    {
                        if (character.Equipment.size() > 0)
                        {
                            character.Equipment.erase(character.Equipment.begin() + selection);

                            if (offset > 0)
                            {
                                offset--;
                            }

                            last = offset + limit;

                            if (last > character.Equipment.size())
                            {
                                last = character.Equipment.size();
                            }

                            controls.clear();

                            controls = equipmentList(window, renderer, character.Equipment, offset, last, limit, offsety, scrolly);
                        }

                        selection = -1;

                        current = -1;

                        selected = false;
                    }
                }
            }
            else if (controls[current].Type == Control::Type::DROP && !hold)
            {
                if (selection >= 0 && selection < character.Equipment.size())
                {
                    auto item = character.Equipment[selection];

                    character.Equipment.erase(character.Equipment.begin() + selection);

                    if (offset > 0)
                    {
                        offset--;
                    }

                    last = offset + limit;

                    if (last > character.Equipment.size())
                    {
                        last = character.Equipment.size();
                    }

                    controls.clear();

                    controls = equipmentList(window, renderer, character.Equipment, offset, last, limit, offsety, scrolly);

                    message = item.Name;

                    if (item.TwoHanded)
                    {
                        message += "*";
                    }

                    if (item.Attribute != Attribute::Type::NONE)
                    {
                        message += " (+" + std::to_string(item.Modifier) + " " + std::string(Attribute::Descriptions[item.Attribute]) + ")";
                    }

                    message += " dropped!";

                    flash_color = intRD;

                    start_ticks = SDL_GetTicks();

                    flash_message = true;

                    selected = false;

                    current = -1;

                    selection = -1;
                }
            }
            else if (controls[current].Type == Control::Type::TRANSFER && !hold)
            {
                if (selection >= 0 && selection < character.Equipment.size())
                {
                    if (Engine::COUNT(party.Party) > 1)
                    {
                        auto item = character.Equipment[selection];

                        auto target = selectPartyMember(window, renderer, party, Team::Type::NONE, item, Control::Type::EQUIPMENT);

                        if (target >= 0 && target < party.Party.size())
                        {
                            if (party.Party[target].Type != character.Type)
                            {
                                character.Equipment.erase(character.Equipment.begin() + selection);

                                party.Party[target].Equipment.push_back(item);

                                if (offset > 0)
                                {
                                    offset--;
                                }

                                last = offset + limit;

                                if (last > character.Equipment.size())
                                {
                                    last = character.Equipment.size();
                                }

                                controls.clear();

                                controls = equipmentList(window, renderer, character.Equipment, offset, last, limit, offsety, scrolly);

                                message = item.Name;

                                if (item.TwoHanded)
                                {
                                    message += "*";
                                }

                                if (item.Attribute != Attribute::Type::NONE)
                                {
                                    message += " (+" + std::to_string(item.Modifier) + " " + std::string(Attribute::Descriptions[item.Attribute]) + ")";
                                }

                                message += " transferred to " + std::string(party.Party[target].Name) + "!";

                                flash_color = intLB;

                                start_ticks = SDL_GetTicks();

                                flash_message = true;

                                selected = false;

                                current = -1;

                                selection = -1;
                            }
                        }
                        else
                        {
                            message = "You can only transfer to another party member!";

                            flash_color = intRD;

                            start_ticks = SDL_GetTicks();

                            flash_message = true;
                        }
                    }
                    else
                    {
                        message = "There is no one else in your party!";

                        flash_color = intRD;

                        start_ticks = SDL_GetTicks();

                        flash_message = true;
                    }
                }
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

    return false;
}

bool armyScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Army::Base> army)
{
    auto done = false;

    if (army.size() > 0)
    {
        auto font_size = 28;
        auto text_space = 8;
        auto scrollSpeed = 1;
        auto offset = 0;
        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto limit = (text_bounds - 2 * text_space - infoh) / (font_size * 2 + text_space * 4);
        auto last = offset + limit;

        if (last > army.size())
        {
            last = army.size();
        }

        std::string message = "";

        auto flash_message = false;

        auto flash_color = intRD;

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

        auto controls = armyList(window, renderer, army, offset, last, limit, textx, texty + infoh, false);

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

        auto selection = std::vector<int>();

        while (!done)
        {
            last = offset + limit;

            if (last > army.size())
            {
                last = army.size();
            }

            SDL_SetWindowTitle(window, "Legendary Kingdoms: Add Army Units");

            fillWindow(renderer, intWH);

            std::string army_string = "";

            if (selection.size() > 0)
            {
                for (auto i = 0; i < selection.size(); i++)
                {
                    if (i > 0)
                    {
                        army_string += ", ";
                    }

                    std::string description = army[selection[i]].Name;

                    army_string += description;
                }
            }

            putText(renderer, "SELECTED", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + infoh - 1));
            putText(renderer, selection.size() > 0 ? army_string.c_str() : "(None)", font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 3 * boxh, startx, starty + text_bounds - 3 * boxh);

            putHeader(renderer, "Select the units to add to your army", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

            fillRect(renderer, textwidth, text_bounds - infoh, textx, texty + infoh, intBE);

            if (last - offset > 0)
            {
                for (auto i = 0; i < last - offset; i++)
                {
                    if (Engine::FIND_LIST(selection, offset + i) >= 0)
                    {
                        thickRect(renderer, controls[i].W + 4, controls[i].H + 4, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                    }
                    else
                    {
                        drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X - 4, controls[i].Y - 4, intBK);
                    }
                }
            }

            if (flash_message)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putHeader(renderer, message.c_str(), font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, boxh * 2, -1, -1);
                }
                else
                {
                    flash_message = false;
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

                        if (last > army.size())
                        {
                            last = army.size();
                        }

                        controls = armyList(window, renderer, army, offset, last, limit, textx, texty + infoh, false);

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
                    if (army.size() - last > 0)
                    {
                        if (offset < army.size() - limit)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > army.size() - limit)
                        {
                            offset = army.size() - limit;
                        }

                        last = offset + limit;

                        if (last > army.size())
                        {
                            last = army.size();
                        }

                        controls = armyList(window, renderer, army, offset, last, limit, textx, texty + infoh, false);

                        SDL_Delay(50);

                        if (offset > 0)
                        {
                            if (controls[current].Type != Control::Type::SCROLL_DOWN)
                            {
                                current++;
                            }
                        }
                    }

                    if (army.size() - last <= 0)
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
                            if (selection.size() < army.size())
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
                    if (selection.size() > 0)
                    {
                        for (auto i = 0; i < selection.size(); i++)
                        {
                            party.Army.push_back(army[selection[i]]);
                        }
                    }

                    done = true;

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

bool spellScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Spells::Base> spells, bool back_button)
{
    auto done = false;

    if (spells.size() > 0)
    {
        auto font_size = 28;
        auto text_space = 8;
        auto scrollSpeed = 1;
        auto offset = 0;
        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto booksize = (int)(2 * (text_bounds) / 3);
        auto box_space = 10;
        auto limit = (int)((booksize - text_space) / ((boxh) + 3 * text_space));
        auto last = offset + limit;

        if (last > spells.size())
        {
            last = spells.size();
        }

        auto scrolly = startx + infoh + booksize - buttonh - text_space + 1;

        std::string message = "";

        auto flash_message = false;

        auto flash_color = intRD;

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

        auto controls = spellList(window, renderer, spells, offset, last, limit, textx, texty + infoh, scrolly, true, true);

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

        auto selection = std::vector<int>();

        while (!done)
        {
            last = offset + limit;

            if (last > spells.size())
            {
                last = spells.size();
            }

            SDL_SetWindowTitle(window, "Legendary Kingdoms: Learn Spells");

            fillWindow(renderer, intWH);

            std::string spell_string = "";

            if (selection.size() > 0)
            {
                for (auto i = 0; i < selection.size(); i++)
                {
                    if (i > 0)
                    {
                        spell_string += ", ";
                    }

                    std::string description = spells[selection[i]].Name;

                    spell_string += description;
                }
            }

            putText(renderer, "SELECTED", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + infoh - 1));
            putText(renderer, selection.size() > 0 ? spell_string.c_str() : "(None)", font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 3 * boxh, startx, starty + text_bounds - 3 * boxh);

            putHeader(renderer, "You can COPY the following spells", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

            fillRect(renderer, textwidth, booksize, textx, texty + infoh, intBE);

            if (last - offset > 0)
            {
                for (auto i = 0; i < last - offset; i++)
                {
                    if (Engine::FIND_LIST(selection, offset + i) >= 0)
                    {
                        thickRect(renderer, controls[i].W + 4, controls[i].H + 4, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                    }
                    else
                    {
                        drawRect(renderer, controls[i].W + 8, controls[i].H + 8, controls[i].X - 4, controls[i].Y - 4, intBK);
                    }
                }
            }

            if (current >= 0 && current < controls.size() && controls[current].Type == Control::Type::ACTION)
            {
                fillRect(renderer, textwidth, text_bounds / 3 - box_space - infoh, textx, texty + infoh + booksize + box_space, intLB);

                auto text = createText(spells[current + offset].Description, FONT_GARAMOND, font_size, clrWH, textwidth - 2 * text_space, TTF_STYLE_NORMAL);

                renderText(renderer, text, intLB, textx + text_space, texty + infoh + booksize + box_space + text_space, text_bounds / 3 - texty, 0);

                SDL_FreeSurface(text);

                text = NULL;
            }

            if (flash_message)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putHeader(renderer, message.c_str(), font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, boxh * 2, -1, -1);
                }
                else
                {
                    flash_message = false;
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

                        if (last > spells.size())
                        {
                            last = spells.size();
                        }

                        controls = spellList(window, renderer, spells, offset, last, limit, textx, texty + infoh, scrolly, true, true);

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

                        controls = spellList(window, renderer, spells, offset, last, limit, textx, texty + infoh, scrolly, true, true);

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
                            if (selection.size() < spells.size())
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
                    if (selection.size() > 0)
                    {
                        auto learn = std::vector<Spells::Base>();

                        for (auto i = 0; i < selection.size(); i++)
                        {
                            learn.push_back(spells[selection[i]]);
                        }

                        auto character = -1;

                        if (Engine::SPELLCASTERS(party.Party) == 1)
                        {
                            character = Engine::FIRST_CASTER(party);
                        }
                        else
                        {
                            character = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::LEARN_SPELL);
                        }

                        if (character >= 0 && character < party.Party.size())
                        {
                            if (party.Party[character].SpellCaster)
                            {
                                auto learned = false;

                                for (auto i = 0; i < learn.size(); i++)
                                {
                                    if (Engine::VERIFY_SPELL(party.Party[character], {learn[i].Type}))
                                    {
                                        learned = true;

                                        message = std::string(party.Party[character].Name) + " already knows the " + std::string(learn[i].Name) + " spell!";

                                        break;
                                    }
                                }

                                if (learned)
                                {
                                    flash_message = true;

                                    start_ticks = SDL_GetTicks();

                                    done = false;

                                    current = -1;

                                    selected = false;

                                    flash_color = intRD;
                                }
                                else
                                {
                                    for (auto i = 0; i < learn.size(); i++)
                                    {
                                        party.Party[character].SpellBook.push_back(learn[i]);
                                    }

                                    while (!Engine::VERIFY_SPELL_LIMIT(party.Party[character]))
                                    {
                                        auto spell_selection = selectSpell(window, renderer, party.Party[character], party.Party[character].SpellBook, party.Party[character].SpellBook.size() - party.Party[character].SpellBookLimit, Spells::Select::UNLEARN);

                                        if (spell_selection.size() > 0)
                                        {
                                            auto newSpellBook = std::vector<Spells::Base>();

                                            for (auto i = 0; i < party.Party[character].SpellBook.size(); i++)
                                            {
                                                if (Engine::FIND_LIST(spell_selection, spell_selection[i]) < 0)
                                                {
                                                    newSpellBook.push_back(party.Party[character].SpellBook[spell_selection[i]]);
                                                }
                                            }

                                            party.Party[character].SpellBook = newSpellBook;
                                        }
                                    }

                                    message = "Spell";

                                    if (selection.size() > 1)
                                    {
                                        message += "s";
                                    }

                                    message += " COPIED to " + std::string(party.Party[character].Name) += "'s spellbook!";

                                    flash_message = true;

                                    flash_color = intLB;

                                    start_ticks = SDL_GetTicks();

                                    done = false;

                                    current = -1;

                                    selected = false;

                                    selection.clear();
                                }
                            }
                            else
                            {
                                message = std::string(party.Party[character].Name) + " is not a SpellCaster!";

                                flash_message = true;

                                flash_color = intRD;

                                start_ticks = SDL_GetTicks();

                                done = false;

                                current = -1;

                                selected = false;
                            }
                        }
                    }
                    else
                    {
                        done = true;

                        break;
                    }
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

            SDL_SetWindowTitle(window, "Legendary Kingdoms: Take Items");

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
                    for (auto i = 0; i < selection.size(); i++)
                    {
                        auto character = -1;

                        while (character < 0 || character > party.Party.size())
                        {
                            if (Engine::COUNT(party.Party) == 1)
                            {
                                character = Engine::FIRST(party);
                            }
                            else
                            {
                                character = selectPartyMember(window, renderer, party, Team::Type::NONE, equipment[selection[i]], Control::Type::EQUIPMENT);
                            }

                            if (character >= 0 && character < party.Party.size())
                            {
                                Engine::GET_EQUIPMENT(party.Party[character], {equipment[selection[i]]});

                                while (!Engine::VERIFY_EQUIPMENT_LIMIT(party.Party[character]))
                                {
                                    inventoryScreen(window, renderer, party, party.Party[character], -1, false);
                                }
                            }
                        }
                    }

                    done = true;

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

std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Engine::ShipPrices> &ships, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (ships.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto ship = std::get<0>(ships[index]);

            auto buy = std::get<1>(ships[index]);

            auto sell = std::get<2>(ships[index]);

            std::string ship_string = "[" + std::string(ship.Name) + "] Fighting: " + std::to_string(ship.Fighting) + ", Health: " + std::to_string(ship.Health) + ", Cargo: " + std::to_string(ship.MaximumCargo) + " unit(s)";

            ship_string += "\nPrice: " + std::string(buy > 0 ? std::to_string(buy) + " silver coins" : "Not available") + ", Sell: " + std::string(sell > 0 ? std::to_string(sell) + " silver coins" : "--");

            auto button = createHeaderButton(window, FONT_GARAMOND, 24, ship_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, button, i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = button->w;

            controls[i].H = button->h;
        }
    }

    auto idx = controls.size();

    if (ships.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (ships.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), (texty + text_bounds - arrow_size - border_space), Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    idx = controls.size();

    auto text_y = (int)(SCREEN_HEIGHT * (1.0 - Margin)) - 48;

    controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "BUY/SELL SHIPS", clrWH, intDB, 220, 48, -1), idx, idx + 1, ships.size() > 0 ? idx - 1 : idx, idx, startx, text_y, Control::Type::BUY_SELL_SHIP));
    controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "REPAIR SHIP", clrWH, intDB, 220, 48, -1), idx, idx + 2, ships.size() > 0 ? idx - 1 : idx + 1, idx + 1, startx + (220 + button_space), text_y, Control::Type::REPAIR_SHIP));
    controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "BUY/SELL CARGO", clrWH, intDB, 220, 48, -1), idx + 1, idx + 3, ships.size() > 0 ? idx - 1 : idx + 2, idx + 2, startx + 2 * (220 + button_space), text_y, Control::Type::BUY_SELL_CARGO));
    controls.push_back(Button(idx + 3, createHeaderButton(window, FONT_DARK11, 22, "BACK", clrWH, intDB, 220, 48, -1), idx + 2, idx + 3, ships.size() > 0 ? idx - 1 : idx + 3, idx + 3, startx + 3 * (220 + button_space), text_y, Control::Type::BACK));

    return controls;
}

std::vector<Button> cargoList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Engine::CargoPrices> &cargo, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (cargo.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto cargo_unit = std::get<0>(cargo[index]);

            auto buy = std::get<1>(cargo[index]);

            auto sell = std::get<2>(cargo[index]);

            std::string cargo_string = Cargo::Description[cargo_unit];

            cargo_string += "\nPrice: " + std::string(buy > 0 ? std::to_string(buy) + " silver coins" : "Not available") + " Sell: " + std::string(sell > 0 ? std::to_string(sell) + " silver coins" : "--");

            auto button = createHeaderButton(window, FONT_GARAMOND, 24, cargo_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, button, i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = button->w;

            controls[i].H = button->h;
        }
    }

    auto idx = controls.size();

    if (cargo.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (cargo.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), (texty + text_bounds - arrow_size - border_space), Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    idx = controls.size();

    auto text_y = (int)(SCREEN_HEIGHT * (1.0 - Margin)) - 48;

    controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "BUY/SELL SHIPS", clrWH, intDB, 220, 48, -1), idx, idx + 1, cargo.size() > 0 ? idx - 1 : idx, idx, startx, text_y, Control::Type::BUY_SELL_SHIP));
    controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "REPAIR SHIP", clrWH, intDB, 220, 48, -1), idx, idx + 2, cargo.size() > 0 ? idx - 1 : idx + 1, idx + 1, startx + (220 + button_space), text_y, Control::Type::REPAIR_SHIP));
    controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "BUY/SELL CARGO", clrWH, intDB, 220, 48, -1), idx + 1, idx + 3, cargo.size() > 0 ? idx - 1 : idx + 2, idx + 2, startx + 2 * (220 + button_space), text_y, Control::Type::BUY_SELL_CARGO));
    controls.push_back(Button(idx + 3, createHeaderButton(window, FONT_DARK11, 22, "BACK", clrWH, intDB, 220, 48, -1), idx + 2, idx + 3, cargo.size() > 0 ? idx - 1 : idx + 3, idx + 3, startx + 3 * (220 + button_space), text_y, Control::Type::BACK));

    return controls;
}

std::vector<Button> harbourControls(SDL_Window *window, SDL_Renderer *renderer)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    auto idx = 0;

    auto text_y = (int)(SCREEN_HEIGHT * (1.0 - Margin)) - 48;

    controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "BUY/SELL SHIPS", clrWH, intDB, 220, 48, -1), idx, idx + 1, idx, idx, startx, text_y, Control::Type::BUY_SELL_SHIP));
    controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "REPAIR SHIP", clrWH, intDB, 220, 48, -1), idx, idx + 2, idx + 1, idx + 1, startx + (220 + button_space), text_y, Control::Type::REPAIR_SHIP));
    controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "BUY/SELL CARGO", clrWH, intDB, 220, 48, -1), idx + 1, idx + 3, idx + 2, idx + 2, startx + 2 * (220 + button_space), text_y, Control::Type::BUY_SELL_CARGO));
    controls.push_back(Button(idx + 3, createHeaderButton(window, FONT_DARK11, 22, "BACK", clrWH, intDB, 220, 48, -1), idx + 2, idx + 3, idx + 3, idx + 3, startx + 3 * (220 + button_space), text_y, Control::Type::BACK));

    return controls;
}

bool harbourScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *harbour)
{
    auto *title = "Legendary Kingdoms: Harbour";

    auto font_size = 28;

    TTF_Init();

    auto font_mason = TTF_OpenFont(FONT_MASON, 32);
    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
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

        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;
        auto text_space = 8;

        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space - infoh) / (font_size * 2 + text_space * 4);
        auto last = offset + limit;

        if (last > harbour->Ships.size())
        {
            last = harbour->Ships.size();
        }

        auto controls = std::vector<Button>();

        controls = shipList(window, renderer, harbour->Ships, offset, last, limit, textx, (texty + infoh));

        auto current_mode = Control::Type::BUY_SELL_SHIP;

        bool scrollUp = false;
        bool scrollDown = false;
        auto scrollSpeed = 1;
        bool hold = false;

        auto done = false;

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

            if (current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::BUY_SELL_SHIP)
                {
                    if (current_mode != Control::Type::BUY_SELL_SHIP)
                    {
                        offset = 0;

                        last = offset + limit;

                        if (last > harbour->Ships.size())
                        {
                            last = harbour->Ships.size();
                        }

                        controls = shipList(window, renderer, harbour->Ships, offset, last, limit, textx, texty + infoh);

                        if (harbour->Ships.size() - last > 0)
                        {
                            current = last - offset + 1;
                        }
                        else
                        {
                            current = last - offset;
                        }
                    }

                    current_mode = Control::Type::BUY_SELL_SHIP;
                }
                else if (controls[current].Type == Control::Type::REPAIR_SHIP)
                {
                    if (current_mode != Control::Type::REPAIR_SHIP)
                    {
                        offset = 0;

                        last = 0;

                        controls = harbourControls(window, renderer);

                        current = 1;
                    }

                    current_mode = Control::Type::REPAIR_SHIP;
                }
                else if (controls[current].Type == Control::Type::BUY_SELL_CARGO)
                {
                    if (current_mode != Control::Type::BUY_SELL_CARGO)
                    {
                        offset = 0;

                        last = offset + limit;

                        if (last > harbour->Cargo.size())
                        {
                            last = harbour->Cargo.size();
                        }

                        controls = cargoList(window, renderer, harbour->Cargo, offset, last, limit, textx, texty + infoh);

                        if (harbour->Cargo.size() - last > 0)
                        {
                            current = last - offset + 3;
                        }
                        else
                        {
                            current = last - offset + 2;
                        }
                    }

                    current_mode = Control::Type::BUY_SELL_CARGO;
                }
            }

            fillRect(renderer, textwidth, (text_bounds - infoh), textx, (texty + infoh), intBE);

            if (current_mode == Control::Type::BUY_SELL_SHIP)
            {
                putHeader(renderer, "Ship Prices", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                if (harbour->Cargo.size() == 0)
                {
                    putText(renderer, "\nYou cannot buy nor sell ships here.", font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, text_bounds - infoh, textx, texty + infoh);
                }
            }
            else if (current_mode == Control::Type::REPAIR_SHIP)
            {
                putHeader(renderer, "Repair Costs", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                if (harbour->ShipRepairPrice > 0)
                {
                    std::string repair_string = "You can repair any ship you have here. It costs " + std::to_string(harbour->ShipRepairPrice) + " silver coins to restore each Health point. You can restore your ship up to its starting Health value.";

                    putText(renderer, repair_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, text_bounds - infoh, textx, texty + infoh);
                }
                else
                {
                    putText(renderer, "\nShip repair services are not available here.", font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, text_bounds - infoh, textx, texty + infoh);
                }
            }
            else if (current_mode == Control::Type::BUY_SELL_CARGO)
            {
                putHeader(renderer, "Cargo Prices", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                if (harbour->Cargo.size() == 0)
                {
                    putText(renderer, "\nYou cannot buy nor sell cargo here.", font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, text_bounds - infoh, textx, texty + infoh);
                }
            }
            else
            {
                fillRect(renderer, textwidth, infoh, textx, texty, intBR);
            }

            for (auto i = offset; i < last; i++)
            {
                auto index = i - offset;

                if (current != index)
                {
                    if (index >= 0 && index < controls.size())
                    {
                        drawRect(renderer, controls[index].W + 16, controls[index].H + 16, controls[index].X - 8, controls[index].Y - 8, intBK);
                    }
                }
            }

            renderButtons(renderer, controls, current, intLB, 8, 4);

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

                        if (current_mode == Control::Type::BUY_SELL_SHIP)
                        {
                            if (last > harbour->Ships.size())
                            {
                                last = harbour->Ships.size();
                            }

                            controls = shipList(window, renderer, harbour->Ships, offset, last, limit, textx, texty + infoh);
                        }
                        else if (current_mode == Control::Type::BUY_SELL_CARGO)
                        {
                            if (last > harbour->Cargo.size())
                            {
                                last = harbour->Cargo.size();
                            }

                            controls = cargoList(window, renderer, harbour->Cargo, offset, last, limit, textx, texty + infoh);
                        }

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
                    if (current_mode == Control::Type::BUY_SELL_SHIP)
                    {
                        if (harbour->Ships.size() - last > 0)
                        {
                            if (offset < harbour->Ships.size() - limit)
                            {
                                offset += scrollSpeed;
                            }

                            if (offset > harbour->Ships.size() - limit)
                            {
                                offset = harbour->Ships.size() - limit;
                            }

                            last = offset + limit;

                            if (last > harbour->Ships.size())
                            {
                                last = harbour->Ships.size();
                            }

                            controls = shipList(window, renderer, harbour->Ships, offset, last, limit, textx, texty + infoh);

                            SDL_Delay(50);

                            if (offset > 0)
                            {
                                if (controls[current].Type != Control::Type::SCROLL_DOWN)
                                {
                                    current++;
                                }
                            }
                        }

                        if (harbour->Ships.size() - last <= 0)
                        {
                            selected = false;

                            current = -1;
                        }
                    }
                    else if (current_mode == Control::Type::BUY_SELL_CARGO)
                    {
                        if (harbour->Cargo.size() - last > 0)
                        {
                            if (offset < harbour->Cargo.size() - limit)
                            {
                                offset += scrollSpeed;
                            }

                            if (offset > harbour->Cargo.size() - limit)
                            {
                                offset = harbour->Cargo.size() - limit;
                            }

                            last = offset + limit;

                            if (last > harbour->Cargo.size())
                            {
                                last = harbour->Cargo.size();
                            }

                            controls = cargoList(window, renderer, harbour->Cargo, offset, last, limit, textx, texty + infoh);

                            SDL_Delay(50);

                            if (offset > 0)
                            {
                                if (controls[current].Type != Control::Type::SCROLL_DOWN)
                                {
                                    current++;
                                }
                            }
                        }

                        if (harbour->Cargo.size() - last <= 0)
                        {
                            selected = false;

                            current = -1;
                        }
                    }

                    SDL_Delay(50);

                    if (offset > 0)
                    {
                        if (controls[current].Type != Control::Type::SCROLL_DOWN)
                        {
                            current++;
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::BACK)
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

std::vector<Button> createChoices(SDL_Window *window, SDL_Renderer *renderer, std::vector<Choice::Base> choices, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (choices.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto button = createHeaderButton(window, FONT_GARAMOND, 24, choices[index].Text, clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, button, i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = button->w;

            controls[i].H = button->h;
        }
    }

    auto idx = controls.size();

    if (choices.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (choices.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size)), (texty + text_bounds - arrow_size - border_space), Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    idx = controls.size();
    controls.push_back(Button(idx, "icons/map.png", idx - 1, idx + 1, idx - 1, idx, startx, buttony, Control::Type::MAP));
    controls.push_back(Button(idx + 1, "icons/user.png", idx, idx + 2, idx - 1, idx + 1, startx + gridsize, buttony, Control::Type::PARTY));
    controls.push_back(Button(idx + 2, "icons/back-button.png", idx + 1, idx + 2, idx - 1, idx + 2, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

    return controls;
}

Story::Base *processChoices(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *story)
{
    Story::Base *next = &Story::notImplemented;

    auto error = false;
    std::string message = "";

    Uint32 start_ticks = 0;
    Uint32 duration = 3000;

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
        auto scrollSpeed = 1;

        auto font_size = 28;
        auto text_space = 8;
        auto textwidth = (int)((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto infoh = (int)(0.07 * SCREEN_HEIGHT);
        auto box_space = 10;
        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space) / (font_size * 2 + text_space * 4);
        auto last = offset + limit;

        if (last > choices.size())
        {
            last = choices.size();
        }

        auto controls = createChoices(window, renderer, choices, offset, last, limit, textx, texty);

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
                if (story->ID != -1)
                {
                    auto storyID = story->ID;

                    if (storyID < 0 && story->DisplayID >= 0)
                    {
                        storyID = story->DisplayID;
                    }

                    std::string title_string = "Legendary Kingdoms - " + std::string(Book::Title[story->BookID]) + ": ";

                    SDL_SetWindowTitle(window, (title_string + std::string(3 - std::to_string(std::abs(storyID)).length(), '0') + std::to_string(std::abs(storyID))).c_str());
                }
                else
                {
                    std::string title_string = "Legendary Kingdoms - " + std::string(Book::Title[story->BookID]) + ": Not Implemented Yet";

                    SDL_SetWindowTitle(window, title_string.c_str());
                }
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
                    auto storyID = story->ID;

                    if (storyID < 0 && story->DisplayID >= 0)
                    {
                        storyID = story->DisplayID;
                    }

                    title_string += std::string(3 - std::to_string(std::abs(storyID)).length(), '0') + std::to_string(std::abs(storyID));

                    putText(renderer, title_string.c_str(), font_mason2, text_space, clrBK, intWH, TTF_STYLE_NORMAL, splashw, infoh, startx, starty);
                }
                else
                {
                    title_string += "Not Implemented";

                    putText(renderer, title_string.c_str(), font_mason2, text_space, clrBK, intWH, TTF_STYLE_NORMAL, splashw, infoh, startx, starty);
                }
            }

            if ((!splash || (splash && splash_h < (text_bounds - 4 * boxh - 2 * infoh - box_space))) && Engine::COUNT_TEAMS(party) > 0)
            {
                putHeader(renderer, "Teams", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (4 * boxh + 2 * infoh + box_space));

                auto teams = Engine::GET_TEAMS(party);

                if (teams.size() > 0)
                {
                    std::string teams_string = "";

                    for (auto i = 0; i < teams.size(); i++)
                    {
                        if (i > 0)
                        {
                            teams_string += ", ";
                        }

                        teams_string += Team::Descriptions[teams[i]];
                    }

                    putText(renderer, teams_string.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 4 * boxh - infoh - box_space);
                }
                else
                {
                    fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 4 * boxh - infoh - box_space, intBE);
                }
            }
            else if (!splash || (splash && splash_h < (text_bounds - 3 * boxh - 2 * infoh - box_space)))
            {
                putHeader(renderer, "Money", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + 2 * infoh + box_space));

                putText(renderer, (std::to_string(party.Money) + " silver coins").c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - 3 * boxh - infoh - box_space);
            }

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

            fillRect(renderer, textwidth, text_bounds, textx, texty, BE_80);

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

            renderButtons(renderer, controls, current, intLB, text_space, text_space / 2);

            for (auto i = offset; i < last; i++)
            {
                auto index = i - offset;

                if (current != index)
                {
                    if (index >= 0 && index < controls.size())
                    {
                        drawRect(renderer, controls[index].W + 16, controls[index].H + 16, controls[index].X - 8, controls[index].Y - 8, intBK);
                    }
                }
            }

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putHeader(renderer, message.c_str(), font_garamond, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw * 2, boxh * 2, -1, -1);
                }
                else
                {
                    error = false;
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

                        if (last > choices.size())
                        {
                            last = choices.size();
                        }

                        controls = createChoices(window, renderer, choices, offset, last, limit, textx, texty);

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
                    if (choices.size() - last > 0)
                    {
                        if (offset < choices.size() - limit)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > choices.size() - limit)
                        {
                            offset = choices.size() - limit;
                        }

                        last = offset + limit;

                        if (last > choices.size())
                        {
                            last = choices.size();
                        }

                        controls = createChoices(window, renderer, choices, offset, last, limit, textx, texty);

                        SDL_Delay(50);

                        if (offset > 0)
                        {
                            if (controls[current].Type != Control::Type::SCROLL_DOWN)
                            {
                                current++;
                            }
                        }
                    }

                    if (choices.size() - last <= 0)
                    {
                        selected = false;

                        current = -1;
                    }
                }
                else if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    auto choice = current + offset;

                    if (choice >= 0 && choice < story->Choices.size())
                    {
                        if (story->Choices[choice].Type == Choice::Type::NORMAL)
                        {
                            next = findStory(story->Choices[choice].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::ATTRIBUTES)
                        {
                            auto selection = std::vector<int>();

                            auto success = skillCheck(window, renderer, party, story->Choices[current].Team, 1, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, selection);

                            if (selection.size() == 1)
                            {
                                story->SkillCheck(party, success, selection);

                                if (success)
                                {
                                    next = findStory(story->Choices[choice].Destination);
                                }
                                else
                                {
                                    next = findStory(story->Choices[choice].DestinationFailed);
                                }

                                done = true;

                                break;
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::CHARACTER_ATTRIBUTES)
                        {
                            auto selection = std::vector<int>();

                            auto success = false;

                            if (party.LastSelected >= 0 && party.LastSelected < party.Party.size())
                            {
                                if (party.Party[party.LastSelected].Health > 0)
                                {
                                    selection.push_back(party.LastSelected);

                                    success = skillTestScreen(window, renderer, party, selection, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, true);
                                }
                                else
                                {
                                    success = skillCheck(window, renderer, party, story->Choices[current].Team, 1, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, selection);
                                }
                            }
                            else
                            {
                                success = skillCheck(window, renderer, party, story->Choices[current].Team, 1, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, selection);
                            }

                            if (selection.size() == 1)
                            {
                                story->SkillCheck(party, success, selection);

                                if (success)
                                {
                                    next = findStory(story->Choices[choice].Destination);
                                }
                                else
                                {
                                    next = findStory(story->Choices[choice].DestinationFailed);
                                }

                                done = true;

                                break;
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::TEAM_ATTRIBUTES)
                        {
                            auto selection = std::vector<int>();

                            auto success = skillCheck(window, renderer, party, story->Choices[choice].Team, 2, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, selection);

                            if (selection.size() == 2)
                            {
                                story->SkillCheck(party, success, selection);

                                if (success)
                                {
                                    next = findStory(story->Choices[choice].Destination);
                                }
                                else
                                {
                                    next = findStory(story->Choices[choice].DestinationFailed);
                                }

                                done = true;

                                break;
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::EQUIPMENT)
                        {
                            auto equipment = std::vector<Equipment::Type>();

                            for (auto i = 0; i < story->Choices[choice].Equipment.size(); i++)
                            {
                                equipment.push_back(story->Choices[choice].Equipment[i].Type);
                            }

                            if (Engine::VERIFY_EQUIPMENT(party.Party, equipment))
                            {
                                next = findStory(story->Choices[choice].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                error = true;

                                if (story->Choices[choice].Equipment.size() > 1)
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
                        else if (story->Choices[choice].Type == Choice::Type::ANY_EQUIPMENT)
                        {
                            auto equipment = std::vector<Equipment::Type>();

                            for (auto i = 0; i < story->Choices[choice].Equipment.size(); i++)
                            {
                                equipment.push_back(story->Choices[choice].Equipment[i].Type);
                            }

                            if (Engine::VERIFY_ANY_EQUIPMENT(party.Party, equipment))
                            {
                                next = findStory(story->Choices[choice].Destination);

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
                        else if (story->Choices[choice].Type == Choice::Type::SHIP)
                        {
                            auto location = story->Choices[choice].Location;

                            if (location == Location::Type::NONE)
                            {
                                location = party.Location;
                            }

                            if (Engine::HAS_SHIP(party, location))
                            {
                                next = findStory(story->Choices[choice].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                error = true;

                                message = "You do not have a ship at this location!";

                                start_ticks = SDL_GetTicks();
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::ADD_MAX_HEALTH)
                        {
                            auto target = -1;

                            if (Engine::COUNT(party.Party) == 1)
                            {
                                target = Engine::FIRST(party);
                            }
                            else
                            {
                                target = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::HEALTH);
                            }

                            party.Party[target].MaximumHealth += story->Choices[choice].Value;

                            Engine::GAIN_HEALTH(party.Party[target], story->Choices[choice].Value);

                            next = findStory(story->Choices[choice].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::GAIN_HEALTH)
                        {
                            auto target = -1;

                            if (Engine::COUNT(party.Party) == 1)
                            {
                                target = Engine::FIRST(party);
                            }
                            else
                            {
                                target = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, (story->Choices[choice].Value > 0 ? Control::Type::GAIN_HEALTH : Control::Type::LOSE_HEALTH));
                            }

                            Engine::GAIN_HEALTH(party.Party[target], story->Choices[choice].Value);

                            next = findStory(story->Choices[choice].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::BRIBE_CODEWORD)
                        {
                            auto equipment = std::vector<Equipment::Type>();

                            for (auto i = 0; i < story->Choices[choice].Equipment.size(); i++)
                            {
                                equipment.push_back(story->Choices[choice].Equipment[i].Type);
                            }

                            auto count = Engine::COUNT_EQUIPMENT(party.Party, equipment);

                            if (count >= story->Choices[choice].Value)
                            {
                                Engine::LOSE_EQUIPMENT(party, equipment[0], story->Choices[choice].Value);

                                Engine::GET_CODES(party, story->Choices[choice].InvisibleCodes);

                                next = findStory(story->Choices[choice].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                if (story->Choices[choice].Value > 1)
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
                        else if (story->Choices[choice].Type == Choice::Type::ROLL_FOR_ATTRIBUTE_INCREASE)
                        {
                            auto target = -1;

                            if (Engine::COUNT(party.Party) == 1)
                            {
                                target = Engine::FIRST(party);
                            }
                            else
                            {
                                target = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::ROLL_FOR_ATTRIBUTE_INCREASE);
                            }

                            if (target >= 0 && target < party.Party.size())
                            {
                                auto increase = gainAttributeScore(window, renderer, party.Party[target], story->Choices[current].Attributes[0], story->Choices[current].Value, 2);

                                if (increase >= 0)
                                {
                                    next = findStory(story->Choices[choice].Destination);

                                    done = true;

                                    break;
                                }
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::RAISE_ATTRIBUTE_SCORE)
                        {
                            auto target = -1;

                            if (Engine::COUNT(party.Party) == 1)
                            {
                                target = Engine::FIRST(party);
                            }
                            else
                            {
                                target = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::RAISE_ATTRIBUTE_SCORE);
                            }

                            if (target >= 0 && target < party.Party.size())
                            {
                                auto increase = gainAttributeScore(window, renderer, party.Party[target], story->Choices[current].Attributes[0], story->Choices[current].Value, 0);

                                if (increase >= 0)
                                {
                                    next = findStory(story->Choices[choice].Destination);

                                    done = true;

                                    break;
                                }
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::PARTY_RAISE_ATTRIBUTE)
                        {
                            for (auto i = 0; i < party.Party.size(); i++)
                            {
                                if (party.Party[i].Health > 0)
                                {
                                    auto increase = -1;

                                    while (increase <= 0)
                                    {
                                        auto attribute = Attribute::Type::NONE;

                                        while (attribute == Attribute::Type::NONE)
                                        {
                                            attribute = selectAttribute(window, renderer, party.Party[i], story->Choices[choice].Value);
                                        }

                                        increase = gainAttributeScore(window, renderer, party.Party[i], attribute, story->Choices[choice].Value, 0);
                                    }
                                }
                            }

                            next = findStory(story->Choices[choice].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::PARTY_RAISE_HEALTH)
                        {
                            for (auto i = 0; i < party.Party.size(); i++)
                            {
                                if (party.Party[i].Health > 0)
                                {
                                    party.Party[i].MaximumHealth += story->Choices[choice].Value;

                                    if (party.Party[i].MaximumHealth < 0)
                                    {
                                        party.Party[i].MaximumHealth = 0;
                                    }

                                    Engine::GAIN_HEALTH(party.Party[i], story->Choices[choice].Value);
                                }
                            }

                            next = findStory(story->Choices[choice].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::RANDOM_EVENT)
                        {
                            auto results = Engine::ROLL_DICE(story->Choices[choice].Value);

                            auto sum = 0;

                            for (auto i = 0; i < results.size(); i++)
                            {
                                sum += results[i];
                            }

                            for (auto i = 0; i < story->Choices[choice].RandomDestinations.size(); i++)
                            {
                                if (sum <= std::get<0>(story->Choices[choice].RandomDestinations[i]))
                                {
                                    story->Bye = std::get<1>(story->Choices[choice].RandomDestinations[i]);

                                    next = findStory(std::get<2>(story->Choices[choice].RandomDestinations[i]));

                                    done = true;

                                    break;
                                }
                            }

                            if (done)
                            {
                                break;
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::CHOOSE_PARTY_MEMBER)
                        {
                            auto selected = -1;

                            while (selected < 0 || selected >= party.Party.size())
                            {
                                if (Engine::COUNT(party.Party, story->Choices[choice].Team) == 1)
                                {
                                    if (story->Choices[choice].Team == Team::Type::NONE)
                                    {
                                        selected = Engine::FIRST(party);
                                    }
                                    else
                                    {
                                        selected = Engine::FIRST(party, story->Choices[choice].Team);
                                    }
                                }
                                else
                                {
                                    selected = selectPartyMember(window, renderer, party, story->Choices[choice].Team, Equipment::NONE, Control::Type::PARTY);
                                }
                            }

                            if (story->Choices[choice].Team == Team::Type::NONE || party.Party[selected].Team == story->Choices[choice].Team)
                            {
                                next = findStory(story->Choices[choice].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                message = std::string(party.Party[selected].Name) + " is not part of the " + std::string(Team::Descriptions[story->Choices[choice].Team]) + " team!";

                                error = true;

                                start_ticks = SDL_GetTicks();
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::RETREAT)
                        {
                            if (party.Army.size() > 0)
                            {
                                std::vector<Location::BattleField> positions = {Location::BattleField::LEFT_FLANK_FRONT, Location::BattleField::LEFT_FLANK_SUPPORT, Location::BattleField::CENTRE_FRONT, Location::BattleField::CENTRE_SUPPORT, Location::BattleField::RIGHT_FLANK_FRONT, Location::BattleField::RIGHT_FLANK_SUPPORT};

                                for (auto i = 0; i < positions.size(); i++)
                                {
                                    auto unit = Engine::FIND_UNIT(party.Army, positions[i]);

                                    if (unit >= 0 && unit < party.Army.size())
                                    {
                                        auto location = story->Choices[choice].Location;

                                        auto retreat = retreatArmy(window, renderer, party, unit, location, story->Choices[choice].Value, 1);

                                        if (retreat)
                                        {
                                            party.Army[unit].Garrison = location;
                                            party.Army[unit].Position = Location::BattleField::NONE;
                                            party.Army[unit].Morale = party.Army[unit].MaximumMorale;
                                        }
                                        else
                                        {
                                            party.Army.erase(party.Army.begin() + unit);
                                        }
                                    }
                                }
                            }

                            next = findStory(story->Choices[choice].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::GET_EQUIPMENT)
                        {
                            done = takeScreen(window, renderer, party, story->Choices[choice].Equipment, story->Choices[choice].Equipment.size(), true);

                            if (done)
                            {
                                next = findStory(story->Choices[choice].Destination);

                                break;
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::GET_EQUIPMENT_CODE)
                        {
                            done = takeScreen(window, renderer, party, story->Choices[choice].Equipment, story->Choices[choice].Equipment.size(), true);

                            if (done)
                            {
                                if (Engine::VERIFY_EQUIPMENT(party.Party, {story->Choices[choice].Equipment[0].Type}))
                                {
                                    Engine::GET_CODES(party, story->Choices[choice].Codes);

                                    Engine::GET_CODES(party, story->Choices[choice].InvisibleCodes);

                                    if (story->Bye == NULL)
                                    {
                                        story->temp_string = "You gained the code";
                                    }
                                    else
                                    {
                                        story->temp_string = std::string(story->Bye) += "\n\nYou gained the code";
                                    }

                                    if (story->Choices[choice].Codes.size() > 1)
                                    {
                                        story->temp_string += "s: ";

                                        for (auto i = 0; i < story->Choices[choice].Codes.size(); i++)
                                        {
                                            if (i > 0)
                                            {
                                                story->temp_string += ", ";
                                            }

                                            story->temp_string += std::string(Codes::Prefix[story->Choices[choice].Codes[i].Type]) += std::to_string(story->Choices[choice].Codes[i].Code);
                                        }
                                    }
                                    else
                                    {
                                        story->temp_string += " " + std::string(Codes::Prefix[story->Choices[choice].Codes[0].Type]) += std::to_string(story->Choices[choice].Codes[0].Code);
                                    }

                                    story->temp_string += ".";

                                    story->Bye = story->temp_string.c_str();
                                }

                                next = findStory(story->Choices[choice].Destination);

                                break;
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::DELIVER)
                        {
                            auto result = Engine::FIND_SHIP(party, story->Choices[choice].Location, story->Choices[choice].Delivery.size());

                            if (result >= 0 && result < party.Fleet.size())
                            {
                                if (party.Money >= story->Choices[choice].Value)
                                {
                                    Engine::GAIN_MONEY(party, -story->Choices[choice].Value);

                                    party.Fleet[result].Cargo.insert(party.Fleet[result].Cargo.end(), story->Choices[choice].Delivery.begin(), story->Choices[choice].Delivery.end());

                                    next = findStory(story->Choices[choice].Destination);

                                    done = true;

                                    break;
                                }
                                else
                                {
                                    message = "You do not have enough silver coins!";

                                    error = true;

                                    start_ticks = SDL_GetTicks();
                                }
                            }
                            else
                            {
                                if (Engine::HAS_SHIP(party, story->Choices[choice].Location))
                                {
                                    message = "You do not have the required cargo space!";
                                }
                                else
                                {
                                    message = "You do not have a ship at " + std::string(Location::Description[story->Choices[choice].Location]) + "!";
                                }

                                error = true;

                                start_ticks = SDL_GetTicks();
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::CODES)
                        {
                            if (Engine::VERIFY_CODES(party, story->Choices[choice].Codes))
                            {
                                next = findStory(story->Choices[choice].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                if (story->Choices[choice].Codes.size() > 1)
                                {
                                    message = "You do not have the required codes!";
                                }
                                else
                                {
                                    message = "You do not have the required code!";
                                }

                                error = true;

                                start_ticks = SDL_GetTicks();
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::SET_STATUS)
                        {
                            auto selected = -1;

                            while (selected < 0 || selected >= party.Party.size())
                            {
                                if (Engine::COUNT(party.Party) == 1)
                                {
                                    selected = Engine::FIRST(party);
                                }
                                else
                                {
                                    selected = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::PARTY);
                                }
                            }

                            for (auto i = 0; i < party.Party.size(); i++)
                            {
                                if (i != selected)
                                {
                                    Engine::REMOVE_STATUS(party.Party[i], story->Choices[choice].Status[story->Choices[choice].Value]);
                                }
                            }

                            for (auto i = 0; i < story->Choices[choice].Status.size(); i++)
                            {
                                if (i == story->Choices[choice].Value)
                                {
                                    Engine::GAIN_STATUS(party.Party[selected], story->Choices[choice].Status[story->Choices[choice].Value]);
                                }
                                else
                                {
                                    Engine::REMOVE_STATUS(party.Party[selected], story->Choices[choice].Status[i]);
                                }
                            }

                            next = findStory(story->Choices[choice].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::HAS_STATUS)
                        {
                            auto result = true;

                            for (auto i = 0; i < story->Choices[choice].Status.size(); i++)
                            {
                                result &= Engine::HAS_STATUS(party.Party, story->Choices[choice].Status[i]);
                            }

                            if (result)
                            {
                                next = findStory(story->Choices[choice].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                error = true;

                                if (story->Choices[choice].Status.size() > 1)
                                {
                                    message = "Your party does not have all of the required status: ";
                                }
                                else
                                {
                                    message = "Your party does not have the required status: ";
                                }

                                for (auto i = 0; i < story->Choices[choice].Status.size(); i++)
                                {
                                    if (i > 0)
                                    {
                                        message += ", ";
                                    }

                                    message += std::string(Character::StatusDescriptions[story->Choices[choice].Status[i]]);
                                }

                                start_ticks = SDL_GetTicks();
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::ASSIGN_TEAMS)
                        {
                            assignTeams(window, renderer, party, story->Choices[choice].Teams, story->Choices[choice].Value);

                            next = findStory(story->Choices[choice].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::LAST_INDIVIDUAL_CHECK)
                        {
                            auto selection = party.LastSelection;

                            auto success = false;

                            if (selection.size() == 1)
                            {
                                auto target = selection[0];

                                if (target >= 0 && target < party.Party.size())
                                {
                                    if (party.Party[target].Health > 0)
                                    {
                                        success = skillTestScreen(window, renderer, party, selection, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, true);
                                    }
                                }
                                else
                                {
                                    selection.clear();

                                    success = skillCheck(window, renderer, party, story->Choices[choice].Team, 1, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, selection);
                                }
                            }
                            else
                            {
                                selection.clear();

                                success = skillCheck(window, renderer, party, story->Choices[choice].Team, 1, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, selection);
                            }

                            if (selection.size() == 1)
                            {
                                story->SkillCheck(party, success, selection);

                                if (success)
                                {
                                    next = findStory(story->Choices[choice].Destination);
                                }
                                else
                                {
                                    next = findStory(story->Choices[choice].DestinationFailed);
                                }

                                done = true;

                                break;
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::RAISE_LOWEST_ATTRIBUTE)
                        {
                            auto attribute_min = Engine::MIN(party, story->Choices[choice].Attributes[0]);
                            auto target = -1;

                            if (Engine::COUNT(party, story->Choices[choice].Attributes[0], attribute_min) > 1)
                            {
                                target = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::RAISE_ATTRIBUTE_SCORE);
                            }
                            else
                            {
                                target = Engine::FIRST(party, story->Choices[choice].Attributes[0], attribute_min);
                            }

                            if (target >= 0 && target < party.Party.size())
                            {
                                if (Engine::SCORE(party.Party[target], story->Choices[choice].Attributes[0]) == attribute_min)
                                {
                                    auto increase = gainAttributeScore(window, renderer, party.Party[target], story->Choices[current].Attributes[0], story->Choices[current].Value, 0);

                                    if (increase >= 0)
                                    {
                                        next = findStory(story->Choices[choice].Destination);

                                        done = true;

                                        break;
                                    }
                                }
                                else
                                {
                                    error = true;

                                    message = std::string(party.Party[target].Name) + " does not have the lowest " + std::string(Attribute::Descriptions[story->Choices[choice].Attributes[0]]) + " score!";

                                    start_ticks = SDL_GetTicks();
                                }
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::PAY_WITH)
                        {
                        }
                        else if (story->Choices[choice].Type == Choice::Type::SELL)
                        {
                        }
                        else if (story->Choices[choice].Type == Choice::Type::LOSE_EQUIPMENT)
                        {
                        }
                        else if (story->Choices[choice].Type == Choice::Type::BRIBE)
                        {
                        }
                        else if (story->Choices[choice].Type == Choice::Type::GET_CODES)
                        {
                        }
                        else if (story->Choices[choice].Type == Choice::Type::LOSE_CODES)
                        {
                        }
                        else if (story->Choices[choice].Type == Choice::Type::LOSE_ALL)
                        {
                        }
                        else if (story->Choices[choice].Type == Choice::Type::LOSE_MONEY)
                        {
                        }
                        else if (story->Choices[choice].Type == Choice::Type::GAIN_MONEY)
                        {
                        }
                        else if (story->Choices[choice].Type == Choice::Type::MONEY)
                        {
                        }
                        else if (story->Choices[choice].Type == Choice::Type::LIFE)
                        {
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::MAP && !hold)
                {
                    mapScreen(window, renderer);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::PARTY && !hold)
                {
                    viewParty(window, renderer, party, false);

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

void storyTransition(Party::Base &party, Story::Base *story, Story::Base *next)
{
    auto storyID = story->ID < 0 ? story->DisplayID : story->ID;
    auto nextID = next->ID < 0 ? next->DisplayID : next->ID;

    if ((story->BookID != next->BookID) || (story->BookID == next->BookID && storyID != nextID))
    {

    }
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

        Uint32 duration = 3000;

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

            if (jumpBook != Book::Type::NONE)
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
        else if (story->Controls == Story::Controls::BARTER)
        {
            controls = Story::BarterControls(compact);
        }
        else if (story->Controls == Story::Controls::HARBOUR)
        {
            controls = Story::HarbourControls(compact);
        }
        else if (story->Controls == Story::Controls::INN)
        {
            controls = Story::InnControls(compact);
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
                    if (story->ID != -1)
                    {
                        auto storyID = story->ID;

                        if (storyID < 0 && story->DisplayID >= 0)
                        {
                            storyID = story->DisplayID;
                        }

                        std::string title_string = "Legendary Kingdoms - " + std::string(Book::Title[story->BookID]) + ": ";

                        SDL_SetWindowTitle(window, (title_string + std::string(3 - std::to_string(std::abs(storyID)).length(), '0') + std::to_string(std::abs(storyID))).c_str());
                    }
                    else
                    {
                        std::string title_string = "Legendary Kingdoms - " + std::string(Book::Title[story->BookID]) + ": Not Implemented Yet";

                        SDL_SetWindowTitle(window, title_string.c_str());
                    }
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
                        auto storyID = story->ID;

                        if (storyID < 0 && story->DisplayID >= 0)
                        {
                            storyID = story->DisplayID;
                        }

                        title_string += std::string(3 - std::to_string(std::abs(storyID)).length(), '0') + std::to_string(std::abs(storyID));

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

                if ((!splash || (splash && splash_h < (text_bounds - 4 * boxh - 2 * infoh - box_space))) && Engine::COUNT_TEAMS(party) > 0)
                {
                    putHeader(renderer, "Teams", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (4 * boxh + 2 * infoh + box_space));

                    auto teams = Engine::GET_TEAMS(party);

                    if (teams.size() > 0)
                    {
                        std::string teams_string = "";

                        for (auto i = 0; i < teams.size(); i++)
                        {
                            if (i > 0)
                            {
                                teams_string += ", ";
                            }

                            teams_string += Team::Descriptions[teams[i]];
                        }

                        putText(renderer, teams_string.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 4 * boxh - infoh - box_space);
                    }
                    else
                    {
                        fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 4 * boxh - infoh - box_space, intBE);
                    }
                }
                else if (!splash || (splash && splash_h < (text_bounds - 3 * boxh - 2 * infoh - box_space)))
                {
                    putHeader(renderer, "Money", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + 2 * infoh + box_space));

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
                        viewParty(window, renderer, party, false);

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
                    else if (controls[current].Type == Control::Type::SHOP && !hold)
                    {
                        auto result = -1;

                        if (Engine::COUNT(party.Party) == 1)
                        {
                            result = Engine::FIRST(party);
                        }
                        else
                        {
                            result = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::ENTER_SHOP);
                        }

                        if (result >= 0 && result < party.Party.size())
                        {
                            shopScreen(window, renderer, party, story->Shop, party.Party[result]);
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
                            auto result = combatScreen(window, renderer, party, story->Team, story->Monsters, story->CanFlee, story->FleeRound, true);

                            story->AfterCombat(party, result);
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

                            if (story->Spells.size() > 0 && Engine::SPELLCASTERS(party.Party) > 0)
                            {
                                auto done = spellScreen(window, renderer, party, story->Spells, true);

                                if (!done)
                                {
                                    continue;
                                }
                                else
                                {
                                    story->Spells.clear();
                                }
                            }

                            if (story->Army.size() > 0)
                            {
                                auto done = armyScreen(window, renderer, party, story->Army);

                                if (!done)
                                {
                                    continue;
                                }
                                else
                                {
                                    story->Army.clear();
                                }
                            }

                            // Remove dead party members
                            auto liveCharacters = std::vector<Character::Base>();

                            auto deadInventory = std::vector<Equipment::Base>();

                            for (auto i = 0; i < party.Party.size(); i++)
                            {
                                if (party.Party[i].Health > 0)
                                {
                                    liveCharacters.push_back(party.Party[i]);
                                }
                                else
                                {
                                    if (party.Current != -1)
                                    {
                                        party.Current = -1;
                                    }

                                    if (party.LastSelected != -1)
                                    {
                                        party.LastSelected = -1;
                                    }

                                    if (party.LastSelection.size() > 0)
                                    {
                                        party.LastSelection.clear();
                                    }

                                    if (party.Party[i].Equipment.size() > 0)
                                    {
                                        deadInventory.insert(deadInventory.end(), party.Party[i].Equipment.begin(), party.Party[i].Equipment.end());
                                    }
                                }
                            }

                            party.Party = liveCharacters;

                            if (deadInventory.size() > 0)
                            {
                                takeScreen(window, renderer, party, deadInventory, deadInventory.size(), false);
                            }

                            while (!Engine::VERIFY_EQUIPMENT_LIMIT(party))
                            {
                                for (auto i = 0; i < party.Party.size(); i++)
                                {
                                    while (!Engine::VERIFY_EQUIPMENT_LIMIT(party.Party[i]))
                                    {
                                        inventoryScreen(window, renderer, party, party.Party[i], -1, false);
                                    }
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
                                    auto bye = createText(story->Bye, FONT_GARAMOND, font_size + 4, clrDB, ((int)(SCREEN_WIDTH * (1.0 - 2.0 * Margin)) - 2 * text_space), TTF_STYLE_NORMAL);
                                    auto forward = createImage("icons/next.png");

                                    if (bye && forward)
                                    {
                                        fillWindow(renderer, intWH);

                                        stretchImage(renderer, background, 0, 0, SCREEN_WIDTH, buttony - button_space);

                                        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

                                        fillRect(renderer, ((int)((1.0 - 2.0 * Margin) * SCREEN_WIDTH)), bye->h + 2 * text_space, startx, ((buttony - button_space) - (bye->h + 2 * text_space)) / 2, BE_80);

                                        renderText(renderer, bye, 0, (SCREEN_WIDTH - bye->w) / 2, ((buttony - button_space) - bye->h) / 2, (buttony - button_space), 0);

                                        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

                                        renderImage(renderer, forward, ((int)(SCREEN_WIDTH * (1.0 - Margin) - buttonw - button_space)), buttony);

                                        SDL_RenderPresent(renderer);

                                        Input::WaitForNext(renderer);

                                        SDL_FreeSurface(bye);

                                        bye = NULL;

                                        SDL_FreeSurface(forward);

                                        forward = NULL;
                                    }
                                }

                                // story transitions
                                storyTransition(party, story, next);

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

    auto text = createText(introduction, FONT_GARAMOND, 28, clrDB, ((int)SCREEN_WIDTH * (1.0 - 3.0 * Margin) - splashw));

    auto title = "Legendary Kingdoms";

    Book1::InitializeStories();

    // Render window
    if (window && renderer && splash && text)
    {
        SDL_SetWindowTitle(window, title);

        const char *choices[4] = {"NEW GAME", "LOAD GAME", "ABOUT", "EXIT"};

        auto current = -1;

        auto selected = false;

        auto main_buttonh = 48;

        auto controls = createHTextButtons(choices, 4, main_buttonh, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));

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

    auto title = "Legendary Kingdoms: Debug";

    auto splash = createImage("images/legendary-kingdoms-logo.png");

    // Render window
    if (window && renderer && text && splash)
    {
        SDL_SetWindowTitle(window, title);

        auto current = -1;

        auto selected = false;

        auto main_buttonh = 48;

        const char *choices[5] = {"1", "2", "3", "4", "Exit"};

        auto controls = createHTextButtons(choices, 5, main_buttonh, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));

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

                    combat = combatScreen(window, renderer, Party, Team::Type::NONE, monsters, true, -1, false);

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

                    skillCheck(window, renderer, Party, Team::Type::NONE, 2, Attribute::Type::STEALTH, 4, 4, selection);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::SKILL)
                {
                    selectParty(window, renderer, Book::Type::BOOK1, Party);

                    auto selection = std::vector<int>();

                    skillCheck(window, renderer, Party, Team::Type::NONE, 1, Attribute::Type::LORE, 4, 3, selection);

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

    auto title = "Legendary Kingdoms";

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
        if (storyID < 2)
        {
            introScreen(window, renderer);
        }

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
