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

// Forward declarations (internal functions)
int fitImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y, int w, int h);
int fadeImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y, int w, int h, Uint8 alpha);

SDL_Surface *createHeaderButton(SDL_Window *window, const char *font, int font_size, const char *text, SDL_Color color, Uint32 bg, int w, int h, int x);
SDL_Surface *createImage(const char *image);
SDL_Surface *createText(const char *text, const char *ttf, int font_size, SDL_Color textColor, int wrap, int style);

std::vector<TextButton> createFixedTextButtons(const char **choices, int num, int text_buttonw, int text_buttonh, int textbutton_space, int text_x, int text_y);
std::vector<TextButton> createHTextButtons(const char **choices, int num, int text_buttonh, int text_x, int text_y);
std::vector<TextButton> createHTextButtons(const char **choices, int num, int text_buttonh, int text_x, int text_y, bool has_scrolls);

void clipValue(int &val, int min, int max);
void createWindow(Uint32 flags, SDL_Window **window, SDL_Renderer **renderer, const char *title, const char *icon);
void drawRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color);
void fillRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color);
void fillWindow(SDL_Renderer *renderer, Uint32 color);
void putHeader(SDL_Renderer *renderer, const char *text, TTF_Font *font, int space, SDL_Color fg, Uint32 bg, int style, int w, int h, int x, int y);
void putText(SDL_Renderer *renderer, const char *text, TTF_Font *font, int space, SDL_Color fg, Uint32 bg, int style, int w, int h, int x, int y);
void renderButtons(SDL_Renderer *renderer, std::vector<Button> controls, int current, int fg, int space, int pts);
void renderButtons(SDL_Renderer *renderer, std::vector<Button> controls, int current, int fg, int space, int pts, bool hide_scroll);
void renderImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y);
void renderText(SDL_Renderer *renderer, SDL_Surface *text, Uint32 bg, int x, int y, int bounds, int offset);
void renderTextButtons(SDL_Renderer *renderer, std::vector<TextButton> controls, const char *ttf, int selected, SDL_Color fg, Uint32 bg, Uint32 bgSelected, int fontsize, int style);
void renderTextButtons(SDL_Renderer *renderer, std::vector<TextButton> controls, const char *ttf, int selected, SDL_Color fg, Uint32 bg, Uint32 bgSelected, int fontsize, int offsetx, int scrolly, bool hide_scroll, int style);
void stretchImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y, int w, int h);
void thickRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color, int pts);

// game screens
bool armyScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Army::Base> army);
bool assignTeams(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Engine::TeamAssignment> teams, int min_teams);
bool inventoryScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Character::Base &character, int equipment_limit, bool InCombat);
bool harbourScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *harbour);
bool innScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, int RestPrice, bool CanRecharge);
bool introScreen(SDL_Window *window, SDL_Renderer *renderer);
bool loseItems(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Character::Type character, Team::Type team, std::vector<Equipment::Base> equipment, std::vector<Equipment::Class> exceptions, int LoseLimit, bool back_button);
bool mainScreen(SDL_Window *window, SDL_Renderer *renderer, Book::Type bookID, int storyID);
bool moraleCheck(SDL_Window *window, SDL_Renderer *renderer, Army::Base &unit, int combatRound);
bool partyDetails(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party);
bool processStory(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Book::Type book, Story::Base *story);
bool rechargeSpells(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Character::Base &character);
bool retreatArmy(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, int unit, Location::Type &location, int threshold, int rolls);
bool selectParty(SDL_Window *window, SDL_Renderer *renderer, Book::Type bookID, Party::Base &party);
bool selectTeam(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, std::vector<Engine::TeamAssignment> teams);
bool shopScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Engine::EquipmentPrice> &shop, Character::Base &character);
bool skillCheck(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, int team_size, Attribute::Type skill, int difficulty, int success, std::vector<int> &selection, bool useEquipment);
bool skillTestScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team_type, std::vector<int> team, Attribute::Type Skill, int difficulty, int success, bool useEquipment);
bool spellBook(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Character::Base &character, int spells_limit);
bool spellScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Spells::Base> spells, bool back_button);
bool storyScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Engine::Destination destination);
bool takeScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Equipment::Base> equipment, int TakeLimit, bool back_button);
bool testScreen(SDL_Window *window, SDL_Renderer *renderer, Book::Type bookID, int storyID);
bool vaultScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Character::Base &character);
bool viewParty(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, bool inCombat);

int armourSave(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, int damage);
int assignDamage(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, int combat_damage);
int attackScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Monster::Base> &monsters, int combatant, int opponent, int direction, int combatRound, bool useEquipment);
int castCombatSpell(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Monster::Base> &monsters, std::vector<int> hasAttacked, int combatRound);
int gainAttributeScore(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, Attribute::Type &attribute, int score, int rolls);
int magicAttackScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Monster::Base> &monsters, Spells::Base &spell, int combatant, int opponent, int fighting_score);
int seaAttackScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Ship::Base> &enemyFleet, int opponent, int direction);

// game screen: select single stuff
int selectOpponent(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &enemyFleet, std::vector<int> previousTargets, int combatRound);
int selectOpponent(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, int attacker, std::vector<Monster::Base> &monsters, std::vector<int> previousTargets, int combatRound, Control::Type mode);
int selectPartyMember(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, Equipment::Base equipment, Control::Type mode);
int selectShip(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> ships, Location::Type location, std::vector<Cargo::Type> cargo, Control::Type mode);

Attribute::Type selectAttribute(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, int increase);

Engine::Combat combatScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Monster::Base> &monsters, std::vector<Allies::Type> &allies, bool storyFlee, int fleeRound, int roundLimit, bool useEquipment);
Engine::Combat deploymentScreen(SDL_Window *window, SDL_Renderer *renderer, Location::Type location, Party::Base &party, std::vector<Army::Base> &enemyArmy, std::vector<Engine::BattlefieldSpells> &enemySpells, std::vector<Engine::ArmyStatus> &enemyStatus);
Engine::Combat massCombatScreen(SDL_Window *window, SDL_Renderer *renderer, Location::Type location, Party::Base &party, std::vector<Army::Base> &enemyArmy, std::vector<Engine::BattlefieldSpells> &enemySpells, std::vector<Engine::ArmyStatus> &enemyStatus);
Engine::Combat seaCombatScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Ship::Base> &enemyFleet, bool storyFlee, int fleeRound, int roundLimit);

// get story sections
Story::Base *findStory(Engine::Destination destination);
Story::Base *processChoices(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *story);
Story::Base *renderChoices(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *story);

// description utilities
std::string characterText(Character::Base &character, bool compact);
std::string itemString(Equipment::Base &equipment);
std::string monsterString(Monster::Base &monster);

// game screens (select multiple stuff)
std::vector<int> selectArmyUnits(SDL_Window *window, SDL_Renderer *renderer, std::vector<Army::Base> army, Location::Type garrison, int num_limit);
std::vector<int> selectArmyUnits(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Location::Type garrison, int num_limit);
std::vector<int> selectPartyMembers(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, int team_size, Control::Type mode);
std::vector<int> selectSpell(SDL_Window *window, SDL_Renderer *renderer, Character::Base &caster, std::vector<Spells::Base> &spells, int select_limit, Spells::Select mode);

void addBye(Story::Base *story, std::string bye);
void renderArmy(SDL_Renderer *renderer, TTF_Font *font, int text_space, std::vector<Army::Base> &army, int boxw, int boxh, int box_space, int offsety, SDL_Color fg, Uint32 bg);
void resolveMassCombat(SDL_Window *window, SDL_Renderer *renderer, Location::Type location, Party::Base &party, std::vector<Army::Base> &enemyArmy, std::vector<Engine::BattlefieldSpells> &enemySpells, std::vector<Engine::ArmyStatus> &enemyStatus, Location::Zone zone, int combatRound);
void storyTransition(Party::Base &party, Story::Base *story, Story::Base *next);

// List Controls
std::vector<Button> armyList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Army::Base> &army, int start, int last, int limit, int offsetx, int offsety, bool party_controls);
std::vector<Button> attributeList(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, std::vector<Attribute::Type> &attributes, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> cargoList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Engine::CargoPrices> &cargo, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> combatantList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> party, int start, int last, int limit, int offsetx, int offsety, bool confirm_button, bool back_button);
std::vector<Button> createChoices(SDL_Window *window, SDL_Renderer *renderer, std::vector<Choice::Base> choices, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> equipmentList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Equipment::Base> list, int start, int last, int limit, int offsety, int scrolly);
std::vector<Button> equipmentList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Equipment::Base> list, int start, int last, int limit, bool confirm_button, bool back_button);
std::vector<Button> innList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> party, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> monsterList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Monster::Base> &monsters, int start, int last, int limit, int offsetx, int offsety, bool confirm_button, bool back_button);
std::vector<Button> monsterList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Monster::Base> &monsters, int start, int last, int limit, int offsetx, int offsety, Control::Type mode);
std::vector<Button> romanceList(SDL_Window *window, SDL_Renderer *renderer, std::map<Character::Romance, int> &hearts, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, int start, int last, int limit, int offsetx, int offsety, Control::Type mode);
std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, int start, int last, int limit, int offsetx, int offsety, bool confirm_button, bool back_button);
std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Engine::ShipPrices> &ships, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> shopList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Engine::EquipmentPrice> &shop, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> rechargeList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Spells::Base> &spells, int start, int last, int limit, int offsetx, int offsety, int scrolly);
std::vector<Button> spellList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Spells::Base> &spells, int start, int last, int limit, int offsetx, int offsety, int scrolly);
std::vector<Button> spellList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Spells::Base> &spells, int start, int last, int limit, int offsetx, int offsety, int scrolly, bool confirm_button, bool back_button);
std::vector<Button> teamsList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Team::Type> &teams, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> vaultList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Equipment::Base> list, int start, int last, int limit, int offsety, int scrolly);

// Controls
std::vector<Button> harbourControls(SDL_Window *window, SDL_Renderer *renderer);
std::vector<Button> popupArmy(SDL_Window *window, SDL_Renderer *renderer, std::vector<Army::Base> &army, int start, int last, int limit, int popupw, int popuph, int infoh, int offsetx, int offsety);

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
SDL_Surface *createText(const char *text, const char *ttf, int font_size, SDL_Color textColor, int wrap, int style)
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

void renderTextButtons(SDL_Renderer *renderer, std::vector<TextButton> controls, const char *ttf, int selected, SDL_Color fg, Uint32 bg, Uint32 bgSelected, int fontsize, int style)
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

void renderTextButtons(SDL_Renderer *renderer, std::vector<TextButton> controls, const char *ttf, int selected, SDL_Color fg, Uint32 bg, Uint32 bgSelected, int fontsize, int offsetx, int scrolly, bool hide_scroll, int style)
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

std::vector<TextButton> createFixedTextButtons(const char **choices, int num, int text_buttonw, int text_buttonh, int textbutton_space, int text_x, int text_y)
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

            auto x = text_x + i * (text_buttonw + textbutton_space);

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

        auto raw_score = Engine::RAW_SCORE(character, character.Attributes[i].Type, true);
        auto mod_score = 0;

        character_text += std::string(Attribute::Descriptions[character.Attributes[i].Type]) + ": " + std::to_string(raw_score);

        if (character.Attributes[i].Type == Attribute::Type::FIGHTING)
        {
            mod_score = Engine::FIGHTING_SCORE(character);
        }
        else
        {
            mod_score = Engine::SCORE(character, character.Attributes[i].Type);
        }

        if (mod_score != raw_score)
        {
            character_text += "(" + std::to_string(mod_score) + ")";
        }
    }

    auto raw_health = character.Health;
    auto mod_health = Engine::HEALTH(character);

    character_text += ", Health: " + std::to_string(raw_health);

    if (raw_health != mod_health)
    {
        character_text += "(" + std::to_string(mod_health) + ")";
    }

    if (character.SpellCaster)
    {
        if (!compact)
        {
            character_text += "\n\n";
        }
        else
        {
            character_text += ", ";
        }

        character_text += "Spellcaster";
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

    auto font_mason = TTF_OpenFont(FONT_MASON, 24);
    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
    auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

    TTF_SetFontKerning(font_dark11, 0);

    // Render window
    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        auto current = 0;

        auto selected = false;

        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;
        auto text_space = 8;

        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space - infoh) / (88);
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

            putHeader(renderer, "Location", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty);
            putText(renderer, Location::Description[party.Location], font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + infoh);

            putHeader(renderer, "Money", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + 2 * infoh + box_space - 1));
            putText(renderer, (std::to_string(party.Money) + std::string(" silver coins")).c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (3 * boxh + infoh + box_space));

            putHeader(renderer, "Party", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

            if (Engine::COUNT(party) > 0)
            {
                std::string party_string = "";

                auto count = 0;

                for (auto i = 0; i < party.Members.size(); i++)
                {
                    if (count > 0)
                    {
                        party_string += "\n";
                    }

                    party_string += party.Members[i].Name;

                    if (Engine::IS_DEAD(party.Members[i]))
                    {
                        party_string += " (D)";
                    }
                    else if (Engine::IS_CURSED(party.Members[i]))
                    {
                        party_string += " (C)";
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

            fillRect(renderer, textwidth, (text_bounds - infoh), textx, (texty + infoh), intBE);

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

            renderButtons(renderer, controls, current, intLB, border_space, border_pts);

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
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

bool viewParty(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, bool inCombat)
{
    SDL_Surface *adventurer = NULL;
    SDL_Texture *adventurerTexture = NULL;
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

    auto character_box = (int)(text_bounds * 2 / 3);
    auto offset = 0;

    // Render window
    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        const char *choices[7] = {"PARTY", "PREVIOUS", "NEXT", "EQUIPMENT", "SPELLBOOK", "FOLLOWERS", "BACK"};

        auto current = 2;

        auto selected = false;

        auto main_buttonh = 48;

        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;

        auto controls = createHTextButtons(choices, 7, main_buttonh, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh), true);

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
        controls[7].Type = Control::Type::FOLLOWERS;
        controls[8].Type = Control::Type::BACK;

        auto done = false;

        auto text_space = 8;

        auto character = 0;

        auto current_mode = Control::Type::PARTY;

        if (character >= 0 && character < party.Members.size())
        {
            if (party.Members[character].Image != NULL)
            {
                if (adventurer != NULL)
                {
                    SDL_FreeSurface(adventurer);

                    adventurer = NULL;
                }

                if (adventurerTexture != NULL)
                {
                    SDL_DestroyTexture(adventurerTexture);

                    adventurerTexture = NULL;
                }

                adventurer = createImage(party.Members[character].Image);

                if (adventurer)
                {
                    adventurerTexture = SDL_CreateTextureFromSurface(renderer, adventurer);
                }
            }

            if (text != NULL)
            {
                SDL_FreeSurface(text);

                text = NULL;
            }

            text = createText(characterText(party.Members[character], false).c_str(), FONT_GARAMOND, garamond_size, clrDB, textwidth - 2 * text_space, TTF_STYLE_NORMAL);
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

            if (current_mode != Control::Type::PARTY)
            {
                if (adventurer)
                {
                    adventurerh = fitImage(renderer, adventurer, startx, starty, splashw, text_bounds);
                }
                else
                {
                    adventurerh = starty - infoh + text_space;
                }
            }
            else
            {
                putHeader(renderer, "Location", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty);
                putText(renderer, Location::Description[party.Location], font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + infoh);

                putHeader(renderer, "Money", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + 2 * infoh + box_space - 1));
                putText(renderer, (std::to_string(party.Money) + std::string(" silver coins")).c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (3 * boxh + infoh + box_space));

                putHeader(renderer, "Party", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (Engine::COUNT(party) > 0)
                {
                    std::string party_string = "";

                    auto count = 0;

                    for (auto i = 0; i < party.Members.size(); i++)
                    {
                        if (count > 0)
                        {
                            party_string += "\n";
                        }

                        party_string += party.Members[i].Name;

                        if (Engine::IS_DEAD(party.Members[i]))
                        {
                            party_string += " (D)";
                        }
                        else if (Engine::IS_CURSED(party.Members[i]))
                        {
                            party_string += " (C)";
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

            if (current_mode != Control::Type::PARTY && character >= 0 && character < party.Members.size())
            {
                putText(renderer, party.Members[character].Name, font_mason, -1, clrDB, intWH, TTF_STYLE_NORMAL, splashw, infoh, startx, adventurerh + infoh - text_space);
            }

            if (current_mode != Control::Type::PARTY && character >= 0 && character < party.Members.size())
            {
                if ((party.Members[character].Status.size() > 0) || (party.Members[character].Team != Team::Type::NONE && !Engine::IS_CHARACTER(party.Members[character].Team)))
                {
                    putHeader(renderer, "Status", font_mason2, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, starty + character_box + 10);

                    std::string status_string = "";

                    for (auto i = 0; i < party.Members[character].Status.size(); i++)
                    {
                        if (i > 0)
                        {
                            status_string += ", ";
                        }

                        status_string += Character::StatusDescriptions[party.Members[character].Status[i]];
                    }

                    if (party.Members[character].Team != Team::Type::NONE)
                    {
                        if (!Engine::IS_CHARACTER(party.Members[character].Team))
                        {
                            if (status_string.length() > 0)
                            {
                                status_string += ", ";
                            }

                            status_string += std::string(Team::Descriptions[party.Members[character].Team]);

                            if (party.Members[character].Team != Team::Type::SOLO)
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
                if (current_mode == Control::Type::SPELLBOOK && party.Members[character].SpellCaster && party.Members[character].SpellBook.size() > 0)
                {
                    putHeader(renderer, "SPELLBOOK", font_mason2, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, starty + character_box + 10);

                    std::string spellbook_string = "";

                    for (auto i = 0; i < party.Members[character].SpellBook.size(); i++)
                    {
                        if (i > 0)
                        {
                            spellbook_string += ", ";
                        }

                        spellbook_string += party.Members[character].SpellBook[i].Name;
                    }

                    putText(renderer, spellbook_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, (text_bounds - character_box) - infoh - box_space, textx, starty + text_bounds + infoh - (text_bounds - character_box) + box_space);
                }
                else if (current_mode == Control::Type::EQUIPMENT && party.Members[character].Equipment.size() > 0)
                {
                    putHeader(renderer, "EQUIPMENT", font_mason2, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, starty + character_box + 10);

                    std::string equipment_string = "";

                    for (auto i = 0; i < party.Members[character].Equipment.size(); i++)
                    {
                        if (i > 0)
                        {
                            equipment_string += ", ";
                        }

                        equipment_string += itemString(party.Members[character].Equipment[i]);
                    }

                    putText(renderer, equipment_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, (text_bounds - character_box) - infoh - box_space, textx, starty + text_bounds + infoh - (text_bounds - character_box) + box_space);
                }
                else if (current_mode == Control::Type::FOLLOWERS && party.Members[character].Followers.size() > 0)
                {
                    putHeader(renderer, "FOLLOWERS", font_mason2, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, starty + character_box + 10);

                    std::string followers_string = "";

                    for (auto i = 0; i < party.Members[character].Followers.size(); i++)
                    {
                        if (i > 0)
                        {
                            followers_string += ", ";
                        }

                        followers_string += std::string(party.Members[character].Followers[i].Name) + " (Health: " + std::to_string(party.Members[character].Followers[i].Health) + ")";
                    }

                    putText(renderer, followers_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, (text_bounds - character_box) - infoh - box_space, textx, starty + text_bounds + infoh - (text_bounds - character_box) + box_space);
                }
            }

            renderTextButtons(renderer, controls, FONT_DARK11, current, clrWH, intDB, intLB, font_size + 2, offsetx, scrolly, ((current_mode != Control::Type::PARTY || compact) ? true : false), TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;
            int scrollSpeed = 20;

            if (adventurer && current_mode != Control::Type::PARTY)
            {
                auto mousex = 0;
                auto mousey = 0;

                SDL_GetMouseState(&mousex, &mousey);

                auto zoomw = (int)(0.80 * (double)textwidth);
                auto zoomh = (int)(0.80 * (double)text_bounds);

                clipValue(zoomw, 0, adventurer->w);
                clipValue(zoomh, 0, adventurer->h);

                auto boundx = splashw;

                if (adventurerh == text_bounds)
                {
                    boundx = (int)((double)adventurerh / adventurer->h * (double)adventurer->w);
                }

                if (mousex >= startx && mousex <= (startx + boundx) && mousey >= starty && mousey <= (starty + adventurerh))
                {
                    auto scalex = (double)(mousex - startx) / boundx;
                    auto scaley = (double)(mousey - starty) / adventurerh;

                    int centerx = (int)(scalex * (double)adventurer->w);
                    int centery = (int)(scaley * (double)adventurer->h);

                    clipValue(centerx, zoomw / 2, adventurer->w - zoomw / 2);
                    clipValue(centery, zoomh / 2, adventurer->h - zoomh / 2);

                    if (adventurerTexture)
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

                        fillRect(renderer, dst.w, dst.h, dst.x, dst.y, intWH);
                        SDL_RenderCopy(renderer, adventurerTexture, &src, &dst);
                        drawRect(renderer, dst.w + 2, dst.h + 2, dst.x - 1, dst.y - 1, intBK);
                    }
                }
            }

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
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

                    if (character >= 0 && character < party.Members.size())
                    {
                        if (adventurer != NULL)
                        {
                            SDL_FreeSurface(adventurer);

                            adventurer = NULL;
                        }

                        if (adventurerTexture != NULL)
                        {
                            SDL_DestroyTexture(adventurerTexture);

                            adventurerTexture = NULL;
                        }

                        if (text != NULL)
                        {
                            SDL_FreeSurface(text);

                            text = NULL;
                        }

                        if (party.Members[character].Image != NULL)
                        {
                            adventurer = createImage(party.Members[character].Image);

                            if (adventurer)
                            {
                                adventurerTexture = SDL_CreateTextureFromSurface(renderer, adventurer);
                            }
                        }

                        text = createText(characterText(party.Members[character], false).c_str(), FONT_GARAMOND, garamond_size, clrDB, textwidth - 2 * text_space, TTF_STYLE_NORMAL);
                    }
                }
                else if (controls[current].Type == Control::Type::PLUS && !hold)
                {
                    if (character < party.Members.size() - 1)
                    {
                        character++;
                    }

                    if (character >= 0 && character < party.Members.size())
                    {
                        if (adventurer != NULL)
                        {
                            SDL_FreeSurface(adventurer);

                            adventurer = NULL;
                        }

                        if (adventurerTexture != NULL)
                        {
                            SDL_DestroyTexture(adventurerTexture);

                            adventurerTexture = NULL;
                        }

                        if (text != NULL)
                        {
                            SDL_FreeSurface(text);

                            text = NULL;
                        }

                        if (party.Members[character].Image != NULL)
                        {

                            adventurer = createImage(party.Members[character].Image);

                            if (adventurer)
                            {
                                adventurerTexture = SDL_CreateTextureFromSurface(renderer, adventurer);
                            }
                        }

                        text = createText(characterText(party.Members[character], false).c_str(), FONT_GARAMOND, garamond_size, clrDB, textwidth - 2 * text_space, TTF_STYLE_NORMAL);
                    }
                }
                else if (controls[current].Type == Control::Type::EQUIPMENT && !hold)
                {
                    if (Engine::IS_ACTIVE(party, character))
                    {
                        inventoryScreen(window, renderer, party, party.Members[character], -1, inCombat);
                    }

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::SPELLBOOK && !hold)
                {
                    if (party.Members[character].SpellCaster && party.Members[character].SpellBook.size() > 0 && !inCombat)
                    {
                        spellBook(window, renderer, party, party.Members[character], party.Members[character].SpellBookLimit);
                    }

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = true;
                }
            }
        }
    }

    if (adventurer != NULL)
    {
        SDL_FreeSurface(adventurer);

        adventurer = NULL;
    }

    if (adventurerTexture != NULL)
    {
        SDL_DestroyTexture(adventurerTexture);

        adventurerTexture = NULL;
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
    party.Members.clear();

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

        auto main_buttonh = 48;

        auto infoh = 48;

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
                    putHeader(renderer, message.c_str(), font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
                }
                else
                {
                    flash_message = false;
                }
            }

            if (Engine::FIND_LIST(selection, character) >= 0)
            {
                Input::GetInput(renderer, controls_del, current, selected, scrollUp, scrollDown, hold);
            }
            else
            {
                Input::GetInput(renderer, controls_add, current, selected, scrollUp, scrollDown, hold);
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

                                party.Members.push_back(character);
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

            renderButtons(renderer, controls, current, intLB, border_space, border_pts);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            if (map_valley && map_kingdom)
            {
                auto mousex = 0;
                auto mousey = 0;

                SDL_GetMouseState(&mousex, &mousey);

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

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size() && controls[current].Type == Control::Type::BACK)
            {
                done = true;
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

std::string itemString(Equipment::Base &equipment)
{
    std::string item_string = equipment.Name;

    if (equipment.TwoHanded)
    {
        item_string += "*";
    }

    if (equipment.Attribute != Attribute::Type::NONE || equipment.AdditionalSlots > 0)
    {
        item_string += " (";

        if (equipment.Attribute != Attribute::Type::NONE)
        {
            if (equipment.Attribute == Attribute::Type::FIGHTING3_LORE2)
            {
                item_string += std::string(Attribute::Descriptions[equipment.Attribute]);
            }
            else
            {
                item_string += "+" + std::to_string(equipment.Modifier) + " " + std::string(Attribute::Descriptions[equipment.Attribute]);
            }
        }

        if (equipment.AdditionalSlots > 0)
        {
            if (equipment.Attribute != Attribute::Type::NONE)
            {
                item_string += ", ";
            }

            item_string += std::to_string(equipment.AdditionalSlots + 1) + " slots";
        }

        item_string += ")";
    }

    return item_string;
}

std::vector<Button> equipmentList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Equipment::Base> list, int start, int last, int limit, bool confirm_button, bool back_button)
{
    auto font_size = 28;
    auto text_space = 8;
    auto listwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

    auto controls = std::vector<Button>();

    if (list.size() > 0)
    {
        for (int i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto item_string = itemString(list[index]);

            auto text = createText(item_string.c_str(), FONT_GARAMOND, font_size, clrBK, listwidth - 4 * text_space, TTF_STYLE_NORMAL);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : texty + 2 * text_space);

            controls.push_back(Button(i, text, i, i, (i > 0 ? i - 1 : i), (i < (last - start) ? i + 1 : i), textx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = listwidth - 4 * text_space;

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
    auto listwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

    auto controls = std::vector<Button>();

    if (list.size() > 0)
    {
        for (int i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto item_string = itemString(list[index]);

            auto text = createText(item_string.c_str(), FONT_GARAMOND, font_size, clrBK, listwidth - 4 * text_space, TTF_STYLE_NORMAL);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, text, i, i, (i > 0 ? i - 1 : i), (i < (last - start) ? i + 1 : i), textx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = listwidth - 4 * text_space;

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

std::vector<Button> vaultList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Equipment::Base> list, int start, int last, int limit, int offsety, int scrolly)
{
    auto font_size = 28;
    auto text_space = 8;
    auto listwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

    auto controls = std::vector<Button>();

    if (list.size() > 0)
    {
        for (int i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto item_string = itemString(list[index]);

            auto text = createText(item_string.c_str(), FONT_GARAMOND, font_size, clrBK, listwidth - 4 * text_space, TTF_STYLE_NORMAL);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, text, i, i, (i > 0 ? i - 1 : i), (i < (last - start) ? i + 1 : i), textx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = listwidth - 4 * text_space;

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
    controls.push_back(Button(idx + 1, "icons/interaction.png", idx, idx + 2, (list.size() > 0 ? (last - start) : idx + 1), idx + 1, startx + gridsize, buttony, Control::Type::TRANSFER));
    controls.push_back(Button(idx + 2, "icons/back-button.png", idx + 1, idx + 2, (list.size() > 0 ? (last - start) : idx + 2), idx + 2, ((int)((1.0 - Margin) * SCREEN_WIDTH) - buttonw), buttony, Control::Type::BACK));

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

            auto button = createHeaderButton(window, FONT_GARAMOND, 24, spell_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

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

std::vector<Button> spellList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Spells::Base> &spells, int start, int last, int limit, int offsetx, int offsety, int scrolly)
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

            auto button = createHeaderButton(window, FONT_GARAMOND, 24, spell_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

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

    controls.push_back(Button(idx, "icons/yes.png", idx, idx + 1, (spells.size() > 0 ? idx - 1 : idx), idx, startx, buttony, Control::Type::SPELL));
    controls.push_back(Button(idx + 1, "icons/no.png", idx, idx + 2, (spells.size() > 0 ? (last - start) : idx + 1), idx + 1, startx + gridsize, buttony, Control::Type::UNLEARN));
    controls.push_back(Button(idx + 2, "icons/back-button.png", idx + 1, idx + 2, (spells.size() > 0 ? (last - start) : idx + 2), idx + 2, ((int)((1.0 - Margin) * SCREEN_WIDTH) - buttonw), buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> rechargeList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Spells::Base> &spells, int start, int last, int limit, int offsetx, int offsety, int scrolly)
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

            auto button = createHeaderButton(window, FONT_GARAMOND, 24, spell_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

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

    controls.push_back(Button(idx, "icons/yes.png", idx, idx + 1, (spells.size() > 0 ? idx - 1 : idx), idx, startx, buttony, Control::Type::RECHARGE));
    controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, (spells.size() > 0 ? (last - start) : idx + 1), idx + 1, ((int)((1.0 - Margin) * SCREEN_WIDTH) - buttonw), buttony, Control::Type::BACK));

    return controls;
}

std::string monsterString(Monster::Base &monster)
{
    std::string monster_string = monster.Name;

    monster_string += "\nAttack: " + (monster.Attack > 0 ? std::to_string(monster.Attack) : std::string("Special")) + std::string(monster.Difficulty > 0 ? (" (" + std::to_string(monster.Difficulty) + "+)") : "");

    if (monster.Auto > 0)
    {
        monster_string += " +" + std::to_string(monster.Auto) + " Auto";
    }

    monster_string += ", Defense: " + std::to_string(monster.Defence) + "+, Health: " + std::to_string(monster.Health);

    return monster_string;
}

std::vector<Button> monsterList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Monster::Base> &monsters, int start, int last, int limit, int offsetx, int offsety, bool confirm_button, bool back_button)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (monsters.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto monster = monsters[index];

            std::string monster_string = monsterString(monster);

            auto button = createHeaderButton(window, FONT_GARAMOND, 24, monster_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

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

std::vector<Button> monsterList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Monster::Base> &monsters, int start, int last, int limit, int offsetx, int offsety, Control::Type mode)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (monsters.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto monster = monsters[index];

            std::string monster_string = monsterString(monster);

            auto button = createHeaderButton(window, FONT_GARAMOND, 24, monster_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

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

    auto text_y = (int)(SCREEN_HEIGHT * (1.0 - Margin)) - 48;

    controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "VIEW PARTY", clrWH, intDB, 220, 48, -1), idx, idx + 1, monsters.size() > 0 ? idx - 1 : idx, idx, startx, text_y, Control::Type::PARTY));

    if (mode == Control::Type::ATTACK)
    {
        controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "ATTACK", clrWH, intDB, 220, 48, -1), idx, idx + 2, monsters.size() > 0 ? idx - 1 : idx + 1, idx + 1, startx + (220 + button_space), text_y, Control::Type::ATTACK));
    }
    else if (mode == Control::Type::DEFEND)
    {
        controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "DEFEND", clrWH, intDB, 220, 48, -1), idx, idx + 2, monsters.size() > 0 ? idx - 1 : idx + 1, idx + 1, startx + (220 + button_space), text_y, Control::Type::ATTACK));
    }
    else if (mode == Control::Type::NEXT)
    {
        controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "NEXT ROUND", clrWH, intDB, 220, 48, -1), idx, idx + 2, monsters.size() > 0 ? idx - 1 : idx + 1, idx + 1, startx + (220 + button_space), text_y, Control::Type::ATTACK));
    }

    controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "CAST SPELL", clrWH, intDB, 220, 48, -1), idx + 1, idx + 3, monsters.size() > 0 ? idx - 1 : idx + 2, idx + 2, startx + 2 * (220 + button_space), text_y, Control::Type::SPELL));
    controls.push_back(Button(idx + 3, createHeaderButton(window, FONT_DARK11, 22, "FLEE", clrWH, intDB, 220, 48, -1), idx + 2, idx + 3, monsters.size() > 0 ? idx - 1 : idx + 3, idx + 3, startx + 3 * (220 + button_space), text_y, Control::Type::FLEE));

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

            auto button = createHeaderButton(window, FONT_GARAMOND, 22, adventurer_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

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

std::vector<Button> innList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> party, int start, int last, int limit, int offsetx, int offsety)
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

            auto button = createHeaderButton(window, FONT_GARAMOND, 22, adventurer_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, button, i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

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

    auto text_y = (int)(SCREEN_HEIGHT * (1.0 - Margin)) - 48;

    controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "HEAL 1", clrWH, intDB, 220, 48, -1), idx, idx + 1, party.size() > 0 ? idx - 1 : idx, idx, startx, text_y, Control::Type::HEAL1));
    controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "FULLY RECOVER", clrWH, intDB, 220, 48, -1), idx, idx + 2, party.size() > 0 ? idx - 1 : idx + 1, idx + 1, startx + (220 + button_space), text_y, Control::Type::FULL_RECOVERY));
    controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "RECHARGE SPELLS", clrWH, intDB, 220, 48, -1), idx + 1, idx + 3, party.size() > 0 ? idx - 1 : idx + 2, idx + 2, startx + 2 * (220 + button_space), text_y, Control::Type::RECHARGE));
    controls.push_back(Button(idx + 3, createHeaderButton(window, FONT_DARK11, 22, "BACK", clrWH, intDB, 220, 48, -1), idx + 2, idx + 3, party.size() > 0 ? idx - 1 : idx + 3, idx + 3, startx + 3 * (220 + button_space), text_y, Control::Type::BACK));

    return controls;
}

int armourSave(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, int damage)
{
    auto combat_damage = damage;
    auto final_damage = damage;

    if (Engine::IS_ALIVE(character) && Engine::ARMOUR(character) > 0)
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

            auto infoh = 48;

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

            std::vector<int> results = {};

            auto save_score = Engine::ARMOUR(character);

            if (save_score > combat_damage)
            {
                save_score = combat_damage;
            }

            auto size_dice = 64;

            auto cols = (fullwidth - 2 * box_space) / (size_dice + box_space);

            auto controls = std::vector<TextButton>();

            auto reduced = false;

            final_damage = combat_damage;

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

                            message = std::string(character.Name) + " dealt " + std::to_string(final_damage) + " damage!";

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
                defender_string += "\nHealth: " + std::to_string(Engine::HEALTH(character));

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

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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

    return std::max(0, final_damage);
}

int assignDamage(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, int combat_damage)
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

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 28);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto text_space = 8;
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space - infoh) / (88);
        auto last = offset + limit;

        if (last > party.Members.size())
        {
            last = party.Members.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = combatantList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh + text_space, true, false);

        auto done = false;

        auto selection = -1;

        while (!done)
        {
            auto current = -1;

            auto selected = false;

            auto scrollUp = false;

            auto scrollDown = false;

            auto hold = false;

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

                    if (index >= 0 && index < party.Members.size())
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
                        else if (Engine::IS_ALIVE(party.Members[index]))
                        {
                            drawRect(renderer, controls[index].W + border_space, controls[index].H + border_space, controls[index].X - 4, controls[index].Y - 4, intBK);
                        }
                        else
                        {
                            drawRect(renderer, controls[index].W + border_space, controls[index].H + border_space, controls[index].X - 4, controls[index].Y - 4, intRD);
                        }
                    }
                }

                renderButtons(renderer, controls, current, intLB, space, border_pts);

                putHeader(renderer, "Party", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                putHeader(renderer, (std::string("Deal " + std::to_string(combat_damage) + " damage to")).c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (selection >= 0 && selection < party.Members.size())
                {
                    if (Engine::IS_ALIVE(party.Members[selection]))
                    {
                        putText(renderer, party.Members[selection].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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
                        if (selection >= 0 && selection < party.Members.size())
                        {
                            done = true;

                            result = selection;

                            current = -1;

                            selected = false;
                        }
                        else
                        {
                            displayMessage("You must select an adventurer damaged by the attack.", intRD);
                        }
                    }
                    else if (controls[current].Type == Control::Type::ACTION)
                    {
                        if (current + offset >= 0 && current + offset < party.Members.size())
                        {
                            if (selection == current + offset)
                            {
                                selection = -1;
                            }
                            else
                            {
                                if (Engine::IS_ALIVE(party.Members[current + offset]))
                                {
                                    if (team == Team::Type::NONE || party.Members[current + offset].Team == team)
                                    {
                                        selection = current + offset;
                                    }
                                    else
                                    {
                                        if (!Engine::IS_CHARACTER(party.Members[current + offset].Team))
                                        {
                                            displayMessage(std::string(party.Members[current + offset].Name) + " is not part of the " + std::string(Team::Descriptions[team]) + " team!", intRD);
                                        }
                                        else
                                        {
                                            displayMessage("You can only assign damage to " + std::string(Team::Descriptions[team]) + "!", intRD);
                                        }
                                    }
                                }
                                else
                                {
                                    displayMessage(std::string(party.Members[current + offset].Name) + " is dead!", intRD);
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

int magicAttackScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Monster::Base> &monsters, Spells::Base &spell, int combatant, int opponent, int fighting_score)
{
    auto combat_damage = 0;

    auto Defence = monsters[opponent].Defence;

    if (Engine::VERIFY_CODES(party, {Codes::Type::ENEMY_DAZING_LIGHTS}))
    {
        Defence += 1;

        Defence = std::min(Defence, 6);
    }

    if (Engine::COUNT(party) > 0 && Engine::COUNT(monsters) > 0)
    {
        if (window && renderer)
        {
            auto flash_message = false;

            auto flash_color = intRD;

            std::string message = "";

            Uint32 start_ticks = 0;

            Uint32 duration = 3000;

            // Lambda functions for displaying flash messages
            auto displayMessage = [&](std::string msg, Uint32 color)
            {
                flash_message = true;

                message = msg;

                flash_color = color;

                start_ticks = SDL_GetTicks();
            };

            auto marginx = (int)(Margin * SCREEN_WIDTH);

            auto fullwidth = SCREEN_WIDTH - 2 * marginx;

            auto boxwidth = (SCREEN_WIDTH - 3 * marginx) / 2;

            auto headerw = (int)(boxwidth * 0.75);

            auto infoh = 48;

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

            std::vector<int> results = {};

            auto size_dice = 64;

            auto cols = (fullwidth - 2 * box_space) / (size_dice + box_space);

            auto controls = std::vector<TextButton>();

            auto damaged = false;

            if (Engine::VERIFY_CODES(party, {Codes::Type::ENEMY_DAZING_LIGHTS}))
            {
                displayMessage("The enemy cast Dazing Lights. Your target is more difficult to hit!", intRD);
            }

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
                                if (results[i] >= Defence)
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
                            std::string stunned = "";

                            if (damage > 0 && monsters[opponent].Type == Monster::Type::FASTILON && !monsters[opponent].Damaged)
                            {
                                stunned = "\n\n" + std::string(party.Members[combatant].Name) + " is stunned next round!";

                                Engine::GAIN_STATUS(party.Members[combatant], Character::Status::STUNNED_NEXT_ROUND);
                            }

                            Engine::GAIN_HEALTH(monsters[opponent], -damage);

                            combat_damage = damage;

                            if (damage > 0)
                            {
                                displayMessage(std::string(party.Members[combatant].Name) + "'s " + std::string(spell.Name) + " deals " + std::to_string(damage) + " to the " + std::string(monsters[opponent].Name) + "!" + stunned, intLB);
                            }
                            else
                            {
                                displayMessage(std::string(party.Members[combatant].Name) + "'s " + std::string(spell.Name) + " was ineffective!", intRD);
                            }

                            damaged = true;
                        }
                    }
                }

                putHeader(renderer, party.Members[combatant].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);
                fillRect(renderer, boxwidth, boxh, startx, starty + infoh, intBE);
                std::string attacker_string = "Magic Fighting Score: " + std::to_string(fighting_score);
                putText(renderer, attacker_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx, starty + infoh);

                putHeader(renderer, monsters[opponent].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx + boxwidth + marginx, starty);
                fillRect(renderer, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh, intBE);
                std::string defender_string = "Defence: " + std::to_string(Defence) + "+\nHealth: " + std::to_string(monsters[opponent].Health);
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

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (stage == Engine::Attack::START && controls[current].Type == Control::Type::BACK)
                    {
                        combat_damage = -1;

                        done = true;

                        current = -1;

                        selected = false;
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

std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, int start, int last, int limit, int offsetx, int offsety, bool confirm_button, bool back_button)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (ships.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto ship = ships[index];

            std::string ship_string = "";

            ship_string += "[" + std::string(ship.Name) + "]";

            ship_string += "\nFighting: " + (ship.Fighting > 0 ? std::to_string(ship.Fighting) : std::string("Special"));

            ship_string += " Health: " + std::to_string(ship.Health);

            auto button = createHeaderButton(window, FONT_GARAMOND, 24, ship_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, button, i, i, (i > 0 ? i - 1 : i), (i < (last - start) ? i + 1 : i), offsetx + 2 * text_space, y, Control::Type::ACTION));

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

    if (confirm_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/yes.png", idx - 1, (back_button ? idx + 1 : idx), ships.size() > 0 ? (last - start) - 1 : idx, idx, startx, buttony, Control::Type::CONFIRM));
    }

    if (back_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/back-button.png", idx - 1, idx, ships.size() > 0 ? (last - start) - 1 : idx, idx, ((int)((1.0 - Margin) * SCREEN_WIDTH) - buttonw), buttony, Control::Type::BACK));
    }

    return controls;
}

std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, int start, int last, int limit, int offsetx, int offsety, Control::Type mode)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (ships.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto ship = ships[index];

            std::string ship_string = "";

            ship_string = "[" + std::string(ship.Name) + "]";

            ship_string += "\nFighting: " + (ship.Fighting > 0 ? std::to_string(ship.Fighting) : std::string("Special")) + " Health: " + std::to_string(ship.Health);

            auto button = createHeaderButton(window, FONT_GARAMOND, 24, ship_string.c_str(), clrBK, intBE, textwidth - 3 * button_space / 2, (text_space + 28) * 2, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, button, i, i, (i > 0 ? i - 1 : i), (i < (last - start) ? i + 1 : i), offsetx + 2 * text_space, y, Control::Type::ACTION));

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

    auto text_y = (int)(SCREEN_HEIGHT * (1.0 - Margin)) - 48;

    if (mode == Control::Type::ATTACK)
    {
        controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "ATTACK", clrWH, intDB, 220, 48, -1), idx, idx + 1, ships.size() > 0 ? idx - 1 : idx, idx, startx, text_y, Control::Type::ATTACK));
    }
    else if (mode == Control::Type::DEFEND)
    {
        controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "DEFEND", clrWH, intDB, 220, 48, -1), idx, idx + 1, ships.size() > 0 ? idx - 1 : idx, idx, startx, text_y, Control::Type::ATTACK));
    }
    else if (mode == Control::Type::NEXT)
    {
        controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "NEXT ROUND", clrWH, intDB, 220, 48, -1), idx, idx + 1, ships.size() > 0 ? idx - 1 : idx, idx, startx, text_y, Control::Type::ATTACK));
    }

    controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "CAST SPELL", clrWH, intDB, 220, 48, -1), idx, idx + 2, ships.size() > 0 ? idx - 1 : idx + 1, idx + 1, startx + (220 + button_space), text_y, Control::Type::SPELL));
    controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "FLEE", clrWH, intDB, 220, 48, -1), idx + 1, idx + 2, ships.size() > 0 ? idx - 1 : idx + 2, idx + 2, startx + 2 * (220 + button_space), text_y, Control::Type::FLEE));

    return controls;
}

int seaAttackScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Ship::Base> &enemyFleet, int opponent, int direction)
{
    auto flash_message = false;

    auto flash_color = intRD;

    std::string message = "";

    Uint32 start_ticks = 0;

    Uint32 duration = 3000;

    // Lambda functions for displaying flash messages
    auto displayMessage = [&](std::string msg, Uint32 color)
    {
        flash_message = true;

        message = msg;

        flash_color = color;

        start_ticks = SDL_GetTicks();
    };

    TTF_Init();

    auto font_mason = TTF_OpenFont(FONT_MASON, 32);

    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 32);

    auto text_space = 8;

    auto font_size = 24;

    SDL_Surface *dice[6];

    dice[0] = createImage("images/dice/dice1.png");
    dice[1] = createImage("images/dice/dice2.png");
    dice[2] = createImage("images/dice/dice3.png");
    dice[3] = createImage("images/dice/dice4.png");
    dice[4] = createImage("images/dice/dice5.png");
    dice[5] = createImage("images/dice/dice6.png");

    auto size_dice = 64;

    auto attacks = 1;

    auto Difficulty = 4;

    std::vector<int> target_damage = {};

    SDL_SetWindowTitle(window, "Legendary Kingdoms: Ship Attack");

    const char *choices_attack[2] = {"Attack", "Cancel"};
    const char *choices_defend[1] = {"Attack"};
    const char *choices_damage[1] = {"Deal Damage"};
    const char *choices_end[1] = {"Done"};

    auto marginx = (int)(Margin * SCREEN_WIDTH);

    auto fullwidth = SCREEN_WIDTH - 2 * marginx;

    auto boxwidth = (SCREEN_WIDTH - 3 * marginx) / 2;

    auto headerw = (int)(boxwidth * 0.75);

    auto infoh = 48;

    auto boxh = (int)(0.125 * SCREEN_HEIGHT);

    auto box_space = 10;

    auto main_buttonh = 48;

    auto main_buttonw = 220;

    auto cols = (fullwidth - 2 * box_space) / (size_dice + box_space);

    auto controls_attack = createFixedTextButtons(choices_attack, 2, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
    controls_attack[0].Type = Control::Type::CONFIRM;
    controls_attack[1].Type = Control::Type::BACK;

    auto controls_defend = createFixedTextButtons(choices_defend, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
    controls_defend[0].Type = Control::Type::CONFIRM;

    auto controls_damage = createFixedTextButtons(choices_damage, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
    controls_damage[0].Type = Control::Type::CONFIRM;

    auto controls_end = createFixedTextButtons(choices_end, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
    controls_end[0].Type = Control::Type::BACK;

    auto combat_damage = 0;

    for (auto num_attacks = 0; num_attacks < attacks; num_attacks++)
    {
        if (Engine::SHIP_INTACT(party) && opponent >= 0 && opponent < enemyFleet.size() && enemyFleet[opponent].Health > 0)
        {
            if (window && renderer)
            {
                auto done = false;

                auto stage = Engine::Attack::START;

                auto current = -1;

                auto selected = false;

                auto scrollUp = false;

                auto scrollDown = false;

                auto hold = false;

                std::vector<int> results = {};

                combat_damage = 0;

                auto attack_score = 1;

                auto controls = std::vector<TextButton>();

                auto damaged = false;

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
                                    if (results[i] >= Difficulty)
                                    {
                                        thickRect(renderer, size_dice, size_dice, offsetx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), (direction == 0 ? intLB : intRD), 2);

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
                                if (direction == 0)
                                {
                                    auto damage_scale = 1;

                                    Engine::GAIN_HEALTH(enemyFleet[opponent], -(damage_scale * damage));

                                    combat_damage = damage_scale * damage;

                                    if (damage_scale * damage > 0)
                                    {
                                        displayMessage("The " + std::string(party.Fleet[party.CurrentShip].Name) + " deals " + std::to_string(damage_scale * damage) + " to the " + std::string(enemyFleet[opponent].Name) + "!", intLB);
                                    }
                                    else
                                    {
                                        displayMessage("The " + std::string(party.Fleet[party.CurrentShip].Name) + "'s attack was ineffective!", intRD);
                                    }
                                }
                                else
                                {
                                    combat_damage = damage;

                                    if (combat_damage > 0)
                                    {
                                        Engine::GAIN_HEALTH(party.Fleet[party.CurrentShip], -damage);

                                        displayMessage(std::string(enemyFleet[opponent].Name) + " deals " + std::to_string(damage) + " to your ship!", intRD);
                                    }
                                    else
                                    {
                                        displayMessage("The " + std::string(enemyFleet[opponent].Name) + "'s attack was ineffective!", intLB);
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
                            special_event_trigger = false;
                        }
                    }

                    std::string attacker_string = "";

                    fillRect(renderer, boxwidth, boxh, startx, starty + infoh, intBE);

                    if (direction == 0)
                    {
                        putHeader(renderer, party.Fleet[party.CurrentShip].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);

                        attack_score = party.Fleet[party.CurrentShip].Fighting;

                        if (Engine::VERIFY_CODES(party, {Codes::Type::CANNOT_USE_SHIPWEAPONS}))
                        {
                            attack_score -= 1;

                            if (attack_score < 0)
                            {
                                attack_score = 0;
                            }
                        }

                        attacker_string = "Fighting: " + std::to_string(attack_score);
                        attacker_string += "\nHealth: " + std::to_string(party.Fleet[party.CurrentShip].Health);
                    }
                    else
                    {
                        putHeader(renderer, enemyFleet[opponent].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);

                        attack_score = enemyFleet[opponent].Fighting;

                        if (attack_score < 0)
                        {
                            attack_score = 0;
                        }

                        attacker_string = "Fighting: " + std::to_string(attack_score);
                        attacker_string += "\nHealth: " + std::to_string(enemyFleet[opponent].Health);
                    }

                    putText(renderer, attacker_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx, starty + infoh);

                    std::string defender_string = "";

                    if (direction == 0)
                    {
                        putHeader(renderer, enemyFleet[opponent].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx + boxwidth + marginx, starty);
                        defender_string = "Defence: " + std::to_string(Difficulty) + "+";
                        defender_string += "\nHealth: " + std::to_string(enemyFleet[opponent].Health);
                    }
                    else
                    {
                        putHeader(renderer, party.Fleet[party.CurrentShip].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx + boxwidth + marginx, starty);
                        defender_string = "Defence: " + std::to_string(Difficulty) + "+";
                        defender_string += "\nHealth: " + std::to_string(party.Fleet[party.CurrentShip].Health);
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

                    Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                    if (selected && current >= 0 && current < controls.size())
                    {
                        if (stage == Engine::Attack::START && controls[current].Type == Control::Type::BACK)
                        {
                            done = true;

                            current = -1;

                            selected = false;

                            combat_damage = -1;
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
                        }
                        else if (stage == Engine::Attack::DAMAGE && controls[current].Type == Control::Type::CONFIRM)
                        {
                            if (Engine::SHIP_INTACT(party))
                            {
                                if (combat_damage > 0)
                                {
                                    displayMessage(std::string(party.Fleet[party.CurrentShip].Name) + " dealt " + std::to_string(combat_damage) + " damage!", intRD);

                                    Engine::GAIN_HEALTH(party.Fleet[party.CurrentShip], -combat_damage);
                                }
                                else
                                {
                                    displayMessage("The " + std::string(enemyFleet[opponent].Name) + "'s attack was ineffective!", intLB);
                                }
                            }
                            else
                            {
                                done = true;

                                selected = false;

                                current = -1;
                            }
                        }
                    }
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

    return std::max(0, combat_damage);
}

int attackScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Monster::Base> &monsters, int combatant, int opponent, int direction, int combatRound, bool useEquipment)
{
    auto combat_damage = 0;

    auto num_attacks = direction == 0 ? 1 : std::min(monsters[opponent].Attacks, Engine::COUNT(party, team));

    auto Difficulty = monsters[opponent].Difficulty;

    auto Defence = monsters[opponent].Defence;

    std::vector<int> target_damage = {};

    auto spell = Spells::Type::NONE;

    if (direction == 1)
    {
        if (monsters[opponent].Type == Monster::Type::IMOPPOSH_THE_MAD)
        {
            if (combatRound == 0)
            {
                num_attacks = std::min(2, Engine::COUNT(party, team));
            }
            else if (combatRound == 2)
            {
                num_attacks = Engine::COUNT(party, team);

                spell = Spells::Type::SANDSTORM;
            }
        }
        else if (monsters[opponent].Type == Monster::Type::MONKEY_WITH_SPELLS)
        {
            auto roll = Engine::ROLL(1);

            if (roll <= 2)
            {
                spell = Spells::Type::ICE_BOLT;

                num_attacks = 1;

                Difficulty = 4;
            }
            else if (roll <= 4)
            {
                spell = Spells::Type::UNFAILING_STRIKE;

                num_attacks = 1;

                Difficulty = 1;
            }
            else if (roll <= 6)
            {
                num_attacks = std::min(2, Engine::COUNT(party, team));

                spell = Spells::Type::POISON_STREAM;

                Difficulty = 4;
            }
        }
        else if (monsters[opponent].Type == Monster::Type::ZEALOT_SORCERER)
        {
            if (combatRound == 0)
            {
                spell = Spells::Type::ICE_BOLT;

                num_attacks = 1;
            }
            else if (combatRound == 2)
            {
                num_attacks = Engine::COUNT(party, team);

                spell = Spells::Type::SANDSTORM;
            }
        }
        else if (monsters[opponent].Type == Monster::Type::FASTILON)
        {
            if (combatRound == 0)
            {
                spell = Spells::Type::RUNE_OF_UNMAKING;

                num_attacks = 1;
            }
            else if (combatRound == 1)
            {
                num_attacks = std::min(2, Engine::COUNT(party, team));
            }
        }
    }
    else
    {
        if (Engine::VERIFY_CODES(party, {Codes::Type::ENEMY_DAZING_LIGHTS}))
        {
            Defence += 1;

            Defence = std::min(Defence, 6);
        }
    }

    auto flash_message = false;

    auto flash_color = intRD;

    std::string message = "";

    Uint32 start_ticks = 0;

    Uint32 duration = 3000;

    // Lambda functions for displaying flash messages
    auto displayMessage = [&](std::string msg, Uint32 color)
    {
        flash_message = true;

        message = msg;

        flash_color = color;

        start_ticks = SDL_GetTicks();
    };

    auto marginx = (int)(Margin * SCREEN_WIDTH);

    auto fullwidth = SCREEN_WIDTH - 2 * marginx;

    auto boxwidth = (SCREEN_WIDTH - 3 * marginx) / 2;

    auto headerw = (int)(boxwidth * 0.75);

    auto infoh = 48;

    auto boxh = (int)(0.125 * SCREEN_HEIGHT);

    auto box_space = 10;

    auto main_buttonh = 48;

    auto main_buttonw = 220;

    TTF_Init();

    auto font_mason = TTF_OpenFont(FONT_MASON, 32);

    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 32);

    auto text_space = 8;

    auto font_size = 24;

    auto size_dice = 64;

    auto cols = (fullwidth - 2 * box_space) / (size_dice + box_space);

    const char *choices_attack[4] = {"Attack", "Add Focus", "Remove Focus", "Cancel"};
    const char *choices_skipattack[5] = {"Attack", "Skip", "Add Focus", "Remove Focus", "Cancel"};
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

    auto controls_attack = createFixedTextButtons(choices_attack, 4, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
    controls_attack[0].Type = Control::Type::CONFIRM;
    controls_attack[1].Type = Control::Type::PLUS;
    controls_attack[2].Type = Control::Type::MINUS;
    controls_attack[3].Type = Control::Type::BACK;

    auto controls_skipattack = createFixedTextButtons(choices_skipattack, 5, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
    controls_skipattack[0].Type = Control::Type::CONFIRM;
    controls_skipattack[1].Type = Control::Type::NEXT;
    controls_skipattack[2].Type = Control::Type::PLUS;
    controls_skipattack[3].Type = Control::Type::MINUS;
    controls_skipattack[4].Type = Control::Type::BACK;

    auto controls_defend = createFixedTextButtons(choices_defend, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
    controls_defend[0].Type = Control::Type::CONFIRM;

    auto controls_damage = createFixedTextButtons(choices_damage, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
    controls_damage[0].Type = Control::Type::CONFIRM;

    auto controls_end = createFixedTextButtons(choices_end, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
    controls_end[0].Type = Control::Type::BACK;

    auto controls_assign = createFixedTextButtons(choices_assign, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
    controls_assign[0].Type = Control::Type::CONFIRM;

    for (auto attacks = 0; attacks < num_attacks; attacks++)
    {
        if (Engine::COUNT(party, team) > 0 && Engine::COUNT(monsters) > 0)
        {
            if (window && renderer)
            {
                auto done = false;

                auto stage = Engine::Attack::START;

                SDL_SetWindowTitle(window, "Legendary Kingdoms: Attack");

                auto current = -1;

                auto selected = false;

                auto scrollUp = false;

                auto scrollDown = false;

                auto hold = false;

                auto focus = 0;

                std::vector<int> results = {};

                combat_damage = 0;

                auto attack_score = 1;

                auto controls = std::vector<TextButton>();

                auto damaged = false;

                auto assigned = false;

                auto special_event_trigger = true;

                flash_message = false;

                while (!done)
                {
                    fillWindow(renderer, intWH);

                    putHeader(renderer, "Attack Results", font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty + infoh + boxh + box_space);

                    fillRect(renderer, fullwidth, boxh * 3, startx, starty + infoh + boxh + box_space + infoh, intBE);

                    putHeader(renderer, (std::string("Focus: " + std::to_string(focus))).c_str(), font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, boxwidth, infoh, startx, starty + 2 * infoh + 4 * boxh + 2 * box_space);

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

                                if (direction == 1 && Engine::VERIFY_CODES(party, {Codes::Type::DAZING_LIGHTS}))
                                {
                                    result = std::max(0, result - 1);
                                }

                                fitImage(renderer, dice[result], offsetx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), size_dice, size_dice);

                                if (stage == Engine::Attack::DAMAGE)
                                {
                                    if (direction == 0)
                                    {
                                        if (results[i] >= (Defence - focus))
                                        {
                                            thickRect(renderer, size_dice, size_dice, offsetx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), intLB, 2);

                                            damage++;
                                        }
                                    }
                                    else
                                    {
                                        auto attack_result = results[i];

                                        if (Engine::VERIFY_CODES(party, {Codes::Type::DAZING_LIGHTS}))
                                        {
                                            attack_result = std::max(1, attack_result - 1);
                                        }

                                        if (attack_result >= Difficulty)
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
                                    auto damage_scale = 1;

                                    auto weapon_used = Engine::FIND_EQUIPMENT(party.Members[combatant], Equipment::Class::WEAPON, Attribute::Type::FIGHTING);

                                    if (weapon_used >= 0 && weapon_used < party.Members[combatant].Equipment.size() && party.Members[combatant].Equipment[weapon_used].Type == Equipment::Type::STONECUTTER_SWORD2)
                                    {
                                        if (monsters[opponent].Type == Monster::Type::ROCK || monsters[opponent].Type == Monster::Type::STONE)
                                        {
                                            damage_scale = 2;
                                        }
                                    }

                                    combat_damage = damage_scale * damage;

                                    std::string stunned = "";

                                    if (combat_damage > 0 && monsters[opponent].Type == Monster::Type::FASTILON && !monsters[opponent].Damaged)
                                    {
                                        Engine::GAIN_STATUS(party.Members[combatant], Character::Status::STUNNED_NEXT_ROUND);

                                        stunned = "\n\n" + std::string(party.Members[combatant].Name) + " is stunned next round!";
                                    }

                                    Engine::GAIN_HEALTH(monsters[opponent], -combat_damage);

                                    if (combat_damage > 0)
                                    {
                                        displayMessage(std::string(party.Members[combatant].Name) + " deals " + std::to_string(damage_scale * damage) + " to the " + std::string(monsters[opponent].Name) + "!" + stunned, intLB);
                                    }
                                    else
                                    {
                                        displayMessage(std::string(party.Members[combatant].Name) + "'s attack was ineffective!", intRD);
                                    }
                                }
                                else
                                {
                                    combat_damage = damage;

                                    if (combat_damage > 0)
                                    {
                                        displayMessage(std::string(monsters[opponent].Name) + " deals " + std::to_string(damage) + " to the party!", intRD);
                                    }
                                    else
                                    {
                                        assigned = true;

                                        displayMessage("The " + std::string(monsters[opponent].Name) + "'s attack was ineffective!", intLB);
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
                            if (Engine::VERIFY_CODES(party, {Codes::Type::DAZING_LIGHTS}))
                            {
                                displayMessage("Dazing Lights reduces " + std::string(monsters[opponent].Name) + "'s Attack results by 1.", intLB);
                            }

                            if (monsters[opponent].Type == Monster::Type::SKALLOS && !monsters[opponent].Damaged)
                            {
                                displayMessage("Skallos unleashes a roar of black magic! Each party member loses 1 Health. Skallos recovers 4 Health points!", intRD);

                                Engine::GAIN_HEALTH(party, -1);

                                Engine::GAIN_HEALTH(monsters[opponent], 4);
                            }
                            else if (monsters[opponent].Type == Monster::Type::SNAKEMAN_PRIEST || monsters[opponent].Type == Monster::Type::SNAKEMAN)
                            {
                                if (Engine::VERIFY_EQUIPMENT(party, {Equipment::Type::HYGLIPH_FLOWER}))
                                {
                                    Difficulty = 5;

                                    if (monsters[opponent].Type == Monster::Type::SNAKEMAN_PRIEST)
                                    {
                                        message = "The priest is put off by the pungent odour of the HYGLIPH FLOWER and requires a 5+ to his attack rolls to inflict damage during this battle.";
                                    }
                                    else if (monsters[opponent].Type == Monster::Type::SNAKEMAN)
                                    {
                                        if (monsters[opponent].Difficulty == 5)
                                        {
                                            Difficulty = 6;
                                        }

                                        message = "The snakeman is put off by the pungent odour of the HYGLIPH FLOWER and requires a " + std::to_string(Difficulty) + "+ to his attack rolls to inflict damage during this battle.";
                                    }

                                    displayMessage(message, intLB);
                                }
                            }
                            else if (monsters[opponent].Type == Monster::Type::IMOPPOSH_THE_MAD)
                            {
                                if (attacks == 0)
                                {
                                    if (combatRound == 0)
                                    {
                                        message = "Imopposh casts a Thunderbolt spell! He makes two Fighting: 10 attacks!";
                                    }
                                    else if (combatRound == 1)
                                    {
                                        message = "Imopposh casts an Orb of Annihilation! He makes a single Fighting: 13 attack!";
                                    }

                                    displayMessage(message, intRD);
                                }
                            }
                            else if (monsters[opponent].Type == Monster::Type::CURSITE_ASSASSIN)
                            {
                                if (!monsters[opponent].Damaged)
                                {
                                    displayMessage("The Cursite Assassin executes a backstabbing attack dealing 4 damage to the party!", intRD);

                                    damaged = true;

                                    assigned = false;

                                    stage = Engine::Attack::DAMAGE;

                                    combat_damage = 4;
                                }
                            }
                            else if (monsters[opponent].Type == Monster::Type::FASTILON)
                            {
                                if (attacks == 0)
                                {
                                    if (combatRound == 1)
                                    {
                                        message = "Fastilon casts a Thunderbolt spell! He makes two Fighting: 5 attacks!";
                                    }

                                    displayMessage(message, intRD);
                                }
                            }

                            if (spell != Spells::Type::NONE && attacks == 0)
                            {
                                message = std::string(monsters[opponent].Name) + " casts ";

                                if (spell == Spells::Type::ICE_BOLT)
                                {
                                    message += "Ice Bolt! " + std::string(monsters[opponent].Name) + " makes a Fighting: 8 attack at Difficulty: " + std::to_string(Difficulty) + "+";
                                }
                                else if (spell == Spells::Type::UNFAILING_STRIKE)
                                {
                                    message += "Unfailing Strike! " + std::string(monsters[opponent].Name) + " deals 3 damage to the party!";

                                    damaged = true;

                                    assigned = false;

                                    stage = Engine::Attack::DAMAGE;

                                    combat_damage = 3;
                                }
                                else if (spell == Spells::Type::RUNE_OF_UNMAKING)
                                {
                                    message += "Rune of Unmaking! " + std::string(monsters[opponent].Name) + " deals 6 damage to the party!";

                                    damaged = true;

                                    assigned = false;

                                    stage = Engine::Attack::DAMAGE;

                                    combat_damage = 6;
                                }
                                else if (spell == Spells::Type::POISON_STREAM)
                                {
                                    message += "Poison Stream! " + std::string(monsters[opponent].Name) + " makes two Fighting: 5 attack at Difficulty: " + std::to_string(Difficulty) + "+";
                                }
                                else if (spell == Spells::Type::SANDSTORM)
                                {
                                    message += "Sandstorm! " + std::string(monsters[opponent].Name) + " makes a Fighting: 3 attack on each party member!";
                                }

                                displayMessage(message, intRD);
                            }

                            special_event_trigger = false;
                        }
                    }

                    std::string attacker_string = "";

                    fillRect(renderer, boxwidth, boxh, startx, starty + infoh, intBE);

                    if (direction == 0)
                    {
                        putHeader(renderer, party.Members[combatant].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);

                        if (useEquipment)
                        {
                            attack_score = Engine::FIGHTING_SCORE(party.Members[combatant]) - (focus * 5);
                        }
                        else
                        {
                            attack_score = Engine::SCORE(party.Members[combatant], Attribute::Type::FIGHTING) - (focus * 5);
                        }

                        attacker_string = "Fighting: " + std::to_string(attack_score);
                        attacker_string += "\nHealth: " + std::to_string(Engine::HEALTH(party.Members[combatant]));
                    }
                    else
                    {
                        putHeader(renderer, monsters[opponent].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);

                        attack_score = monsters[opponent].Attack;

                        if (monsters[opponent].Type == Monster::Type::IMOPPOSH_THE_MAD)
                        {
                            if (combatRound == 0)
                            {
                                attack_score = 10;
                            }
                            else if (combatRound == 1)
                            {
                                attack_score = 13;
                            }
                            else if (combatRound == 2)
                            {
                                attack_score = 3;
                            }
                        }
                        else if (monsters[opponent].Type == Monster::Type::FASTILON)
                        {
                            if (combatRound == 1)
                            {
                                attack_score = 5;
                            }
                        }

                        if (spell == Spells::Type::ICE_BOLT)
                        {
                            attack_score = 8;
                        }
                        else if (spell == Spells::Type::POISON_STREAM)
                        {
                            attack_score = 5;
                        }
                        else if (spell == Spells::Type::SANDSTORM)
                        {
                            attack_score = 3;
                        }

                        if (attack_score < 0)
                        {
                            attack_score = 0;
                        }

                        attacker_string = "Attack: " + std::to_string(attack_score) + " (" + std::to_string(Difficulty) + "+)";

                        if (monsters[opponent].Auto > 0)
                        {
                            attacker_string += " +" + std::to_string(monsters[opponent].Auto) + " Auto";
                        }

                        attacker_string += "\nHealth: " + std::to_string(monsters[opponent].Health);
                    }

                    putText(renderer, attacker_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx, starty + infoh);

                    std::string defender_string = "";

                    if (direction == 0)
                    {
                        putHeader(renderer, monsters[opponent].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx + boxwidth + marginx, starty);
                        defender_string = "Defence: " + std::to_string(Defence - focus) + "+";
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
                            putHeader(renderer, party.Members[combatant].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx + boxwidth + marginx, starty);
                            defender_string = "Health: " + std::to_string(Engine::HEALTH(party.Members[combatant]));
                        }
                    }

                    fillRect(renderer, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh, intBE);

                    putText(renderer, defender_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh);

                    if (stage == Engine::Attack::START)
                    {
                        if (direction == 0)
                        {
                            if (monsters[opponent].Type == Monster::Type::JUNGLE)
                            {
                                controls = controls_skipattack;
                            }
                            else
                            {
                                controls = controls_attack;
                            }
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

                    Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                    if (selected && current >= 0 && current < controls.size())
                    {
                        if (stage == Engine::Attack::START && controls[current].Type == Control::Type::BACK)
                        {
                            done = true;

                            current = -1;

                            selected = false;

                            combat_damage = -1;
                        }
                        else if (stage == Engine::Attack::START && controls[current].Type == Control::Type::NEXT)
                        {
                            done = true;

                            current = -1;

                            selected = false;

                            combat_damage = 100;
                        }
                        else if (stage == Engine::Attack::START && controls[current].Type == Control::Type::PLUS)
                        {
                            if (direction == 0)
                            {
                                if ((attack_score - 5) > 0)
                                {
                                    focus += 1;
                                }
                                else
                                {
                                    displayMessage("You cannot add more focus points!", intRD);
                                }
                            }

                            selected = false;
                        }
                        else if (stage == Engine::Attack::START && controls[current].Type == Control::Type::MINUS)
                        {
                            if (direction == 0)
                            {
                                if (focus > 0)
                                {
                                    focus -= 1;
                                }
                                else
                                {
                                    displayMessage("All focus points have been removed!", intRD);
                                }
                            }

                            selected = false;
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
                        }
                        else if (stage == Engine::Attack::DAMAGE && controls[current].Type == Control::Type::CONFIRM)
                        {
                            if (Engine::COUNT(party, team) > 0)
                            {
                                if (combat_damage > 0)
                                {
                                    auto result = -1;

                                    if (Engine::COUNT(party, team) == 1)
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
                                        result = assignDamage(window, renderer, party, team, combat_damage);
                                    }

                                    if (result >= 0 && result < party.Members.size())
                                    {
                                        if (Engine::IS_ALIVE(party.Members[result]))
                                        {
                                            if (Engine::FIND_LIST(target_damage, result) < 0)
                                            {
                                                assigned = true;

                                                target_damage.push_back(result);

                                                if (Engine::HAS_STATUS(party.Members[result], Character::Status::POTION_OF_INVULNERABILITY))
                                                {
                                                    displayMessage(std::string(party.Members[result].Name) + "'s Invulnerability cancels the damage!", intLB);

                                                    Engine::REMOVE_STATUS(party.Members[result], Character::Status::POTION_OF_INVULNERABILITY);

                                                    combat_damage = 0;
                                                }
                                                else if (Engine::HAS_FOLLOWER(party.Members[result], Follower::Type::MORDAIN_SKELETONS))
                                                {
                                                    message = std::string(party.Members[result].Name) + "'s [SKELETON] steps in the way and takes " + std::to_string(combat_damage) + " damage!";

                                                    auto follower = Engine::FIND_FOLLOWER(party.Members[result], Follower::Type::MORDAIN_SKELETONS);

                                                    if (follower >= 0 && follower < party.Members[result].Followers.size())
                                                    {
                                                        Engine::GAIN_HEALTH(party.Members[result].Followers[follower], -combat_damage);

                                                        if (party.Members[result].Followers[follower].Health <= 0)
                                                        {
                                                            party.Members[result].Followers.erase(party.Members[result].Followers.begin() + follower);

                                                            message += " The [SKELETON] is destroyed!";
                                                        }
                                                    }

                                                    displayMessage(message, intLB);

                                                    combat_damage = 0;
                                                }
                                                else if (Engine::ARMOUR(party.Members[result]) > 0 && monsters[opponent].Type != Monster::Type::PAPER && monsters[opponent].Type != Monster::Type::NAGA)
                                                {
                                                    auto reduced_damage = armourSave(window, renderer, party.Members[result], combat_damage);

                                                    combat_damage = std::max(0, reduced_damage);

                                                    selected = false;

                                                    current = -1;

                                                    if (monsters[opponent].Type == Monster::Type::UNBRAAKI && reduced_damage > 0)
                                                    {
                                                        Engine::GAIN_HEALTH(monsters[opponent], reduced_damage);

                                                        displayMessage("Unbraaki gains " + std::to_string(reduced_damage) + " health!", intRD);
                                                    }
                                                    else
                                                    {
                                                        done = true;
                                                    }
                                                }
                                                else
                                                {
                                                    message = std::string(party.Members[result].Name) + " dealt " + std::to_string(combat_damage) + " damage!";

                                                    Engine::GAIN_HEALTH(party.Members[result], -combat_damage);

                                                    if (monsters[opponent].Type == Monster::Type::UNBRAAKI && combat_damage > 0)
                                                    {
                                                        Engine::GAIN_HEALTH(monsters[opponent], combat_damage);

                                                        message += " " + std::string(monsters[opponent].Name) + " gains " + std::to_string(combat_damage) + " health point";

                                                        if (combat_damage > 1)
                                                        {
                                                            message += "s";
                                                        }

                                                        message += ".";
                                                    }

                                                    displayMessage(message, intRD);
                                                }
                                            }
                                            else
                                            {
                                                displayMessage(std::string(party.Members[result].Name) + " was already assigned damage. Please choose another target.", intRD);
                                            }
                                        }
                                        else
                                        {
                                            displayMessage(std::string(party.Members[result].Name) + " is already dead!", intRD);
                                        }
                                    }
                                }
                                else
                                {
                                    assigned = true;

                                    displayMessage("The " + std::string(monsters[opponent].Name) + "'s attack was ineffective!", intLB);
                                }
                            }
                            else
                            {
                                done = true;

                                selected = false;

                                current = -1;
                            }
                        }
                    }
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

    // if opponent left any loot that can be used, allow party to take it
    if (monsters[opponent].Health <= 0 && monsters[opponent].Loot.size() > 0)
    {
        takeScreen(window, renderer, party, team, monsters[opponent].Loot, monsters[opponent].Loot.size(), false);
    }

    return direction == 0 ? std::max(-1, combat_damage) : std::max(0, combat_damage);
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

            // Lambda functions for displaying flash messages
            auto displayMessage = [&](std::string msg, Uint32 color)
            {
                flash_message = true;

                message = msg;

                flash_color = color;

                start_ticks = SDL_GetTicks();
            };

            auto marginx = (int)(Margin * SCREEN_WIDTH);

            auto fullwidth = SCREEN_WIDTH - 2 * marginx;

            auto boxwidth = (SCREEN_WIDTH - 3 * marginx) / 2;

            auto headerw = (int)(boxwidth * 0.75);

            auto infoh = 48;

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

            std::vector<int> results = {};

            auto size_dice = 64;

            auto cols = (fullwidth - 2 * box_space) / (size_dice + box_space);

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

                                    displayMessage(std::string(party.Army[unit].Name) + " retreats safely to " + std::string(Location::Description[location]) + ".", intLB);

                                    party.Army[unit].Morale = party.Army[unit].MaximumMorale;

                                    party.Army[unit].Garrison = location;
                                }
                                else
                                {
                                    retreats = false;

                                    displayMessage(std::string(party.Army[unit].Name) + " routed.", intRD);
                                }
                            }
                            else
                            {
                                retreats = true;

                                displayMessage(std::string(party.Army[unit].Name) + " retreats safely to " + std::string(Location::Description[location]) + ".", intLB);

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

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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

    if (Engine::IS_ALIVE(character))
    {
        if (window && renderer)
        {
            auto flash_message = false;

            auto flash_color = intRD;

            std::string message = "";

            Uint32 start_ticks = 0;

            Uint32 duration = 3000;

            // Lambda functions for displaying flash messages
            auto displayMessage = [&](std::string msg, Uint32 color)
            {
                flash_message = true;

                message = msg;

                flash_color = color;

                start_ticks = SDL_GetTicks();
            };

            auto marginx = (int)(Margin * SCREEN_WIDTH);

            auto fullwidth = SCREEN_WIDTH - 2 * marginx;

            auto boxwidth = (SCREEN_WIDTH - 3 * marginx) / 2;

            auto headerw = (int)(boxwidth * 0.75);

            auto infoh = 48;

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

            std::vector<int> results = {};

            auto size_dice = 64;

            auto cols = (fullwidth - 2 * box_space) / (size_dice + box_space);

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
                                    increase = score;

                                    Engine::GAIN_SCORE(character, attribute, score);

                                    displayMessage(std::string(character.Name) + "'s " + std::string(Attribute::Descriptions[attribute]) + " increased by " + std::to_string(score) + "!", intLB);
                                }
                                else
                                {
                                    increase = 0;

                                    displayMessage(std::string(character.Name) + "'s " + std::string(Attribute::Descriptions[attribute]) + " score did not increase.", intRD);
                                }
                            }
                            else
                            {
                                increase = score;

                                Engine::GAIN_SCORE(character, attribute, score);

                                displayMessage(std::string(character.Name) + "'s " + std::string(Attribute::Descriptions[attribute]) + " increased by " + std::to_string(score) + "!", intLB);
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

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (stage == Engine::RaiseAttribute::START && controls[current].Type == Control::Type::BACK)
                    {
                        increase = -1;

                        done = true;

                        current = -1;

                        selected = false;
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

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_garamond2 = TTF_OpenFont(FONT_GARAMOND, 28);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto font_size = 24;
        auto text_space = 8;
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;
        auto offset = 0;
        auto booksize = (int)(2 * (text_bounds) / 3 - infoh - box_space);
        auto limit = (int)((booksize - 2 * text_space) / (88));
        auto last = offset + limit;

        if (last > spells.size())
        {
            last = spells.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto scrolly = starty + infoh + booksize - arrow_size - border_space;

        auto controls = spellList(window, renderer, spells, offset, last, limit, textx, texty + infoh + text_space, scrolly, true, (mode == Spells::Select::CAST_SPELL ? true : false));

        auto done = false;

        auto selection = std::vector<int>();

        if (mode == Spells::Select::UNLEARN && !Engine::VERIFY_SPELL_LIMIT(caster))
        {
            displayMessage("Your spellbook is holding too many spells! You must unlearn spells from your spellbook.", intRD);
        }

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
                            thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                        }
                        else if (spells[offset + i].Charged)
                        {
                            drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intRD);
                        }
                    }
                }

                if (current >= 0 && current < controls.size() && controls[current].Type == Control::Type::ACTION)
                {
                    if ((current + offset) >= 0 && (current + offset) < spells.size())
                    {
                        fillRect(renderer, textwidth, text_bounds / 3 - box_space, textx, texty + infoh + booksize + box_space, intLB);

                        auto text = createText(spells[current + offset].Description, FONT_GARAMOND, font_size, clrWH, textwidth - 2 * text_space, TTF_STYLE_NORMAL);

                        renderText(renderer, text, intLB, textx + text_space, texty + infoh + booksize + box_space + text_space, text_bounds / 3 - texty, 0);

                        SDL_FreeSurface(text);

                        text = NULL;
                    }
                }

                renderButtons(renderer, controls, current, intLB, space, border_pts);

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
                        putText(renderer, "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                    }
                }
                else
                {
                    putText(renderer, "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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
                    else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
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

                            displayMessage(message, intRD);
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
                                    displayMessage(std::string(spells[current + offset].Name) + " cannot be cast!", intRD);
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

int selectOpponent(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, int attacker, std::vector<Monster::Base> &monsters, std::vector<int> previousTargets, int combatRound, Control::Type mode)
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

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto text_space = 8;
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;
        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space - infoh) / (88);
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
                            thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                        }
                        else if (monsters[offset + i].Health > 0)
                        {
                            if (combatRound >= monsters[offset + i].Round)
                            {
                                drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                            }
                            else
                            {
                                thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intGR, 2);
                            }
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intRD);
                        }
                    }
                }

                renderButtons(renderer, controls, current, intLB, space, border_pts);

                putHeader(renderer, "Select Opponent", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                if (mode == Control::Type::COMBAT)
                {
                    putHeader(renderer, "Attacker", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + 2 * infoh + box_space));
                }
                else if (mode == Control::Type::SPELL)
                {
                    putHeader(renderer, "Spell Caster", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + 2 * infoh + box_space));
                }

                putText(renderer, party.Members[attacker].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - 3 * boxh - infoh - box_space);

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

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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
                    else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
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
                                    displayMessage("That opponent has been attacked before! Choose another target!", intRD);
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
                            displayMessage("You must select an opponent to attack this round.", intRD);
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
                                    displayMessage(std::string(monsters[current + offset].Name) + " has not joined the battle yet!", intRD);
                                }
                                else
                                {
                                    displayMessage(std::string(monsters[current + offset].Name) + " is dead!", intRD);
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

int selectOpponent(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &enemyFleet, std::vector<int> previousTargets, int combatRound)
{
    auto result = -1;

    auto title = "Legendary Kingdoms: Select Enemy Ship";

    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto text_space = 8;
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space - infoh) / (88);
        auto last = offset + limit;

        if (last > enemyFleet.size())
        {
            last = enemyFleet.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = shipList(window, renderer, enemyFleet, offset, last, limit, textx, texty + infoh + text_space, true, false);

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
                            thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                        }
                        else if (enemyFleet[offset + i].Health > 0)
                        {
                            drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intRD);
                        }
                    }
                }

                renderButtons(renderer, controls, current, intLB, space, border_pts);

                putHeader(renderer, "Select Opponent", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                putHeader(renderer, "Opponent", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (selection >= 0 && selection < enemyFleet.size())
                {
                    if (enemyFleet[selection].Health > 0)
                    {
                        putText(renderer, enemyFleet[selection].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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

                            if (last > enemyFleet.size())
                            {
                                last = enemyFleet.size();
                            }

                            controls.clear();

                            controls = shipList(window, renderer, enemyFleet, offset, last, limit, textx, texty + infoh + text_space, true, false);

                            SDL_Delay(50);
                        }

                        if (offset <= 0)
                        {
                            current = -1;

                            selected = false;
                        }
                    }
                    else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
                    {
                        if (enemyFleet.size() - last > 0)
                        {
                            if (offset < enemyFleet.size() - limit)
                            {
                                offset += scrollSpeed;
                            }

                            if (offset > enemyFleet.size() - limit)
                            {
                                offset = enemyFleet.size() - limit;
                            }

                            last = offset + limit;

                            if (last > enemyFleet.size())
                            {
                                last = enemyFleet.size();
                            }

                            controls.clear();

                            controls = shipList(window, renderer, enemyFleet, offset, last, limit, textx, texty + infoh + text_space, true, false);

                            SDL_Delay(50);

                            if (offset > 0)
                            {
                                if (controls[current].Type != Control::Type::SCROLL_DOWN)
                                {
                                    current++;
                                }
                            }
                        }

                        if (enemyFleet.size() - last <= 0)
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
                        if (selection >= 0 && selection < enemyFleet.size())
                        {
                            if (previousTargets.size() > 0)
                            {
                                if (Engine::FIND_LIST(previousTargets, selection) >= 0)
                                {
                                    displayMessage("That opponent has been attacked before! Choose another target!", intRD);
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
                            displayMessage("You must select an opponent to attack this round.", intRD);
                        }
                    }
                    else if (controls[current].Type == Control::Type::ACTION)
                    {
                        if (current + offset >= 0 && current + offset < enemyFleet.size())
                        {
                            if (selection == current + offset)
                            {
                                selection = -1;
                            }
                            else
                            {
                                if (enemyFleet[current + offset].Health > 0)
                                {
                                    selection = current + offset;
                                }
                                else
                                {
                                    displayMessage(std::string(enemyFleet[current + offset].Name) + " is destroyed!", intRD);
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

bool skillTestScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team_type, std::vector<int> team, Attribute::Type Skill, int difficulty, int success, bool useEquipment)
{
    if (Skill == Attribute::Type::CHARISMA && Engine::VERIFY_CODES(party, {Codes::Type::CHARISMA_SUCCESS_CHALICE}) && party.Location == Location::Type::CHALICE && party.InCity == true)
    {
        return true;
    }

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

            // Lambda functions for displaying flash messages
            auto displayMessage = [&](std::string msg, Uint32 color)
            {
                flash_message = true;

                message = msg;

                flash_color = color;

                start_ticks = SDL_GetTicks();
            };

            auto marginx = (int)(Margin * SCREEN_WIDTH);

            auto fullwidth = SCREEN_WIDTH - 2 * marginx;

            auto boxwidth = (SCREEN_WIDTH - 3 * marginx) / 2;

            auto headerw = (int)(boxwidth * 0.75);

            auto infoh = 48;

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
            const char *choices_wolfspirit[2] = {"Wolf Spirit", "Done"};
            const char *choices_wisdom[2] = {"Wisdom", "Done"};
            const char *choices_silvertongue[2] = {"Silver Tongue", "Done"};
            const char *choices_charisma_potion[2] = {"Potion of Fluency", "Done"};
            const char *choices_silvertongue_potion[3] = {"Silver Tongue", "Potion of Fluency", "Done"};
            const char *choices_end[1] = {"Done"};

            SDL_Surface *dice[6];

            dice[0] = createImage("images/dice/dice1.png");
            dice[1] = createImage("images/dice/dice2.png");
            dice[2] = createImage("images/dice/dice3.png");
            dice[3] = createImage("images/dice/dice4.png");
            dice[4] = createImage("images/dice/dice5.png");
            dice[5] = createImage("images/dice/dice6.png");

            auto main_buttonw = 240;

            auto controls_skill = createFixedTextButtons(choices_skill, 3, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_skill[0].Type = Control::Type::CONFIRM;
            controls_skill[1].Type = Control::Type::PLUS;
            controls_skill[2].Type = Control::Type::MINUS;

            auto controls_confirm = createFixedTextButtons(choices_confirm, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_confirm[0].Type = Control::Type::CONFIRM;

            auto controls_end = createFixedTextButtons(choices_end, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_end[0].Type = Control::Type::BACK;

            auto controls_wolfspirit = createFixedTextButtons(choices_wolfspirit, 2, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_wolfspirit[0].Type = Control::Type::CONFIRM;
            controls_wolfspirit[1].Type = Control::Type::BACK;

            auto controls_wisdom = createFixedTextButtons(choices_wisdom, 2, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_wisdom[0].Type = Control::Type::CONFIRM;
            controls_wisdom[1].Type = Control::Type::BACK;

            auto controls_silvertongue = createFixedTextButtons(choices_silvertongue, 2, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_silvertongue[0].Type = Control::Type::CONFIRM;
            controls_silvertongue[1].Type = Control::Type::BACK;

            auto controls_charisma_potion = createFixedTextButtons(choices_charisma_potion, 2, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_charisma_potion[0].Type = Control::Type::DRINK;
            controls_charisma_potion[1].Type = Control::Type::BACK;

            auto controls_silvertongue_potion = createFixedTextButtons(choices_silvertongue_potion, 3, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
            controls_silvertongue_potion[0].Type = Control::Type::CONFIRM;
            controls_silvertongue_potion[1].Type = Control::Type::DRINK;
            controls_silvertongue_potion[2].Type = Control::Type::BACK;

            auto current = -1;

            auto selected = false;

            auto scrollUp = false;

            auto scrollDown = false;

            auto hold = false;

            auto focus = 0;

            std::vector<int> results = {};

            auto skill_score = 0;

            auto score1 = 0;

            auto score2 = 0;

            auto computed_score1 = false;

            auto computed_score2 = false;

            auto size_dice = 64;

            auto cols = (fullwidth - 2 * box_space) / (size_dice + box_space);

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

                            if (stage == Attribute::Test::CHECK || stage == Attribute::Test::MAGIC)
                            {
                                if (results[i] >= (difficulty - focus))
                                {
                                    thickRect(renderer, size_dice, size_dice, offsetx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), intLB, 2);

                                    if (stage == Attribute::Test::CHECK)
                                    {
                                        success_counter++;
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

                    if (stage == Attribute::Test::CHECK)
                    {
                        if (!skill_checked)
                        {
                            party.RecentSuccesses = success_counter;

                            if (success_counter >= success)
                            {
                                displayMessage("Skill Check PASSED!", intLB);

                                test_result = true;
                            }
                            else
                            {
                                displayMessage("Skill Check FAILED!", intRD);

                                test_result = false;
                            }

                            skill_checked = true;
                        }
                    }
                }

                std::string test_string = std::string(Attribute::Descriptions[Skill]) + ": " + std::to_string(difficulty - focus) + "+, Success: " + (success > 0 ? std::to_string(success) : std::string("Special"));

                test_string += ", Focus: " + std::to_string(focus);

                putHeader(renderer, test_string.c_str(), font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, fullwidth, infoh, startx, starty + 2 * infoh + 4 * boxh + 2 * box_space);

                putHeader(renderer, party.Members[team[0]].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);

                fillRect(renderer, boxwidth, boxh, startx, starty + infoh, intBE);

                if (!computed_score1)
                {
                    if (useEquipment)
                    {
                        if (Skill == Attribute::Type::FIGHTING)
                        {
                            score1 = Engine::SCORE(party.Members[team[0]], Attribute::Type::FIGHTING);

                            if (Engine::HAS_WEAPON(party.Members[team[0]]))
                            {
                                score1 += Engine::MAX_WEAPON(party.Members[team[0]]);
                            }

                            score1 -= (focus * 5);
                        }
                        else
                        {
                            score1 = Engine::SCORE(party.Members[team[0]], Skill) - (focus * 5);
                        }
                    }
                    else
                    {
                        if (Skill == Attribute::Type::FIGHTING)
                        {
                            score1 = Engine::SCORE(party.Members[team[0]], Attribute::Type::FIGHTING) - (focus * 5);
                        }
                        else
                        {
                            score1 = Engine::RAW_SCORE(party.Members[team[0]], Skill, true) - (focus * 5);
                        }
                    }

                    if (score1 < 0)
                    {
                        score1 = 0;
                    }

                    computed_score1 = true;

                    skill_score = score1;
                }

                std::string adventurer1 = std::string(Attribute::Descriptions[Skill]) + ": " + std::to_string(score1);

                putText(renderer, adventurer1.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx, starty + infoh);

                if (team.size() > 1)
                {
                    putHeader(renderer, party.Members[team[1]].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx + boxwidth + marginx, starty);

                    fillRect(renderer, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh, intBE);

                    if (!computed_score2)
                    {
                        if (useEquipment)
                        {
                            if (Skill == Attribute::Type::FIGHTING)
                            {
                                score2 = Engine::SCORE(party.Members[team[0]], Attribute::Type::FIGHTING);

                                if (Engine::HAS_WEAPON(party.Members[team[1]]))
                                {
                                    score2 += Engine::MAX_WEAPON(party.Members[team[1]]);
                                }

                                score2 -= (focus * 5);
                            }
                            else
                            {
                                score2 = Engine::SCORE(party.Members[team[1]], Skill) - (focus * 5);
                            }
                        }
                        else
                        {
                            if (Skill == Attribute::Type::FIGHTING)
                            {
                                score2 = Engine::SCORE(party.Members[team[1]], Attribute::Type::FIGHTING) - (focus * 5);
                            }
                            else
                            {
                                score2 = Engine::RAW_SCORE(party.Members[team[1]], Skill, true) - (focus * 5);
                            }
                        }

                        if (score2 < 0)
                        {
                            score2 = 0;
                        }

                        computed_score2 = true;

                        skill_score += score2;
                    }

                    std::string adventurer2 = std::string(Attribute::Descriptions[Skill]) + ": " + std::to_string(score2);

                    putText(renderer, adventurer2.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh);
                }

                if (skill_score > 20)
                {
                    skill_score = 20;
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
                    if (test_result)
                    {
                        controls = controls_end;
                    }
                    else
                    {
                        if (Skill == Attribute::Type::SURVIVAL && Engine::CAN_CAST(party, team_type, Spells::Type::WOLF_SPIRIT) && ((success_counter + 3) >= success))
                        {
                            stage = Attribute::Test::MAGIC;

                            controls = controls_wolfspirit;
                        }
                        else if (Skill == Attribute::Type::LORE && Engine::CAN_CAST(party, team_type, Spells::Type::WISDOM) && ((success_counter + 3) >= success))
                        {
                            stage = Attribute::Test::MAGIC;

                            controls = controls_wisdom;
                        }
                        else if (Skill == Attribute::Type::CHARISMA && (Engine::CAN_CAST(party, team_type, Spells::Type::SILVER_TONGUE) || Engine::VERIFY_EQUIPMENT(party, team_type, {Equipment::Type::POTION_OF_CHARISMA})) && ((success_counter + 3) >= success))
                        {
                            auto has_spells = Engine::CAN_CAST(party, team_type, Spells::Type::SILVER_TONGUE);
                            auto has_potion = Engine::VERIFY_EQUIPMENT(party, team_type, {Equipment::Type::POTION_OF_CHARISMA});

                            stage = Attribute::Test::MAGIC;

                            if (has_spells && has_potion)
                            {
                                controls = controls_silvertongue_potion;
                            }
                            else if (has_spells)
                            {
                                controls = controls_silvertongue;
                            }
                            else if (has_potion)
                            {
                                controls = controls_charisma_potion;
                            }
                            else
                            {
                                controls = controls_end;
                            }
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

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (stage == Attribute::Test::START && controls[current].Type == Control::Type::CONFIRM)
                    {
                        stage = Attribute::Test::CONFIRM;
                    }
                    else if (stage == Attribute::Test::START && controls[current].Type == Control::Type::PLUS)
                    {
                        if (team.size() > 1)
                        {
                            if ((score1 - 5) > 0 && (score2 - 5) > 0)
                            {
                                computed_score1 = false;

                                computed_score2 = false;

                                focus += 1;
                            }
                            else
                            {
                                displayMessage("You cannot add more focus points!", intRD);
                            }
                        }
                        else
                        {
                            if ((score1 - 5) > 0)
                            {
                                computed_score1 = false;

                                focus += 1;
                            }
                            else
                            {
                                displayMessage("You cannot add more focus points!", intRD);
                            }
                        }

                        selected = false;
                    }
                    else if (stage == Attribute::Test::START && controls[current].Type == Control::Type::MINUS)
                    {
                        if (focus > 0)
                        {
                            focus -= 1;

                            computed_score1 = false;

                            computed_score2 = false;
                        }
                        else
                        {
                            displayMessage("All focus points have been removed!", intRD);
                        }

                        selected = false;
                    }
                    else if (stage == Attribute::Test::CONFIRM && controls[current].Type == Control::Type::CONFIRM)
                    {
                        stage = Attribute::Test::CHECK;
                    }
                    else if (stage == Attribute::Test::MAGIC && controls[current].Type == Control::Type::CONFIRM)
                    {
                        stage = Attribute::Test::END;

                        if (Skill == Attribute::Type::SURVIVAL)
                        {
                            Engine::CAST_SPELL(party, team_type, Spells::Type::WOLF_SPIRIT);

                            party.RecentSuccesses += 3;

                            test_result = true;
                        }
                        else if (Skill == Attribute::Type::LORE)
                        {
                            Engine::CAST_SPELL(party, team_type, Spells::Type::WISDOM);

                            party.RecentSuccesses += 3;

                            test_result = true;
                        }
                        else if (Skill == Attribute::Type::CHARISMA)
                        {
                            Engine::CAST_SPELL(party, team_type, Spells::Type::SILVER_TONGUE);

                            party.RecentSuccesses += 3;

                            test_result = true;
                        }

                        done = true;

                        current = -1;

                        selected = false;
                    }
                    else if (stage == Attribute::Test::MAGIC && controls[current].Type == Control::Type::DRINK)
                    {
                        stage = Attribute::Test::END;

                        if (Skill == Attribute::Type::CHARISMA)
                        {
                            Engine::LOSE_EQUIPMENT(party, team_type, {Equipment::Type::POTION_OF_CHARISMA});

                            party.RecentSuccesses += 3;

                            test_result = true;
                        }

                        done = true;

                        current = -1;

                        selected = false;
                    }
                    else if (stage == Attribute::Test::MAGIC && controls[current].Type == Control::Type::BACK)
                    {
                        stage = Attribute::Test::END;

                        done = true;

                        current = -1;

                        selected = false;
                    }
                    else if (stage == Attribute::Test::CHECK && controls[current].Type == Control::Type::BACK)
                    {
                        stage = Attribute::Test::END;

                        done = true;

                        current = -1;

                        selected = false;
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

            if (team.size() == 1)
            {
                party.LastSelected = team[0];
            }

            party.Order.clear();

            for (auto i = 0; i < team.size(); i++)
            {
                party.Order.push_back(party.Members[team[i]].Type);
            }
        }
    }

    return test_result;
}

bool magicRound0(Character::Base &character, int combatRound)
{
    auto result = true;

    result &= (!Engine::HAS_STATUS(character, Character::Status::EXTRA_MAGIC_ROUND0) || (Engine::HAS_STATUS(character, Character::Status::EXTRA_MAGIC_ROUND0) && combatRound != 0));
    result &= (!Engine::HAS_STATUS(character, Character::Status::UNLIMITED_MAGIC_ROUND0) || (Engine::HAS_STATUS(character, Character::Status::UNLIMITED_MAGIC_ROUND0) && combatRound != 0));

    return result;
}

int castCombatSpell(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Monster::Base> &monsters, std::vector<int> hasAttacked, int combatRound)
{
    auto result = -1;

    auto title = "Legendary Kingdoms: Cast Spell";

    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto text_space = 8;
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space - infoh) / (88);
        auto last = offset + limit;

        if (last > party.Members.size())
        {
            last = party.Members.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = combatantList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh + text_space, true, true);

        auto done = false;

        auto selection = -1;

        while (!done)
        {
            auto current = -1;

            auto selected = false;

            auto scrollUp = false;

            auto scrollDown = false;

            auto hold = false;

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
                            thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                        }
                        else if (Engine::IS_ALIVE(party.Members[offset + i]))
                        {
                            drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intRD);
                        }
                    }
                }

                renderButtons(renderer, controls, current, intLB, space, border_pts);

                putHeader(renderer, "Select Caster", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                putHeader(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (selection >= 0 && selection < party.Members.size())
                {
                    putText(renderer, party.Members[selection].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                }
                else
                {
                    putText(renderer, "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (controls[current].Type == Control::Type::BACK)
                    {
                        done = true;

                        current = -1;

                        selected = false;

                        result = -1;
                    }
                    else if (controls[current].Type == Control::Type::CONFIRM)
                    {
                        if (selection >= 0 && selection < party.Members.size())
                        {
                            if (party.Members[selection].SpellCaster && Engine::CAN_SPEAK(party.Members[selection]) && !Engine::IS_CAPTURED(party.Members[selection]) && !Engine::IS_CURSED(party.Members[selection]))
                            {
                                if (hasAttacked.size() > 0 && Engine::FIND_LIST(hasAttacked, selection) >= 0 && magicRound0(party.Members[selection], combatRound))
                                {
                                    displayMessage(std::string(party.Members[selection].Name) + " already attacked this turn!", intRD);
                                }
                                else
                                {
                                    auto spell = selectSpell(window, renderer, party.Members[selection], party.Members[selection].SpellBook, 1, Spells::Select::CAST_SPELL);

                                    if (spell.size() > 0)
                                    {
                                        auto i = spell[0];

                                        if (party.Members[selection].SpellBook[i].Scope == Spells::Scope::COMBAT || party.Members[selection].SpellBook[i].Scope == Spells::Scope::ADVENTURE_COMBAT)
                                        {
                                            auto cast = false;

                                            if (party.Members[selection].SpellBook[i].Type == Spells::Type::ARMOUR_OF_HEAVEN)
                                            {
                                                auto target = -1;

                                                if (Engine::COUNT(party, team) == 1)
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

                                                if (Engine::IS_ACTIVE(party, target))
                                                {
                                                    if (Engine::HAS_STATUS(party.Members[target], Character::Status::ARMOUR3))
                                                    {
                                                        displayMessage(std::string(party.Members[target].Name) + " already has the " + std::string(party.Members[selection].SpellBook[i].Name) + "!", intRD);
                                                    }
                                                    else
                                                    {
                                                        Engine::GAIN_STATUS(party.Members[target], Character::Status::ARMOUR3);

                                                        cast = true;
                                                    }
                                                }
                                                else
                                                {
                                                    displayMessage(std::string(party.Members[target].Name) + " is an invalid target!", intRD);
                                                }
                                            }
                                            else if (party.Members[selection].SpellBook[i].Type == Spells::Type::SOOTHING_TOUCH)
                                            {
                                                auto target = -1;

                                                if (Engine::COUNT(party, team) == 1)
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

                                                if (target >= 0 && target < party.Members.size())
                                                {
                                                    if (party.Members[target].Health <= 0 || party.Members[target].Health == party.Members[target].MaximumHealth)
                                                    {
                                                        message = std::string(party.Members[target].Name);

                                                        if (party.Members[target].Health <= 0)
                                                        {
                                                            message += " is dead!";
                                                        }
                                                        else
                                                        {
                                                            message += " is not injured!";
                                                        }

                                                        displayMessage(message, intRD);
                                                    }
                                                    else
                                                    {
                                                        if (Engine::IS_ACTIVE(party, target))
                                                        {
                                                            Engine::GAIN_HEALTH(party.Members[target], 5);

                                                            cast = true;
                                                        }
                                                        else
                                                        {
                                                            displayMessage(std::string(party.Members[target].Name) + " is an invalid target!", intRD);
                                                        }
                                                    }
                                                }
                                            }
                                            else if (party.Members[selection].SpellBook[i].Type == Spells::Type::DAZING_LIGHTS)
                                            {
                                                if (Engine::VERIFY_CODES(party, {Codes::Type::DAZING_LIGHTS}))
                                                {
                                                    displayMessage("Dazing Lights has already been cast!", intRD);
                                                }
                                                else
                                                {
                                                    Engine::GET_CODES(party, {Codes::Type::DAZING_LIGHTS});

                                                    cast = true;
                                                }
                                            }
                                            else if (party.Members[selection].SpellBook[i].Type == Spells::Type::ICE_BOLT)
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
                                                        target = selectOpponent(window, renderer, party, selection, monsters, {}, combatRound, Control::Type::SPELL);
                                                    }

                                                    if (target >= 0)
                                                    {
                                                        auto damage = magicAttackScreen(window, renderer, party, monsters, party.Members[selection].SpellBook[i], selection, target, 8);

                                                        if (damage >= 0)
                                                        {
                                                            cast = true;
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    displayMessage("There are no targets for " + std::string(party.Members[selection].SpellBook[i].Name) + "!", intRD);
                                                }
                                            }
                                            else if (party.Members[selection].SpellBook[i].Type == Spells::Type::UNFAILING_STRIKE)
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
                                                        target = selectOpponent(window, renderer, party, selection, monsters, {}, combatRound, Control::Type::SPELL);
                                                    }

                                                    if (target >= 0)
                                                    {
                                                        if (monsters[target].Type == Monster::Type::FASTILON && !monsters[target].Damaged)
                                                        {
                                                            Engine::GAIN_STATUS(party.Members[selection], Character::Status::STUNNED_NEXT_ROUND);
                                                        }

                                                        Engine::GAIN_HEALTH(monsters[target], -3);

                                                        cast = true;
                                                    }
                                                }
                                                else
                                                {
                                                    displayMessage("There are no targets for " + std::string(party.Members[selection].SpellBook[i].Name) + "!", intRD);
                                                }
                                            }
                                            else if (party.Members[selection].SpellBook[i].Type == Spells::Type::POISON_STREAM)
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
                                                            target = selectOpponent(window, renderer, party, selection, monsters, targets, combatRound, Control::Type::SPELL);
                                                        }

                                                        if (target >= 0)
                                                        {
                                                            auto damage = magicAttackScreen(window, renderer, party, monsters, party.Members[selection].SpellBook[i], selection, target, 5);

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
                                                    displayMessage("There are no targets for " + std::string(party.Members[selection].SpellBook[i].Name) + "!", intRD);
                                                }
                                            }
                                            else if (party.Members[selection].SpellBook[i].Type == Spells::Type::SANDSTORM)
                                            {
                                                if (Engine::COUNT(monsters, combatRound) > 0)
                                                {
                                                    auto max_targets = Engine::COUNT(monsters, combatRound);

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
                                                            target = selectOpponent(window, renderer, party, selection, monsters, targets, combatRound, Control::Type::SPELL);
                                                        }

                                                        if (target >= 0)
                                                        {
                                                            auto damage = magicAttackScreen(window, renderer, party, monsters, party.Members[selection].SpellBook[i], selection, target, 3);

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
                                                    displayMessage("There are no targets for " + std::string(party.Members[selection].SpellBook[i].Name) + "!", intRD);
                                                }
                                            }

                                            if (cast)
                                            {
                                                if (party.Location == Location::Type::SALTDAD_ARENA)
                                                {
                                                    Engine::GAIN_STATUS(party.Members[selection], Character::Status::USED_MAGIC_INARENA);
                                                }

                                                party.Members[selection].SpellBook[i].Charged = false;

                                                result = selection;

                                                done = true;
                                            }
                                        }
                                        else
                                        {
                                            displayMessage(std::string(party.Members[selection].Name) + " cannot cast " + std::string(party.Members[selection].SpellBook[spell[0]].Name) + " during combat!", intRD);
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
                                displayMessage(std::string(party.Members[selection].Name) + " cannot cast spells!", intRD);
                            }
                        }
                        else
                        {
                            displayMessage("You must select the adventurer to cast a spell.", intRD);
                        }
                    }
                    else if (controls[current].Type == Control::Type::ACTION)
                    {
                        if (current + offset >= 0 && current + offset < party.Members.size())
                        {
                            if (selection == current + offset)
                            {
                                selection = -1;
                            }
                            else
                            {
                                if (Engine::IS_ALIVE(party.Members[current + offset]))
                                {
                                    if (team == Team::Type::NONE || party.Members[current + offset].Team == team)
                                    {
                                        selection = current + offset;
                                    }
                                    else
                                    {
                                        if (!Engine::IS_CHARACTER(team))
                                        {
                                            message = std::string(party.Members[current + offset].Name) + " is not part of the " + std::string(Team::Descriptions[team]) + " team!";
                                        }
                                        else
                                        {
                                            message = "You can only choose " + std::string(Team::Descriptions[team]) + "!";
                                        }

                                        displayMessage(message, intRD);
                                    }
                                }
                                else
                                {
                                    displayMessage(std::string(party.Members[current + offset].Name) + " is dead!", intRD);
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

int castMassCombatSpell(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Army::Base> &enemyArmy, Location::Type battlefield, int combatRound)
{
    auto result = -1;

    auto title = "Legendary Kingdoms: Cast Spell";

    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto text_space = 8;
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space - infoh) / (88);
        auto last = offset + limit;

        if (last > party.Members.size())
        {
            last = party.Members.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = combatantList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh + text_space, true, true);

        auto done = false;

        auto selection = -1;

        while (!done)
        {
            auto current = -1;

            auto selected = false;

            auto scrollUp = false;

            auto scrollDown = false;

            auto hold = false;

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
                            thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                        }
                        else if (Engine::IS_ALIVE(party.Members[offset + i]))
                        {
                            drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intRD);
                        }
                    }
                }

                renderButtons(renderer, controls, current, intLB, space, border_pts);

                putHeader(renderer, "Select Caster", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                putHeader(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (selection >= 0 && selection < party.Members.size())
                {
                    putText(renderer, party.Members[selection].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                }
                else
                {
                    putText(renderer, "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (controls[current].Type == Control::Type::BACK)
                    {
                        done = true;

                        current = -1;

                        selected = false;

                        result = -1;
                    }
                    else if (controls[current].Type == Control::Type::CONFIRM)
                    {
                        if (selection >= 0 && selection < party.Members.size())
                        {
                            if (party.Members[selection].SpellCaster && Engine::CAN_SPEAK(party.Members[selection]) && !Engine::IS_CAPTURED(party.Members[selection]))
                            {
                                auto spell = selectSpell(window, renderer, party.Members[selection], party.Members[selection].SpellBook, 1, Spells::Select::CAST_SPELL);

                                if (spell.size() > 0)
                                {
                                    auto i = spell[0];

                                    if (party.Members[selection].SpellBook[i].Scope == Spells::Scope::MASS_COMBAT)
                                    {
                                        auto cast = false;

                                        if (party.Members[selection].SpellBook[i].Type == Spells::Type::CLINGING_DREAD)
                                        {
                                            if (Engine::COUNT(enemyArmy) > 0)
                                            {
                                                auto target = selectArmyUnits(window, renderer, enemyArmy, Location::Type::NONE, 1);

                                                if (target.size() > 0)
                                                {
                                                    if (target[0] >= 0 && target[0] < enemyArmy.size())
                                                    {
                                                        Engine::GAIN_MORALE(enemyArmy[target[0]], -1);

                                                        Engine::UPDATE_ARMY(enemyArmy, Location::Type::NONE);

                                                        cast = true;
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                displayMessage("There are no targets for " + std::string(party.Members[selection].SpellBook[i].Name) + "!", intRD);
                                            }
                                        }

                                        if (cast)
                                        {
                                            party.Members[selection].SpellBook[i].Charged = false;

                                            result = selection;

                                            done = true;
                                        }
                                    }
                                    else
                                    {
                                        displayMessage(std::string(party.Members[selection].Name) + " cannot cast " + std::string(party.Members[selection].SpellBook[spell[0]].Name) + " during combat!", intRD);
                                    }
                                }
                                else
                                {
                                    selected = false;

                                    current = -1;
                                }
                            }
                            else
                            {
                                displayMessage(std::string(party.Members[selection].Name) + " cannot cast spells!", intRD);
                            }
                        }
                        else
                        {
                            displayMessage("You must select the adventurer to cast a spell.", intRD);
                        }
                    }
                    else if (controls[current].Type == Control::Type::ACTION)
                    {
                        if (current + offset >= 0 && current + offset < party.Members.size())
                        {
                            if (Engine::IS_ACTIVE(party, current + offset))
                            {
                                if (Engine::CAN_SPEAK(party.Members[current + offset]))
                                {
                                    selection = current + offset;
                                }
                                else
                                {
                                    displayMessage(std::string(party.Members[current + offset].Name) + " cannot cast spells", intRD);
                                }
                            }
                            else if (Engine::IS_ALIVE(party.Members[current + offset]))
                            {
                                displayMessage(std::string(party.Members[current + offset].Name) + " has been captured!", intRD);
                            }
                            else
                            {
                                displayMessage(std::string(party.Members[current + offset].Name) + " is dead!", intRD);
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

bool skillCheck(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, int team_size, Attribute::Type skill, int difficulty, int success, std::vector<int> &selection, bool useEquipment)
{
    if (skill == Attribute::Type::CHARISMA && Engine::VERIFY_CODES(party, {Codes::Type::CHARISMA_SUCCESS_CHALICE}) && party.Location == Location::Type::CHALICE && party.InCity == true)
    {
        return true;
    }

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

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto text_space = 8;
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space - infoh) / (88);
        auto last = offset + limit;

        if (last > party.Members.size())
        {
            last = party.Members.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = combatantList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh + text_space, true, true);

        auto done = false;

        selection = std::vector<int>();

        while (!done)
        {
            auto current = -1;

            auto selected = false;

            auto scrollUp = false;

            auto scrollDown = false;

            auto hold = false;

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

                    if (index >= 0 && index < party.Members.size())
                    {
                        if (Engine::FIND_LIST(selection, index) >= 0)
                        {
                            thickRect(renderer, controls[index].W + border_pts, controls[index].H + border_pts, controls[index].X - 2, controls[index].Y - 2, intLB, 2);
                        }
                        else if (Engine::IS_ALIVE(party.Members[index]))
                        {
                            if (team != Team::Type::NONE)
                            {
                                if (party.Members[index].Team == team)
                                {
                                    drawRect(renderer, controls[index].W + border_space, controls[index].H + border_space, controls[index].X - 4, controls[index].Y - 4, intBK);
                                }
                                else
                                {
                                    thickRect(renderer, controls[index].W + border_pts, controls[index].H + border_pts, controls[index].X - 2, controls[index].Y - 2, intGR, 2);
                                }
                            }
                            else
                            {
                                drawRect(renderer, controls[index].W + border_space, controls[index].H + border_space, controls[index].X - 4, controls[index].Y - 4, intBK);
                            }
                        }
                        else
                        {
                            drawRect(renderer, controls[index].W + border_space, controls[index].H + border_space, controls[index].X - 4, controls[index].Y - 4, intRD);
                        }
                    }
                }

                renderButtons(renderer, controls, current, intLB, space, border_pts);

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
                        if (selection[i] >= 0 && selection[i] < party.Members.size())
                        {
                            if (Engine::IS_ALIVE(party.Members[selection[i]]))
                            {
                                if (adventurers.length() > 0)
                                {
                                    adventurers += "\n";
                                }

                                adventurers += party.Members[selection[i]].Name;
                            }
                        }
                    }

                    if (adventurers.length() > 0)
                    {
                        putText(renderer, adventurers.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                    }
                    else
                    {
                        putText(renderer, "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                    }
                }
                else
                {
                    putText(renderer, "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (controls[current].Type == Control::Type::BACK)
                    {
                        done = true;

                        current = -1;

                        selected = false;

                        selection = {};
                    }
                    else if (controls[current].Type == Control::Type::CONFIRM)
                    {
                        if ((selection.size() >= team_size && selection.size() <= party.Members.size()) || (selection.size() == Engine::COUNT(party, team) && selection.size() <= team_size))
                        {
                            done = true;

                            result = skillTestScreen(window, renderer, party, team, selection, skill, difficulty, success, useEquipment);

                            current = -1;

                            selected = false;
                        }
                        else
                        {
                            displayMessage("You must select the adventurer(s) who will perform the skill check.", intRD);
                        }
                    }
                    else if (controls[current].Type == Control::Type::ACTION)
                    {
                        if (current + offset >= 0 && current + offset < party.Members.size())
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
                                    if (Engine::IS_ALIVE(party.Members[current + offset]))
                                    {
                                        if (team == Team::Type::NONE)
                                        {
                                            selection.push_back(current + offset);
                                        }
                                        else if (party.Members[current + offset].Team == team)
                                        {
                                            selection.push_back(current + offset);
                                        }
                                        else
                                        {
                                            displayMessage(std::string(party.Members[current + offset].Name) + " is not part of the " + std::string(Team::Descriptions[team]) + " team!", intRD);
                                        }
                                    }
                                    else
                                    {
                                        displayMessage(std::string(party.Members[current + offset].Name) + " is dead!", intRD);
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

        auto text_space = 8;
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space - infoh) / (88);
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
                            thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                        }
                    }
                }

                renderButtons(renderer, controls, current, intLB, space, border_pts);

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

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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
                    else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
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

                            message = "You must select an ATTRIBUTE!";

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

        auto text_space = 8;
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space - infoh) / (88);

        auto teams_list = std::vector<Team::Type>();

        for (auto i = 0; i < teams.size(); i++)
        {
            teams_list.push_back(std::get<0>(teams[i]));
        }

        teams_list.push_back(Team::Type::NONE);

        auto last = offset + limit;

        if (last > teams_list.size())
        {
            last = teams_list.size();
        }

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
                            thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
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
                    putText(renderer, "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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
                    else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
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

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto text_space = 8;
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space - infoh) / (88);
        auto last = offset + limit;

        if (last > party.Members.size())
        {
            last = party.Members.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = combatantList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh + text_space, true, false);

        auto done = false;

        auto selection = -1;

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
                    drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
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

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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

                        if (last > party.Members.size())
                        {
                            last = party.Members.size();
                        }

                        controls.clear();

                        controls = combatantList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh + text_space, true, false);

                        SDL_Delay(50);
                    }

                    if (offset <= 0)
                    {
                        current = -1;

                        selected = false;
                    }
                }
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
                {
                    if (party.Members.size() - last > 0)
                    {
                        if (offset < party.Members.size() - limit)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > party.Members.size() - limit)
                        {
                            offset = party.Members.size() - limit;
                        }

                        last = offset + limit;

                        if (last > party.Members.size())
                        {
                            last = party.Members.size();
                        }

                        controls.clear();

                        controls = combatantList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh + text_space, true, false);

                        SDL_Delay(50);

                        if (offset > 0)
                        {
                            if (controls[current].Type != Control::Type::SCROLL_DOWN)
                            {
                                current++;
                            }
                        }
                    }

                    if (party.Members.size() - last <= 0)
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
                        auto count = Engine::COUNT(party, std::get<0>(teams[i]));

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
                        displayMessage("Please complete team selection", intRD);

                        done = false;
                    }
                    else
                    {
                        done = true;
                    }
                }
                else if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (current + offset >= 0 && current + offset < party.Members.size())
                    {
                        selection = current + offset;

                        if (selection >= 0 && selection < party.Members.size())
                        {
                            if (Engine::IS_ALIVE(party.Members[selection]))
                            {
                                selectTeam(window, renderer, party.Members[selection], teams);

                                controls.clear();

                                controls = combatantList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh + text_space, true, false);
                            }
                            else
                            {
                                displayMessage(std::string(party.Members[selection].Name) + " is dead!", intRD);
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

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto text_space = 8;
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;
        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space) / (88);
        auto last = offset + limit;

        if (last > party.Members.size())
        {
            last = party.Members.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = combatantList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh + text_space, true, true);

        auto done = false;

        auto selection = -1;

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
                        thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                    }
                    else if (Engine::IS_ALIVE(party.Members[offset + i]))
                    {
                        if (team != Team::Type::NONE)
                        {
                            if (party.Members[offset + i].Team == team)
                            {
                                drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                            }
                            else
                            {
                                thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intGR, 2);
                            }
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                        }
                    }
                    else
                    {
                        drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intRD);
                    }
                }
            }

            renderButtons(renderer, controls, current, intLB, space, border_pts);

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

            if (mode == Control::Type::EQUIPMENT && equipment.Type != Equipment::Type::NONE)
            {
                putHeader(renderer, "Details", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + 2 * infoh + box_space));

                putText(renderer, itemString(equipment).c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - 3 * boxh - infoh - box_space);
            }

            putHeader(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

            if (selection >= 0 && selection < party.Members.size())
            {
                if (Engine::IS_ALIVE(party.Members[selection]))
                {
                    putText(renderer, party.Members[selection].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                }
                else
                {
                    putText(renderer, "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                }
            }
            else
            {
                putText(renderer, "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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

                        if (last > party.Members.size())
                        {
                            last = party.Members.size();
                        }

                        controls.clear();

                        controls = combatantList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh + text_space, true, true);

                        SDL_Delay(50);
                    }

                    if (offset <= 0)
                    {
                        current = -1;

                        selected = false;
                    }
                }
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
                {
                    if (party.Members.size() - last > 0)
                    {
                        if (offset < party.Members.size() - limit)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > party.Members.size() - limit)
                        {
                            offset = party.Members.size() - limit;
                        }

                        last = offset + limit;

                        if (last > party.Members.size())
                        {
                            last = party.Members.size();
                        }

                        controls.clear();

                        controls = combatantList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh + text_space, true, true);

                        SDL_Delay(50);

                        if (offset > 0)
                        {
                            if (controls[current].Type != Control::Type::SCROLL_DOWN)
                            {
                                current++;
                            }
                        }
                    }

                    if (party.Members.size() - last <= 0)
                    {
                        selected = false;

                        current = -1;
                    }
                }
                else if (controls[current].Type == Control::Type::CONFIRM)
                {
                    if (selection >= 0 && selection < party.Members.size())
                    {
                        if (mode == Control::Type::EQUIPMENT)
                        {
                            if (party.Members[selection].Type == Character::Type::SKULLCRACKER)
                            {
                                displayMessage("Skullcracker refuses!", intRD);
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
                        displayMessage("You must select a party member!", intRD);
                    }
                }
                else if (controls[current].Type == Control::Type::ACTION)
                {
                    if (current + offset >= 0 && current + offset < party.Members.size())
                    {
                        if (selection == current + offset)
                        {
                            selection = -1;
                        }
                        else
                        {
                            if (Engine::IS_ALIVE(party.Members[current + offset]))
                            {
                                if (team == Team::Type::NONE || party.Members[current + offset].Team == team)
                                {
                                    if (Engine::IS_CAPTURED(party.Members[current + offset]))
                                    {
                                        displayMessage(std::string(party.Members[current + offset].Name) + " has been captured!", intRD);
                                    }
                                    else if (Engine::IS_CURSED(party.Members[current + offset]))
                                    {
                                        displayMessage(std::string(party.Members[current + offset].Name) + " is cursed!", intRD);
                                    }
                                    else if (party.InCity && !party.Members[current + offset].IsCivilized)
                                    {
                                        displayMessage(std::string(party.Members[current + offset].Name) + " is waiting outside the city!", intRD);
                                    }
                                    else
                                    {
                                        selection = current + offset;
                                    }
                                }
                                else
                                {
                                    if (!Engine::IS_CHARACTER(team))
                                    {
                                        displayMessage(std::string(party.Members[current + offset].Name) + " is not part of the " + std::string(Team::Descriptions[team]) + " team!", intRD);
                                    }
                                    else
                                    {
                                        displayMessage("You can only choose " + std::string(Team::Descriptions[team]) + "!", intRD);
                                    }
                                }
                            }
                            else
                            {
                                displayMessage(std::string(party.Members[current + offset].Name) + " is dead!", intRD);
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

    if (result >= 0 && result < party.Members.size() & mode != Control::Type::EQUIPMENT)
    {
        party.LastSelected = result;
    }

    return result;
}

int selectShip(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> ships, Location::Type location, std::vector<Cargo::Type> cargo, Control::Type mode)
{
    auto result = -1;

    auto title = "Legendary Kingdoms: Select Ship";

    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto text_space = 8;
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space) / (88);
        auto last = offset + limit;

        if (last > ships.size())
        {
            last = ships.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = shipList(window, renderer, ships, offset, last, limit, textx, texty + infoh, true, true);

        auto done = false;

        auto selection = -1;

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
                        thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                    }
                    else if (ships[offset + i].Health > 0)
                    {
                        drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                    }
                    else
                    {
                        drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intRD);
                    }
                }
            }

            renderButtons(renderer, controls, current, intLB, space, border_pts);

            if (mode == Control::Type::COMBAT)
            {
                putHeader(renderer, "Choose the ship attacking this round", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
            else if (mode == Control::Type::CARGO)
            {
                putHeader(renderer, "Choose the ship to receive cargo", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
            else if (mode == Control::Type::SPELL_TARGET)
            {
                putHeader(renderer, "Choose target ship for this spell", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
            else if (mode == Control::Type::GAIN_HEALTH)
            {
                putHeader(renderer, "Choose the ship to repair", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
            else if (mode == Control::Type::LOSE_HEALTH)
            {
                putHeader(renderer, "Choose the ship to receive damage", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
            else if (mode == Control::Type::SAIL)
            {
                putHeader(renderer, "Choose the ship to board", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
            else
            {
                putHeader(renderer, "Choose ship", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }

            putHeader(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

            if (selection >= 0 && selection < ships.size())
            {
                if (ships[selection].Health > 0)
                {
                    putText(renderer, ships[selection].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                }
                else
                {
                    putText(renderer, "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
                }
            }
            else
            {
                putText(renderer, "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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

                        if (last > ships.size())
                        {
                            last = ships.size();
                        }

                        controls.clear();

                        controls = shipList(window, renderer, ships, offset, last, limit, textx, texty + infoh, true, true);

                        SDL_Delay(50);
                    }

                    if (offset <= 0)
                    {
                        current = -1;

                        selected = false;
                    }
                }
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
                {
                    if (ships.size() - last > 0)
                    {
                        if (offset < ships.size() - limit)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > ships.size() - limit)
                        {
                            offset = ships.size() - limit;
                        }

                        last = offset + limit;

                        if (last > ships.size())
                        {
                            last = ships.size();
                        }

                        controls.clear();

                        controls = shipList(window, renderer, ships, offset, last, limit, textx, texty + infoh, true, true);
                        ;

                        SDL_Delay(50);

                        if (offset > 0)
                        {
                            if (controls[current].Type != Control::Type::SCROLL_DOWN)
                            {
                                current++;
                            }
                        }
                    }

                    if (ships.size() - last <= 0)
                    {
                        selected = false;

                        current = -1;
                    }
                }
                else if (controls[current].Type == Control::Type::CONFIRM)
                {
                    if (selection >= 0 && selection < ships.size())
                    {
                        done = true;

                        result = selection;

                        current = -1;

                        selected = false;
                    }
                    else
                    {
                        displayMessage("You must select a ship!", intRD);
                    }
                }
                else if (controls[current].Type == Control::Type::ACTION)
                {
                    if (current + offset >= 0 && current + offset < ships.size())
                    {
                        if (selection == current + offset)
                        {
                            selection = -1;
                        }
                        else
                        {
                            if (ships[current + offset].Health > 0)
                            {
                                if (location == Location::Type::NONE || ships[current + offset].Location == location)
                                {
                                    if (mode == Control::Type::CARGO)
                                    {
                                        if ((ships[current + offset].MaximumCargo - ships[current + offset].Cargo.size()) >= cargo.size())
                                        {
                                            selection = current + offset;
                                        }
                                        else
                                        {
                                            displayMessage("This ship does not have enough space!", intRD);
                                        }
                                    }
                                    else
                                    {
                                        selection = current + offset;
                                    }
                                }
                                else
                                {
                                    displayMessage("You can only choose a ship docked at " + std::string(Location::Description[location]) + "!", intRD);
                                }
                            }
                            else
                            {
                                displayMessage("The " + std::string(ships[current + offset].Name) + " is destroyed!", intRD);
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

std::vector<int> selectPartyMembers(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, int team_size, Control::Type mode)
{
    auto selected_party = std::vector<int>();

    auto title = "Legendary Kingdoms: Select Party Members";

    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto text_space = 8;
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space) / (88);
        auto last = offset + limit;

        if (last > party.Members.size())
        {
            last = party.Members.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto controls = combatantList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh + text_space, true, true);

        auto done = false;

        auto selection = std::vector<int>();

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
                    auto result = Engine::FIND_LIST(selection, offset + i);

                    if (result >= 0 && result < selection.size())
                    {
                        thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                    }
                    else if (Engine::IS_ALIVE(party.Members[offset + i]))
                    {
                        if (team != Team::Type::NONE)
                        {
                            if (party.Members[offset + i].Team == team)
                            {
                                drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                            }
                            else
                            {
                                thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intGR, 2);
                            }
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                        }
                    }
                    else
                    {
                        drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intRD);
                    }
                }
            }

            renderButtons(renderer, controls, current, intLB, space, border_pts);

            if (mode == Control::Type::SPELL_TARGET)
            {
                putHeader(renderer, "Choose targets for this spell", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
            else if (mode == Control::Type::SKILL)
            {
                if (team_size > 1)
                {
                    putHeader(renderer, "Choose party members to perform skill check", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, "Choose party member to perform skill check", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
                }
            }
            else if (mode == Control::Type::RAISE_MAX_HEALTH)
            {
                putHeader(renderer, "Choose party members to raise maximum health points", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
            else if (mode == Control::Type::GAIN_HEALTH)
            {
                putHeader(renderer, "Choose party members to recover health points", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
            else if (mode == Control::Type::LOSE_HEALTH)
            {
                putHeader(renderer, "Choose party members to lose health points", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
            else if (mode == Control::Type::SELECT_ORDER)
            {
                putHeader(renderer, "Select the order of characters to perform the task", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }
            else
            {
                putHeader(renderer, "Choose Party Members", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);
            }

            putHeader(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

            if (selection.size() > 0)
            {
                std::string party_string = "";

                for (auto i = 0; i < selection.size(); i++)
                {
                    if (i > 0)
                    {
                        party_string += "\n";
                    }

                    party_string += party.Members[selection[i]].Name;

                    if (Engine::IS_DEAD(party.Members[selection[i]]))
                    {
                        party_string += " (D)";
                    }
                    else if (Engine::IS_CURSED(party.Members[i]))
                    {
                        party_string += " (C)";
                    }
                }

                putText(renderer, party_string.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
            }
            else
            {
                putText(renderer, "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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

                        if (last > party.Members.size())
                        {
                            last = party.Members.size();
                        }

                        controls.clear();

                        controls = combatantList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh + text_space, true, true);

                        SDL_Delay(50);
                    }

                    if (offset <= 0)
                    {
                        current = -1;

                        selected = false;
                    }
                }
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
                {
                    if (party.Members.size() - last > 0)
                    {
                        if (offset < party.Members.size() - limit)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > party.Members.size() - limit)
                        {
                            offset = party.Members.size() - limit;
                        }

                        last = offset + limit;

                        if (last > party.Members.size())
                        {
                            last = party.Members.size();
                        }

                        controls.clear();

                        controls = combatantList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh + text_space, true, true);

                        SDL_Delay(50);

                        if (offset > 0)
                        {
                            if (controls[current].Type != Control::Type::SCROLL_DOWN)
                            {
                                current++;
                            }
                        }
                    }

                    if (party.Members.size() - last <= 0)
                    {
                        selected = false;

                        current = -1;
                    }
                }
                else if (controls[current].Type == Control::Type::CONFIRM)
                {
                    if (selection.size() > 0 && (selection.size() >= team_size || selection.size() >= Engine::COUNT(party, team)))
                    {
                        done = true;

                        selected_party = selection;

                        current = -1;

                        selected = false;
                    }
                    else
                    {
                        displayMessage("You must select " + std::to_string(team_size) + " party members!", intRD);
                    }
                }
                else if (controls[current].Type == Control::Type::ACTION)
                {
                    if (current + offset >= 0 && current + offset < party.Members.size())
                    {
                        auto result = Engine::FIND_LIST(selection, current + offset);

                        if (result >= 0 && result < selection.size())
                        {
                            selection.erase(selection.begin() + result);
                        }
                        else
                        {
                            if (Engine::IS_ALIVE(party.Members[current + offset]))
                            {
                                if (team == Team::Type::NONE || party.Members[current + offset].Team == team)
                                {
                                    if (Engine::IS_CAPTURED(party.Members[current + offset]))
                                    {
                                        displayMessage(std::string(party.Members[current + offset].Name) + " has been captured!", intRD);
                                    }
                                    else if (Engine::IS_CURSED(party.Members[current + offset]))
                                    {
                                        displayMessage(std::string(party.Members[current + offset].Name) + " is cursed!", intRD);
                                    }
                                    else if (Engine::IS_DEAD(party.Members[current + offset]))
                                    {
                                        displayMessage(std::string(party.Members[current + offset].Name) + " is dead!", intRD);
                                    }
                                    else if (party.InCity && !party.Members[current + offset].IsCivilized)
                                    {
                                        displayMessage(std::string(party.Members[current + offset].Name) + " is waiting outside the city!", intRD);
                                    }
                                    else if (selection.size() < team_size)
                                    {
                                        selection.push_back(current + offset);
                                    }
                                }
                                else
                                {
                                    if (!Engine::IS_CHARACTER(team))
                                    {
                                        displayMessage(std::string(party.Members[current + offset].Name) + " is not part of the " + std::string(Team::Descriptions[team]) + " team!", intRD);
                                    }
                                    else
                                    {
                                        displayMessage("You can only choose " + std::string(Team::Descriptions[team]) + "!", intRD);
                                    }
                                }
                            }
                            else
                            {
                                displayMessage(std::string(party.Members[current + offset].Name) + " is dead!", intRD);
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

    party.LastSelection = selected_party;

    if (selected_party.size() > 0)
    {
        party.Order.clear();

        for (auto i = 0; i < selected_party.size(); i++)
        {
            party.Order.push_back(party.Members[selected_party[i]].Type);
        }
    }

    if (selected_party.size() == 1)
    {
        if (selected_party[0] >= 0 && selected_party[0] < party.Members.size())
        {
            party.LastSelected = selected_party[0];
        }
    }

    return selected_party;
}

Engine::Combat seaCombatScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Ship::Base> &enemyFleet, bool storyFlee, int fleeRound, int roundLimit)
{
    auto combatResult = Engine::Combat::NONE;

    auto title = "Legendary Kindoms: Sea Combat";

    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        std::vector<Button> controls;

        auto text_space = 8;
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space - infoh) / (88);
        auto last = offset + limit;

        if (last > enemyFleet.size())
        {
            last = enemyFleet.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto hasAttacked = false;

        if (Engine::VERIFY_CODES(party, {Codes::Type::LAST_IN_COMBAT}))
        {
            hasAttacked = true;

            if (fleeRound == 0)
            {
                fleeRound += 1;
            }
        }

        auto combatRound = 0;

        auto current_mode = Control::Type::ATTACK;

        auto canFlee = storyFlee;

        while (Engine::COUNT(enemyFleet) > 0 && Engine::SHIP_INTACT(party) && (roundLimit == -1 || (roundLimit > 0 && combatRound < roundLimit)))
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

                if (last > enemyFleet.size())
                {
                    last = enemyFleet.size();
                }

                if (Engine::COUNT(enemyFleet) == 0)
                {
                    current_mode = Control::Type::NEXT;
                }
                else if (!hasAttacked)
                {
                    current_mode = Control::Type::ATTACK;
                }
                else
                {
                    current_mode = Control::Type::DEFEND;
                }

                controls = shipList(window, renderer, enemyFleet, offset, last, limit, textx, texty + infoh + text_space, current_mode);

                fillWindow(renderer, intWH);

                if (splash)
                {
                    fitImage(renderer, splash, startx, starty, splashw, text_bounds);
                }

                fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

                renderButtons(renderer, controls, current, intLB, space, border_pts);

                if (last - offset > 0)
                {
                    for (auto i = 0; i < last - offset; i++)
                    {
                        if (enemyFleet[offset + i].Health > 0)
                        {
                            if (current + offset == offset + i)
                            {
                                thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                            }
                            else
                            {
                                drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                            }
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intRD);
                        }
                    }
                }

                putHeader(renderer, "Opponents", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                putHeader(renderer, "Current Ship", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (party.CurrentShip >= 0 && party.CurrentShip < party.Fleet.size())
                {
                    std::string ship_string = "[" + std::string(party.Fleet[party.CurrentShip].Name) + "]";
                    ship_string += "\nFighting: " + std::to_string(party.Fleet[party.CurrentShip].Fighting);
                    ship_string += "\nHealth: " + std::to_string(party.Fleet[party.CurrentShip].Health);

                    putText(renderer, ship_string.c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold, 200);

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

                            if (last > enemyFleet.size())
                            {
                                last = enemyFleet.size();
                            }

                            controls.clear();

                            SDL_Delay(50);
                        }

                        if (offset <= 0)
                        {
                            current = -1;

                            selected = false;
                        }
                    }
                    else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
                    {
                        if (enemyFleet.size() - last > 0)
                        {
                            if (offset < enemyFleet.size() - limit)
                            {
                                offset += scrollSpeed;
                            }

                            if (offset > enemyFleet.size() - limit)
                            {
                                offset = enemyFleet.size() - limit;
                            }

                            last = offset + limit;

                            if (last > enemyFleet.size())
                            {
                                last = enemyFleet.size();
                            }

                            controls.clear();

                            SDL_Delay(50);

                            if (offset > 0)
                            {
                                if (controls[current].Type != Control::Type::SCROLL_DOWN)
                                {
                                    current++;
                                }
                            }
                        }

                        if (enemyFleet.size() - last <= 0)
                        {
                            selected = false;

                            current = -1;
                        }
                    }
                    else if (controls[current].Type == Control::Type::FLEE && !hold)
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
                                if (combatRound < fleeRound)
                                {
                                    displayMessage("You cannot flee at this time.", intRD);
                                }
                                else
                                {
                                    displayMessage("You can no longer flee from this battle.", intRD);
                                }
                            }
                        }
                        else
                        {
                            displayMessage("You cannot flee from this battle.", intRD);
                        }
                    }
                    else if (controls[current].Type == Control::Type::ATTACK && !hold)
                    {
                        if (Engine::SHIP_INTACT(party) && !hasAttacked && Engine::COUNT(enemyFleet) > 0)
                        {
                            if (hasAttacked)
                            {
                                displayMessage("Your ship has already attacked this round.", intRD);
                            }
                            else
                            {
                                int opponent = -1;

                                if (Engine::COUNT(enemyFleet) == 1)
                                {
                                    opponent = Engine::FIRST(enemyFleet);
                                }
                                else
                                {
                                    opponent = selectOpponent(window, renderer, enemyFleet, {}, combatRound);
                                }

                                if (opponent >= 0 && opponent < enemyFleet.size() && enemyFleet[opponent].Health > 0)
                                {
                                    seaAttackScreen(window, renderer, party, enemyFleet, opponent, 0);
                                }

                                hasAttacked = true;
                            }
                        }

                        if (hasAttacked)
                        {
                            if (Engine::COUNT(enemyFleet) > 0)
                            {
                                for (auto i = 0; i < enemyFleet.size(); i++)
                                {
                                    if (enemyFleet[i].Health > 0 && Engine::SHIP_INTACT(party))
                                    {
                                        seaAttackScreen(window, renderer, party, enemyFleet, i, 1);
                                    }
                                }
                            }

                            hasAttacked = false;
                        }

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::SPELL && !hold)
                    {
                        if (!Engine::VERIFY_CODES(party, {Codes::Type::NO_COMBAT_SPELLS}))
                        {
                            if (Engine::SHIP_INTACT(party) && !hasAttacked)
                            {
                                if (Engine::SPELLCASTERS(party) > 0)
                                {
                                    auto combat_spells = 0;

                                    for (auto i = 0; i < party.Members.size(); i++)
                                    {
                                        if (Engine::IS_ALIVE(party.Members[i]) && party.Members[i].SpellCaster)
                                        {
                                            auto spells = Engine::COUNT(party.Members[i].SpellBook, Spells::Scope::SEA_COMBAT);

                                            combat_spells += spells;
                                        }
                                    }

                                    if (combat_spells <= 0)
                                    {
                                        displayMessage("Your party does not have any usable sea combat spells.", intRD);
                                    }
                                    else
                                    {
                                        // TODO: Sea combat spell
                                        selected = false;

                                        current = -1;
                                    }
                                }
                                else
                                {
                                    displayMessage("There are no spell casters in your party!", intRD);
                                }
                            }
                            else
                            {
                                if (Engine::VERIFY_CODES(party, {Codes::Type::LAST_IN_COMBAT}) && combatRound == 0)
                                {
                                    displayMessage("Your ship does not get to attack first nor cast spells this round!", intRD);
                                }
                                else
                                {
                                    displayMessage("Your ship has already attacked this round.", intRD);
                                }
                            }
                        }
                        else
                        {
                            displayMessage("You cannot cast spells in this battle!", intRD);
                        }
                    }
                }

                if (!Engine::SHIP_INTACT(party) || Engine::COUNT(enemyFleet) == 0)
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

        if (combatResult != Engine::Combat::FLEE)
        {
            if (roundLimit > 0 && combatRound >= roundLimit)
            {
                combatResult = Engine::Combat::EXCEED_LIMIT;
            }
            else
            {
                combatResult = Engine::SHIP_INTACT(party) ? Engine::Combat::VICTORY : Engine::Combat::DEFEAT;
            }
        }
    }

    // Clear temporary status, e.g. magic effects
    if (Engine::VERIFY_CODES(party, {Codes::Type::LAST_IN_COMBAT}))
    {
        Engine::LOSE_CODES(party, {Codes::Type::LAST_IN_COMBAT});
    }

    if (Engine::VERIFY_CODES(party, {Codes::Type::CANNOT_USE_SHIPWEAPONS}))
    {
        Engine::LOSE_CODES(party, {Codes::Type::CANNOT_USE_SHIPWEAPONS});
    }

    if (combatResult != Engine::Combat::NONE)
    {
        Engine::LOSE_CODES(party, {Codes::Type::NO_COMBAT_SPELLS});
    }

    return combatResult;
}

Engine::Combat combatScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Monster::Base> &monsters, std::vector<Allies::Type> &allies, bool storyFlee, int fleeRound, int roundLimit, bool useEquipment)
{
    auto combatResult = Engine::Combat::NONE;

    auto title = "Legendary Kindoms: Combat";

    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        auto allies_attack = false;

        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        auto allyMessage = [&](std::string msg, Uint32 color)
        {
            allies_attack = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);
        auto font_mason = TTF_OpenFont(FONT_MASON, 24);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        std::vector<Button> controls;

        auto text_space = 8;
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;
        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space - infoh) / (88);
        auto last = offset + limit;

        if (last > monsters.size())
        {
            last = monsters.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto hasAttacked = std::vector<int>();

        if (Engine::VERIFY_CODES(party, {Codes::Type::LAST_IN_COMBAT}))
        {
            for (auto i = 0; i < party.Members.size(); i++)
            {
                if (Engine::IS_ALIVE(party.Members[i]))
                {
                    hasAttacked.push_back(i);
                }
            }

            if (fleeRound == 0)
            {
                fleeRound++;
            }
        }

        auto combatRound = 0;

        auto round0_attacks = 0;

        auto spells_cast = 0;

        auto allyAttack = std::vector<Allies::Type>();

        auto current_mode = Control::Type::ATTACK;

        auto canFlee = storyFlee;

        while (Engine::COUNT(monsters) > 0 && Engine::COUNT(party, team) > 0 && (roundLimit == -1 || (roundLimit > 0 && combatRound < roundLimit)))
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
                if (combatRound == 0 && Engine::VERIFY_CODES(party, {Codes::Type::ENEMY1_FREEATTACK_ROUND0}))
                {
                    if (Engine::IS_ACTIVE(party, party.LastSelected) && monsters.size() > 0)
                    {
                        auto free_attack = Engine::COUNT(monsters[0].Attack, monsters[0].Difficulty);

                        message = "The " + std::string(monsters[0].Name);

                        if (free_attack > 0)
                        {
                            if (Engine::ARMOUR(party.Members[party.LastSelected]) > 0 && monsters[0].Type != Monster::Type::PAPER && monsters[0].Type != Monster::Type::NAGA)
                            {
                                free_attack = std::max(0, armourSave(window, renderer, party.Members[party.LastSelected], free_attack));
                            }

                            if (free_attack > 0)
                            {
                                message += " deals " + std::to_string(free_attack) + " damage to " + std::string(party.Members[party.LastSelected].Name) + "!";

                                Engine::GAIN_HEALTH(party.Members[party.LastSelected], -free_attack);

                                displayMessage(message, intRD);
                            }
                            else
                            {
                                message = +"'s attack was ineffective!";

                                displayMessage(message, intLB);
                            }
                        }
                        else
                        {
                            message = +"'s attack was ineffective!";

                            displayMessage(message, intLB);
                        }
                    }

                    Engine::LOSE_CODES(party, {Codes::Type::ENEMY1_FREEATTACK_ROUND0});
                }

                auto last = offset + limit;

                if (last > monsters.size())
                {
                    last = monsters.size();
                }

                if (Engine::COUNT(monsters, combatRound) == 0)
                {
                    current_mode = Control::Type::NEXT;
                }
                else if (hasAttacked.size() < Engine::TEAM_SIZE(party, team))
                {
                    current_mode = Control::Type::ATTACK;
                }
                else
                {
                    current_mode = Control::Type::DEFEND;
                }

                controls = monsterList(window, renderer, monsters, offset, last, limit, textx, texty + infoh + text_space, current_mode);

                fillWindow(renderer, intWH);

                if (splash)
                {
                    fitImage(renderer, splash, startx, starty, splashw, text_bounds);
                }

                putHeader(renderer, "Opponents", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                fillRect(renderer, textwidth, text_bounds - infoh, textx, texty + infoh, intBE);

                renderButtons(renderer, controls, current, intLB, space, border_pts);

                if (last - offset > 0)
                {
                    for (auto i = 0; i < last - offset; i++)
                    {
                        if (monsters[offset + i].Health > 0)
                        {
                            if (combatRound >= monsters[offset + i].Round)
                            {
                                if (current + offset == offset + i)
                                {
                                    thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                                }
                                else
                                {
                                    drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                                }
                            }
                            else
                            {
                                thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intGR, 2);
                            }
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intRD);
                        }
                    }
                }

                if (team != Team::Type::NONE)
                {
                    if (Engine::IS_CHARACTER(team))
                    {
                        putHeader(renderer, "Current", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + 2 * infoh + box_space));
                    }
                    else
                    {
                        putHeader(renderer, "Team", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + 2 * infoh + box_space));
                    }

                    putText(renderer, Team::Descriptions[team], font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - 3 * boxh - infoh - box_space);
                }

                putHeader(renderer, "Party", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

                if (Engine::COUNT(party, team) > 0)
                {
                    std::string party_string = "";

                    auto count = 0;

                    for (auto i = 0; i < party.Members.size(); i++)
                    {
                        if (count > 0)
                        {
                            party_string += "\n";
                        }

                        party_string += party.Members[i].Name;

                        if (Engine::IS_DEAD(party.Members[i]))
                        {
                            party_string += " (D)";
                        }
                        else
                        {
                            if (Engine::FIND_LIST(hasAttacked, i) >= 0)
                            {
                                if (Engine::HAS_STATUS(party.Members[i], Character::Status::STUNNED))
                                {
                                    party_string += " (S)";
                                }
                                else if (Engine::IS_CURSED(party.Members[i]))
                                {
                                    party_string += " (C)";
                                }
                                else
                                {
                                    party_string += " (A)";
                                }
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

                if (allies_attack)
                {
                    if ((SDL_GetTicks() - start_ticks) < duration)
                    {
                        putHeader(renderer, message.c_str(), font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, infoh * 2, -1, -1);
                    }
                    else
                    {
                        allies_attack = false;
                    }
                }

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold, 200);

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

                            SDL_Delay(50);
                        }

                        if (offset <= 0)
                        {
                            current = -1;

                            selected = false;
                        }
                    }
                    else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
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
                    else if (controls[current].Type == Control::Type::FLEE && !hold)
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
                                if (combatRound < fleeRound)
                                {
                                    message = "You cannot flee at this time.";
                                }
                                else
                                {
                                    message = "You can no longer flee from this battle.";
                                }

                                displayMessage(message, intRD);
                            }
                        }
                        else
                        {
                            displayMessage("You cannot flee from this battle.", intRD);
                        }
                    }
                    else if (controls[current].Type == Control::Type::PARTY && !hold)
                    {
                        viewParty(window, renderer, party, true);

                        selected = false;

                        current = -1;
                    }
                    else if (controls[current].Type == Control::Type::ATTACK && !hold)
                    {
                        if (allies_attack)
                        {
                            allies_attack = false;
                        }

                        if (Engine::COUNT(party, team) > 0 && hasAttacked.size() < Engine::TEAM_SIZE(party, team) && Engine::COUNT(monsters, combatRound) > 0)
                        {
                            auto result = -1;

                            if (Engine::COUNT(party, team, hasAttacked) == 1)
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

                            if (Engine::FIND_LIST(hasAttacked, result) >= 0 && !(Engine::HAS_STATUS(party.Members[result], Character::Status::ATTACK2_ENEMY0_ROUND0) && combatRound == 0 && round0_attacks < 2))
                            {
                                if (Engine::HAS_STATUS(party.Members[result], Character::Status::STUNNED))
                                {
                                    message = std::string(party.Members[result].Name) + " is stunned!";
                                }
                                else
                                {
                                    message = std::string(party.Members[result].Name) + " already attacked this round.";
                                }

                                displayMessage(message, intRD);
                            }
                            else
                            {
                                if (result >= 0 && result < party.Members.size())
                                {
                                    if (Engine::IS_ALIVE(party.Members[result]))
                                    {
                                        if (Engine::COUNT(monsters, combatRound) > 0)
                                        {
                                            auto opponent = -1;

                                            if (Engine::COUNT(monsters, combatRound) == 1)
                                            {
                                                opponent = Engine::FIRST(monsters, combatRound);
                                            }
                                            else
                                            {
                                                opponent = selectOpponent(window, renderer, party, result, monsters, {}, combatRound, Control::Type::COMBAT);
                                            }

                                            if (opponent >= 0 && opponent < monsters.size())
                                            {
                                                if (Engine::HAS_STATUS(party.Members[result], Character::Status::ATTACK2_ENEMY0_ROUND0) && opponent != 0 && round0_attacks > 0 && combatRound == 0)
                                                {
                                                    displayMessage("You cannot attack another opponent.", intRD);
                                                }
                                                else
                                                {
                                                    auto useWeapons = useEquipment;

                                                    if (monsters[opponent].Type == Monster::Type::BEETLE_SWARM)
                                                    {
                                                        useWeapons = false;
                                                    }

                                                    auto damage = attackScreen(window, renderer, party, team, monsters, result, opponent, 0, combatRound, useWeapons);

                                                    if (damage >= 0)
                                                    {
                                                        if (monsters[opponent].Type != Monster::Type::JUNGLE)
                                                        {
                                                            hasAttacked.push_back(result);

                                                            if (Engine::HAS_STATUS(party.Members[result], Character::Status::ATTACK2_ENEMY0_ROUND0))
                                                            {
                                                                round0_attacks++;

                                                                if (opponent != 0 || round0_attacks > 1 || combatRound != 0)
                                                                {
                                                                    Engine::REMOVE_STATUS(party.Members[result], Character::Status::ATTACK2_ENEMY0_ROUND0);
                                                                }
                                                            }

                                                            canFlee = false;
                                                        }
                                                        else
                                                        {
                                                            if (damage == 100 && hasAttacked.size() == (Engine::TEAM_SIZE(party, team) - 1))
                                                            {
                                                                displayMessage("There must be at least 1 attack on the Jungle each round.", intRD);
                                                            }
                                                            else
                                                            {
                                                                hasAttacked.push_back(result);

                                                                if (damage == 100)
                                                                {
                                                                    message = std::string(party.Members[result].Name) + " did not attack this round.";

                                                                    displayMessage(message, intLB);
                                                                }
                                                                else
                                                                {
                                                                    if (monsters[opponent].Health > 0)
                                                                    {
                                                                        Engine::GAIN_HEALTH(party.Members[result], -1);

                                                                        message = "The " + std::string(monsters[opponent].Name) + " deals 1 damage to " + std::string(party.Members[result].Name) + "!";

                                                                        displayMessage(message, intRD);
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                displayMessage("The attack has failed!", intRD);
                                            }
                                        }
                                        else
                                        {
                                            displayMessage("Your party has prevailed!", intLB);
                                        }
                                    }
                                    else
                                    {
                                        displayMessage(message, intRD);
                                    }
                                }
                            }
                        }

                        if (hasAttacked.size() >= Engine::COUNT(party, team) || Engine::COUNT(monsters, combatRound) == 0)
                        {
                            if (Engine::HAS_ALLY(allies, Allies::Type::SLAVES) && !Engine::HAS_ALLY(allyAttack, Allies::Type::SLAVES))
                            {
                                if (Engine::HAS_MONSTER(monsters, Monster::Type::ORC))
                                {
                                    auto slaves_damage = -1;

                                    message = "The slaves attack the orcs! All orcs lose ";

                                    if (Engine::VERIFY_CODES(party, {Codes::Type::SLAVES_DAMAGE_FOR2}))
                                    {
                                        slaves_damage = -2;
                                    }

                                    message += std::to_string(-slaves_damage) + " Health point";

                                    if (std::abs(slaves_damage) > 1)
                                    {
                                        message += "s";
                                    }

                                    message += "!";

                                    Engine::GAIN_HEALTH(monsters, Monster::Type::ORC, slaves_damage);

                                    allyAttack.push_back(Allies::Type::SLAVES);

                                    allyMessage(message, intLB);
                                }
                            }

                            if (Engine::HAS_ALLY(allies, Allies::Type::YU_YUAN) && !Engine::HAS_ALLY(allyAttack, Allies::Type::YU_YUAN))
                            {
                                if (Engine::COUNT(monsters, combatRound) > 0)
                                {
                                    auto target = Engine::FIRST(monsters);

                                    if (target >= 0 && target < monsters.size())
                                    {
                                        auto attack_result = Engine::COUNT(5, monsters[target].Defence);

                                        if (attack_result > 0)
                                        {
                                            message = "Yu Yuan deals " + std::to_string(attack_result) + " to the " + std::string(monsters[target].Name) + "!";

                                            Engine::GAIN_HEALTH(monsters[target], -attack_result);

                                            allyMessage(message, intLB);
                                        }
                                        else
                                        {
                                            allyMessage("Yu Yuan's attack was ineffective!", intRD);
                                        }
                                    }

                                    allyAttack.push_back(Allies::Type::YU_YUAN);
                                }
                            }

                            if (Engine::COUNT(monsters, combatRound) > 0 && !allies_attack)
                            {
                                Engine::REMOVE_STATUS(party, Character::Status::STUNNED);

                                if (Engine::VERIFY_CODES(party, {Codes::Type::ENEMY_DAZING_LIGHTS}))
                                {
                                    Engine::LOSE_CODES(party, {Codes::Type::ENEMY_DAZING_LIGHTS});
                                }

                                auto everchild_assassin = Engine::FIND_MONSTER(monsters, Monster::Type::EVERCHILD_ASSASSIN);

                                if (everchild_assassin >= 0 && everchild_assassin < monsters.size() && monsters[everchild_assassin].Health > 0 && !monsters[everchild_assassin].Damaged)
                                {
                                    combatResult = Engine::Combat::FAILED_ATTACK;

                                    done = true;
                                }

                                if (!done)
                                {
                                    for (auto i = 0; i < monsters.size(); i++)
                                    {
                                        if (monsters[i].Health > 0 && Engine::COUNT(party, team) > 0 && combatRound >= monsters[i].Round)
                                        {
                                            if (monsters[i].Type != Monster::Type::ZEALOT_HEALER || Engine::COUNT(monsters) == 1)
                                            {
                                                if ((monsters[i].Attack > 0 && monsters[i].Difficulty > 0) || monsters[i].Type == Monster::Type::MONKEY_WITH_SPELLS)
                                                {
                                                    if (monsters[i].Type == Monster::Type::ZEALOT_SORCERER && combatRound == 1)
                                                    {
                                                        displayMessage("The sorcerer casts a Dazing Lights spell!", intRD);

                                                        Engine::GET_CODES(party, {Codes::Type::ENEMY_DAZING_LIGHTS});
                                                    }
                                                    else if (monsters[i].Type == Monster::Type::FASTILON)
                                                    {
                                                        if (!monsters[i].Damaged || spells_cast >= 3)
                                                        {
                                                            if (spells_cast == 2)
                                                            {
                                                                displayMessage("Fastilon a Healing Word! Fastilon recovers 8 Health points!", intRD);

                                                                Engine::GAIN_HEALTH(monsters[i], 8);
                                                            }
                                                            else
                                                            {
                                                                attackScreen(window, renderer, party, team, monsters, -1, i, 1, spells_cast, useEquipment);
                                                            }

                                                            spells_cast += 1;
                                                        }
                                                        else
                                                        {
                                                            attackScreen(window, renderer, party, team, monsters, -1, i, 1, (combatRound < 3 ? 3 : combatRound), useEquipment);
                                                        }
                                                    }
                                                    else
                                                    {
                                                        attackScreen(window, renderer, party, team, monsters, -1, i, 1, combatRound, useEquipment);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            if (!allies_attack)
                            {
                                // After combat round trigger
                                if (Engine::HAS_MONSTER(monsters, Monster::Type::SNAKEMAN_PRIEST))
                                {
                                    Engine::GAIN_HEALTH(party, -1);

                                    Engine::GAIN_HEALTH(monsters, -1);

                                    displayMessage("Blocks of stone come raining down from the walls! The priest and each party member loses 1 Health!", intRD);
                                }
                                else if (Engine::HAS_MONSTER(monsters, Monster::Type::SPIDER_WITH_SWARM))
                                {
                                    Engine::GAIN_HEALTH(party, team, -1);

                                    displayMessage("Swarms of tiny spiders attack the party and deal 1 damage to everyone!", intRD);
                                }
                                else if (Engine::HAS_MONSTER(monsters, Monster::Type::BEETLE_SWARM))
                                {
                                    Engine::GAIN_HEALTH(party, team, -1);

                                    displayMessage("The beetles deals 1 damage to the entire party!", intRD);
                                }
                                else if (Engine::HAS_MONSTER(monsters, Monster::Type::ZEALOT_HEALER) && Engine::COUNT(monsters) > 1)
                                {
                                    Engine::GAIN_HEALTH(monsters, 2);

                                    displayMessage("The Zealot Healer heals each Zealot for 2 Health points!", intRD);
                                }
                                else if (Engine::HAS_MONSTER(monsters, Monster::Type::SKELETON_ARCHERS))
                                {
                                    auto result = Engine::FIND_MONSTER(monsters, Monster::Type::SKELETON_ARCHERS);

                                    if (result >= 0 && result < monsters.size() && !monsters[result].Damaged)
                                    {
                                        auto selection = selectPartyMembers(window, renderer, party, team, 2, Control::Type::LOSE_HEALTH);

                                        if (selection.size() > 0)
                                        {
                                            message = "Skeleton Archers deal 2 damage to: ";

                                            for (auto i = 0; i < selection.size(); i++)
                                            {
                                                if (i > 0)
                                                {
                                                    message += ", ";
                                                }

                                                message += party.Members[selection[i]].Name;

                                                Engine::GAIN_HEALTH(party.Members[selection[i]], -2);
                                            }

                                            displayMessage(message, intRD);
                                        }
                                    }
                                }

                                Engine::LOSE_CODES(party, {Codes::Type::DAZING_LIGHTS});

                                allyAttack.clear();

                                hasAttacked.clear();

                                canFlee = storyFlee;

                                combatRound++;

                                Engine::REMOVE_STATUS(party, Character::Status::EXTRA_MAGIC_ROUND0);

                                Engine::REMOVE_STATUS(party, Character::Status::UNLIMITED_MAGIC_ROUND0);

                                // clear damaged flag for next round
                                for (auto i = 0; i < monsters.size(); i++)
                                {
                                    monsters[i].Damaged = false;
                                }

                                // Setup stunned characters
                                for (auto i = 0; i < party.Members.size(); i++)
                                {
                                    if (Engine::IS_ACTIVE(party, i) && Engine::HAS_STATUS(party.Members[i], Character::Status::STUNNED_NEXT_ROUND))
                                    {
                                        Engine::REMOVE_STATUS(party.Members[i], Character::Status::STUNNED_NEXT_ROUND);

                                        Engine::GAIN_STATUS(party.Members[i], Character::Status::STUNNED);

                                        hasAttacked.push_back(i);
                                    }
                                }
                            }
                        }

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::SPELL && !hold)
                    {
                        if (!Engine::VERIFY_CODES(party, {Codes::Type::NO_COMBAT_SPELLS}))
                        {
                            if (Engine::COUNT(party, team) > 0 && hasAttacked.size() < Engine::TEAM_SIZE(party, team))
                            {
                                if (Engine::SPELLCASTERS(party) > 0)
                                {
                                    auto combat_spells = 0;

                                    for (auto i = 0; i < party.Members.size(); i++)
                                    {
                                        if (Engine::IS_ALIVE(party.Members[i]) && party.Members[i].SpellCaster && (team == Team::Type::NONE || party.Members[i].Team == team))
                                        {
                                            auto spells = Engine::COUNT(party.Members[i].SpellBook, Spells::Scope::COMBAT) + Engine::COUNT(party.Members[i].SpellBook, Spells::Scope::ADVENTURE_COMBAT);

                                            combat_spells += spells;
                                        }
                                    }

                                    if (combat_spells <= 0)
                                    {
                                        displayMessage("Your party does not have any usable combat spells.", intRD);
                                    }
                                    else
                                    {
                                        auto result = castCombatSpell(window, renderer, party, team, monsters, hasAttacked, combatRound);

                                        if (result >= 0 && result < party.Members.size())
                                        {
                                            if (Engine::FIND_LIST(hasAttacked, result) < 0)
                                            {
                                                hasAttacked.push_back(result);
                                            }
                                            else
                                            {
                                                if (Engine::HAS_STATUS(party.Members[result], Character::Status::EXTRA_MAGIC_ROUND0))
                                                {
                                                    Engine::REMOVE_STATUS(party.Members[result], Character::Status::EXTRA_MAGIC_ROUND0);
                                                }
                                            }

                                            message = std::string(party.Members[result].Name) + " casts a spell!";

                                            displayMessage(message, intLB);
                                        }

                                        selected = false;

                                        current = -1;
                                    }
                                }
                                else
                                {
                                    displayMessage("There are no spell casters in your party!", intRD);
                                }
                            }
                            else
                            {
                                if (Engine::VERIFY_CODES(party, {Codes::Type::LAST_IN_COMBAT}) && combatRound == 0)
                                {
                                    message = "Your party does not get to attack first nor cast spells this round!";
                                }
                                else
                                {
                                    message = "Your entire party has already attacked this round.";
                                }

                                displayMessage(message, intRD);
                            }
                        }
                        else
                        {
                            displayMessage("You cannot cast spells in this battle!", intRD);
                        }
                    }
                }

                if (Engine::COUNT(party, team) == 0 || Engine::COUNT(monsters) == 0)
                {
                    done = true;
                }

                if (roundLimit > 0 && combatRound >= roundLimit)
                {
                    done = true;
                }
            }

            if (combatResult == Engine::Combat::FLEE || combatResult == Engine::Combat::FAILED_ATTACK)
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

        if (combatResult != Engine::Combat::FLEE)
        {
            if (roundLimit > 0 && combatRound >= roundLimit)
            {
                combatResult = Engine::Combat::EXCEED_LIMIT;
            }
            else if (combatResult != Engine::Combat::FAILED_ATTACK)
            {
                combatResult = Engine::COUNT(party, team) > 0 ? Engine::Combat::VICTORY : Engine::Combat::DEFEAT;
            }
        }
    }

    // Clear temporary status, e.g. magic effects
    if (Engine::VERIFY_CODES(party, {Codes::Type::LAST_IN_COMBAT}))
    {
        Engine::LOSE_CODES(party, {Codes::Type::LAST_IN_COMBAT});
    }

    if (combatResult != Engine::Combat::NONE)
    {
        for (auto i = 0; i < party.Members.size(); i++)
        {
            if (team == Team::Type::NONE || party.Members[i].Team == team || party.Members[i].Team == Team::Type::SOLO)
            {
                Engine::REMOVE_STATUS(party.Members[i], Character::Status::ARMOUR3);
                Engine::REMOVE_STATUS(party.Members[i], Character::Status::ENRAGED);
                Engine::REMOVE_STATUS(party.Members[i], Character::Status::POTION_OF_INVULNERABILITY);
                Engine::REMOVE_STATUS(party.Members[i], Character::Status::STUNNED);
                Engine::REMOVE_STATUS(party.Members[i], Character::Status::STUNNED_NEXT_ROUND);
            }
        }

        Engine::LOSE_CODES(party, {Codes::Type::NO_COMBAT_SPELLS});
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

            auto supply = std::get<3>(shop[index]);

            auto item_string = itemString(item);

            if (supply >= 0)
            {
                if (supply == 0)
                {
                    item_string += ", out of stock";
                }
                else
                {
                    item_string += ", " + std::to_string(supply) + " stock";

                    if (supply > 1)
                    {
                        item_string += "s";
                    }

                    item_string += " remaining";
                }
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

bool shopScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Engine::EquipmentPrice> &shop, Character::Base &character)
{
    auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

    auto infoh = 48;
    auto boxh = (int)(0.150 * SCREEN_HEIGHT);
    auto box_space = 10;

    auto font_size = 28;
    auto text_space = 8;
    auto scrollSpeed = 1;
    auto limit = (text_bounds - 2 * text_space - infoh) / (88);

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

    // Lambda functions for displaying flash messages
    auto displayMessage = [&](std::string msg, Uint32 color)
    {
        flash_message = true;

        message = msg;

        flash_color = color;

        start_ticks = SDL_GetTicks();
    };

    auto done = false;

    auto listwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

    auto offsety = (texty + infoh);

    auto controls = shopList(window, renderer, shop, offset, last, limit, textx, offsety);

    TTF_Init();

    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
    auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);
    auto font_mason = TTF_OpenFont(FONT_MASON, 24);

    TTF_SetFontKerning(font_dark11, 0);

    auto selected = false;
    auto current = -1;
    auto scrollUp = false;
    auto scrollDown = false;
    auto hold = false;

    auto selection = std::vector<int>();

    while (!done)
    {
        if (!Engine::VERIFY_EQUIPMENT_LIMIT(character))
        {
            displayMessage("You are carrying too many items! Drop or transfer excess items.", intRD);
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

        putHeader(renderer, "Money", font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (5 * boxh / 2) - 2 * infoh - box_space);
        putText(renderer, (std::to_string(party.Money) + std::string(" silver coins")).c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh / 2, startx, starty + text_bounds - (5 * boxh / 2) - infoh - box_space);

        putHeader(renderer, (selection.size() > 0 ? (std::string("Selected (") + std::to_string(selection.size()) + std::string(")")).c_str() : "Selected"), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

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

            fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);

            auto text = createText(selection_string.c_str(), FONT_MASON, 24, clrBK, splashw - 2 * text_space, TTF_STYLE_NORMAL);

            if (text)
            {
                renderText(renderer, text, intBE, startx + text_space, starty + text_bounds - 2 * boxh + text_space, 2 * (boxh - text_space), 0);

                SDL_FreeSurface(text);

                text = NULL;
            }
        }
        else
        {
            putText(renderer, "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
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

                    putHeader(renderer, buy_string.c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
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

                    putHeader(renderer, sell_string.c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::EQUIPMENT)
                {
                    std::string view_string = "View " + std::string(character.Name) + "'s items";

                    putHeader(renderer, view_string.c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, "Items for Sale", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
            }
            else
            {
                putHeader(renderer, "Items for Sale", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
            }
        }
        else
        {
            if (current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::EQUIPMENT)
                {
                    std::string view_string = "View " + std::string(character.Name) + "'s items";

                    putHeader(renderer, view_string.c_str(), font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, "Items for Sale", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
            }
            else
            {
                putHeader(renderer, "Items for Sale", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
            }
        }

        fillRect(renderer, listwidth, text_bounds - infoh, textx, texty + infoh, intBE);

        if (last - offset > 0)
        {
            for (auto i = 0; i < last - offset; i++)
            {
                if (Engine::FIND_LIST(selection, offset + i) >= 0)
                {
                    thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                }
                else
                {
                    drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
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

        Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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
            else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
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

                    auto items = std::vector<std::tuple<Equipment::Base, int>>();
                    auto codes_gained = std::vector<Codes::Base>();

                    for (auto i = 0; i < selection.size(); i++)
                    {
                        if (selection[i] >= 0 && selection[i] < shop.size())
                        {
                            auto item = std::get<0>(shop[selection[i]]);

                            auto price = std::get<1>(shop[selection[i]]);

                            auto supply = std::get<3>(shop[selection[i]]);

                            if (price > 0 && (supply == -1 || supply > 0))
                            {
                                items.push_back({item, selection[i]});

                                total += price;
                            }
                        }
                    }

                    if (items.size() > 0)
                    {
                        if (party.Money >= total)
                        {
                            Engine::GAIN_MONEY(party, -total);

                            message = "Item";

                            if (items.size() > 1)
                            {
                                message += "s";
                            }

                            message += " purchased: ";

                            for (auto i = 0; i < items.size(); i++)
                            {
                                auto item = std::get<0>(items[i]);
                                auto index = std::get<1>(items[i]);

                                character.Equipment.push_back(item);

                                if (index >= 0 && index < shop.size())
                                {
                                    auto supply = std::get<3>(shop[index]);
                                    auto codes = std::get<4>(shop[index]);

                                    if (supply > 0)
                                    {
                                        std::get<3>(shop[index]) = supply - 1;
                                    }

                                    if (codes.size() > 0)
                                    {
                                        if (!Engine::VERIFY_CODES(party, codes))
                                        {
                                            codes_gained.insert(codes_gained.end(), codes.begin(), codes.end());
                                        }

                                        Engine::GET_CODES(party, codes);
                                    }
                                }

                                if (i > 0)
                                {
                                    message += ", ";
                                }

                                message += item.Name;
                            }

                            if (codes_gained.size() > 0)
                            {
                                message += ", Codes gained: ";

                                for (auto i = 0; i < codes_gained.size(); i++)
                                {
                                    message += std::string(Codes::Prefix[codes_gained[i].Type]) + std::to_string(codes_gained[i].Code);
                                }
                            }

                            while (!Engine::VERIFY_EQUIPMENT_LIMIT(character))
                            {
                                inventoryScreen(window, renderer, party, character, -1, false);
                            }

                            displayMessage(message, intLB);

                            controls.clear();

                            controls = shopList(window, renderer, shop, offset, last, limit, textx, offsety);
                        }
                        else
                        {
                            displayMessage("You do not have enough silver coins!", intRD);
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

                    if (sold > unsold)
                    {
                        flash_color = intLB;
                    }
                    else
                    {
                        flash_color = intRD;
                    }

                    displayMessage(message, flash_color);
                }
            }
            else if (controls[current].Type == Control::Type::EQUIPMENT && !hold)
            {
                if (Engine::IS_ACTIVE(party, character))
                {
                    inventoryScreen(window, renderer, party, character, -1, false);
                }
            }
            else if (controls[current].Type == Control::Type::BACK && !hold)
            {
                done = true;
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

bool innScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, int RestPrice, bool CanRecharge)
{
    auto *title = "Legendary Kingdoms: Inn";

    auto text_space = 8;

    TTF_Init();

    auto font_mason = TTF_OpenFont(FONT_MASON, 24);
    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 28);
    auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

    TTF_SetFontKerning(font_dark11, 0);

    // Render window
    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title);

        auto current = -1;

        auto selection = std::vector<int>();

        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;

        int offset = 0;
        auto limit = (text_bounds - 2 * text_space - infoh) / (88);
        auto last = offset + limit;

        if (last > party.Members.size())
        {
            last = party.Members.size();
        }

        auto controls = innList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh);

        auto done = false;

        auto text_space = 8;

        bool scrollUp = false;
        bool selected = false;
        bool scrollDown = false;
        bool hold = false;
        int scrollSpeed = 1;

        std::string message = "";

        auto flash_message = false;

        auto flash_color = intRD;

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        while (!done)
        {
            // Fill the surface with background
            fillWindow(renderer, intWH);

            if (current >= 0 && current < controls.size())
            {
                auto cost = 0;

                if (selection.size() > 0)
                {
                    for (auto i = 0; i < selection.size(); i++)
                    {
                        if (selection[i] >= 0 && selection[i] < party.Members.size() && RestPrice >= 0)
                        {
                            if (controls[current].Type != Control::Type::FULL_RECOVERY)
                            {
                                cost += (Engine::IS_INJURED(party.Members[selection[i]])) ? RestPrice : 0;
                            }
                            else
                            {
                                cost += (Engine::IS_INJURED(party.Members[selection[i]])) ? RestPrice * (party.Members[selection[i]].MaximumHealth - Engine::HEALTH(party.Members[selection[i]])) : 0;
                            }
                        }
                    }

                    if (cost > 0 && RestPrice >= 0)
                    {
                        std::string heal_string = "";

                        if (controls[current].Type != Control::Type::FULL_RECOVERY)
                        {
                            heal_string = "Heal (" + std::to_string(cost);
                        }
                        else
                        {
                            heal_string = "Fully Recover (" + std::to_string(cost);
                        }

                        heal_string += ")";

                        putText(renderer, heal_string.c_str(), font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh - 1));
                    }
                    else
                    {
                        putText(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh - 1));
                    }
                }
                else
                {
                    putText(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh - 1));
                }
            }
            else
            {
                putText(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh - 1));
            }

            std::string party_string = "";

            if (selection.size() > 0)
            {
                for (auto i = 0; i < selection.size(); i++)
                {
                    if (i > 0)
                    {
                        party_string += "\n";
                    }

                    party_string += party.Members[selection[i]].Name;
                }
            }

            putText(renderer, selection.size() > 0 ? party_string.c_str() : "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);

            putHeader(renderer, "Money", font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty);
            putText(renderer, (std::to_string(party.Money) + std::string(" silver coins")).c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + infoh);

            if (RestPrice >= 0)
            {
                putHeader(renderer, "Healing Costs", font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + 2 * infoh + box_space - 1));
                putText(renderer, (RestPrice > 0 ? std::to_string(RestPrice) + std::string(" silver coins") : std::string("Free")).c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (3 * boxh + infoh + box_space));
            }

            fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);
            putHeader(renderer, "Party", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

            if (last - offset > 0)
            {
                for (auto i = 0; i < last - offset; i++)
                {
                    if (Engine::FIND_LIST(selection, offset + i) >= 0)
                    {
                        thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                    }
                    else
                    {
                        drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
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

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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

                        if (last > party.Members.size())
                        {
                            last = party.Members.size();
                        }

                        controls.clear();

                        controls = innList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh);

                        SDL_Delay(50);
                    }

                    if (offset <= 0)
                    {
                        current = -1;

                        selected = false;
                    }
                }
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
                {
                    if (party.Members.size() - last > 0)
                    {
                        if (offset < party.Members.size() - limit)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > party.Members.size() - limit)
                        {
                            offset = party.Members.size() - limit;
                        }

                        last = offset + limit;

                        if (last > party.Members.size())
                        {
                            last = party.Members.size();
                        }

                        controls.clear();

                        controls = innList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh);

                        SDL_Delay(50);

                        if (offset > 0)
                        {
                            if (controls[current].Type != Control::Type::SCROLL_DOWN)
                            {
                                current++;
                            }
                        }
                    }

                    if (party.Members.size() - last <= 0)
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
                            if (selection.size() < party.Members.size())
                            {
                                selection.push_back(offset + current);
                            }
                        }
                    }

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::HEAL1 && !hold)
                {
                    if (selection.size() > 0 && RestPrice >= 0)
                    {
                        auto cost = 0;

                        for (auto i = 0; i < selection.size(); i++)
                        {
                            cost += (Engine::IS_INJURED(party.Members[selection[i]])) ? RestPrice : 0;
                        }

                        if (party.Money < cost)
                        {
                            displayMessage("You do not have enough silver coins!", intRD);
                        }
                        else
                        {
                            for (auto i = 0; i < selection.size(); i++)
                            {
                                if (Engine::IS_INJURED(party.Members[selection[i]]))
                                {
                                    Engine::GAIN_HEALTH(party.Members[selection[i]], 1);
                                }
                            }

                            Engine::GAIN_MONEY(party, -cost);

                            if (cost > 0)
                            {
                                message = "Your party was healed for " + std::to_string(cost) + " silver coins.";
                            }
                            else
                            {
                                if (RestPrice > 0)
                                {
                                    message = "None of your party members are injured.";
                                }
                                else
                                {
                                    message = "Your party was healed for free.";
                                }
                            }

                            displayMessage(message, intLB);

                            offset = 0;

                            last = offset + limit;

                            if (last > party.Members.size())
                            {
                                last = party.Members.size();
                            }

                            controls.clear();

                            controls = innList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh);
                        }
                    }

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::FULL_RECOVERY && !hold)
                {
                    if (selection.size() > 0 && RestPrice >= 0)
                    {
                        auto cost = 0;

                        for (auto i = 0; i < selection.size(); i++)
                        {
                            cost += (Engine::IS_INJURED(party.Members[selection[i]])) ? RestPrice * (party.Members[selection[i]].MaximumHealth - Engine::HEALTH(party.Members[selection[i]])) : 0;
                        }

                        if (party.Money < cost)
                        {
                            displayMessage("You do not have enough silver coins!", intRD);
                        }
                        else
                        {
                            for (auto i = 0; i < selection.size(); i++)
                            {
                                if (Engine::IS_INJURED(party.Members[selection[i]]))
                                {
                                    Engine::GAIN_HEALTH(party.Members[selection[i]], party.Members[selection[i]].MaximumHealth - Engine::HEALTH(party.Members[selection[i]]));
                                }
                            }

                            Engine::GAIN_MONEY(party, -cost);

                            if (cost > 0)
                            {
                                message = "Your party was healed for " + std::to_string(cost) + " silver coins.";
                            }
                            else
                            {
                                if (RestPrice > 0)
                                {
                                    message = "None of your party members are injured.";
                                }
                                else
                                {
                                    message = "Your party was healed for free.";
                                }
                            }

                            displayMessage(message, intLB);

                            offset = 0;

                            last = offset + limit;

                            if (last > party.Members.size())
                            {
                                last = party.Members.size();
                            }

                            controls.clear();

                            controls = innList(window, renderer, party.Members, offset, last, limit, textx, texty + infoh);
                        }
                    }

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::RECHARGE && !hold)
                {
                    if (CanRecharge && Engine::SPELLCASTERS(party) > 0)
                    {
                        auto character = -1;

                        if (Engine::SPELLCASTERS(party) == 1)
                        {
                            character = Engine::FIRST_CASTER(party);
                        }
                        else
                        {
                            character = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::PARTY);
                        }

                        if (Engine::IS_ACTIVE(party, character))
                        {
                            if (party.Members[character].SpellCaster)
                            {
                                if (party.Members[character].SpellBook.size() > 0)
                                {
                                    rechargeSpells(window, renderer, party, party.Members[character]);
                                }
                                else
                                {
                                    displayMessage(std::string(party.Members[character].Name) + "'s spellbook is empty!", intRD);
                                }
                            }
                            else
                            {
                                displayMessage(std::string(party.Members[character].Name) + " is not a spell caster!", intRD);
                            }
                        }
                        else
                        {
                            if (character >= 0 && character < party.Members.size())
                            {
                                displayMessage(std::string(party.Members[character].Name) + " cannot recharge spells!", intRD);
                            }
                        }
                    }
                    else if (Engine::SPELLCASTERS(party) > 0)
                    {
                        displayMessage("You cannot recharge your spells here!", intRD);
                    }
                    else
                    {
                        displayMessage("You do not have any spell casters in your party!", intRD);
                    }

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = true;
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

bool vaultScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Character::Base &character)
{
    auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

    auto infoh = 48;
    auto boxh = (int)(0.150 * SCREEN_HEIGHT);

    auto font_size = 28;
    auto text_space = 8;
    auto scrollSpeed = 1;
    auto limit = (text_bounds - 2 * text_space - infoh) / (font_size + 7 * text_space / 2);

    auto offset = 0;

    auto last = offset + limit;

    if (last > party.Vault.size())
    {
        last = party.Vault.size();
    }

    std::string message = "";

    auto flash_message = false;

    auto flash_color = intRD;

    Uint32 start_ticks = 0;

    Uint32 duration = 3000;

    // Lambda functions for displaying flash messages
    auto displayMessage = [&](std::string msg, Uint32 color)
    {
        flash_message = true;

        message = msg;

        flash_color = color;

        start_ticks = SDL_GetTicks();
    };

    auto done = false;

    auto listwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

    auto scrolly = (texty + text_bounds - arrow_size - border_space);
    auto offsety = (texty + infoh);

    auto controls = vaultList(window, renderer, party.Vault, offset, last, limit, offsety, scrolly);

    TTF_Init();

    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
    auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);
    auto font_mason = TTF_OpenFont(FONT_MASON, 24);

    TTF_SetFontKerning(font_dark11, 0);

    auto selected = false;
    auto current = -1;
    auto scrollUp = false;
    auto scrollDown = false;
    auto hold = false;

    auto selection = -1;

    while (!done)
    {
        last = offset + limit;

        if (last > party.Vault.size())
        {
            last = party.Vault.size();
        }

        SDL_SetWindowTitle(window, "Legendary Kingdoms: The Vault");

        fillWindow(renderer, intWH);

        if (splash)
        {
            fitImage(renderer, splash, startx, starty, splashw, text_bounds);
        }

        putHeader(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

        if (selection >= 0 && selection < party.Vault.size())
        {
            putText(renderer, party.Vault[selection].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
        }
        else
        {
            putText(renderer, "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
        }

        if (selection >= 0 && selection < party.Vault.size())
        {
            auto item = party.Vault[selection];

            if (current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::USE)
                {
                    putHeader(renderer, (std::string("Use the ") + std::string(item.Name)).c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::TRANSFER)
                {
                    putHeader(renderer, (std::string("Transfer the ") + std::string(item.Name)).c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, "Items inside the Vault", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
            }
            else
            {
                putHeader(renderer, "Items inside the Vault", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
            }
        }
        else
        {
            if (current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::USE)
                {
                    putHeader(renderer, "Use item", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::TRANSFER)
                {
                    putHeader(renderer, "Transfer item to another party member", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, "Items inside the Vault", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
            }
            else
            {
                putHeader(renderer, "Items inside the Vault", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
            }
        }

        fillRect(renderer, listwidth, text_bounds - infoh, textx, texty + infoh, intBE);

        if (last - offset > 0)
        {
            for (auto i = 0; i < last - offset; i++)
            {
                if (selection != offset + i)
                {
                    drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                }
                else
                {
                    thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
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

        Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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

                    if (last > party.Vault.size())
                    {
                        last = party.Vault.size();
                    }

                    controls.clear();

                    controls = vaultList(window, renderer, party.Vault, offset, last, limit, offsety, scrolly);

                    SDL_Delay(50);
                }

                if (offset <= 0)
                {
                    current = -1;

                    selected = false;
                }
            }
            else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
            {
                if (party.Vault.size() - last > 0)
                {
                    if (offset < party.Vault.size() - limit)
                    {
                        offset += scrollSpeed;
                    }

                    if (offset > party.Vault.size() - limit)
                    {
                        offset = party.Vault.size() - limit;
                    }

                    last = offset + limit;

                    if (last > party.Vault.size())
                    {
                        last = party.Vault.size();
                    }

                    controls.clear();

                    controls = vaultList(window, renderer, party.Vault, offset, last, limit, offsety, scrolly);

                    SDL_Delay(50);

                    if (offset > 0)
                    {
                        if (controls[current].Type != Control::Type::SCROLL_DOWN)
                        {
                            current++;
                        }
                    }
                }

                if (party.Vault.size() - last <= 0)
                {
                    selected = false;

                    current = -1;
                }
            }
            else if (controls[current].Type == Control::Type::ACTION && !hold)
            {
                if ((current + offset >= 0) && (current + offset) < party.Vault.size())
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
                if (selection >= 0 && selection < party.Vault.size())
                {
                    auto used_up = false;

                    auto item = party.Vault[selection];

                    if (item.Type == Equipment::Type::HEALING_POTION)
                    {
                        if (character.Health < character.MaximumHealth)
                        {
                            displayMessage(std::string(character.Name) + " recovers up to 5 Health points!", intLB);

                            Engine::GAIN_HEALTH(character, 5);

                            used_up = true;
                        }
                        else
                        {
                            displayMessage(std::string(character.Name) + " is not injured!", intRD);
                        }
                    }

                    if (used_up)
                    {
                        if (party.Vault.size() > 0)
                        {
                            party.Vault.erase(party.Vault.begin() + selection);

                            if (offset > 0)
                            {
                                offset--;
                            }

                            last = offset + limit;

                            if (last > party.Vault.size())
                            {
                                last = party.Vault.size();
                            }

                            controls.clear();

                            controls = vaultList(window, renderer, party.Vault, offset, last, limit, offsety, scrolly);
                        }

                        selection = -1;

                        current = -1;

                        selected = false;
                    }
                }
            }
            else if (controls[current].Type == Control::Type::TRANSFER && !hold)
            {
                if (selection >= 0 && selection < party.Vault.size())
                {
                    auto item = party.Vault[selection];

                    if (Engine::COUNT(party) > 1)
                    {
                        if (character.Team != Team::Type::SOLO)
                        {
                            auto target = selectPartyMember(window, renderer, party, character.Team, item, Control::Type::EQUIPMENT);

                            if (target >= 0 && target < party.Members.size() && party.Members[target].Type != Character::Type::SKULLCRACKER)
                            {
                                party.Vault.erase(party.Vault.begin() + selection);

                                party.Members[target].Equipment.push_back(item);

                                if (offset > 0)
                                {
                                    offset--;
                                }

                                last = offset + limit;

                                if (last > party.Vault.size())
                                {
                                    last = party.Vault.size();
                                }

                                controls.clear();

                                controls = vaultList(window, renderer, party.Vault, offset, last, limit, offsety, scrolly);

                                message = itemString(item);

                                message += " transferred to " + std::string(party.Members[target].Name) + "!";

                                displayMessage(message, intLB);

                                selected = false;

                                current = -1;

                                selection = -1;
                            }
                            else if (target >= 0 && target < party.Members.size() && party.Members[target].Type == Character::Type::SKULLCRACKER)
                            {
                                displayMessage(std::string(party.Members[target].Name) + " refuses to accept it!", intRD);

                                selected = false;

                                current = -1;

                                selection = -1;
                            }
                        }
                        else
                        {
                            party.Vault.erase(party.Vault.begin() + selection);

                            character.Equipment.push_back(item);

                            if (offset > 0)
                            {
                                offset--;
                            }

                            last = offset + limit;

                            if (last > party.Vault.size())
                            {
                                last = party.Vault.size();
                            }

                            controls.clear();

                            controls = vaultList(window, renderer, party.Vault, offset, last, limit, offsety, scrolly);

                            message = itemString(item);

                            message += " transferred to " + std::string(character.Name) + "!";

                            displayMessage(message, intLB);

                            selected = false;

                            current = -1;

                            selection = -1;
                        }
                    }
                    else
                    {
                        party.Vault.erase(party.Vault.begin() + selection);

                        character.Equipment.push_back(item);

                        if (offset > 0)
                        {
                            offset--;
                        }

                        last = offset + limit;

                        if (last > party.Vault.size())
                        {
                            last = party.Vault.size();
                        }

                        controls.clear();

                        controls = vaultList(window, renderer, party.Vault, offset, last, limit, offsety, scrolly);

                        message = itemString(item);

                        message += " transferred to " + std::string(character.Name) + "!";

                        displayMessage(message, intLB);

                        selected = false;

                        current = -1;

                        selection = -1;
                    }
                }
            }
            else if (controls[current].Type == Control::Type::BACK && !hold)
            {
                done = true;
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

    auto infoh = 48;
    auto boxh = (int)(0.150 * SCREEN_HEIGHT);

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

    // Lambda functions for displaying flash messages
    auto displayMessage = [&](std::string msg, Uint32 color)
    {
        flash_message = true;

        message = msg;

        flash_color = color;

        start_ticks = SDL_GetTicks();
    };

    auto done = false;

    auto listwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

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

            displayMessage(message, intRD);
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
            putText(renderer, "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
        }

        if (selection >= 0 && selection < character.Equipment.size())
        {
            auto item = character.Equipment[selection];

            if (current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::USE)
                {
                    putHeader(renderer, (std::string("Use the ") + std::string(item.Name)).c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::DROP)
                {
                    putHeader(renderer, (std::string("Drop the ") + std::string(item.Name)).c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::TRANSFER)
                {
                    putHeader(renderer, (std::string("Transfer the ") + std::string(item.Name)).c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::VAULT)
                {
                    putHeader(renderer, (std::string("Send the ") + std::string(item.Name) + " to the vault").c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, (std::string(character.Name) + "'s items").c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
            }
            else
            {
                putHeader(renderer, (std::string(character.Name) + "'s items").c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
            }
        }
        else
        {
            if (current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::USE)
                {
                    putHeader(renderer, "Use item", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::DROP)
                {
                    putHeader(renderer, "Drop item", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::TRANSFER)
                {
                    putHeader(renderer, "Transfer item to another party member", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::VAULT)
                {
                    putHeader(renderer, "Access magic vault", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, (std::string(character.Name) + "'s items").c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
            }
            else
            {
                putHeader(renderer, (std::string(character.Name) + "'s items").c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
            }
        }

        fillRect(renderer, listwidth, text_bounds - infoh, textx, texty + infoh, intBE);

        if (last - offset > 0)
        {
            for (auto i = 0; i < last - offset; i++)
            {
                if (selection != offset + i)
                {
                    drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                }
                else
                {
                    thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
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

        Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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
            else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
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

                            for (auto i = 0; i < party.Members.size(); i++)
                            {
                                Engine::GAIN_STATUS(party.Members[i], Character::Status::ENRAGED);
                            }

                            displayMessage("Your party gains +1 Fighting score until end of combat!", intLB);
                        }
                        else
                        {
                            displayMessage("You cannot use the " + std::string(item.Name) + " when not in combat!", intRD);
                        }
                    }
                    else if (item.Type == Equipment::Type::POTION_OF_INVULNERABILITY)
                    {
                        if (InCombat)
                        {
                            used_up = true;

                            Engine::GAIN_STATUS(character, Character::Status::POTION_OF_INVULNERABILITY);

                            displayMessage(std::string(character.Name) + " becomes Invulnerable!", intLB);
                        }
                        else
                        {
                            displayMessage("You cannot use the " + std::string(item.Name) + " when not in combat!", intRD);
                        }
                    }
                    else if (item.Type == Equipment::Type::HEALING_POTION)
                    {
                        if (character.Health < character.MaximumHealth)
                        {
                            displayMessage(std::string(character.Name) + " recovers up to 5 Health points!", intLB);

                            Engine::GAIN_HEALTH(character, 5);

                            used_up = true;
                        }
                        else
                        {
                            displayMessage(std::string(character.Name) + " is not injured!", intRD);
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
                    }

                    selected = false;
                }
            }
            else if (controls[current].Type == Control::Type::DROP && !hold)
            {
                if (selection >= 0 && selection < character.Equipment.size())
                {
                    if (character.Type != Character::Type::SKULLCRACKER)
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

                        message = itemString(item);

                        message += " dropped!";

                        displayMessage(message, intRD);
                    }
                    else
                    {
                        displayMessage("Skullcracker refuses!", intRD);
                    }

                    selected = false;

                    current = -1;

                    selection = -1;
                }
            }
            else if (controls[current].Type == Control::Type::TRANSFER && !hold)
            {
                if (selection >= 0 && selection < character.Equipment.size())
                {
                    if (Engine::COUNT(party) > 1)
                    {
                        if (character.Team != Team::Type::SOLO)
                        {
                            auto item = character.Equipment[selection];

                            if (character.Type != Character::Type::SKULLCRACKER)
                            {
                                auto target = selectPartyMember(window, renderer, party, character.Team, item, Control::Type::EQUIPMENT);

                                if (target >= 0 && target < party.Members.size())
                                {
                                    if (party.Members[target].Type != character.Type && party.Members[target].Type != Character::Type::SKULLCRACKER)
                                    {
                                        character.Equipment.erase(character.Equipment.begin() + selection);

                                        party.Members[target].Equipment.push_back(item);

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

                                        message = itemString(item);

                                        message += " transferred to " + std::string(party.Members[target].Name) + "!";

                                        displayMessage(message, intLB);

                                        selected = false;

                                        current = -1;

                                        selection = -1;
                                    }
                                    else
                                    {
                                        if (party.Members[target].Type != Character::Type::SKULLCRACKER)
                                        {
                                            displayMessage("You can only transfer to another party member!", intRD);
                                        }
                                        else
                                        {
                                            displayMessage("Skullcracker refuses!", intRD);
                                        }
                                    }
                                }
                            }
                            else
                            {
                                displayMessage("Skullcracker refuses!", intRD);
                            }
                        }
                        else
                        {
                            displayMessage(std::string(character.Name) + " is alone right now!", intRD);
                        }
                    }
                    else
                    {
                        displayMessage("There is no one else in your party!", intRD);
                    }
                }
            }
            else if (controls[current].Type == Control::Type::VAULT && !hold)
            {
                if (Engine::VERIFY_CODES(party, {Codes::Type::MAGIC_VAULT}))
                {
                    if (!InCombat)
                    {
                        if (selection >= 0 && selection < character.Equipment.size())
                        {
                            auto item = character.Equipment[selection];

                            character.Equipment.erase(character.Equipment.begin() + selection);

                            party.Vault.push_back(item);

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

                            message = itemString(item);

                            message += " transferred to the Vault!";

                            displayMessage(message, intLB);

                            selected = false;

                            current = -1;

                            selection = -1;
                        }
                        else
                        {
                            vaultScreen(window, renderer, party, character);

                            offset = 0;

                            last = offset + limit;

                            if (last > character.Equipment.size())
                            {
                                last = character.Equipment.size();
                            }

                            controls.clear();

                            controls = equipmentList(window, renderer, character.Equipment, offset, last, limit, offsety, scrolly);

                            current = -1;

                            selection = -1;
                        }
                    }
                    else
                    {
                        displayMessage("You cannot access this while in combat!", intRD);
                    }
                }
                else
                {
                    displayMessage("The Vault is not accessible at this time!", intRD);
                }
            }
            else if (controls[current].Type == Control::Type::BACK && !hold)
            {
                if ((equipment_limit > -1 && !Engine::VERIFY_EQUIPMENT_LIMIT(character, equipment_limit)) || !Engine::VERIFY_EQUIPMENT_LIMIT(character))
                {
                    displayMessage("You are carrying too many items! Drop or transfer excess items.", intRD);
                }
                else
                {
                    done = true;
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

    return false;
}

bool spellBook(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Character::Base &character, int spells_limit)
{
    auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

    auto infoh = 48;
    auto boxh = (int)(0.150 * SCREEN_HEIGHT);
    auto box_space = 10;

    auto font_size = 28;
    auto text_space = 8;
    auto scrollSpeed = 1;
    auto booksize = (int)(2 * (text_bounds) / 3 - infoh - box_space);
    auto limit = (int)((booksize - 2 * text_space) / (88));

    auto offset = 0;

    auto last = offset + limit;

    if (last > character.SpellBook.size())
    {
        last = character.SpellBook.size();
    }

    std::string message = "";

    auto flash_message = false;

    auto flash_color = intRD;

    Uint32 start_ticks = 0;

    Uint32 duration = 3000;

    // Lambda functions for displaying flash messages
    auto displayMessage = [&](std::string msg, Uint32 color)
    {
        flash_message = true;

        message = msg;

        flash_color = color;

        start_ticks = SDL_GetTicks();
    };

    auto done = false;

    auto listwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

    auto scrolly = startx + infoh + booksize - buttonh - text_space + 1;
    auto offsety = (texty + infoh);

    auto controls = spellList(window, renderer, character.SpellBook, offset, last, limit, textx, offsety, scrolly);

    TTF_Init();

    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
    auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);
    auto font_mason = TTF_OpenFont(FONT_MASON, 24);

    TTF_SetFontKerning(font_dark11, 0);

    auto selected = false;
    auto current = -1;
    auto scrollUp = false;
    auto scrollDown = false;
    auto hold = false;

    auto selection = -1;

    while (!done)
    {
        if (!Engine::VERIFY_SPELL_LIMIT(character) || (spells_limit > -1 && !Engine::VERIFY_SPELL_LIMIT(character, spells_limit)))
        {
            if (spells_limit > -1)
            {
                if (spells_limit > 0)
                {
                    message = "Your spellbook is holding more than " + std::to_string(spells_limit) + " spell";

                    if (spells_limit > 1)
                    {
                        message += "s";
                    }

                    message += ". Unlearn some spells from your spellbook.";
                }
                else
                {
                    message = "Unlearn all of your spells.";
                }
            }
            else
            {
                message = "Your spellbook is holding too many spells! You must unlearn spells from your spellbook.";
            }

            displayMessage(message, intRD);
        }

        last = offset + limit;

        if (last > character.SpellBook.size())
        {
            last = character.SpellBook.size();
        }

        SDL_SetWindowTitle(window, "Legendary Kingdoms: Spellbook");

        fillWindow(renderer, intWH);

        if (splash)
        {
            fitImage(renderer, splash, startx, starty, splashw, text_bounds);
        }

        putHeader(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

        if (selection >= 0 && selection < character.SpellBook.size())
        {
            putText(renderer, character.SpellBook[selection].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
        }
        else
        {
            putText(renderer, "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
        }

        if (selection >= 0 && selection < character.SpellBook.size())
        {
            auto spell = character.SpellBook[selection];

            if (current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::SPELL)
                {
                    putHeader(renderer, (std::string("Cast ") + std::string(spell.Name)).c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::UNLEARN)
                {
                    putHeader(renderer, (std::string("Unlearn ") + std::string(spell.Name)).c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, (std::string(character.Name) + "'s spells").c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
            }
            else
            {
                putHeader(renderer, (std::string(character.Name) + "'s spellbook").c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
            }
        }
        else
        {
            if (current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::SPELL)
                {
                    putHeader(renderer, "Cast spell", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else if (controls[current].Type == Control::Type::UNLEARN)
                {
                    putHeader(renderer, "Unlearn Spell", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, (std::string(character.Name) + "'s spellbook").c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
            }
            else
            {
                putHeader(renderer, (std::string(character.Name) + "'s spellbook").c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
            }
        }

        fillRect(renderer, listwidth, booksize, textx, texty + infoh, intBE);

        if (last - offset > 0)
        {
            for (auto i = 0; i < last - offset; i++)
            {
                if (selection != offset + i)
                {
                    if (character.SpellBook[offset + i].Charged)
                    {
                        drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                    }
                    else
                    {
                        drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intRD);
                    }
                }
                else
                {
                    thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                }
            }
        }

        if (current >= 0 && current < controls.size() && controls[current].Type == Control::Type::ACTION)
        {
            if (((current + offset) >= 0) && ((current + offset) < character.SpellBook.size()))
            {
                fillRect(renderer, listwidth, text_bounds / 3, textx, texty + infoh + booksize + box_space, intLB);

                auto text = createText(character.SpellBook[current + offset].Description, FONT_GARAMOND, font_size, clrWH, listwidth - 2 * text_space, TTF_STYLE_NORMAL);

                renderText(renderer, text, intLB, textx + text_space, texty + infoh + booksize + box_space + text_space, text_bounds / 3 - texty, 0);

                SDL_FreeSurface(text);

                text = NULL;
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

        Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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

                    if (last > character.SpellBook.size())
                    {
                        last = character.SpellBook.size();
                    }

                    controls.clear();

                    controls = spellList(window, renderer, character.SpellBook, offset, last, limit, textx, offsety, scrolly);

                    SDL_Delay(50);
                }

                if (offset <= 0)
                {
                    current = -1;

                    selected = false;
                }
            }
            else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
            {
                if (character.SpellBook.size() - last > 0)
                {
                    if (offset < character.SpellBook.size() - limit)
                    {
                        offset += scrollSpeed;
                    }

                    if (offset > character.SpellBook.size() - limit)
                    {
                        offset = character.SpellBook.size() - limit;
                    }

                    last = offset + limit;

                    if (last > character.SpellBook.size())
                    {
                        last = character.SpellBook.size();
                    }

                    controls.clear();

                    controls = spellList(window, renderer, character.SpellBook, offset, last, limit, textx, offsety, scrolly);

                    SDL_Delay(50);

                    if (offset > 0)
                    {
                        if (controls[current].Type != Control::Type::SCROLL_DOWN)
                        {
                            current++;
                        }
                    }
                }

                if (character.SpellBook.size() - last <= 0)
                {
                    selected = false;

                    current = -1;
                }
            }
            else if (controls[current].Type == Control::Type::ACTION && !hold)
            {
                if ((current + offset >= 0) && (current + offset) < character.SpellBook.size())
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
            else if (controls[current].Type == Control::Type::SPELL && !hold)
            {
                if (selection >= 0 && selection < character.SpellBook.size())
                {
                    auto used_up = false;

                    if (character.SpellBook[selection].Charged && Engine::CAN_SPEAK(character))
                    {
                        if (character.SpellBook[selection].Type == Spells::Type::MAGIC_CABINET)
                        {
                            if (!Engine::VERIFY_CODES(party, {Codes::Type::MAGIC_VAULT}))
                            {
                                if (!Engine::VERIFY_CODES(party, {Codes::Type::NO_VAULT_ACCESS}))
                                {
                                    used_up = true;

                                    Engine::GET_CODES(party, {Codes::Type::MAGIC_VAULT});
                                }
                                else
                                {
                                    displayMessage("The vault is not accessible at this time!", intRD);
                                }
                            }
                            else
                            {
                                displayMessage("The magic vault is already accessible!", intRD);
                            }
                        }
                        else if (character.SpellBook[selection].Type == Spells::Type::SOOTHING_TOUCH)
                        {
                            auto target = -1;

                            if (Engine::COUNT(party) == 1)
                            {
                                target = Engine::FIRST(party);
                            }
                            else
                            {
                                target = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::SPELL_TARGET);
                            }

                            if (target >= 0 && target < party.Members.size())
                            {
                                if (party.Members[target].Health <= 0 || party.Members[target].Health == party.Members[target].MaximumHealth || Engine::IS_CAPTURED(party.Members[target]))
                                {
                                    message = std::string(party.Members[target].Name);

                                    if (party.Members[target].Health <= 0)
                                    {
                                        message += " is dead!";
                                    }
                                    else if (Engine::IS_CAPTURED(party.Members[target]))
                                    {
                                        message += " has been captured!";
                                    }
                                    else
                                    {
                                        message += " is not injured!";
                                    }

                                    displayMessage(message, intRD);
                                }
                                else
                                {
                                    Engine::GAIN_HEALTH(party.Members[target], 5);

                                    used_up = true;
                                }
                            }
                        }
                        else
                        {
                            displayMessage(std::string(character.Name) + " cannot cast " + std::string(character.SpellBook[selection].Name) + " at this time!", intRD);
                        }
                    }
                    else
                    {
                        displayMessage(std::string(character.Name) + " cannot cast " + std::string(character.SpellBook[selection].Name) + " at this time!", intRD);
                    }

                    if (used_up)
                    {
                        displayMessage(std::string(character.Name) + " casts " + std::string(character.SpellBook[selection].Name), intLB);

                        character.SpellBook[selection].Charged = false;
                    }
                }

                selection = -1;

                current = -1;

                selected = false;
            }
            else if (controls[current].Type == Control::Type::UNLEARN && !hold)
            {
                if (selection >= 0 && selection < character.SpellBook.size())
                {
                    auto spell = character.SpellBook[selection];

                    character.SpellBook.erase(character.SpellBook.begin() + selection);

                    if (offset > 0)
                    {
                        offset--;
                    }

                    last = offset + limit;

                    if (last > character.SpellBook.size())
                    {
                        last = character.SpellBook.size();
                    }

                    controls.clear();

                    controls = spellList(window, renderer, character.SpellBook, offset, last, limit, textx, offsety, scrolly);

                    message = spell.Name;

                    message += " erased from spellbook!";

                    displayMessage(message, intRD);

                    selected = false;

                    current = -1;

                    selection = -1;
                }
            }
            else if (controls[current].Type == Control::Type::BACK && !hold)
            {
                if (!Engine::VERIFY_SPELL_LIMIT(character))
                {
                    displayMessage("Your spellbook is holding too many spells! You must unlearn spells from your spellbook.", intRD);
                }
                else
                {
                    done = true;
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

    return false;
}

bool rechargeSpells(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Character::Base &character)
{
    auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

    auto infoh = 48;
    auto boxh = (int)(0.150 * SCREEN_HEIGHT);
    auto box_space = 10;

    auto font_size = 28;
    auto text_space = 8;
    auto scrollSpeed = 1;
    auto booksize = (int)(2 * (text_bounds) / 3 - infoh - box_space);
    auto limit = (int)((booksize - 2 * text_space) / (88));

    auto offset = 0;

    auto last = offset + limit;

    if (last > character.SpellBook.size())
    {
        last = character.SpellBook.size();
    }

    std::string message = "";

    auto flash_message = false;

    auto flash_color = intRD;

    Uint32 start_ticks = 0;

    Uint32 duration = 3000;

    // Lambda functions for displaying flash messages
    auto displayMessage = [&](std::string msg, Uint32 color)
    {
        flash_message = true;

        message = msg;

        flash_color = color;

        start_ticks = SDL_GetTicks();
    };

    auto done = false;

    auto listwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

    auto scrolly = startx + infoh + booksize - buttonh - text_space + 1;
    auto offsety = (texty + infoh);

    auto controls = rechargeList(window, renderer, character.SpellBook, offset, last, limit, textx, offsety, scrolly);

    TTF_Init();

    auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
    auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);
    auto font_mason = TTF_OpenFont(FONT_MASON, 24);

    TTF_SetFontKerning(font_dark11, 0);

    auto selected = false;
    auto current = -1;
    auto scrollUp = false;
    auto scrollDown = false;
    auto hold = false;

    auto selection = -1;

    if (Engine::VERIFY_CODES(party, {Codes::Type::RECHARGE_COSTS_HALF}))
    {
        displayMessage("The Everchild will sponsor your spellcasters, allowing you to recharge your spells at half the usual cost.", intLB);
    }

    while (!done)
    {
        last = offset + limit;

        if (last > character.SpellBook.size())
        {
            last = character.SpellBook.size();
        }

        SDL_SetWindowTitle(window, "Legendary Kingdoms: Recharge Spells");

        fillWindow(renderer, intWH);

        if (splash)
        {
            fitImage(renderer, splash, startx, starty, splashw, text_bounds);
        }

        putHeader(renderer, "Money", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + 2 * infoh + box_space - 1));
        putText(renderer, (std::to_string(party.Money) + std::string(" silver coins")).c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (3 * boxh + infoh + box_space));

        putHeader(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

        if (selection >= 0 && selection < character.SpellBook.size())
        {
            putText(renderer, character.SpellBook[selection].Name, font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
        }
        else
        {
            putText(renderer, "(None)", font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
        }

        if (selection >= 0 && selection < character.SpellBook.size())
        {
            auto spell = character.SpellBook[selection];

            if (current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::RECHARGE)
                {
                    putHeader(renderer, (std::string("Recharge ") + std::string(spell.Name)).c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, (std::string(character.Name) + "'s spells").c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
            }
            else
            {
                putHeader(renderer, (std::string(character.Name) + "'s spellbook").c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
            }
        }
        else
        {
            if (current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::RECHARGE)
                {
                    putHeader(renderer, "Recharge spell", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
                else
                {
                    putHeader(renderer, (std::string(character.Name) + "'s spellbook").c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
                }
            }
            else
            {
                putHeader(renderer, (std::string(character.Name) + "'s spellbook").c_str(), font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);
            }
        }

        fillRect(renderer, listwidth, booksize, textx, texty + infoh, intBE);

        if (last - offset > 0)
        {
            for (auto i = 0; i < last - offset; i++)
            {
                if (selection != offset + i)
                {
                    if (character.SpellBook[offset + i].Charged)
                    {
                        drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                    }
                    else
                    {
                        drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intRD);
                    }
                }
                else
                {
                    thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                }
            }
        }

        if (current >= 0 && current < controls.size() && controls[current].Type == Control::Type::ACTION)
        {
            if (((current + offset) >= 0) && ((current + offset) < character.SpellBook.size()))
            {
                fillRect(renderer, listwidth, text_bounds / 3, textx, texty + infoh + booksize + box_space, intLB);

                auto text = createText(character.SpellBook[current + offset].Description, FONT_GARAMOND, font_size, clrWH, listwidth - 2 * text_space, TTF_STYLE_NORMAL);

                renderText(renderer, text, intLB, textx + text_space, texty + infoh + booksize + box_space + text_space, text_bounds / 3 - texty, 0);

                SDL_FreeSurface(text);

                text = NULL;
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

        Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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

                    if (last > character.SpellBook.size())
                    {
                        last = character.SpellBook.size();
                    }

                    controls.clear();

                    controls = rechargeList(window, renderer, character.SpellBook, offset, last, limit, textx, offsety, scrolly);

                    SDL_Delay(50);
                }

                if (offset <= 0)
                {
                    current = -1;

                    selected = false;
                }
            }
            else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
            {
                if (character.SpellBook.size() - last > 0)
                {
                    if (offset < character.SpellBook.size() - limit)
                    {
                        offset += scrollSpeed;
                    }

                    if (offset > character.SpellBook.size() - limit)
                    {
                        offset = character.SpellBook.size() - limit;
                    }

                    last = offset + limit;

                    if (last > character.SpellBook.size())
                    {
                        last = character.SpellBook.size();
                    }

                    controls.clear();

                    controls = rechargeList(window, renderer, character.SpellBook, offset, last, limit, textx, offsety, scrolly);

                    SDL_Delay(50);

                    if (offset > 0)
                    {
                        if (controls[current].Type != Control::Type::SCROLL_DOWN)
                        {
                            current++;
                        }
                    }
                }

                if (character.SpellBook.size() - last <= 0)
                {
                    selected = false;

                    current = -1;
                }
            }
            else if (controls[current].Type == Control::Type::ACTION && !hold)
            {
                if ((current + offset >= 0) && (current + offset) < character.SpellBook.size())
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
            else if (controls[current].Type == Control::Type::RECHARGE && !hold)
            {
                if (selection >= 0 && selection < character.SpellBook.size())
                {
                    if (!character.SpellBook[selection].Charged)
                    {
                        auto recharge_cost = character.SpellBook[selection].Recharge;

                        if (Engine::VERIFY_CODES(party, {Codes::Type::RECHARGE_COSTS_HALF}))
                        {
                            recharge_cost /= 2;
                        }

                        if (party.Money >= recharge_cost)
                        {
                            Engine::GAIN_MONEY(party, -recharge_cost);

                            character.SpellBook[selection].Charged = true;

                            displayMessage(std::string(character.SpellBook[selection].Name) + " charged!", intLB);
                        }
                        else
                        {
                            displayMessage("You cannot afford to recharge " + std::string(character.SpellBook[selection].Name) + "!", intRD);
                        }
                    }
                    else
                    {
                        displayMessage(std::string(character.SpellBook[selection].Name) + " is charged!", intRD);
                    }
                }

                selection = -1;

                current = -1;

                selected = false;
            }
            else if (controls[current].Type == Control::Type::BACK && !hold)
            {
                done = true;
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

std::vector<int> selectArmyUnits(SDL_Window *window, SDL_Renderer *renderer, std::vector<Army::Base> army, Location::Type garrison, int num_limit)
{
    auto selected_units = std::vector<int>();

    if (army.size() > 0)
    {
        auto font_size = 28;
        auto text_space = 8;
        auto scrollSpeed = 1;
        auto offset = 0;
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto limit = (text_bounds - 2 * text_space - infoh) / (88);
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

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        auto listwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

        auto controls = armyList(window, renderer, army, offset, last, limit, textx, texty + infoh, false);

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto selected = false;
        auto current = -1;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto selection = std::vector<int>();

        auto done = false;

        while (!done)
        {
            last = offset + limit;

            if (last > army.size())
            {
                last = army.size();
            }

            SDL_SetWindowTitle(window, "Legendary Kingdoms: Select Army Units");

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

            putText(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + infoh - 1));
            putText(renderer, selection.size() > 0 ? army_string.c_str() : "(None)", font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 3 * boxh, startx, starty + text_bounds - 3 * boxh);

            putHeader(renderer, "Select units", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);

            fillRect(renderer, listwidth, text_bounds - infoh, textx, texty + infoh, intBE);

            if (last - offset > 0)
            {
                for (auto i = 0; i < last - offset; i++)
                {
                    if (Engine::FIND_LIST(selection, offset + i) >= 0)
                    {
                        thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                    }
                    else
                    {
                        drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
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

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
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
                    auto result = Engine::FIND_LIST(selection, offset + current);

                    if (result >= 0 && result < army.size())
                    {
                        selection.erase(selection.begin() + result);
                    }
                    else
                    {
                        if (offset + current >= 0 && offset + current < army.size())
                        {
                            if (garrison == Location::Type::NONE || army[offset + current].Garrison == garrison)
                            {
                                if (selection.size() < num_limit)
                                {
                                    selection.push_back(offset + current);
                                }
                            }
                            else
                            {
                                displayMessage("You can only select units from the " + std::string(Location::Description[garrison]) + " garrison!", intRD);
                            }
                        }
                    }

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::CONFIRM && !hold)
                {
                    if (selection.size() > 0)
                    {
                        if (selection.size() >= num_limit || selection.size() >= Engine::COUNT(army, garrison))
                        {
                            selected_units = selection;

                            done = true;
                        }
                        else
                        {
                            displayMessage("Please complete your selection!", intRD);
                        }
                    }
                    else
                    {
                        displayMessage("Please complete your selection!", intRD);
                    }
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    selected_units = {};

                    done = true;
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

    return selected_units;
}

std::vector<int> selectArmyUnits(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Location::Type garrison, int num_limit)
{
    return selectArmyUnits(window, renderer, party.Army, garrison, num_limit);
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
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto limit = (text_bounds - 2 * text_space - infoh) / (88);
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

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        auto listwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

        auto controls = armyList(window, renderer, army, offset, last, limit, textx, texty + infoh, false);

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto selected = false;
        auto current = -1;
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

            putText(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + infoh - 1));
            putText(renderer, selection.size() > 0 ? army_string.c_str() : "(None)", font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 3 * boxh, startx, starty + text_bounds - 3 * boxh);

            putHeader(renderer, "Select the units to add to your army", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);

            fillRect(renderer, listwidth, text_bounds - infoh, textx, texty + infoh, intBE);

            if (last - offset > 0)
            {
                for (auto i = 0; i < last - offset; i++)
                {
                    if (Engine::FIND_LIST(selection, offset + i) >= 0)
                    {
                        thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                    }
                    else
                    {
                        drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
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

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
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
                                if (army[offset + current].Unique && Engine::HAS_UNIT(party, army[offset + current].Type))
                                {
                                    displayMessage("You already command the " + std::string(army[offset + current].Name) + "!", intRD);
                                }
                                else
                                {
                                    selection.push_back(offset + current);
                                }
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
                            if ((!army[selection[i]].Unique) || (army[selection[i]].Unique && !Engine::HAS_UNIT(party, army[selection[i]].Type)))
                            {
                                party.Army.push_back(army[selection[i]]);
                            }
                        }
                    }

                    done = true;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = false;
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
        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;
        auto booksize = (int)(2 * (text_bounds) / 3 - infoh - box_space);
        auto limit = (int)((booksize - 2 * text_space) / (88));
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

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        auto listwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

        auto controls = spellList(window, renderer, spells, offset, last, limit, textx, texty + infoh, scrolly, true, true);

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto selected = false;
        auto current = -1;
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

            putText(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + infoh - 1));
            putText(renderer, selection.size() > 0 ? spell_string.c_str() : "(None)", font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 3 * boxh, startx, starty + text_bounds - 3 * boxh);

            putHeader(renderer, "You can copy the following spells", font_garamond, text_space, clrWH, intBR, TTF_STYLE_NORMAL, listwidth, infoh, textx, texty);

            fillRect(renderer, listwidth, booksize, textx, texty + infoh, intBE);

            if (last - offset > 0)
            {
                for (auto i = 0; i < last - offset; i++)
                {
                    if (Engine::FIND_LIST(selection, offset + i) >= 0)
                    {
                        thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                    }
                    else
                    {
                        drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                    }
                }
            }

            if (current >= 0 && current < controls.size() && controls[current].Type == Control::Type::ACTION)
            {
                fillRect(renderer, listwidth, text_bounds / 3 - box_space, textx, texty + infoh + booksize + box_space, intLB);

                auto text = createText(spells[current + offset].Description, FONT_GARAMOND, font_size, clrWH, listwidth - 2 * text_space, TTF_STYLE_NORMAL);

                renderText(renderer, text, intLB, textx + text_space, texty + infoh + booksize + box_space + text_space, text_bounds / 3 - texty, 0);

                SDL_FreeSurface(text);

                text = NULL;
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

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
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

                        if (Engine::SPELLCASTERS(party) == 1)
                        {
                            character = Engine::FIRST_CASTER(party);
                        }
                        else
                        {
                            character = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::LEARN_SPELL);
                        }

                        if (character >= 0 && character < party.Members.size())
                        {
                            if (party.Members[character].SpellCaster)
                            {
                                auto learned = false;

                                for (auto i = 0; i < learn.size(); i++)
                                {
                                    if (Engine::VERIFY_SPELL(party.Members[character], {learn[i].Type}))
                                    {
                                        learned = true;

                                        displayMessage(std::string(party.Members[character].Name) + " already knows the " + std::string(learn[i].Name) + " spell!", intRD);

                                        break;
                                    }
                                }

                                if (learned)
                                {
                                    current = -1;

                                    selected = false;
                                }
                                else
                                {
                                    for (auto i = 0; i < learn.size(); i++)
                                    {
                                        party.Members[character].SpellBook.push_back(learn[i]);
                                    }

                                    while (!Engine::VERIFY_SPELL_LIMIT(party.Members[character]))
                                    {
                                        auto spell_selection = selectSpell(window, renderer, party.Members[character], party.Members[character].SpellBook, party.Members[character].SpellBook.size() - party.Members[character].SpellBookLimit, Spells::Select::UNLEARN);

                                        if (spell_selection.size() > 0)
                                        {
                                            auto newSpellBook = std::vector<Spells::Base>();

                                            for (auto i = 0; i < party.Members[character].SpellBook.size(); i++)
                                            {
                                                if (Engine::FIND_LIST(spell_selection, i) < 0)
                                                {
                                                    newSpellBook.push_back(party.Members[character].SpellBook[i]);
                                                }
                                            }

                                            party.Members[character].SpellBook = newSpellBook;
                                        }
                                    }

                                    message = "Spell";

                                    if (selection.size() > 1)
                                    {
                                        message += "s";
                                    }

                                    message += " COPIED to " + std::string(party.Members[character].Name) += "'s spellbook!";

                                    displayMessage(message, intLB);

                                    current = -1;

                                    selected = false;

                                    selection.clear();
                                }
                            }
                            else
                            {
                                displayMessage(std::string(party.Members[character].Name) + " is not a SpellCaster!", intRD);

                                current = -1;

                                selected = false;
                            }
                        }
                    }
                    else
                    {
                        done = true;
                    }
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = false;
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

bool takeScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Equipment::Base> equipment, int TakeLimit, bool back_button)
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

        auto listwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

        auto controls = equipmentList(window, renderer, equipment, offset, last, limit, true, back_button);

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto selected = false;
        auto current = -1;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto infoh = 48;
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

            putText(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + infoh - 1));
            putText(renderer, selection.size() > 0 ? take.c_str() : "(None)", font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 3 * boxh, startx, starty + text_bounds - 3 * boxh);

            fillRect(renderer, listwidth, text_bounds, textx, texty, intBE);

            if (last - offset > 0)
            {
                for (auto i = 0; i < last - offset; i++)
                {
                    if (Engine::FIND_LIST(selection, offset + i) >= 0)
                    {
                        thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                    }
                    else
                    {
                        drawRect(renderer, controls[i].W + text_space, controls[i].H + text_space, controls[i].X - text_space / 2, controls[i].Y - text_space / 2, intBK);
                    }
                }
            }

            renderButtons(renderer, controls, current, intLB, text_space, text_space / 2);

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
                            take_message = "You can take any number of items.";
                        }
                        else
                        {
                            take_message = "You can take up to " + std::to_string(TakeLimit) + " items.";
                        }
                    }
                    else
                    {
                        take_message = "Choose an item to keep.";
                    }
                }
                else
                {
                    take_message = "Keep this item?";
                }

                putText(renderer, take_message.c_str(), font_garamond, text_space, clrWH, intLB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
            }

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
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

                        if (result >= 0 && result < selection.size())
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

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::CONFIRM && !hold)
                {
                    for (auto i = 0; i < selection.size(); i++)
                    {
                        if (equipment[selection[i]].Type == Equipment::Type::SILVER_COINS)
                        {
                            Engine::GAIN_MONEY(party, equipment[selection[i]].Value);
                        }
                        else
                        {
                            auto character = -1;

                            while (character < 0 || character > party.Members.size())
                            {
                                if (Engine::COUNT(party) == 1)
                                {
                                    character = Engine::FIRST(party);
                                }
                                else
                                {
                                    party.CurrentCharacter = Engine::FIND_SOLO(party);

                                    if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                                    {
                                        character = party.CurrentCharacter;
                                    }
                                    else
                                    {
                                        character = selectPartyMember(window, renderer, party, team, equipment[selection[i]], Control::Type::EQUIPMENT);
                                    }
                                }

                                if (Engine::IS_ACTIVE(party, character) && party.Members[character].Type != Character::Type::SKULLCRACKER)
                                {
                                    Engine::GET_EQUIPMENT(party.Members[character], {equipment[selection[i]]});

                                    while (!Engine::VERIFY_EQUIPMENT_LIMIT(party.Members[character]))
                                    {
                                        inventoryScreen(window, renderer, party, party.Members[character], -1, false);
                                    }
                                }
                            }
                        }
                    }

                    done = true;

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = false;
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

bool loseItems(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Character::Type character, Team::Type team, std::vector<Equipment::Base> equipment, std::vector<Equipment::Class> exceptions, int LoseLimit, bool back_button)
{
    auto done = false;

    if (LoseLimit > 0)
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

        auto listwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

        auto controls = equipmentList(window, renderer, equipment, offset, last, limit, true, back_button);

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, font_size);
        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        TTF_SetFontKerning(font_dark11, 0);

        auto selected = false;
        auto current = -1;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);

        auto selection = std::vector<int>();

        while (!done)
        {
            last = offset + limit;

            if (last > equipment.size())
            {
                last = equipment.size();
            }

            SDL_SetWindowTitle(window, "Legendary Kingdoms: Lose Items");

            fillWindow(renderer, intWH);

            std::string lose = "";

            if (selection.size() > 0)
            {
                for (auto i = 0; i < selection.size(); i++)
                {
                    if (i > 0)
                    {
                        lose += ", ";
                    }

                    std::string description = equipment[selection[i]].Name;

                    lose += description;
                }
            }

            putText(renderer, "Selected", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + infoh - 1));
            putText(renderer, selection.size() > 0 ? lose.c_str() : "(None)", font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 3 * boxh, startx, starty + text_bounds - 3 * boxh);

            fillRect(renderer, listwidth, text_bounds, textx, texty, intBE);

            if (last - offset > 0)
            {
                for (auto i = 0; i < last - offset; i++)
                {
                    if (Engine::FIND_LIST(selection, offset + i) >= 0)
                    {
                        thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                    }
                    else
                    {
                        drawRect(renderer, controls[i].W + text_space, controls[i].H + text_space, controls[i].X - text_space / 2, controls[i].Y - text_space / 2, intBK);
                    }
                }
            }

            renderButtons(renderer, controls, current, intLB, text_space, text_space / 2);

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
                std::string lose_message = "";

                if (equipment.size() > 1)
                {
                    if (LoseLimit > 1)
                    {
                        if (LoseLimit == equipment.size())
                        {
                            lose_message = "You must lose all items.";
                        }
                        else
                        {
                            lose_message = "You must lose up to " + std::to_string(LoseLimit) + " items.";
                        }
                    }
                    else
                    {
                        lose_message = "Choose an item to lose.";
                    }
                }
                else
                {
                    lose_message = "Lose this item?";
                }

                putText(renderer, lose_message.c_str(), font_garamond, text_space, clrWH, intLB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
            }

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
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
                            if (selection.size() < LoseLimit)
                            {
                                if (exceptions.size() > 0)
                                {
                                    if (Engine::FIND_LIST(exceptions, equipment[offset + current].Class) < 0)
                                    {
                                        selection.push_back(offset + current);
                                    }
                                    else
                                    {
                                        error = true;

                                        message = "You cannot drop this item!";

                                        start_ticks = SDL_GetTicks();
                                    }
                                }
                                else
                                {
                                    selection.push_back(offset + current);
                                }
                            }
                        }
                    }

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::CONFIRM && !hold)
                {
                    if (selection.size() >= LoseLimit)
                    {
                        auto items = std::vector<Equipment::Type>();

                        for (auto i = 0; i < selection.size(); i++)
                        {
                            items.push_back(equipment[selection[i]].Type);
                        }

                        if (character != Character::Type::NONE)
                        {
                            Engine::LOSE_EQUIPMENT(party, team, items);
                        }
                        else
                        {
                            auto find_character = Engine::FIND_CHARACTER(party, character);

                            if (Engine::IS_ACTIVE(party, find_character))
                            {
                                Engine::LOSE_EQUIPMENT(party.Members[find_character], items);
                            }
                        }

                        done = true;

                        current = -1;

                        selected = false;
                    }
                    else
                    {
                        error = true;

                        message = "You must lose up to the required number of items!";

                        start_ticks = SDL_GetTicks();
                    }

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = false;
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

    // Render window
    if (window && renderer)
    {
        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        SDL_SetWindowTitle(window, title);

        auto current = 0;

        auto selected = false;

        auto infoh = 48;
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto box_space = 10;
        auto text_space = 8;

        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space - infoh) / (88);
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

            putHeader(renderer, "Location", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty);
            putText(renderer, Location::Description[party.Location], font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + infoh);

            putHeader(renderer, "Money", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + 2 * infoh + box_space - 1));
            putText(renderer, (std::to_string(party.Money) + std::string(" silver coins")).c_str(), font_mason, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (3 * boxh + infoh + box_space));

            putHeader(renderer, "Party", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

            if (Engine::COUNT(party) > 0)
            {
                std::string party_string = "";

                auto count = 0;

                for (auto i = 0; i < party.Members.size(); i++)
                {
                    if (count > 0)
                    {
                        party_string += "\n";
                    }

                    party_string += party.Members[i].Name;

                    if (Engine::IS_DEAD(party.Members[i]))
                    {
                        party_string += " (D)";
                    }
                    else if (Engine::IS_CURSED(party.Members[i]))
                    {
                        party_string += " (C)";
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

            if (current_mode == Control::Type::BUY_SELL_SHIP)
            {
                putHeader(renderer, "Ship Prices", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                fillRect(renderer, textwidth, (text_bounds - infoh), textx, (texty + infoh), intBE);

                if (harbour->Ships.size() == 0)
                {
                    putText(renderer, "\nYou cannot buy nor sell ships here.", font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, text_bounds - infoh, textx, texty + infoh);
                }
            }
            else if (current_mode == Control::Type::REPAIR_SHIP)
            {
                putHeader(renderer, "Repair Costs", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                fillRect(renderer, textwidth, (text_bounds - infoh), textx, (texty + infoh), intBE);

                if (harbour->ShipRepairPrice > 0)
                {
                    std::string repair_string = "You can repair any ship you have here. It costs " + std::to_string(harbour->ShipRepairPrice) + " silver coins to restore each Health point. You can restore your ship up to its starting Health value.";

                    putText(renderer, repair_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, text_bounds - infoh, textx, texty + infoh);
                }
                else
                {
                    putText(renderer, "Ship repair services are not available here.", font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, text_bounds - infoh, textx, texty + infoh);
                }
            }
            else if (current_mode == Control::Type::BUY_SELL_CARGO)
            {
                putHeader(renderer, "Cargo Prices", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                fillRect(renderer, textwidth, (text_bounds - infoh), textx, (texty + infoh), intBE);

                if (harbour->Cargo.size() == 0)
                {
                    putText(renderer, "You cannot buy nor sell cargo here.", font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, text_bounds - infoh, textx, texty + infoh);
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

            renderButtons(renderer, controls, current, intLB, border_space, border_pts);

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

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
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
                else if (controls[current].Type == Control::Type::BUY_SELL_SHIP && !hold)
                {
                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BUY_SELL_CARGO && !hold)
                {
                    if (Engine::HAS_SHIP(party, harbour->Location))
                    {
                    }
                    else
                    {
                        displayMessage("You do not have a ship docked at " + std::string(Location::Description[harbour->Location]) + "!", intRD);
                    }

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::REPAIR_SHIP && !hold)
                {
                    if (harbour->ShipRepairPrice >= 0)
                    {
                        if (Engine::HAS_SHIP(party, harbour->Location))
                        {
                        }
                        else
                        {
                            displayMessage("You do not have a ship docked at " + std::string(Location::Description[harbour->Location]) + "!", intRD);
                        }
                    }
                    else
                    {
                        displayMessage("It is not possible to repair ships at this harbour!", intRD);
                    }

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK)
                {
                    done = true;
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

void renderArmy(SDL_Renderer *renderer, TTF_Font *font, int text_space, std::vector<Army::Base> &army, int boxw, int boxh, int box_space, int offsety, SDL_Color fg, Uint32 bg)
{
    if (army.size() > 0)
    {
        std::string left_front = "";
        std::string left_support = "";

        std::string center_front = "";
        std::string center_support = "";

        std::string right_front = "";
        std::string right_support = "";

        // Render Enemy army
        for (auto i = 0; i < army.size(); i++)
        {
            if (army[i].Position == Location::BattleField::LEFT_FLANK_FRONT || army[i].Position == Location::BattleField::CENTER_FRONT || army[i].Position == Location::BattleField::RIGHT_FLANK_FRONT)
            {
                if (army[i].Position == Location::BattleField::LEFT_FLANK_FRONT)
                {
                    left_front = "[" + std::string(army[i].Name) + "]\nStrength: " + std::to_string(army[i].Strength) + " Morale: " + std::to_string(army[i].Morale);
                }
                else if (army[i].Position == Location::BattleField::CENTER_FRONT)
                {
                    center_front = "[" + std::string(army[i].Name) + "]\nStrength: " + std::to_string(army[i].Strength) + " Morale: " + std::to_string(army[i].Morale);
                }
                else if (army[i].Position == Location::BattleField::RIGHT_FLANK_FRONT)
                {
                    right_front = "[" + std::string(army[i].Name) + "]\nStrength: " + std::to_string(army[i].Strength) + " Morale: " + std::to_string(army[i].Morale);
                }
            }
            else if (army[i].Position == Location::BattleField::LEFT_FLANK_SUPPORT || army[i].Position == Location::BattleField::CENTER_SUPPORT || army[i].Position == Location::BattleField::RIGHT_FLANK_SUPPORT)
            {
                if (army[i].Position == Location::BattleField::LEFT_FLANK_SUPPORT)
                {
                    left_support = "\n\nSupport:\n[" + std::string(army[i].Name) + "]\nStrength: " + std::to_string(army[i].Strength) + " Morale: " + std::to_string(army[i].Morale);
                }
                else if (army[i].Position == Location::BattleField::CENTER_SUPPORT)
                {
                    center_support = "\n\nSupport:\n[" + std::string(army[i].Name) + "]\nStrength: " + std::to_string(army[i].Strength) + " Morale: " + std::to_string(army[i].Morale);
                }
                else if (army[i].Position == Location::BattleField::RIGHT_FLANK_SUPPORT)
                {
                    right_support = "\n\nSupport:\n[" + std::string(army[i].Name) + "]\nStrength: " + std::to_string(army[i].Strength) + " Morale: " + std::to_string(army[i].Morale);
                }
            }
        }

        auto left_flank = left_front + left_support;
        auto center = center_front + center_support;
        auto right_flank = right_front + right_support;

        if (left_flank.length() > 0)
        {
            putText(renderer, left_flank.c_str(), font, text_space, fg, bg, TTF_STYLE_NORMAL, boxw - 2 * text_space, boxh - 4 * text_space, startx + text_space, offsety + 2 * text_space);
        }

        if (center.length() > 0)
        {
            putText(renderer, center.c_str(), font, text_space, fg, bg, TTF_STYLE_NORMAL, boxw - 2 * text_space, boxh - 4 * text_space, startx + (boxw + box_space) + text_space, offsety + 2 * text_space);
        }

        if (right_flank.length() > 0)
        {
            putText(renderer, right_flank.c_str(), font, text_space, fg, bg, TTF_STYLE_NORMAL, boxw - 2 * text_space, boxh - 4 * text_space, startx + 2 * (boxw + box_space) + text_space, offsety + 2 * text_space);
        }
    }
}

std::vector<Button> popupArmy(SDL_Window *window, SDL_Renderer *renderer, std::vector<Army::Base> &army, int start, int last, int limit, int popupw, int popuph, int infoh, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (army.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto unit = army[index];

            std::string army_string = "[" + std::string(unit.Name) + "] Strength: " + std::to_string(unit.Strength) + ", Morale: " + std::to_string(unit.Morale);

            army_string += "\nPosition: " + std::string(Location::BattleFieldDescription[unit.Position]) + " Garrison: " + std::string(Location::Description[unit.Garrison]);

            auto button = createHeaderButton(window, FONT_GARAMOND, 24, army_string.c_str(), clrBK, intBE, popupw - 3 * button_space / 2 - button_space - arrow_size - border_space, (text_space + 28) * 2, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + infoh + 3 * text_space);

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
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, offsetx + (popupw - arrow_size - button_space), offsety + infoh + 7 * border_space / 2, Control::Type::SCROLL_UP));

            idx++;
        }

        if (army.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, offsetx + (popupw - arrow_size - button_space), offsety + (popuph - arrow_size - 2 * border_space - buttonh - infoh), Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/yes.png", army.size() > 0 ? idx - 1 : idx, idx + 1, army.size() > 0 ? idx - 1 : idx, idx, offsetx + button_space, offsety + popuph - button_space - buttonh, Control::Type::CONFIRM));
    controls.push_back(Button(idx + 1, "icons/no.png", idx, idx + 1, army.size() > 0 ? idx - 1 : idx + 1, idx + 1, offsetx + popupw - buttonw - button_space, offsety + popuph - button_space - buttonh, Control::Type::BACK));

    return controls;
}

bool moraleCheck(SDL_Window *window, SDL_Renderer *renderer, Army::Base &unit, int combatRound)
{
    bool morale_result = false;

    if (window && renderer)
    {
        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        auto marginx = (int)(Margin * SCREEN_WIDTH);

        auto fullwidth = SCREEN_WIDTH - 2 * marginx;

        auto boxwidth = (SCREEN_WIDTH - 3 * marginx) / 2;

        auto headerw = (int)(boxwidth * 0.75);

        auto infoh = 48;

        auto boxh = (int)(0.125 * SCREEN_HEIGHT);

        auto box_space = 10;

        auto main_buttonh = 48;

        auto done = false;

        auto stage = Engine::MassCombat::START;

        SDL_SetWindowTitle(window, "Legendary Kingdoms: Morale Check");

        TTF_Init();

        auto font_mason = TTF_OpenFont(FONT_MASON, 32);

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 28);

        auto text_space = 8;

        auto font_size = 28;

        const char *choices_morale[1] = {"Check Morale"};
        const char *choices_end[1] = {"Done"};

        SDL_Surface *dice[6];

        dice[0] = createImage("images/dice/dice1.png");
        dice[1] = createImage("images/dice/dice2.png");
        dice[2] = createImage("images/dice/dice3.png");
        dice[3] = createImage("images/dice/dice4.png");
        dice[4] = createImage("images/dice/dice5.png");
        dice[5] = createImage("images/dice/dice6.png");

        auto main_buttonw = 220;

        auto controls_morale = createFixedTextButtons(choices_morale, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
        controls_morale[0].Type = Control::Type::CONFIRM;

        auto controls_end = createFixedTextButtons(choices_end, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
        controls_end[0].Type = Control::Type::BACK;

        auto current = -1;

        auto selected = false;

        auto scrollUp = false;

        auto scrollDown = false;

        auto hold = false;

        std::vector<int> morale = {};

        int morale_score = 0;

        auto size_dice = 64;

        auto cols = (fullwidth - 2 * box_space) / (size_dice + box_space);

        auto controls = std::vector<TextButton>();

        auto morale_checked = false;

        auto offsety = starty + infoh + boxh + box_space + infoh + box_space;
        auto offsetx = startx + box_space;

        std::string mass_combat = "Round " + std::to_string(combatRound + 1) + " - Morale Check Results ";

        while (!done)
        {
            fillWindow(renderer, intWH);

            putHeader(renderer, mass_combat.c_str(), font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, fullwidth, infoh, startx, starty + infoh + boxh + box_space);

            fillRect(renderer, fullwidth, boxh * 3, startx, starty + infoh + boxh + box_space + infoh, intBE);

            if (stage == Engine::MassCombat::MORALE)
            {
                if (morale.size() == 0)
                {
                    morale = Engine::ROLL_DICE(1);
                }

                auto row = 0;
                auto col = 0;

                morale_score = 0;

                for (auto i = 0; i < morale.size(); i++)
                {
                    if (morale[i] >= 1 && morale[i] <= 6)
                    {
                        auto result = morale[i] - 1;

                        morale_score += morale[i];

                        fitImage(renderer, dice[result], offsetx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), size_dice, size_dice);

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

                if (!morale_checked)
                {
                    if (morale_score <= unit.Morale)
                    {
                        Engine::GAIN_MORALE(unit, -1);

                        message = "The " + std::string(unit.Name) + " loses 1 point of Morale!";

                        morale_result = true;
                    }
                    else
                    {
                        message = "The " + std::string(unit.Name) + " flee!";

                        unit.Position = Location::BattleField::NONE;

                        morale_result = false;
                    }

                    displayMessage(message, intRD);

                    morale_checked = true;
                }
            }

            putHeader(renderer, unit.Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);

            std::string unit_string = "Strength: " + std::to_string(unit.Strength);
            unit_string += "\nMorale: " + std::to_string(unit.Morale);

            putText(renderer, unit_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx, starty + infoh);

            if (stage == Engine::MassCombat::MORALE)
            {
                std::string morale_string = "Morale Check: " + std::to_string(morale_score);

                putHeader(renderer, morale_string.c_str(), font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, boxwidth, infoh, startx, starty + infoh + 4 * boxh + 2 * box_space + infoh);
            }

            if (stage == Engine::MassCombat::START)
            {
                controls = controls_morale;
            }
            else if (stage == Engine::MassCombat::MORALE)
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

            renderTextButtons(renderer, controls, FONT_DARK11, current, clrWH, intDB, intLB, font_size, TTF_STYLE_NORMAL);

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size())
            {
                if (stage == Engine::MassCombat::START && controls[current].Type == Control::Type::BACK)
                {
                    done = true;

                    current = -1;

                    selected = false;
                }
                else if (stage == Engine::MassCombat::START && controls[current].Type == Control::Type::CONFIRM)
                {
                    stage = Engine::MassCombat::MORALE;
                }
                else if (stage == Engine::MassCombat::MORALE && controls[current].Type == Control::Type::BACK)
                {
                    stage = Engine::MassCombat::END;

                    done = true;

                    current = -1;

                    selected = false;
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

    return morale_result;
}

void resolveMassCombat(SDL_Window *window, SDL_Renderer *renderer, Location::Type location, Party::Base &party, std::vector<Army::Base> &enemyArmy, std::vector<Engine::BattlefieldSpells> &enemySpells, std::vector<Engine::ArmyStatus> &enemyStatus, Location::Zone zone, int combatRound)
{
    if (window && renderer)
    {
        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        auto marginx = (int)(Margin * SCREEN_WIDTH);

        auto fullwidth = SCREEN_WIDTH - 2 * marginx;

        auto boxwidth = (SCREEN_WIDTH - 3 * marginx) / 2;

        auto headerw = (int)(boxwidth * 0.75);

        auto infoh = 48;

        auto boxh = (int)(0.125 * SCREEN_HEIGHT);

        auto box_space = 10;

        auto main_buttonh = 48;

        auto done = false;

        auto stage = Engine::MassCombat::START;

        SDL_SetWindowTitle(window, "Legendary Kingdoms: Resolve Mass Combat");

        TTF_Init();

        auto font_mason = TTF_OpenFont(FONT_MASON, 32);

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 28);

        auto text_space = 8;

        auto font_size = 28;

        const char *choices_combat[1] = {"Resolve Combat"};
        const char *choices_morale[1] = {"Check Morale"};
        const char *choices_end[1] = {"Done"};

        SDL_Surface *dice[6];

        dice[0] = createImage("images/dice/dice1.png");
        dice[1] = createImage("images/dice/dice2.png");
        dice[2] = createImage("images/dice/dice3.png");
        dice[3] = createImage("images/dice/dice4.png");
        dice[4] = createImage("images/dice/dice5.png");
        dice[5] = createImage("images/dice/dice6.png");

        auto main_buttonw = 220;

        auto controls_combat = createFixedTextButtons(choices_combat, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
        controls_combat[0].Type = Control::Type::CONFIRM;

        auto controls_morale = createFixedTextButtons(choices_morale, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
        controls_morale[0].Type = Control::Type::CONFIRM;

        auto controls_end = createFixedTextButtons(choices_end, 1, main_buttonw, main_buttonh, 10, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));
        controls_end[0].Type = Control::Type::BACK;

        auto current = -1;

        auto selected = false;

        auto scrollUp = false;

        auto scrollDown = false;

        auto hold = false;

        std::vector<int> party_combat_results = {};
        std::vector<int> enemy_combat_results = {};
        std::vector<int> morale = {};

        int party_combat_score = 0;
        int enemy_combat_score = 0;
        int morale_score = 0;

        auto size_dice = 64;

        auto cols = (fullwidth - 2 * box_space) / (size_dice + box_space);

        auto controls = std::vector<TextButton>();

        auto combat_resolved = false;
        auto morale_checked = false;

        auto party_unit = Engine::FIND_UNIT(party.Army, zone);
        auto enemy_unit = Engine::FIND_UNIT(enemyArmy, zone);

        auto offsety = starty + infoh + boxh + box_space + infoh + box_space;
        auto offsetx = startx + box_space;

        std::string mass_combat = "Round " + std::to_string(combatRound + 1) + " - Mass Combat Results: ";

        if (zone == Location::Zone::LEFT_FLANK)
        {
            mass_combat += "Left Flank";
        }
        else if (zone == Location::Zone::CENTER)
        {
            mass_combat += "Center";
        }
        else if (zone == Location::Zone::RIGHT_FLANK)
        {
            mass_combat += "Right Flank";
        }

        while (!done)
        {
            fillWindow(renderer, intWH);

            putHeader(renderer, mass_combat.c_str(), font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, fullwidth, infoh, startx, starty + infoh + boxh + box_space);

            fillRect(renderer, fullwidth, boxh * 3, startx, starty + infoh + boxh + box_space + infoh, intBE);

            if (stage != Engine::MassCombat::START)
            {
                if (stage == Engine::MassCombat::COMBAT)
                {
                    if (party_combat_results.size() == 0)
                    {
                        party_combat_results = Engine::ROLL_DICE(1);
                    }

                    if (enemy_combat_results.size() == 0)
                    {
                        enemy_combat_results = Engine::ROLL_DICE(1);
                    }
                }

                if (stage == Engine::MassCombat::MORALE)
                {
                    if (party_combat_score != enemy_combat_score)
                    {
                        if (morale.size() == 0)
                        {
                            morale = Engine::ROLL_DICE(1);
                        }
                    }
                }

                if (stage == Engine::MassCombat::COMBAT)
                {
                    auto row = 0;
                    auto col = 0;

                    party_combat_score = party.Army[party_unit].Strength;

                    for (auto i = 0; i < party_combat_results.size(); i++)
                    {
                        if (party_combat_results[i] >= 1 && party_combat_results[i] <= 6)
                        {
                            auto result = party_combat_results[i] - 1;

                            party_combat_score += party_combat_results[i];

                            fitImage(renderer, dice[result], offsetx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), size_dice, size_dice);

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

                    row = 0;
                    col = 0;

                    enemy_combat_score = enemyArmy[enemy_unit].Strength;

                    if (enemyArmy[enemy_unit].Status != Army::Status::NONE && enemyArmy[enemy_unit].StatusRound >= 0 && enemyArmy[enemy_unit].StatusDuration >= 0 && ((combatRound - enemyArmy[enemy_unit].StatusRound) < enemyArmy[enemy_unit].StatusDuration))
                    {
                        if (enemyArmy[enemy_unit].Status == Army::Status::STRENGTH1)
                        {
                            enemy_combat_score += 1;
                        }
                    }

                    for (auto i = 0; i < enemy_combat_results.size(); i++)
                    {
                        if (enemy_combat_results[i] >= 1 && enemy_combat_results[i] <= 6)
                        {
                            auto result = enemy_combat_results[i] - 1;

                            enemy_combat_score += enemy_combat_results[i];

                            fitImage(renderer, dice[result], offsetx + boxwidth + marginx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), size_dice, size_dice);

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

                    if (!combat_resolved)
                    {
                        if (party_combat_score != enemy_combat_score)
                        {
                            if ((party_unit >= 0 && party_unit < party.Army.size()) && (enemy_unit >= 0 && enemy_unit < enemyArmy.size()))
                            {
                                if (party_combat_score < enemy_combat_score)
                                {
                                    message = "The " + std::string(enemyArmy[enemy_unit].Name) + " defeats your " + std::string(party.Army[party_unit].Name) + "!";

                                    flash_color = intRD;
                                }
                                else if (enemy_combat_score < party_combat_score)
                                {
                                    message = "Your " + std::string(party.Army[party_unit].Name) + " defeats the " + std::string(enemyArmy[enemy_unit].Name) + "!";

                                    flash_color = intLB;
                                }
                            }
                        }
                        else
                        {
                            message = "The fight is inconclusive!";

                            flash_color = intLB;
                        }

                        displayMessage(message, flash_color);

                        combat_resolved = true;
                    }
                }
            }

            if (stage == Engine::MassCombat::MORALE)
            {
                auto row = 0;
                auto col = 0;

                morale_score = 0;

                for (auto i = 0; i < morale.size(); i++)
                {
                    if (morale[i] >= 1 && morale[i] <= 6)
                    {
                        auto result = morale[i] - 1;

                        morale_score += morale[i];

                        if (party_combat_score > enemy_combat_score)
                        {
                            fitImage(renderer, dice[result], offsetx + boxwidth + marginx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), size_dice, size_dice);
                        }
                        else if (enemy_combat_score > party_combat_score)
                        {
                            fitImage(renderer, dice[result], offsetx + (col) * (box_space + size_dice), offsety + (row) * (box_space + size_dice), size_dice, size_dice);
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

                if (!morale_checked)
                {
                    if (party_combat_score != enemy_combat_score)
                    {
                        if ((party_unit >= 0 && party_unit < party.Army.size()) && (enemy_unit >= 0 && enemy_unit < enemyArmy.size()))
                        {
                            if (party_combat_score < enemy_combat_score)
                            {
                                if (morale_score <= party.Army[party_unit].Morale)
                                {
                                    Engine::GAIN_MORALE(party.Army[party_unit], -1);

                                    message = "Your " + std::string(party.Army[party_unit].Name) + " loses 1 point of Morale!";
                                }
                                else
                                {
                                    message = "Your " + std::string(party.Army[party_unit].Name) + " is routed!";
                                }

                                Engine::UPDATE_ARMY(party.Army, location);

                                flash_color = intRD;
                            }
                            else if (enemy_combat_score < party_combat_score)
                            {
                                if (morale_score <= enemyArmy[enemy_unit].Morale)
                                {
                                    Engine::GAIN_MORALE(enemyArmy[enemy_unit], -1);

                                    message = "The " + std::string(enemyArmy[enemy_unit].Name) + " loses 1 point of Morale!";
                                }
                                else
                                {
                                    message = "The " + std::string(enemyArmy[enemy_unit].Name) + " is routed!";
                                }

                                Engine::UPDATE_ARMY(enemyArmy, Location::Type::NONE);

                                flash_color = intLB;
                            }
                        }
                    }
                    else
                    {
                        message = "The fight is inconclusive!";

                        flash_color = intLB;
                    }

                    displayMessage(message, flash_color);

                    morale_checked = true;
                }
            }

            if (party_unit >= 0 && party_unit < party.Army.size())
            {
                std::string army_string = "";

                putHeader(renderer, party.Army[party_unit].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx, starty);

                army_string = "Strength: " + std::to_string(party.Army[party_unit].Strength);
                army_string += "\nMorale: " + std::to_string(party.Army[party_unit].Morale);

                putText(renderer, army_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx, starty + infoh);

                if (party_combat_score > 0)
                {
                    if (stage == Engine::MassCombat::COMBAT)
                    {
                        std::string combat_string = "Combat Result: " + std::to_string(party_combat_score);

                        putHeader(renderer, combat_string.c_str(), font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, boxwidth, infoh, startx, starty + infoh + 4 * boxh + 2 * box_space + infoh);
                    }
                    else if (stage == Engine::MassCombat::MORALE)
                    {
                        if (party_combat_score < enemy_combat_score)
                        {
                            std::string morale_string = "Morale Check: " + std::to_string(morale_score);

                            putHeader(renderer, morale_string.c_str(), font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, boxwidth, infoh, startx, starty + infoh + 4 * boxh + 2 * box_space + infoh);
                        }
                    }
                }
            }
            else
            {
                fillRect(renderer, headerw, infoh, startx, starty, intBR);
                fillRect(renderer, boxwidth, boxh, startx, starty + infoh, intBE);
            }

            if (enemy_unit >= 0 && enemy_unit < enemyArmy.size())
            {
                std::string enemy_string = "";

                putHeader(renderer, enemyArmy[enemy_unit].Name, font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, headerw, infoh, startx + boxwidth + marginx, starty);

                auto score = enemyArmy[enemy_unit].Strength;

                if (enemyArmy[enemy_unit].Status != Army::Status::NONE && enemyArmy[enemy_unit].StatusRound >= 0 && enemyArmy[enemy_unit].StatusDuration > 0 && ((combatRound - enemyArmy[enemy_unit].StatusRound) < enemyArmy[enemy_unit].StatusDuration))
                {
                    if (enemyArmy[enemy_unit].Status == Army::Status::STRENGTH1)
                    {
                        score += 1;
                    }
                }

                enemy_string = "Strength: " + std::to_string(score);
                enemy_string += "\nMorale: " + std::to_string(enemyArmy[enemy_unit].Morale);

                putText(renderer, enemy_string.c_str(), font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh);

                if (enemy_combat_score > 0)
                {
                    if (stage == Engine::MassCombat::COMBAT)
                    {
                        std::string combat_string = "Combat Result: " + std::to_string(enemy_combat_score);

                        putHeader(renderer, combat_string.c_str(), font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, boxwidth, infoh, startx + boxwidth + marginx, starty + infoh + 4 * boxh + 2 * box_space + infoh);
                    }
                    else if (stage == Engine::MassCombat::MORALE)
                    {
                        if (enemy_combat_score < party_combat_score)
                        {
                            std::string morale_string = "Morale Check: " + std::to_string(morale_score);

                            putHeader(renderer, morale_string.c_str(), font_mason, text_space, clrWH, intBR, TTF_STYLE_NORMAL, boxwidth, infoh, startx + boxwidth + marginx, starty + infoh + 4 * boxh + 2 * box_space + infoh);
                        }
                    }
                }
            }
            else
            {
                fillRect(renderer, headerw, infoh, startx + boxwidth + marginx, starty, intBR);
                fillRect(renderer, boxwidth, boxh, startx + boxwidth + marginx, starty + infoh, intBE);
            }

            if (stage == Engine::MassCombat::START)
            {
                controls = controls_combat;
            }
            else if (stage == Engine::MassCombat::COMBAT)
            {
                if (party_combat_score != enemy_combat_score)
                {
                    controls = controls_morale;
                }
                else
                {
                    stage = Engine::MassCombat::MORALE;

                    controls = controls_end;
                }
            }
            else if (stage == Engine::MassCombat::MORALE)
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

            renderTextButtons(renderer, controls, FONT_DARK11, current, clrWH, intDB, intLB, font_size, TTF_STYLE_NORMAL);

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size())
            {
                if (stage == Engine::MassCombat::START && controls[current].Type == Control::Type::BACK)
                {
                    done = true;

                    current = -1;

                    selected = false;
                }
                else if (stage == Engine::MassCombat::START && controls[current].Type == Control::Type::CONFIRM)
                {
                    stage = Engine::MassCombat::COMBAT;
                }
                else if (stage == Engine::MassCombat::COMBAT && controls[current].Type == Control::Type::CONFIRM)
                {
                    stage = Engine::MassCombat::MORALE;
                }
                else if (stage == Engine::MassCombat::MORALE && controls[current].Type == Control::Type::BACK)
                {
                    stage = Engine::MassCombat::END;

                    done = true;

                    current = -1;

                    selected = false;
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

Engine::Combat massCombatScreen(SDL_Window *window, SDL_Renderer *renderer, Location::Type location, Party::Base &party, std::vector<Army::Base> &enemyArmy, std::vector<Engine::BattlefieldSpells> &enemySpells, std::vector<Engine::ArmyStatus> &enemyStatus)
{
    auto combatResult = Engine::Combat::NONE;

    if (window && renderer)
    {
        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        auto messageh = (int)(0.125 * SCREEN_HEIGHT);

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        TTF_Init();

        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);
        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 28);

        TTF_SetFontKerning(font_dark11, 0);

        auto box_space = 10;

        auto text_space = 8;

        auto marginx = (int)(Margin * SCREEN_WIDTH);

        auto fullwidth = SCREEN_WIDTH - 2 * marginx;

        auto boxw = (int)((fullwidth - 2 * box_space) / 3);

        auto infoh = 48;

        auto boxh = (int)((text_bounds - box_space - infoh) / 2);

        auto popupw = (int)(0.6 * SCREEN_WIDTH);
        auto popuph = (int)(0.6 * SCREEN_HEIGHT);
        auto popupx = (SCREEN_WIDTH - popupw) / 2;
        auto popupy = (SCREEN_HEIGHT - popuph) / 2;

        auto main_buttonw = 220;

        std::vector<Button> controls_yes = {Button(0, "icons/yes.png", 0, 0, 0, 0, popupx + button_space, popupy + popuph - button_space - buttonh, Control::Type::CONFIRM)};

        auto controls = std::vector<Button>();

        auto controls_battlefield = std::vector<Button>();

        for (auto y = 0; y < 2; y++)
        {
            for (auto x = 0; x < 3; x++)
            {
                SDL_Rect rect;

                rect.w = boxw - 2 * 8;
                rect.h = boxh - 2 * 8;
                rect.x = 0;
                rect.y = 0;

                auto button = Button();

                button.Surface = SDL_CreateRGBSurface(0, boxw - 2 * 8, boxh - 2 * 8, 32, 0, 0, 0, 0);

                if (y > 0)
                {
                    SDL_FillRect(button.Surface, &rect, intBE);
                }
                else
                {
                    SDL_FillRect(button.Surface, &rect, intBE);
                }

                button.ID = y * 3 + x;
                button.W = boxw - 2 * 8;
                button.H = boxh - 2 * 8;
                button.Left = x > 0 ? y * 3 + x - 1 : y * 3 + x;
                button.Right = x < 2 ? y * 3 + x + 1 : y * 3 + x;
                button.Up = y > 0 ? (y - 1) * 3 + x : y * 3 + x;
                button.Down = y < 1 ? (y + 1) * 3 + x : (x < 2 ? (y + 1) * 3 + x : y * 3 + x);
                button.X = startx + x * (boxw + box_space) + border_space;
                button.Y = starty + infoh + y * (boxh + box_space) + border_space + text_space;
                button.Type = Control::Type::ACTION;

                controls_battlefield.push_back(button);
            }
        }

        auto text_y = (int)(SCREEN_HEIGHT * (1.0 - Margin)) - 48;

        controls_battlefield.push_back(Button(6, createHeaderButton(window, FONT_DARK11, 28, "Fight", clrWH, intDB, 220, 48, -1), 6, 7, 3, 6, startx, text_y, Control::Type::CONFIRM));
        controls_battlefield.push_back(Button(7, createHeaderButton(window, FONT_DARK11, 28, "Cast Spell", clrWH, intDB, 220, 48, -1), 6, 7, 4, 7, startx + (main_buttonw + button_space), text_y, Control::Type::SPELL));

        auto current = -1;
        auto selected = false;
        auto hold = false;
        auto scrollUp = false;
        auto scrollDown = false;

        auto current_mode = Engine::MassCombatMode::NORMAL;

        auto combat_round = 0;

        auto party_has_cast = false;
        auto enemy_has_cast = false;
        auto enemy_spell = Spells::MassCombat::NONE;

        while (Engine::ZONES(party.Army, enemyArmy) < 2 && Engine::ZONES(enemyArmy, party.Army) < 2)
        {
            for (auto i = 0; i < enemyStatus.size(); i++)
            {
                if (std::get<2>(enemyStatus[i]) == combat_round)
                {
                    auto enemy_unit = std::get<1>(enemyStatus[i]);

                    if (enemy_unit >= 0 && enemy_unit < enemyArmy.size())
                    {
                        enemyArmy[enemy_unit].Status = std::get<0>(enemyStatus[i]);

                        enemyArmy[enemy_unit].StatusRound = combat_round;

                        enemyArmy[enemy_unit].StatusDuration = std::get<3>(enemyStatus[i]);
                    }
                }
            }

            if (!enemy_has_cast)
            {
                enemy_spell = Engine::GET_SPELL(enemySpells, combat_round);

                if (enemy_spell != Spells::MassCombat::NONE)
                {
                    current_mode = Engine::MassCombatMode::SPELL;

                    enemy_has_cast = true;
                }
            }

            SDL_SetWindowTitle(window, "Legendary Kingdoms: Mass Combat");

            fillWindow(renderer, intWH);

            for (auto y = 0; y < 2; y++)
            {
                for (auto x = 0; x < 3; x++)
                {
                    if (y > 0)
                    {
                        fillRect(renderer, boxw, boxh, startx + x * (boxw + box_space), starty + infoh + y * (boxh + box_space) + text_space, intBE);

                        thickRect(renderer, boxw - 2 * text_space, boxh - 2 * text_space, startx + x * (boxw + box_space) + border_space, starty + infoh + y * (boxh + box_space) + text_space + border_space, intBR, border_pts);
                    }
                    else
                    {
                        fillRect(renderer, boxw, boxh, startx + x * (boxw + box_space), starty + infoh + y * (boxh + box_space) + text_space, intBE);
                    }
                }
            }

            putHeader(renderer, "Left Flank", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, boxw, infoh, startx, starty);
            putHeader(renderer, "Center", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, boxw, infoh, startx + (boxw + box_space), starty);
            putHeader(renderer, "Right Flank", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, boxw, infoh, startx + 2 * (boxw + box_space), starty);

            if (current_mode == Engine::MassCombatMode::NORMAL)
            {
                renderButtons(renderer, controls_battlefield, current, intLB, border_space, border_pts);

                controls = controls_battlefield;
            }
            else
            {
                renderButtons(renderer, controls_battlefield, -1, intLB, border_space, border_pts);
            }

            // Render Enemy army
            renderArmy(renderer, font_garamond, text_space, enemyArmy, boxw, boxh, box_space, starty + infoh, clrBK, intBE);

            // Render Party army
            renderArmy(renderer, font_garamond, text_space, party.Army, boxw, boxh, box_space, starty + infoh + boxh + box_space, clrBK, intBE);

            if (current_mode == Engine::MassCombatMode::SPELL)
            {
                fillRect(renderer, popupw, popuph, popupx, popupy, intBE);

                drawRect(renderer, popupw, popuph, popupx, popupy, intBK);

                putText(renderer, Spells::MassCombatDescriptions[enemy_spell], font_garamond, text_space, clrBK, intBE, TTF_STYLE_NORMAL, popupw - 2 * text_space, popupy - infoh - 2 * text_space, popupx + text_space, popupy + infoh + text_space);

                std::string spell_string = "Your enemy has cast " + std::string(Spells::MassCombatNames[enemy_spell]);

                putHeader(renderer, spell_string.c_str(), font_dark11, text_space, clrWH, intDB, TTF_STYLE_NORMAL, popupw, infoh, popupx, popupy);

                renderButtons(renderer, controls_yes, current, intLB, border_space, border_pts);

                controls = controls_yes;

                enemy_has_cast = true;
            }

            if (flash_message)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putHeader(renderer, message.c_str(), font_garamond, -1, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, messageh, -1, -1);
                }
                else
                {
                    flash_message = false;
                }
            }

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
            {
                if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (current_mode == Engine::MassCombatMode::NORMAL)
                    {
                        if (current >= 0 && current < 6)
                        {
                        }

                        selected = false;
                    }
                }
                else if (controls[current].Type == Control::Type::CONFIRM && !hold)
                {
                    if (current_mode == Engine::MassCombatMode::NORMAL)
                    {
                        std::vector<Location::Zone> zones = {Location::Zone::LEFT_FLANK, Location::Zone::CENTER, Location::Zone::RIGHT_FLANK};

                        for (auto i = 0; i < zones.size(); i++)
                        {
                            if (Engine::ACTIVE(party.Army, enemyArmy, zones[i]))
                            {
                                // resolve combat, spells, special status
                                resolveMassCombat(window, renderer, location, party, enemyArmy, enemySpells, enemyStatus, zones[i], combat_round);
                            }
                        }

                        combat_round++;

                        party_has_cast = false;

                        enemy_has_cast = false;

                        enemy_spell = Spells::MassCombat::NONE;
                    }
                    else if (current_mode == Engine::MassCombatMode::SPELL)
                    {
                        if (enemy_spell == Spells::MassCombat::ENFEEBLEMENT_CENTER_FRONT)
                        {
                            auto party_unit = Engine::FIND_UNIT(party.Army, Location::BattleField::CENTER_FRONT);

                            if (party_unit >= 0 && party_unit < party.Army.size())
                            {
                                Engine::GAIN_STRENGTH(party.Army[party_unit], -1);
                            }
                        }
                        else if (enemy_spell == Spells::MassCombat::CLINGING_DREAD_LEFT_FRONT)
                        {
                            auto party_unit = Engine::FIND_UNIT(party.Army, Location::BattleField::LEFT_FLANK_FRONT);

                            if (party_unit >= 0 && party_unit < party.Army.size())
                            {
                                Engine::GAIN_MORALE(party.Army[party_unit], -1);
                            }
                        }
                        else if (enemy_spell == Spells::MassCombat::ROUT_LEFT_FRONT)
                        {
                            auto party_unit = Engine::FIND_UNIT(party.Army, Location::BattleField::LEFT_FLANK_FRONT);

                            if (party_unit >= 0 && party_unit < party.Army.size())
                            {
                                auto result = moraleCheck(window, renderer, party.Army[party_unit], combat_round);

                                if (!result)
                                {
                                    party.Army[party_unit].Position = Location::BattleField::NONE;

                                    auto next_unit = Engine::FIND_UNIT(party.Army, Location::BattleField::LEFT_FLANK_SUPPORT);

                                    if (next_unit >= 0 && next_unit < party.Army.size())
                                    {
                                        party.Army[next_unit].Position = Location::BattleField::LEFT_FLANK_FRONT;
                                    }
                                }
                            }
                        }

                        current_mode = Engine::MassCombatMode::NORMAL;
                    }

                    selected = false;

                    current = -1;
                }
                else if (controls[current].Type == Control::Type::SPELL && !hold)
                {
                    if (Engine::SPELLCASTERS(party) > 0)
                    {
                        auto combat_spells = 0;

                        for (auto i = 0; i < party.Members.size(); i++)
                        {
                            if (Engine::IS_ALIVE(party.Members[i]) && party.Members[i].SpellCaster)
                            {
                                auto spells = Engine::COUNT(party.Members[i].SpellBook, Spells::Scope::SEA_COMBAT);

                                combat_spells += spells;
                            }
                        }

                        if (combat_spells <= 0)
                        {
                            displayMessage("Your party does not have any usable mass combat spells.", intRD);
                        }
                        else
                        {
                            // TODO: Sea combat spell
                            selected = false;

                            current = -1;
                        }
                    }
                    else
                    {
                        displayMessage("There are no spell casters in your party!", intRD);
                    }
                }
            }
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
    }

    if (Engine::ZONES(party.Army, enemyArmy) > 1)
    {
        combatResult = Engine::Combat::VICTORY;
    }
    else if (Engine::ZONES(enemyArmy, party.Army) > 1)
    {
        combatResult = Engine::Combat::DEFEAT;
    }

    return combatResult;
}

Engine::Combat deploymentScreen(SDL_Window *window, SDL_Renderer *renderer, Location::Type location, Party::Base &party, std::vector<Army::Base> &enemyArmy, std::vector<Engine::BattlefieldSpells> &enemySpells, std::vector<Engine::ArmyStatus> &enemyStatus)
{
    auto combatResult = Engine::Combat::NONE;

    if (window && renderer)
    {
        auto flash_message = false;

        auto flash_color = intRD;

        std::string message = "";

        auto messageh = (int)(0.125 * SCREEN_HEIGHT);

        Uint32 start_ticks = 0;

        Uint32 duration = 3000;

        // Lambda functions for displaying flash messages
        auto displayMessage = [&](std::string msg, Uint32 color)
        {
            flash_message = true;

            message = msg;

            flash_color = color;

            start_ticks = SDL_GetTicks();
        };

        TTF_Init();

        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);
        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 28);

        TTF_SetFontKerning(font_dark11, 0);

        auto font_size = 28;

        auto box_space = 10;

        auto text_space = 8;

        auto marginx = (int)(Margin * SCREEN_WIDTH);

        auto fullwidth = SCREEN_WIDTH - 2 * marginx;

        auto boxw = (int)((fullwidth - 2 * box_space) / 3);

        auto infoh = 48;

        auto boxh = (int)((text_bounds - box_space - infoh) / 2);

        auto popupw = (int)(0.6 * SCREEN_WIDTH);
        auto popuph = (int)(0.6 * SCREEN_HEIGHT);
        auto popupx = (SCREEN_WIDTH - popupw) / 2;
        auto popupy = (SCREEN_HEIGHT - popuph) / 2;

        auto offset = 0;
        auto limit = (popuph - infoh - buttonh - button_space) / (88);
        auto last = offset + limit;

        if (last > party.Army.size())
        {
            last = party.Army.size();
        }

        auto done = false;

        auto main_buttonw = 220;

        auto controls = std::vector<Button>();

        auto controls_battlefield = std::vector<Button>();

        for (auto y = 0; y < 2; y++)
        {
            for (auto x = 0; x < 3; x++)
            {
                SDL_Rect rect;

                rect.w = boxw - 2 * 8;
                rect.h = boxh - 2 * 8;
                rect.x = 0;
                rect.y = 0;

                auto button = Button();

                button.Surface = SDL_CreateRGBSurface(0, boxw - 2 * 8, boxh - 2 * 8, 32, 0, 0, 0, 0);

                if (y > 0)
                {
                    SDL_FillRect(button.Surface, &rect, intBE);
                }
                else
                {
                    SDL_FillRect(button.Surface, &rect, intBE);
                }

                button.ID = y * 3 + x;
                button.W = boxw - 2 * 8;
                button.H = boxh - 2 * 8;
                button.Left = x > 0 ? y * 3 + x - 1 : y * 3 + x;
                button.Right = x < 2 ? y * 3 + x + 1 : y * 3 + x;
                button.Up = y > 0 ? (y - 1) * 3 + x : y * 3 + x;
                button.Down = y < 1 ? (y + 1) * 3 + x : (x < 2 ? (y + 1) * 3 + x : y * 3 + x);
                button.X = startx + x * (boxw + box_space) + border_space;
                button.Y = starty + infoh + y * (boxh + box_space) + border_space + text_space;
                button.Type = Control::Type::ACTION;

                controls_battlefield.push_back(button);
            }
        }

        auto text_y = (int)(SCREEN_HEIGHT * (1.0 - Margin)) - 48;

        controls_battlefield.push_back(Button(6, createHeaderButton(window, FONT_DARK11, font_size, "Start Battle", clrWH, intDB, 220, 48, -1), 6, 7, 3, 6, startx, text_y, Control::Type::CONFIRM));
        controls_battlefield.push_back(Button(7, createHeaderButton(window, FONT_DARK11, font_size, "Cancel", clrWH, intDB, 220, 48, -1), 6, 7, 4, 7, startx + (main_buttonw + button_space), text_y, Control::Type::BACK));

        auto controls_deploy = popupArmy(window, renderer, party.Army, offset, last, limit, popupw, popuph, infoh, popupx, popupy);

        auto current = -1;
        auto selected = false;
        auto hold = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto scrollSpeed = 1;

        auto current_zone = Location::Zone::NONE;
        auto current_mode = Engine::MassCombatMode::NORMAL;

        auto left_flank = std::vector<int>();
        auto center = std::vector<int>();
        auto right_flank = std::vector<int>();

        Engine::CLEAR_POSITIONS(party.Army);

        while (!done)
        {
            if (last > party.Army.size())
            {
                last = party.Army.size();
            }

            SDL_SetWindowTitle(window, "Legendary Kingdoms: Army Deployment");

            fillWindow(renderer, intWH);

            for (auto y = 0; y < 2; y++)
            {
                for (auto x = 0; x < 3; x++)
                {
                    if (y > 0)
                    {
                        fillRect(renderer, boxw, boxh, startx + x * (boxw + box_space), starty + infoh + y * (boxh + box_space) + text_space, intBE);

                        thickRect(renderer, boxw - 2 * text_space, boxh - 2 * text_space, startx + x * (boxw + box_space) + border_space, starty + infoh + y * (boxh + box_space) + text_space + border_space, intBR, border_pts);
                    }
                    else
                    {
                        fillRect(renderer, boxw, boxh, startx + x * (boxw + box_space), starty + infoh + y * (boxh + box_space) + text_space, intBE);
                    }
                }
            }

            putHeader(renderer, "Left Flank", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, boxw, infoh, startx, starty);
            putHeader(renderer, "Center", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, boxw, infoh, startx + (boxw + box_space), starty);
            putHeader(renderer, "Right Flank", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, boxw, infoh, startx + 2 * (boxw + box_space), starty);

            if (current_mode == Engine::MassCombatMode::NORMAL)
            {
                renderButtons(renderer, controls_battlefield, current, intLB, border_space, border_pts);

                controls = controls_battlefield;
            }
            else
            {
                renderButtons(renderer, controls_battlefield, -1, intLB, border_space, border_pts);
            }

            // Render Enemy army
            renderArmy(renderer, font_garamond, text_space, enemyArmy, boxw, boxh, box_space, starty + infoh, clrBK, intBE);

            // Render Party army
            renderArmy(renderer, font_garamond, text_space, party.Army, boxw, boxh, box_space, starty + infoh + boxh + box_space, clrBK, intBE);

            if (current_mode == Engine::MassCombatMode::DEPLOY)
            {
                fillRect(renderer, popupw, popuph, popupx, popupy, intBE);

                drawRect(renderer, popupw, popuph, popupx, popupy, intBK);

                std::string deploy_string = "Select unit to deploy to the ";

                if (current_zone == Location::Zone::LEFT_FLANK)
                {
                    deploy_string += "left flank";
                }
                else if (current_zone == Location::Zone::CENTER)
                {
                    deploy_string += "center";
                }
                else if (current_zone == Location::Zone::RIGHT_FLANK)
                {
                    deploy_string += "right flank";
                }
                else
                {
                    deploy_string += "battlefield";
                }

                putHeader(renderer, deploy_string.c_str(), font_dark11, text_space, clrWH, intDB, TTF_STYLE_NORMAL, popupw, infoh, popupx, popupy);

                controls = controls_deploy;

                if (last - offset > 0)
                {
                    for (auto i = 0; i < last - offset; i++)
                    {
                        auto selection = left_flank;

                        if (current_zone == Location::Zone::LEFT_FLANK)
                        {
                            selection = left_flank;
                        }
                        else if (current_zone == Location::Zone::CENTER)
                        {
                            selection = center;
                        }
                        else if (current_zone == Location::Zone::RIGHT_FLANK)
                        {
                            selection = right_flank;
                        }

                        if (Engine::FIND_LIST(selection, offset + i) >= 0)
                        {
                            thickRect(renderer, controls[i].W + border_pts, controls[i].H + border_pts, controls[i].X - 2, controls[i].Y - 2, intLB, 2);
                        }
                        else
                        {
                            drawRect(renderer, controls[i].W + border_space, controls[i].H + border_space, controls[i].X - 4, controls[i].Y - 4, intBK);
                        }
                    }
                }

                renderButtons(renderer, controls_deploy, current, intLB, border_space, border_pts);
            }

            if (flash_message)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putHeader(renderer, message.c_str(), font_garamond, -1, clrWH, flash_color, TTF_STYLE_NORMAL, splashw * 2, messageh, -1, -1);
                }
                else
                {
                    flash_message = false;
                }
            }

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
            {
                if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                {
                    if (current_mode == Engine::MassCombatMode::DEPLOY)
                    {
                        if (offset > 0)
                        {
                            offset -= scrollSpeed;

                            if (offset < 0)
                            {
                                offset = 0;
                            }

                            last = offset + limit;

                            if (last > party.Army.size())
                            {
                                last = party.Army.size();
                            }

                            controls_deploy.clear();

                            controls_deploy = popupArmy(window, renderer, party.Army, offset, last, limit, popupw, popuph, infoh, popupx, popupy);

                            SDL_Delay(50);
                        }

                        if (offset <= 0)
                        {
                            current = -1;

                            selected = false;
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
                {
                    if (current_mode == Engine::MassCombatMode::DEPLOY)
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

                            controls_deploy.clear();

                            controls_deploy = popupArmy(window, renderer, party.Army, offset, last, limit, popupw, popuph, infoh, popupx, popupy);

                            if (offset > 0)
                            {
                                if (controls_deploy[current].Type != Control::Type::SCROLL_DOWN)
                                {
                                    current++;
                                }
                            }

                            SDL_Delay(50);
                        }

                        if (party.Army.size() - last <= 0)
                        {
                            selected = false;

                            current = -1;
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (current_mode == Engine::MassCombatMode::NORMAL)
                    {
                        if (current >= 0 && current < 6)
                        {
                            if (current > 2 && current < 6)
                            {
                                if (current == 3)
                                {
                                    current_zone = Location::Zone::LEFT_FLANK;
                                }
                                else if (current == 4)
                                {
                                    current_zone = Location::Zone::CENTER;
                                }
                                else if (current == 5)
                                {
                                    current_zone = Location::Zone::RIGHT_FLANK;
                                }

                                current_mode = Engine::MassCombatMode::DEPLOY;

                                current = -1;
                            }
                        }

                        selected = false;
                    }
                    else if (current_mode == Engine::MassCombatMode::DEPLOY)
                    {
                        if (current_zone == Location::Zone::LEFT_FLANK)
                        {
                            if (current + offset >= 0 && current + offset < party.Army.size())
                            {
                                auto result = Engine::FIND_LIST(left_flank, current + offset);
                                auto center_result = Engine::FIND_LIST(center, current + offset);
                                auto right_result = Engine::FIND_LIST(right_flank, current + offset);

                                if (result >= 0 && result < left_flank.size())
                                {
                                    left_flank.erase(left_flank.begin() + result);
                                }
                                else if (left_flank.size() < 2)
                                {
                                    if (party.Army[current + offset].Garrison != location)
                                    {
                                        displayMessage("You can only deploy a unit from the " + std::string(Location::Description[location]) + " barracks!", intRD);
                                    }
                                    else
                                    {
                                        left_flank.push_back(current + offset);

                                        if (center_result >= 0 && center_result < center.size())
                                        {
                                            center.erase(center.begin() + center_result);
                                        }

                                        if (right_result >= 0 && right_result < right_flank.size())
                                        {
                                            right_flank.erase(right_flank.begin() + right_result);
                                        }
                                    }
                                }
                            }
                        }
                        else if (current_zone == Location::Zone::CENTER)
                        {
                            if (current + offset >= 0 && current + offset < party.Army.size())
                            {
                                auto result = Engine::FIND_LIST(center, current + offset);
                                auto right_result = Engine::FIND_LIST(right_flank, current + offset);
                                auto left_result = Engine::FIND_LIST(left_flank, current + offset);

                                if (result >= 0 && result < center.size())
                                {
                                    center.erase(center.begin() + result);
                                }
                                else if (center.size() < 2)
                                {
                                    if (party.Army[current + offset].Garrison != location)
                                    {
                                        displayMessage("You can only deploy a unit from the " + std::string(Location::Description[location]) + " barracks!", intRD);
                                    }
                                    else
                                    {
                                        center.push_back(current + offset);

                                        if (right_result >= 0 && right_result < right_flank.size())
                                        {
                                            right_flank.erase(right_flank.begin() + right_result);
                                        }

                                        if (left_result >= 0 && left_result < left_flank.size())
                                        {
                                            left_flank.erase(left_flank.begin() + left_result);
                                        }
                                    }
                                }
                            }
                        }
                        else if (current_zone == Location::Zone::RIGHT_FLANK)
                        {
                            if (current + offset >= 0 && current + offset < party.Army.size())
                            {
                                auto result = Engine::FIND_LIST(right_flank, current + offset);
                                auto center_result = Engine::FIND_LIST(center, current + offset);
                                auto left_result = Engine::FIND_LIST(left_flank, current + offset);

                                if (result >= 0 && result < right_flank.size())
                                {
                                    right_flank.erase(right_flank.begin() + result);
                                }
                                else if (right_flank.size() < 2)
                                {
                                    if (party.Army[current + offset].Garrison != location)
                                    {
                                        displayMessage("You can only deploy a unit from the " + std::string(Location::Description[location]) + " barracks!", intRD);
                                    }
                                    else
                                    {
                                        right_flank.push_back(current + offset);

                                        if (center_result >= 0 && center_result < center.size())
                                        {
                                            center.erase(center.begin() + center_result);
                                        }

                                        if (left_result >= 0 && left_result < left_flank.size())
                                        {
                                            left_flank.erase(left_flank.begin() + left_result);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::CONFIRM && !hold)
                {
                    if (current_mode == Engine::MassCombatMode::NORMAL)
                    {
                        if (Engine::ZONES(enemyArmy, party.Army) > 1)
                        {
                            combatResult = Engine::Combat::DEFEAT;

                            done = true;
                        }
                        else if (Engine::ZONES(party.Army, enemyArmy) > 1)
                        {
                            combatResult = Engine::Combat::VICTORY;

                            done = true;
                        }
                        else
                        {
                            combatResult = massCombatScreen(window, renderer, location, party, enemyArmy, enemySpells, enemyStatus);

                            done = true;
                        }
                    }
                    else if (current_mode == Engine::MassCombatMode::DEPLOY)
                    {
                        Engine::CLEAR_POSITIONS(party.Army);

                        if (left_flank.size() > 0)
                        {
                            party.Army[left_flank[0]].Position = Location::BattleField::LEFT_FLANK_FRONT;

                            if (left_flank.size() > 1)
                            {
                                party.Army[left_flank[1]].Position = Location::BattleField::LEFT_FLANK_SUPPORT;
                            }
                        }

                        if (center.size() > 0)
                        {
                            party.Army[center[0]].Position = Location::BattleField::CENTER_FRONT;

                            if (center.size() > 1)
                            {
                                party.Army[center[1]].Position = Location::BattleField::CENTER_SUPPORT;
                            }
                        }

                        if (right_flank.size() > 0)
                        {
                            party.Army[right_flank[0]].Position = Location::BattleField::RIGHT_FLANK_FRONT;

                            if (right_flank.size() > 1)
                            {
                                party.Army[right_flank[1]].Position = Location::BattleField::RIGHT_FLANK_SUPPORT;
                            }
                        }

                        current_mode = Engine::MassCombatMode::NORMAL;

                        current_zone = Location::Zone::NONE;

                        current = -1;

                        selected = false;
                    }
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    if (current_mode == Engine::MassCombatMode::NORMAL)
                    {
                        done = true;
                    }
                    else if (current_mode == Engine::MassCombatMode::DEPLOY)
                    {
                        current_mode = Engine::MassCombatMode::NORMAL;

                        current_zone = Location::Zone::NONE;

                        current = -1;

                        selected = false;
                    }
                }
            }
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
    }

    return combatResult;
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

void addBye(Story::Base *story, std::string bye)
{
    if (bye.length() > 0)
    {
        if (story->Bye == NULL)
        {
            story->temp_string = bye;
        }
        else
        {
            story->temp_string = std::string(story->Bye) + "\n\n" + bye;
        }

        story->Bye = story->temp_string.c_str();
    }
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
        SDL_Texture *splashTexture = NULL;

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
        auto listwidth = (int)((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);
        auto boxh = (int)(0.125 * SCREEN_HEIGHT);
        auto infoh = 48;
        auto box_space = 10;
        auto offset = 0;
        auto limit = (text_bounds - 2 * text_space) / (88);
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

            splashTexture = SDL_CreateTextureFromSurface(renderer, splash);
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

                if (Engine::COUNT(party) > 0)
                {
                    std::string party_string = "";

                    auto count = 0;

                    for (auto i = 0; i < party.Members.size(); i++)
                    {
                        if (count > 0)
                        {
                            party_string += "\n";
                        }

                        party_string += party.Members[i].Name;

                        if (Engine::IS_DEAD(party.Members[i]))
                        {
                            party_string += " (D)";
                        }
                        else if (Engine::IS_CURSED(party.Members[i]))
                        {
                            party_string += " (C)";
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

            if ((!splash || (splash && splash_h < (text_bounds - 4 * boxh - 2 * infoh - box_space))) && (Engine::COUNT_TEAMS(party) > 0 || story->Team != Team::Type::NONE))
            {
                std::vector<Team::Type> teams = {};

                if (Engine::IS_ACTIVE(party, party.CurrentCharacter) || story->Team != Team::Type::NONE)
                {
                    putHeader(renderer, "Current", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (4 * boxh + 2 * infoh + box_space));

                    if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                    {
                        teams.push_back(Engine::GET_TEAM(party.Members[party.CurrentCharacter]));
                    }
                    else
                    {
                        teams.push_back(story->Team);
                    }
                }
                else
                {
                    putHeader(renderer, "Teams", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (4 * boxh + 2 * infoh + box_space));

                    teams = Engine::GET_TEAMS(party);
                }

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

            fillRect(renderer, listwidth, text_bounds, textx, texty, BE_80);

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

            if (splash)
            {
                auto mousex = 0;
                auto mousey = 0;

                SDL_GetMouseState(&mousex, &mousey);

                auto zoomw = (int)(0.80 * (double)listwidth);
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
                        dst.x = (textx + (listwidth - zoomw) / 2);
                        dst.y = (texty + (text_bounds - zoomh) / 2);

                        fillRect(renderer, dst.w, dst.h, dst.x, dst.y, intWH);
                        SDL_RenderCopy(renderer, splashTexture, &src, &dst);
                        drawRect(renderer, dst.w + 2, dst.h + 2, dst.x - 1, dst.y - 1, intBK);
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

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
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
                        }
                        else if (story->Choices[choice].Type == Choice::Type::RESET_SHOP)
                        {
                            story->ResetShop = true;

                            next = findStory(story->Choices[choice].Destination);

                            done = true;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::ATTRIBUTES)
                        {
                            party.CurrentCharacter = Engine::FIND_SOLO(party);

                            auto selection = std::vector<int>();

                            auto success = false;

                            if (Engine::IS_ACTIVE(party, party.CurrentCharacter) && party.Members[party.CurrentCharacter].Team == Team::Type::SOLO)
                            {
                                selection = {party.CurrentCharacter};

                                success = skillTestScreen(window, renderer, party, story->Choices[choice].Team, selection, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, story->Choices[choice].UseWeapon);
                            }
                            else
                            {
                                success = skillCheck(window, renderer, party, story->Choices[choice].Team, 1, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, selection, story->Choices[choice].UseWeapon);
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
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::EVERYONE_SKILL_CHECKS)
                        {
                            if (Engine::COUNT(party, story->Choices[choice].Team) > 0)
                            {
                                for (auto i = 0; i < party.Members.size(); i++)
                                {
                                    if (Engine::IS_ACTIVE(party, i) && (story->Choices[choice].Team == Team::Type::NONE || party.Members[i].Team == story->Choices[choice].Team))
                                    {
                                        auto success = skillTestScreen(window, renderer, party, story->Choices[choice].Team, {i}, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, story->Choices[choice].UseWeapon);

                                        story->SkillCheck(party, success, {i});
                                    }
                                }
                            }

                            next = findStory(story->Choices[choice].Destination);

                            done = true;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::LAST_CHARACTER)
                        {
                            party.CurrentCharacter = Engine::FIND_SOLO(party);

                            auto selection = std::vector<int>();

                            if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                            {
                                party.LastSelected = party.CurrentCharacter;
                            }

                            auto success = false;

                            if (Engine::IS_ACTIVE(party, party.LastSelected))
                            {
                                if (Engine::IS_ALIVE(party.Members[party.LastSelected]))
                                {
                                    selection.push_back(party.LastSelected);

                                    success = skillTestScreen(window, renderer, party, story->Choices[choice].Team, selection, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, story->Choices[choice].UseWeapon);
                                }
                                else
                                {
                                    success = skillCheck(window, renderer, party, story->Choices[choice].Team, 1, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, selection, story->Choices[choice].UseWeapon);
                                }
                            }
                            else
                            {
                                success = skillCheck(window, renderer, party, story->Choices[choice].Team, 1, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, selection, story->Choices[choice].UseWeapon);
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
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::ORDER_SKILL_CHECK)
                        {
                            if (party.Order.size() > 0 && ((story->Choices[choice].Value - 1) >= 0) && ((story->Choices[choice].Value - 1) < party.Order.size()))
                            {
                                party.LastSelected = Engine::FIND_CHARACTER(party, party.Order[story->Choices[choice].Value - 1]);
                            }
                            else
                            {
                                party.CurrentCharacter = Engine::FIND_SOLO(party);

                                if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                                {
                                    party.LastSelected = party.CurrentCharacter;
                                }
                            }

                            auto selection = std::vector<int>();

                            auto success = false;

                            if (Engine::IS_ACTIVE(party, party.LastSelected))
                            {
                                selection.push_back(party.LastSelected);

                                success = skillTestScreen(window, renderer, party, story->Choices[choice].Team, selection, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, story->Choices[choice].UseWeapon);
                            }
                            else
                            {
                                success = skillCheck(window, renderer, party, story->Choices[choice].Team, 1, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, selection, story->Choices[choice].UseWeapon);
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
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::TEAM_ATTRIBUTES)
                        {
                            auto selection = std::vector<int>();

                            auto success = skillCheck(window, renderer, party, story->Choices[choice].Team, 2, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, selection, story->Choices[choice].UseWeapon);

                            if (selection.size() == 2 || (selection.size() > 0 && selection.size() >= Engine::COUNT(party, story->Choices[choice].Team)))
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
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::PARTY_EXCEPT_WITHSTATUS)
                        {
                            auto with_status = Engine::COUNT(party, story->Choices[choice].Status[0]);
                            auto party_count = Engine::COUNT(party, story->Choices[choice].Team);

                            auto team_size = std::min(2, party_count - with_status);

                            if (team_size > 0)
                            {
                                auto selection = selectPartyMembers(window, renderer, party, story->Choices[choice].Team, team_size, Control::Type::SKILL);

                                if (selection.size() >= team_size)
                                {
                                    auto valid = true;

                                    for (auto i = 0; i < selection.size(); i++)
                                    {
                                        if (Engine::HAS_STATUS(party.Members[selection[i]], story->Choices[choice].Status[0]))
                                        {
                                            valid = false;
                                        }
                                    }

                                    if (valid)
                                    {
                                        auto success = skillTestScreen(window, renderer, party, story->Choices[choice].Team, selection, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, story->Choices[choice].UseWeapon);

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
                                    }
                                    else
                                    {
                                        error = true;

                                        if (selection.size() == 1)
                                        {
                                            message = "The party member selected is not valid";
                                        }
                                        else
                                        {
                                            message = "One of the party members selected is not valid";
                                        }
                                    }
                                }
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::EQUIPMENT)
                        {
                            auto equipment = std::vector<Equipment::Type>();

                            for (auto i = 0; i < story->Choices[choice].Equipment.size(); i++)
                            {
                                equipment.push_back(story->Choices[choice].Equipment[i].Type);
                            }

                            party.CurrentCharacter = Engine::FIND_SOLO(party);

                            if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                            {
                                if (Engine::VERIFY_EQUIPMENT(party.Members[party.CurrentCharacter], equipment))
                                {
                                    next = findStory(story->Choices[choice].Destination);

                                    done = true;
                                }
                                else
                                {
                                    error = true;

                                    if (story->Choices[choice].Equipment.size() > 1)
                                    {
                                        message = "You do not have the required items!";
                                    }
                                    else
                                    {
                                        message = "You do not have the required item!";
                                    }
                                }
                            }
                            else
                            {
                                if (Engine::VERIFY_EQUIPMENT(party, equipment))
                                {
                                    next = findStory(story->Choices[choice].Destination);

                                    done = true;
                                }
                                else
                                {
                                    error = true;

                                    if (story->Choices[choice].Equipment.size() > 1)
                                    {
                                        message = "You do not have the required items!";
                                    }
                                    else
                                    {
                                        message = "You do not have the required item!";
                                    }
                                }
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::ANY_EQUIPMENT)
                        {
                            auto equipment = std::vector<Equipment::Type>();

                            for (auto i = 0; i < story->Choices[choice].Equipment.size(); i++)
                            {
                                equipment.push_back(story->Choices[choice].Equipment[i].Type);
                            }

                            party.CurrentCharacter = Engine::FIND_SOLO(party);

                            if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                            {
                                if (Engine::VERIFY_ANY_EQUIPMENT(party.Members[party.CurrentCharacter], equipment))
                                {
                                    next = findStory(story->Choices[choice].Destination);

                                    done = true;
                                }
                                else
                                {
                                    error = true;

                                    message = "You do not have any of the required items!";
                                }
                            }
                            else
                            {
                                if (Engine::VERIFY_ANY_EQUIPMENT(party, equipment))
                                {
                                    next = findStory(story->Choices[choice].Destination);

                                    done = true;
                                }
                                else
                                {
                                    error = true;

                                    message = "You do not have any of the required items!";
                                }
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
                            }
                            else
                            {
                                error = true;

                                if (location != Location::Type::NONE)
                                {
                                    message = "You do not have a ship at " + std::string(Location::Description[location]);
                                }
                                else
                                {
                                    message = "You do not have a ship at this location!";
                                }
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::SAIL)
                        {
                            auto location = story->Choices[choice].Location;

                            if (location == Location::Type::NONE)
                            {
                                location = party.Location;
                            }

                            if (Engine::HAS_SHIP(party, location))
                            {
                                auto result = selectShip(window, renderer, party.Fleet, story->Choices[choice].Location, {}, Control::Type::SAIL);

                                if (result >= 0 && result < party.Fleet.size())
                                {
                                    party.CurrentShip = result;

                                    next = findStory(story->Choices[choice].Destination);

                                    done = true;
                                }
                            }
                            else
                            {
                                error = true;

                                if (location != Location::Type::NONE)
                                {
                                    message = "You do not have a ship at " + std::string(Location::Description[location]);
                                }
                                else
                                {
                                    message = "You do not have a ship at this location!";
                                }
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::TEAM_MAX_HEALTH)
                        {
                            auto target = std::vector<int>();

                            party.CurrentCharacter = Engine::FIND_SOLO(party);

                            if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                            {
                                target = {party.CurrentCharacter};
                            }
                            else if (Engine::COUNT(party) == 1)
                            {
                                target = {Engine::FIRST(party)};
                            }
                            else
                            {
                                target = selectPartyMembers(window, renderer, party, story->Choices[choice].Team, story->Choices[choice].Value, Control::Type::RAISE_MAX_HEALTH);
                            }

                            if (target.size() > 0)
                            {
                                for (auto i = 0; i < target.size(); i++)
                                {
                                    if (target[i] >= 0 && target[i] < party.Members.size())
                                    {
                                        party.Members[target[i]].MaximumHealth += story->Choices[choice].Success;

                                        Engine::GAIN_HEALTH(party.Members[target[i]], story->Choices[choice].Success);
                                    }
                                }

                                next = findStory(story->Choices[choice].Destination);

                                done = true;
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::ADD_MAX_HEALTH)
                        {
                            auto target = -1;

                            party.CurrentCharacter = Engine::FIND_SOLO(party);

                            if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                            {
                                target = party.CurrentCharacter;
                            }
                            else if (Engine::COUNT(party) == 1)
                            {
                                target = Engine::FIRST(party);
                            }
                            else
                            {
                                target = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::HEALTH);
                            }

                            party.Members[target].MaximumHealth += story->Choices[choice].Value;

                            Engine::GAIN_HEALTH(party.Members[target], story->Choices[choice].Value);

                            next = findStory(story->Choices[choice].Destination);

                            done = true;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::GAIN_HEALTH)
                        {
                            auto target = -1;

                            party.CurrentCharacter = Engine::FIND_SOLO(party);

                            if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                            {
                                target = party.CurrentCharacter;
                            }
                            else
                            {
                                if (Engine::COUNT(party) == 1)
                                {
                                    target = Engine::FIRST(party);
                                }
                                else
                                {
                                    target = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, (story->Choices[choice].Value > 0 ? Control::Type::GAIN_HEALTH : Control::Type::LOSE_HEALTH));
                                }
                            }

                            if (target >= 0 && target < party.Members.size())
                            {
                                story->temp_string = std::string(party.Members[target].Name) + " ";

                                if (story->Choices[choice].Value > 0)
                                {
                                    story->temp_string += "gains " + std::to_string(story->Choices[choice].Value);
                                }
                                else
                                {
                                    story->temp_string += "loses " + std::to_string(-story->Choices[choice].Value);
                                }

                                story->temp_string += " Health point";

                                if (std::abs(story->Choices[choice].Value) > 1)
                                {
                                    story->temp_string += "s";
                                }

                                story->temp_string += ".";

                                addBye(story, story->temp_string.c_str());

                                Engine::GAIN_HEALTH(party.Members[target], story->Choices[choice].Value);
                            }

                            next = findStory(story->Choices[choice].Destination);

                            done = true;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::GAIN_HEALTH_ATTRIBUTE)
                        {
                            auto target = -1;

                            party.CurrentCharacter = Engine::FIND_SOLO(party);

                            if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                            {
                                target = party.CurrentCharacter;
                            }
                            else
                            {
                                if (Engine::COUNT(party) == 1)
                                {
                                    target = Engine::FIRST(party);
                                }
                                else
                                {
                                    target = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, (story->Choices[choice].Value > 0 ? Control::Type::GAIN_HEALTH : Control::Type::LOSE_HEALTH));
                                }
                            }

                            if (target >= 0 && target < party.Members.size())
                            {
                                story->temp_string = std::string(party.Members[target].Name) + " ";

                                if (story->Choices[choice].Value > 0)
                                {
                                    story->temp_string += "gains " + std::to_string(story->Choices[choice].Value);
                                }
                                else
                                {
                                    story->temp_string += "loses " + std::to_string(-story->Choices[choice].Value);
                                }

                                story->temp_string += " Health point";

                                if (std::abs(story->Choices[choice].Value) > 1)
                                {
                                    story->temp_string += "s";
                                }

                                story->temp_string += " and ";

                                if (story->Choices[choice].Difficulty > 0)
                                {
                                    story->temp_string += "gains " + std::to_string(story->Choices[choice].Difficulty);
                                }
                                else
                                {
                                    story->temp_string += "loses " + std::to_string(-story->Choices[choice].Difficulty);
                                }

                                story->temp_string += " " + std::string(Attribute::Descriptions[story->Choices[choice].Attributes[0]]) + ".";

                                addBye(story, story->temp_string.c_str());

                                Engine::GAIN_HEALTH(party.Members[target], story->Choices[choice].Value);

                                Engine::GAIN_SCORE(party.Members[target], story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty);
                            }

                            next = findStory(story->Choices[choice].Destination);

                            done = true;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::BRIBE_CODEWORD || story->Choices[choice].Type == Choice::Type::PAY_WITH)
                        {
                            auto equipment = std::vector<Equipment::Type>();

                            for (auto i = 0; i < story->Choices[choice].Equipment.size(); i++)
                            {
                                equipment.push_back(story->Choices[choice].Equipment[i].Type);
                            }

                            auto count = Engine::COUNT_EQUIPMENT(party, equipment);

                            if (count >= story->Choices[choice].Value)
                            {
                                Engine::LOSE_EQUIPMENT(party, equipment[0], story->Choices[choice].Value);

                                if (story->Choices[choice].Type == Choice::Type::BRIBE_CODEWORD)
                                {
                                    Engine::GET_CODES(party, story->Choices[choice].InvisibleCodes);
                                }

                                next = findStory(story->Choices[choice].Destination);

                                done = true;
                            }
                            else
                            {
                                if (story->Choices[choice].Value > 1)
                                {
                                    message = "You do not have the required items!";
                                }
                                else
                                {
                                    message = "You do not have the required item!";
                                }

                                error = true;
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::ROLL_FOR_ATTRIBUTE_INCREASE)
                        {
                            auto target = -1;

                            auto team = story->Choices[choice].Team;

                            party.CurrentCharacter = Engine::FIND_SOLO(party);

                            if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                            {
                                target = party.CurrentCharacter;

                                team = party.Members[party.CurrentCharacter].Team;
                            }
                            else if (Engine::COUNT(party, story->Choices[choice].Team) == 1)
                            {
                                target = Engine::FIRST(party, story->Choices[choice].Team);
                            }
                            else
                            {
                                target = selectPartyMember(window, renderer, party, story->Choices[choice].Team, Equipment::NONE, Control::Type::ROLL_FOR_ATTRIBUTE_INCREASE);
                            }

                            if (target >= 0 && target < party.Members.size())
                            {
                                auto increase = gainAttributeScore(window, renderer, party.Members[target], story->Choices[choice].Attributes[0], story->Choices[choice].Value, story->Choices[choice].Difficulty);

                                if (increase >= 0)
                                {
                                    next = findStory(story->Choices[choice].Destination);

                                    done = true;
                                }
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::LASTCHARACTER_ROLLS_FOR_ATTRIBUTEINCREASE)
                        {
                            if (Engine::IS_ACTIVE(party, party.LastSelected))
                            {
                                auto increase = gainAttributeScore(window, renderer, party.Members[party.LastSelected], story->Choices[choice].Attributes[0], story->Choices[choice].Value, story->Choices[choice].Difficulty);

                                if (increase >= 0)
                                {
                                    next = findStory(story->Choices[choice].Destination);

                                    done = true;
                                }
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::ROLL_ATTRIBUTE_WITH_STATUS)
                        {
                            auto target = -1;

                            if (Engine::HAS_STATUS(party, story->Choices[choice].Status[0]))
                            {
                                target = Engine::FIND_CHARACTER(party, story->Choices[choice].Status[0]);
                            }
                            else
                            {
                                party.CurrentCharacter = Engine::FIND_SOLO(party);

                                if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                                {
                                    target = party.CurrentCharacter;
                                }
                                else if (Engine::COUNT(party) == 1)
                                {
                                    target = Engine::FIRST(party);
                                }
                            }

                            if (target >= 0 && target < party.Members.size())
                            {
                                auto increase = gainAttributeScore(window, renderer, party.Members[target], story->Choices[choice].Attributes[0], story->Choices[choice].Value, story->Choices[choice].Difficulty);

                                if (increase >= 0)
                                {
                                    next = findStory(story->Choices[choice].Destination);

                                    done = true;
                                }
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::RAISE_ATTRIBUTE_SCORE)
                        {
                            auto target = -1;

                            party.CurrentCharacter = Engine::FIND_SOLO(party);

                            if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                            {
                                target = party.CurrentCharacter;
                            }
                            else if (Engine::COUNT(party) == 1)
                            {
                                target = Engine::FIRST(party);
                            }
                            else
                            {
                                target = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::RAISE_ATTRIBUTE_SCORE);
                            }

                            if (target >= 0 && target < party.Members.size())
                            {
                                auto increase = gainAttributeScore(window, renderer, party.Members[target], story->Choices[choice].Attributes[0], story->Choices[choice].Value, 0);

                                if (increase >= 0)
                                {
                                    next = findStory(story->Choices[choice].Destination);

                                    done = true;
                                }
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::PARTY_RAISE_ATTRIBUTE)
                        {
                            for (auto i = 0; i < party.Members.size(); i++)
                            {
                                if (Engine::IS_ALIVE(party.Members[i]))
                                {
                                    auto increase = -1;

                                    while (increase <= 0)
                                    {
                                        auto attribute = Attribute::Type::NONE;

                                        while (attribute == Attribute::Type::NONE)
                                        {
                                            attribute = selectAttribute(window, renderer, party.Members[i], story->Choices[choice].Value);
                                        }

                                        increase = gainAttributeScore(window, renderer, party.Members[i], attribute, story->Choices[choice].Value, 0);
                                    }
                                }
                            }

                            next = findStory(story->Choices[choice].Destination);

                            done = true;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::PARTY_RAISE_HEALTH)
                        {
                            Engine::RAISE_HEALTH(party, story->Choices[choice].Value);

                            next = findStory(story->Choices[choice].Destination);

                            done = true;
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
                                if (sum <= std::get<0>(story->Choices[choice].RandomDestinations[i]) && !done)
                                {
                                    story->Bye = std::get<1>(story->Choices[choice].RandomDestinations[i]);

                                    next = findStory(std::get<2>(story->Choices[choice].RandomDestinations[i]));

                                    done = true;
                                }
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::CHOOSE_PARTY_MEMBER)
                        {
                            auto selected = -1;

                            while (selected < 0 || selected >= party.Members.size())
                            {
                                if (Engine::COUNT(party, story->Choices[choice].Team) == 1)
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

                            if (story->Choices[choice].Team == Team::Type::NONE || party.Members[selected].Team == story->Choices[choice].Team)
                            {
                                next = findStory(story->Choices[choice].Destination);

                                done = true;
                            }
                            else
                            {
                                message = std::string(party.Members[selected].Name) + " is not part of the " + std::string(Team::Descriptions[story->Choices[choice].Team]) + " team!";

                                error = true;
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::CHOOSE_CHARACTER_EXCEPT)
                        {
                            auto selected = -1;

                            while (selected < 0 || selected >= party.Members.size())
                            {
                                if (Engine::COUNT(party, story->Choices[choice].Team) == 1)
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

                            if ((story->Choices[choice].Team == Team::Type::NONE || party.Members[selected].Team == story->Choices[choice].Team) && party.Members[selected].Type != story->Choices[choice].Character)
                            {
                                next = findStory(story->Choices[choice].Destination);

                                done = true;
                            }
                            else
                            {
                                if (party.Members[selected].Type == story->Choices[choice].Character)
                                {
                                    message = "You cannot select " + std::string(party.Members[selected].Name) + "!";
                                }
                                else
                                {
                                    message = std::string(party.Members[selected].Name) + " is not part of the " + std::string(Team::Descriptions[story->Choices[choice].Team]) + " team!";
                                }

                                error = true;
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::RETREAT)
                        {
                            if (party.Army.size() > 0)
                            {
                                std::vector<Location::BattleField> positions = {Location::BattleField::LEFT_FLANK_FRONT, Location::BattleField::LEFT_FLANK_SUPPORT, Location::BattleField::CENTER_FRONT, Location::BattleField::CENTER_SUPPORT, Location::BattleField::RIGHT_FLANK_FRONT, Location::BattleField::RIGHT_FLANK_SUPPORT};

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
                        }
                        else if (story->Choices[choice].Type == Choice::Type::GET_EQUIPMENT)
                        {
                            auto take_done = takeScreen(window, renderer, party, story->Choices[choice].Team, story->Choices[choice].Equipment, story->Choices[choice].Equipment.size(), true);

                            if (take_done)
                            {
                                next = findStory(story->Choices[choice].Destination);

                                done = true;
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::GET_EQUIPMENT_CODE)
                        {
                            auto take_done = takeScreen(window, renderer, party, story->Choices[choice].Team, story->Choices[choice].Equipment, story->Choices[choice].Equipment.size(), true);

                            if (take_done)
                            {
                                if (Engine::VERIFY_EQUIPMENT(party, {story->Choices[choice].Equipment[0].Type}))
                                {
                                    Engine::GET_CODES(party, story->Choices[choice].Codes);

                                    Engine::GET_CODES(party, story->Choices[choice].InvisibleCodes);

                                    std::string bye_string = "You gained the code";

                                    if (story->Choices[choice].Codes.size() > 1)
                                    {
                                        bye_string += "s: ";

                                        for (auto i = 0; i < story->Choices[choice].Codes.size(); i++)
                                        {
                                            if (i > 0)
                                            {
                                                bye_string += ", ";
                                            }

                                            bye_string += std::string(Codes::Prefix[story->Choices[choice].Codes[i].Type]) += std::to_string(story->Choices[choice].Codes[i].Code);
                                        }
                                    }
                                    else
                                    {
                                        bye_string += " " + std::string(Codes::Prefix[story->Choices[choice].Codes[0].Type]) += std::to_string(story->Choices[choice].Codes[0].Code);
                                    }

                                    bye_string += ".";

                                    addBye(story, bye_string);
                                }

                                next = findStory(story->Choices[choice].Destination);

                                done = true;
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::DELIVER)
                        {
                            if (Engine::HAS_SHIP(party, story->Choices[choice].Location, story->Choices[choice].Cargo.size()))
                            {
                                if (party.Money >= story->Choices[choice].Value)
                                {
                                    auto result = selectShip(window, renderer, party.Fleet, story->Choices[choice].Location, story->Choices[choice].Cargo, Control::Type::CARGO);

                                    if (result >= 0 && result < party.Fleet.size())
                                    {
                                        Engine::GAIN_MONEY(party, -story->Choices[choice].Value);

                                        party.Fleet[result].Cargo.insert(party.Fleet[result].Cargo.end(), story->Choices[choice].Cargo.begin(), story->Choices[choice].Cargo.end());

                                        next = findStory(story->Choices[choice].Destination);

                                        done = true;
                                    }
                                }
                                else
                                {
                                    message = "You do not have enough silver coins!";

                                    error = true;
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
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::GAIN_CARGO)
                        {
                            if (party.CurrentShip >= 0 && party.CurrentShip < party.Fleet.size())
                            {
                                if (((party.Fleet[party.CurrentShip].MaximumCargo - party.Fleet[party.CurrentShip].Cargo.size()) - story->Choices[choice].Cargo.size()) >= 0)
                                {
                                    party.Fleet[party.CurrentShip].Cargo.insert(party.Fleet[party.CurrentShip].Cargo.end(), story->Choices[choice].Cargo.begin(), story->Choices[choice].Cargo.end());

                                    next = findStory(story->Choices[choice].Destination);

                                    done = true;
                                }
                                else
                                {
                                    message = "You do not have enough space in your ship!";

                                    error = true;
                                }
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::CODES)
                        {
                            if (Engine::VERIFY_CODES(party, story->Choices[choice].Codes))
                            {
                                next = findStory(story->Choices[choice].Destination);

                                done = true;
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
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::SET_STATUS_FROM_LIST)
                        {
                            auto selected = -1;

                            while (selected < 0 || selected >= party.Members.size())
                            {
                                party.CurrentCharacter = Engine::FIND_SOLO(party);

                                if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                                {
                                    selected = party.CurrentCharacter;
                                }
                                else if (Engine::COUNT(party) == 1)
                                {
                                    selected = Engine::FIRST(party);
                                }
                                else
                                {
                                    selected = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::PARTY);
                                }
                            }

                            for (auto i = 0; i < party.Members.size(); i++)
                            {
                                if (i != selected)
                                {
                                    Engine::REMOVE_STATUS(party.Members[i], story->Choices[choice].Status[story->Choices[choice].Value]);
                                }
                            }

                            for (auto i = 0; i < story->Choices[choice].Status.size(); i++)
                            {
                                if (i == story->Choices[choice].Value)
                                {
                                    Engine::GAIN_STATUS(party.Members[selected], story->Choices[choice].Status[story->Choices[choice].Value]);
                                }
                                else
                                {
                                    Engine::REMOVE_STATUS(party.Members[selected], story->Choices[choice].Status[i]);
                                }
                            }

                            next = findStory(story->Choices[choice].Destination);

                            done = true;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::HAS_STATUS)
                        {
                            auto result = true;

                            party.CurrentCharacter = Engine::FIND_SOLO(party);

                            if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                            {
                                for (auto i = 0; i < story->Choices[choice].Status.size(); i++)
                                {
                                    result &= Engine::HAS_STATUS(party.Members[party.CurrentCharacter], story->Choices[choice].Status[i]);
                                }
                            }
                            else
                            {
                                for (auto i = 0; i < story->Choices[choice].Status.size(); i++)
                                {
                                    result &= Engine::HAS_STATUS(party, story->Choices[choice].Status[i]);
                                }
                            }

                            if (result)
                            {
                                next = findStory(story->Choices[choice].Destination);

                                done = true;
                            }
                            else
                            {
                                error = true;

                                if (story->Choices[choice].Status.size() > 1)
                                {
                                    if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                                    {
                                        message = "You do not have all of the required status: ";
                                    }
                                    else
                                    {
                                        message = "Your party does not have all of the required status: ";
                                    }
                                }
                                else
                                {
                                    if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                                    {
                                        message = "You do not have the required status: ";
                                    }
                                    else
                                    {
                                        message = "Your party does not have the required status: ";
                                    }
                                }

                                for (auto i = 0; i < story->Choices[choice].Status.size(); i++)
                                {
                                    if (i > 0)
                                    {
                                        message += ", ";
                                    }

                                    message += std::string(Character::StatusDescriptions[story->Choices[choice].Status[i]]);
                                }
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::PAYFORSTATUS_WITH_HEALTH)
                        {
                            auto target = -1;

                            party.CurrentCharacter = Engine::FIND_SOLO(party);

                            if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                            {
                                target = party.CurrentCharacter;
                            }
                            else
                            {
                                target = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::PARTY);
                            }

                            if (target >= 0 && target < party.Members.size())
                            {
                                if (!Engine::HAS_STATUS(party.Members[target], story->Choices[choice].Status[0]))
                                {
                                    story->temp_string = std::string(party.Members[target].Name) + " gains " + std::string(Character::StatusDescriptions[story->Choices[choice].Status[0]]);

                                    party.Members[target].MaximumHealth += story->Choices[choice].Value;

                                    if (party.Members[target].Health > party.Members[target].MaximumHealth)
                                    {
                                        party.Members[target].Health = party.Members[target].MaximumHealth;
                                    }

                                    Engine::GAIN_STATUS(party.Members[target], story->Choices[choice].Status[0]);

                                    addBye(story, story->temp_string.c_str());

                                    next = findStory(story->Choices[choice].Destination);

                                    done = true;
                                }
                                else
                                {
                                    error = true;

                                    message = std::string(party.Members[target].Name) + " already has " + std::string(Character::StatusDescriptions[story->Choices[choice].Status[0]]) + "!";
                                }
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::ASSIGN_TEAMS)
                        {
                            Engine::CONSOLIDATE(party);

                            assignTeams(window, renderer, party, story->Choices[choice].Teams, story->Choices[choice].Value);

                            next = findStory(story->Choices[choice].Destination);

                            done = true;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::LAST_INDIVIDUAL_CHECK)
                        {
                            auto selection = std::vector<int>();

                            party.CurrentCharacter = Engine::FIND_SOLO(party);

                            if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                            {
                                selection.push_back(party.CurrentCharacter);
                            }
                            else if (Engine::IS_ACTIVE(party, party.LastSelected))
                            {
                                selection.push_back(party.LastSelected);
                            }
                            else if (party.LastSelection.size() > 0)
                            {
                                selection = party.LastSelection;
                            }

                            auto success = false;

                            if (selection.size() == 1)
                            {
                                auto target = selection[0];

                                if (Engine::IS_ACTIVE(party, target))
                                {
                                    success = skillTestScreen(window, renderer, party, story->Choices[choice].Team, selection, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, story->Choices[choice].UseWeapon);
                                }
                                else
                                {
                                    selection.clear();

                                    success = skillCheck(window, renderer, party, story->Choices[choice].Team, 1, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, selection, story->Choices[choice].UseWeapon);
                                }
                            }
                            else
                            {
                                selection.clear();

                                success = skillCheck(window, renderer, party, story->Choices[choice].Team, 1, story->Choices[choice].Attributes[0], story->Choices[choice].Difficulty, story->Choices[choice].Success, selection, story->Choices[choice].UseWeapon);
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
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::RAISE_LOWEST_ATTRIBUTE)
                        {
                            auto attribute_min = Engine::MIN(party, story->Choices[choice].Attributes[0]);

                            auto target = -1;

                            party.CurrentCharacter = Engine::FIND_SOLO(party);

                            if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                            {
                                target = party.CurrentCharacter;
                            }
                            else if (Engine::COUNT(party, story->Choices[choice].Attributes[0], attribute_min) > 1)
                            {
                                target = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::RAISE_ATTRIBUTE_SCORE);
                            }
                            else
                            {
                                target = Engine::FIRST(party, story->Choices[choice].Attributes[0], attribute_min);
                            }

                            if (target >= 0 && target < party.Members.size())
                            {
                                if (Engine::SCORE(party.Members[target], story->Choices[choice].Attributes[0]) == attribute_min)
                                {
                                    auto increase = gainAttributeScore(window, renderer, party.Members[target], story->Choices[choice].Attributes[0], story->Choices[choice].Value, 0);

                                    if (increase >= 0)
                                    {
                                        next = findStory(story->Choices[choice].Destination);

                                        done = true;
                                    }
                                }
                                else
                                {
                                    error = true;

                                    message = std::string(party.Members[target].Name) + " does not have the lowest " + std::string(Attribute::Descriptions[story->Choices[choice].Attributes[0]]) + " score!";
                                }
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::RAISEATTRIBUTE_WITH_BLESSING)
                        {
                            if (party.Money >= story->Choices[choice].Value)
                            {
                                auto attribute_min = Engine::MIN(party, story->Choices[choice].Attributes[0]);

                                auto target = -1;

                                party.CurrentCharacter = Engine::FIND_SOLO(party);

                                if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                                {
                                    target = party.CurrentCharacter;
                                }
                                else if (Engine::COUNT(party, story->Choices[choice].Attributes[0], attribute_min) > 1)
                                {
                                    target = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::RAISE_ATTRIBUTE_SCORE);
                                }
                                else
                                {
                                    target = Engine::FIRST(party, story->Choices[choice].Attributes[0], attribute_min);
                                }

                                if (target >= 0 && target < party.Members.size())
                                {
                                    if (Engine::SCORE(party.Members[target], story->Choices[choice].Attributes[0]) == attribute_min)
                                    {
                                        if (Engine::SCORE(party.Members[target], story->Choices[choice].Attributes[0]) < story->Choices[choice].Difficulty)
                                        {
                                            auto increase = gainAttributeScore(window, renderer, party.Members[target], story->Choices[choice].Attributes[0], story->Choices[choice].Success, 0);

                                            if (increase >= 0)
                                            {
                                                Engine::GAIN_MONEY(party, -story->Choices[choice].Value);

                                                next = findStory(story->Choices[choice].Destination);

                                                done = true;
                                            }
                                        }
                                        else
                                        {
                                            error = true;

                                            message = "This blessing will have no effect on " + std::string(party.Members[target].Name) + "'s " + std::string(Attribute::Descriptions[story->Choices[choice].Attributes[0]]) + "!";
                                        }
                                    }
                                    else
                                    {
                                        error = true;

                                        message = std::string(party.Members[target].Name) + " does not have the lowest " + std::string(Attribute::Descriptions[story->Choices[choice].Attributes[0]]) + " score!";
                                    }
                                }
                            }
                            else
                            {
                                error = true;

                                message = "You do not have enough money for the blessing!";
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::PAYFORBLESSING_WITH_ITEM)
                        {
                            auto equipment = std::vector<Equipment::Type>();

                            for (auto i = 0; i < story->Choices[choice].Equipment.size(); i++)
                            {
                                equipment.push_back(story->Choices[choice].Equipment[i].Type);
                            }

                            if (equipment.size() > 0 && Engine::VERIFY_EQUIPMENT(party, equipment))
                            {
                                auto attribute_min = Engine::MIN(party, story->Choices[choice].Attributes[0]);

                                auto target = -1;

                                party.CurrentCharacter = Engine::FIND_SOLO(party);

                                if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                                {
                                    target = party.CurrentCharacter;
                                }
                                else if (Engine::COUNT(party, story->Choices[choice].Attributes[0], attribute_min) > 1)
                                {
                                    target = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::RAISE_ATTRIBUTE_SCORE);
                                }
                                else
                                {
                                    target = Engine::FIRST(party, story->Choices[choice].Attributes[0], attribute_min);
                                }

                                if (target >= 0 && target < party.Members.size())
                                {
                                    if (Engine::SCORE(party.Members[target], story->Choices[choice].Attributes[0]) == attribute_min)
                                    {
                                        Engine::GAIN_SCORE(party.Members[target], story->Choices[choice].Attributes[0], story->Choices[choice].Value);

                                        Engine::LOSE_EQUIPMENT(party, story->Choices[choice].Team, equipment);

                                        next = findStory(story->Choices[choice].Destination);

                                        done = true;
                                    }
                                    else
                                    {
                                        error = true;

                                        message = std::string(party.Members[target].Name) + " does not have the lowest " + std::string(Attribute::Descriptions[story->Choices[choice].Attributes[0]]) + " score!";
                                    }
                                }
                            }
                            else
                            {
                                error = true;

                                message = "You do not have the required item";

                                if (story->Choices[choice].Equipment.size() > 1)
                                {
                                    message += "s";
                                }

                                message += " for the blessing!";
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::GAIN_MORALE)
                        {
                            auto target = selectArmyUnits(window, renderer, party, story->Choices[choice].Location, story->Choices[choice].Value);

                            if (target.size() >= story->Choices[choice].Value || target.size() >= Engine::COUNT(party.Army, story->Choices[choice].Location))
                            {
                                for (auto i = 0; i < target.size(); i++)
                                {
                                    if (target[i] >= 0 && target[i] < party.Members.size())
                                    {
                                        party.Army[target[i]].MaximumMorale += story->Choices[choice].Success;

                                        Engine::GAIN_MORALE(party.Army[target[i]], story->Choices[choice].Success);
                                    }
                                }

                                next = findStory(story->Choices[choice].Destination);

                                done = true;
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::SET_PARTY_ORDER)
                        {
                            auto team_size = Engine::COUNT(party, story->Choices[choice].Team);

                            auto target = selectPartyMembers(window, renderer, party, story->Choices[choice].Team, team_size, Control::Type::SELECT_ORDER);

                            if (target.size() > 0 && target.size() >= team_size)
                            {
                                next = findStory(story->Choices[choice].Destination);

                                done = true;
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::GET_CODES)
                        {
                            if (story->Choices[choice].Codes.size() > 0)
                            {
                                Engine::GET_CODES(party, story->Choices[choice].Codes);
                            }

                            if (story->Choices[choice].InvisibleCodes.size() > 0)
                            {
                                Engine::GET_CODES(party, story->Choices[choice].InvisibleCodes);
                            }

                            next = findStory(story->Choices[choice].Destination);

                            done = true;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::GAIN_MONEY)
                        {
                            if (story->Choices[choice].Value >= 0)
                            {
                                Engine::GAIN_MONEY(party, story->Choices[choice].Value);

                                next = findStory(story->Choices[choice].Destination);

                                done = true;
                            }
                            else
                            {
                                if (party.Money >= -(story->Choices[choice].Value))
                                {
                                    Engine::GAIN_MONEY(party, -story->Choices[choice].Value);

                                    next = findStory(story->Choices[choice].Destination);

                                    done = true;
                                }
                                else
                                {
                                    error = true;

                                    message = "You do not have " + std::to_string(-story->Choices[choice].Value) + " silver coins!";
                                }
                            }
                        }
                        else if (story->Choices[choice].Type == Choice::Type::LOSE_EQUIPMENT)
                        {
                            auto equipment = std::vector<Equipment::Base>();

                            for (auto i = 0; i < party.Members.size(); i++)
                            {
                                if (Engine::IS_ACTIVE(party, i) && (party.Members[i].Team == story->Choices[choice].Team || story->Choices[choice].Team == Team::Type::NONE))
                                {
                                    for (auto j = 0; j < party.Members[i].Equipment.size(); i++)
                                    {
                                        equipment.push_back(party.Members[i].Equipment[j]);
                                    }
                                }
                            }

                            if (story->Choices[choice].Value >= equipment.size())
                            {
                                Engine::LOSE_ALL(party);
                            }
                            else
                            {
                                loseItems(window, renderer, party, Character::Type::NONE, story->Choices[choice].Team, equipment, story->Choices[choice].EquipmentExceptions, story->Choices[choice].Value, false);
                            }

                            next = findStory(story->Choices[choice].Destination);

                            done = true;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::EVERYONE_LOSES_EQUIPMENT)
                        {
                            auto equipmentClasses = std::vector<Equipment::Class>();

                            if (story->Choices[choice].EquipmentExceptions.size() > 0)
                            {
                                for (auto i = 0; i < Equipment::Classes.size(); i++)
                                {
                                    if (Engine::FIND_LIST(story->Choices[choice].EquipmentExceptions, Equipment::Classes[i]) < 0)
                                    {
                                        equipmentClasses.push_back(Equipment::Classes[i]);
                                    }
                                }
                            }
                            else
                            {
                                equipmentClasses = Equipment::Classes;
                            }

                            for (auto i = 0; i < party.Members.size(); i++)
                            {
                                if (Engine::IS_ACTIVE(party, i) && party.Members[i].Equipment.size() > 0)
                                {
                                    if (Engine::COUNT_EQUIPMENT(party.Members[i], equipmentClasses) < story->Choices[choice].Value)
                                    {
                                        for (auto j = 0; j < equipmentClasses.size(); j++)
                                        {
                                            Engine::LOSE_ALL(party.Members[i], equipmentClasses[j]);
                                        }
                                    }
                                    else
                                    {
                                        loseItems(window, renderer, party, party.Members[i].Type, story->Choices[choice].Team, party.Members[i].Equipment, story->Choices[choice].EquipmentExceptions, story->Choices[choice].Value, false);
                                    }
                                }
                            }

                            next = findStory(story->Choices[choice].Destination);

                            done = true;
                        }
                        else if (story->Choices[choice].Type == Choice::Type::GAIN_HEART)
                        {
                            Engine::GAIN_HEARTS(party, story->Choices[choice].Character, story->Choices[choice].SecondCharacter, story->Choices[choice].Value);

                            next = findStory(story->Choices[choice].Destination);

                            done = true;
                        }

                        if (error)
                        {
                            if (SDL_GetTicks() - start_ticks > duration)
                            {
                                start_ticks = SDL_GetTicks();
                            }
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

        if (splashTexture)
        {
            SDL_DestroyTexture(splashTexture);

            splashTexture = NULL;
        }

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
    std::string temp_string = "";

    auto storyID = story->ID < 0 ? story->DisplayID : story->ID;
    auto nextID = next->ID < 0 ? next->DisplayID : next->ID;

    if ((story->BookID != next->BookID) || (story->BookID == next->BookID && storyID != nextID))
    {
        Engine::LOSE_CODES(party, {Codes::Type::NO_VAULT_ACCESS, Codes::Type::MAGIC_VAULT});

        if (Engine::VERIFY_EQUIPMENT(party, {Equipment::Type::RUNESWORD3}))
        {
            auto result = Engine::FIND_BEARER(party, Equipment::Type::RUNESWORD3);

            if (result >= 0 && result < party.Members.size())
            {
                Engine::GAIN_HEALTH(party.Members[result], -1);

                temp_string = std::string(party.Members[result].Name) + " loses 1 Health point.";

                addBye(story, temp_string);
            }
        }

        if (story->BookID != next->BookID && Engine::IN_PARTY(party, Character::Type::SKULLCRACKER))
        {
            auto result = Engine::FIND_CHARACTER(party, Character::Type::SKULLCRACKER);

            if (result >= 0 && result < party.Members.size())
            {
                party.Members.erase(party.Members.begin() + result);

                addBye(story, "Skullcracker leaves and wanders into the desert.");
            }
        }
    }

    if (next->Location != Location::Type::NONE)
    {
        if (Engine::HAS_FOLLOWER(party, Follower::Type::MORDAIN_SKELETONS) && next->Location != Location::Type::MORDAIN)
        {
            Engine::LOSE_FOLLOWERS(party, {Follower::Type::MORDAIN_SKELETONS});

            addBye(story, "The [SKELETONS] crumble to dust!");
        }
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

    auto infoh = 48;
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

            if (jumpBook != Book::Type::NONE)
            {
                story = findStory(jump);

                continue;
            }

            story->Event(party);
        }

        if (story->Location != Location::Type::NONE)
        {
            Engine::SET_LOCATION(party, story->Location, story->IsCity);
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
            auto listwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * space;

            text = createText(story->Text, FONT_GARAMOND, font_size, clrDB, listwidth, TTF_STYLE_NORMAL);
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
        else if (story->Controls == Story::Controls::RECRUIT)
        {
            controls = Story::RecruitmentConrols(compact);
        }
        else
        {
            controls = Story::ExitControls(compact);
        }

        if (story->Type != Story::Type::NORMAL || (Engine::COUNT(party) + Engine::OUTSIDE(party)) <= 0)
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

            auto transition = false;

            while (!transition)
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

                    if (Engine::COUNT(party) > 0)
                    {
                        std::string party_string = "";

                        auto count = 0;

                        for (auto i = 0; i < party.Members.size(); i++)
                        {
                            if (count > 0)
                            {
                                party_string += "\n";
                            }

                            party_string += party.Members[i].Name;

                            if (Engine::IS_DEAD(party.Members[i]))
                            {
                                party_string += " (D)";
                            }
                            else if (Engine::IS_CURSED(party.Members[i]))
                            {
                                party_string += " (C)";
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

                if ((!splash || (splash && splash_h < (text_bounds - 4 * boxh - 2 * infoh - box_space))) && (Engine::COUNT_TEAMS(party) > 0 || story->Team != Team::Type::NONE))
                {
                    std::vector<Team::Type> teams = {};

                    if (Engine::IS_ACTIVE(party, party.CurrentCharacter) || story->Team != Team::Type::NONE)
                    {
                        putHeader(renderer, "Current", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (4 * boxh + 2 * infoh + box_space));

                        if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                        {
                            teams.push_back(Engine::GET_TEAM(party.Members[party.CurrentCharacter]));
                        }
                        else
                        {
                            teams.push_back(story->Team);
                        }
                    }
                    else
                    {
                        putHeader(renderer, "Teams", font_dark11, text_space, clrWH, intBR, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (4 * boxh + 2 * infoh + box_space));

                        teams = Engine::GET_TEAMS(party);
                    }

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

                    SDL_GetMouseState(&mousex, &mousey);

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

                            fillRect(renderer, dst.w, dst.h, dst.x, dst.y, intWH);
                            SDL_RenderCopy(renderer, splashTexture, &src, &dst);
                            drawRect(renderer, dst.w + 2, dst.h + 2, dst.x - 1, dst.y - 1, intBK);
                        }
                    }
                }

                if (story->Type == Story::Type::DOOM)
                {
                    putText(renderer, "This adventure is over.", font_garamond, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }
                else if ((Engine::COUNT(party) + Engine::OUTSIDE(party)) <= 0)
                {
                    putText(renderer, "Your party has died. This adventure is over.", font_garamond, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }

                renderButtons(renderer, controls, current, intLB, border_space, border_pts);

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
                    else if (controls[current].Type == Control::Type::SCROLL_DOWN || (controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown)
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

                        if (Engine::COUNT(party) == 1)
                        {
                            result = Engine::FIRST(party);
                        }
                        else
                        {
                            result = selectPartyMember(window, renderer, party, Team::Type::NONE, Equipment::NONE, Control::Type::ENTER_SHOP);
                        }

                        if (result >= 0 && result < party.Members.size())
                        {
                            shopScreen(window, renderer, party, story->Shop, party.Members[result]);
                        }

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::INN && !hold)
                    {
                        innScreen(window, renderer, party, story->RestPrice, story->CanRecharge);

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
                        Engine::Destination final_destination = {Book::Type::NONE, -1};

                        if (Engine::COUNT(story->Monsters) > 0)
                        {
                            Team::Type team = story->Team;

                            party.CurrentCharacter = Engine::FIND_SOLO(party);

                            if (Engine::IS_ACTIVE(party, party.CurrentCharacter))
                            {
                                team = Team::Type::SOLO;
                            }

                            auto result = combatScreen(window, renderer, party, team, story->Monsters, story->Allies, story->CanFlee, story->FleeRound, story->RoundLimit, true);

                            story->AfterCombat(party, result);
                        }

                        if (Engine::COUNT(story->EnemyFleet) > 0)
                        {
                            auto result = seaCombatScreen(window, renderer, party, story->EnemyFleet, story->CanFlee, story->FleeRound, story->RoundLimit);

                            story->AfterCombat(party, result);
                        }

                        if (story->EnemyArmy.size() > 0)
                        {
                            auto result = deploymentScreen(window, renderer, story->BattleLocation, party, story->EnemyArmy, story->EnemySpells, story->EnemyArmyStatus);

                            if (result != Engine::Combat::NONE)
                            {
                                story->AfterCombat(party, result);

                                if (result == Engine::Combat::VICTORY)
                                {
                                    addBye(story, "Your forces were victorious in battle.");
                                }
                                else if (result == Engine::Combat::DEFEAT)
                                {
                                    addBye(story, "Your forces were defeated.");
                                }

                                story->EnemyArmy.clear();
                                story->EnemySpells.clear();
                                story->EnemyArmyStatus.clear();
                            }
                            else
                            {
                                continue;
                            }
                        }

                        if (Engine::COUNT(party) > 0)
                        {
                            if (story->Take.size() > 0 && story->Limit > 0)
                            {
                                auto done = takeScreen(window, renderer, party, story->Team, story->Take, story->Limit, true);

                                if (!done)
                                {
                                    continue;
                                }
                                else
                                {
                                    story->Limit = 0;
                                }
                            }

                            if (story->Spells.size() > 0 && Engine::SPELLCASTERS(party) > 0)
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

                            for (auto i = 0; i < party.Members.size(); i++)
                            {
                                if (Engine::IS_ALIVE(party.Members[i]))
                                {
                                    liveCharacters.push_back(party.Members[i]);
                                }
                                else
                                {
                                    party.Dead.push_back(party.Members[i].Type);

                                    if (party.Members[i].Team == Team::Type::SOLO && party.CurrentCharacter == i)
                                    {
                                        party.Members[i].Equipment.clear();
                                    }

                                    if (party.Members[i].Type == Character::Type::AKIHIRO_OF_CHALICE && party.Members[i].Team == Team::Type::SOLO && party.CurrentCharacter == i)
                                    {
                                        if (book == Book::Type::BOOK1)
                                        {
                                            final_destination = {Book::Type::BOOK1, 450};
                                        }
                                    }

                                    if (party.CurrentCharacter != -1)
                                    {
                                        party.CurrentCharacter = -1;
                                    }

                                    if (party.LastSelected != -1)
                                    {
                                        party.LastSelected = -1;
                                    }

                                    if (party.LastSelection.size() > 0)
                                    {
                                        party.LastSelection.clear();
                                    }

                                    if (party.Members[i].Equipment.size() > 0)
                                    {
                                        deadInventory.insert(deadInventory.end(), party.Members[i].Equipment.begin(), party.Members[i].Equipment.end());
                                    }

                                    // TODO: Consolidate party.LastSection and party.Order

                                    // FOR NOW: Remove characer from the ordered list
                                    auto deadCharacter = Engine::FIND_CHARACTER(party.Order, party.Members[i].Type);

                                    if (deadCharacter >= 0 && deadCharacter < party.Order.size())
                                    {
                                        party.Order.erase(party.Order.begin() + deadCharacter);
                                    }
                                }
                            }

                            party.Members = liveCharacters;

                            if (deadInventory.size() > 0)
                            {
                                takeScreen(window, renderer, party, Team::Type::NONE, deadInventory, deadInventory.size(), false);
                            }

                            while (!Engine::VERIFY_EQUIPMENT_LIMIT(party))
                            {
                                for (auto i = 0; i < party.Members.size(); i++)
                                {
                                    if (Engine::IS_ACTIVE(party, i))
                                    {
                                        while (!Engine::VERIFY_EQUIPMENT_LIMIT(party.Members[i]))
                                        {
                                            inventoryScreen(window, renderer, party, party.Members[i], -1, false);
                                        }
                                    }
                                }
                            }

                            current = -1;

                            selected = false;

                            Story::Base *next;

                            // handle special destinations for character deaths
                            if (final_destination.first != Book::Type::NONE && final_destination.second != -1)
                            {
                                next = findStory(final_destination);
                            }
                            else
                            {
                                next = renderChoices(window, renderer, party, story);
                            }

                            book = next->BookID;

                            // story transitions
                            storyTransition(party, story, next);

                            if ((next->ID != story->ID) || (story->BookID != next->BookID))
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

                                story = next;

                                transition = true;
                            }
                            else if (Engine::COUNT(party) <= 0)
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
                        transition = true;

                        quit = true;
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

    auto text = createText(introduction, FONT_GARAMOND, 28, clrDB, ((int)SCREEN_WIDTH * (1.0 - 3.0 * Margin) - splashw), TTF_STYLE_NORMAL);

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

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size())
            {
                switch (controls[current].Type)
                {
                case Control::Type::NEW:

                    done = selectParty(window, renderer, bookID, Party);

                    if (Party.Members.size() == 4)
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

    auto *introduction = "This is the DEBUG screen. Testing facilities for various gamebook functions such as COMBAT, SKILL CHECKS, MAGIC, etc, can be accessed here. While the game is still in the ALPHA stage, this is the default screen.\n\nTests:\n1 - Combat\n2 - Map\n3 - Team Skill check\n4 - Individual Skill check\n5 - Mass Combat\n6 - Sea Combat";

    auto text = createText(introduction, FONT_GARAMOND, 28, clrDB, textwidth - 2 * text_space, TTF_STYLE_NORMAL);

    auto title = "Legendary Kingdoms: Debug";

    auto splash = createImage("images/legendary-kingdoms-logo.png");

    // Render window
    if (window && renderer && text && splash)
    {
        SDL_SetWindowTitle(window, title);

        auto current = -1;

        auto selected = false;

        auto main_buttonh = 48;

        const char *choices[7] = {"1", "2", "3", "4", "5", "6", "Exit"};

        auto controls = createHTextButtons(choices, 7, main_buttonh, startx, ((int)SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh));

        controls[0].Type = Control::Type::COMBAT;
        controls[1].Type = Control::Type::MAP;
        controls[2].Type = Control::Type::TEAM_SKILL;
        controls[3].Type = Control::Type::SKILL;
        controls[4].Type = Control::Type::MASS_COMBAT;
        controls[5].Type = Control::Type::SEA_COMBAT;
        controls[6].Type = Control::Type::QUIT;

        auto done = false;

        auto text_space = 8;

        auto Party = Party::Base();

        while (!done)
        {
            SDL_SetWindowTitle(window, title);

            // Fill the surface with background
            fillWindow(renderer, intWH);

            fitImage(renderer, splash, startx, starty, splashw, text_bounds);

            fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

            renderText(renderer, text, intBK, startx * 2 + splashw + text_space, starty + text_space, SCREEN_HEIGHT * (1.0 - 2 * Margin) - 2 * text_space, 0);

            renderTextButtons(renderer, controls, FONT_MASON, current, clrWH, intDB, intLB, font_size + 2, TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            auto combat = Engine::Combat::NONE;

            if (selected && current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::COMBAT && !hold)
                {
                    selectParty(window, renderer, Book::Type::BOOK1, Party);

                    std::vector<Monster::Base> monsters = {
                        Monster::Base("Goblin", 4, 5, 4, 6, 0),
                        Monster::Base("Orc Bodyguard", 6, 4, 4, 10, 0)};

                    std::vector<Allies::Type> allies = {};

                    combat = combatScreen(window, renderer, Party, Team::Type::NONE, monsters, allies, true, -1, -1, false);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::MAP && !hold)
                {
                    mapScreen(window, renderer);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::TEAM_SKILL && !hold)
                {
                    selectParty(window, renderer, Book::Type::BOOK1, Party);

                    auto selection = std::vector<int>();

                    skillCheck(window, renderer, Party, Team::Type::NONE, 2, Attribute::Type::STEALTH, 4, 4, selection, true);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::SKILL && !hold)
                {
                    selectParty(window, renderer, Book::Type::BOOK1, Party);

                    auto selection = std::vector<int>();

                    skillCheck(window, renderer, Party, Team::Type::NONE, 1, Attribute::Type::LORE, 4, 3, selection, true);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::MASS_COMBAT && !hold)
                {
                    Party.Members.clear();
                    Party.Members.push_back(Character::AMELIA_PASS_DAYNE);
                    Party.Members.push_back(Character::TASHA);
                    Party.Members.push_back(Character::AKIHIRO_OF_CHALICE);
                    Party.Members.push_back(Character::BRASH);

                    std::vector<Engine::BattlefieldSpells> EnemySpells = {
                        {Spells::MassCombat::ENFEEBLEMENT_CENTER_FRONT, 0},
                        {Spells::MassCombat::CLINGING_DREAD_LEFT_FRONT, 1},
                        {Spells::MassCombat::ROUT_LEFT_FRONT, 2}};

                    std::vector<Engine::ArmyStatus> EnemyArmyStatus = {
                        {Army::Status::STRENGTH1, 2, 0, 1}};

                    std::vector<Army::Base> EnemyArmy = {
                        Army::Base("Curzite Zealots", Army::Type::CURSITE_ZEALOTS, Location::Type::SALTDAD, Location::BattleField::LEFT_FLANK_FRONT, 4, 5, false),
                        Army::Base("Cursite Infantry", Army::Type::CURSITE_INFANTRY, Location::Type::SALTDAD, Location::BattleField::LEFT_FLANK_SUPPORT, 4, 4, false),
                        Army::Base("Mercenary Knights", Army::Type::MERCENARY_KNIGHTS, Location::Type::SALTDAD, Location::BattleField::CENTER_FRONT, 5, 3, false),
                        Army::Base("Citizen Archers", Army::Type::CITIZEN_ARCHERS, Location::Type::SALTDAD, Location::BattleField::CENTER_SUPPORT, 2, 4, false),
                        Army::Base("Cursite Riders", Army::Type::CURSITE_RIDERS, Location::Type::SALTDAD, Location::BattleField::RIGHT_FLANK_FRONT, 5, 4, false),
                        Army::Base("Mercenary Spears", Army::Type::MERCENARY_SPEARS, Location::Type::SALTDAD, Location::BattleField::RIGHT_FLANK_SUPPORT, 3, 2, false)};

                    Party.Army = {
                        Army::Base("Curzite Zealots", Army::Type::CURSITE_ZEALOTS, Location::Type::SALTDAD, Location::BattleField::LEFT_FLANK_FRONT, 4, 5, false),
                        Army::Base("Cursite Infantry", Army::Type::CURSITE_INFANTRY, Location::Type::SALTDAD, Location::BattleField::LEFT_FLANK_SUPPORT, 4, 4, false),
                        Army::Base("Bronzeguard", Army::Type::BRONZEGUARD, Location::Type::SALTDAD, Location::BattleField::CENTER_FRONT, 5, 5, true),
                        Army::Base("Lhasbreath Berserkers", Army::Type::LHASBREATH_BERSERKERS, Location::Type::SALTDAD, Location::BattleField::CENTER_SUPPORT, 5, 2, false),
                        Army::Base("Cursite Riders", Army::Type::CURSITE_RIDERS, Location::Type::SALTDAD, Location::BattleField::RIGHT_FLANK_FRONT, 5, 4, false),
                        Army::Base("Lhasbreath Berserkers", Army::Type::LHASBREATH_BERSERKERS, Location::Type::SALTDAD, Location::BattleField::RIGHT_FLANK_SUPPORT, 5, 2, false)};

                    deploymentScreen(window, renderer, Location::Type::SALTDAD, Party, EnemyArmy, EnemySpells, EnemyArmyStatus);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::SEA_COMBAT && !hold)
                {
                    selectParty(window, renderer, Book::Type::BOOK1, Party);

                    Party.Fleet.clear();

                    Party.Fleet.push_back(Ship::Base("CURSITE WAR GALLEY", Ship::Type::CURSITE_WAR_GALLEY, Location::Type::NONE, 6, 9, 1));

                    Party.CurrentShip = 0;

                    std::vector<Ship::Base> enemyFleet = {
                        Ship::Base("HULK", Ship::Type::HULK, Location::Type::NONE, 3, 5, 3),
                        Ship::Base("COG", Ship::Type::COG, Location::Type::NONE, 4, 7, 2)};

                    std::vector<Allies::Type> allies = {};

                    combat = seaCombatScreen(window, renderer, Party, enemyFleet, true, 0, -1);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::QUIT)
                {
                    done = true;
                }
            }
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

    Input::InitializeGamePads();

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
        if (storyID == 1 || storyID == 0)
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
