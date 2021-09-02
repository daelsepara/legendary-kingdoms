#ifndef __LEGENDARY_KINGDOMS__HPP__
#define __LEGENDARY_KINGDOMS__HPP__

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

#if defined(_WIN32)
#include <windows.h>
#include <shlobj.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

// Using SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "nlohmann/json.hpp"

#include "constants.hpp"
#include "color.hpp"
#include "controls.hpp"
#include "engine.hpp"
#include "location.hpp"
#include "input.hpp"
#include "ship.hpp"
#include "sound.hpp"
#include "story.hpp"
#include "topic.hpp"

// Book Data
#include "book1.hpp"

// Forward declarations
// create textures, images
SDL_Surface *createHeaderButton(SDL_Window *window, const char *font, int font_size, const char *text, SDL_Color color, Uint32 bg, int w, int h, int x);
SDL_Surface *createImage(const char *image);
SDL_Surface *createImage(const char *image, int w, Uint32 bg);
SDL_Surface *createText(const char *text, const char *ttf, int font_size, SDL_Color textColor, int wrap, int style);
SDL_Surface *createTextAndImage(const char *text, const char *image, const char *ttf, int font_size, SDL_Color textColor, Uint32 bg, int wrap, int style);

// sdl helper functions
void clipValue(int &val, int min, int max);
void createWindow(Uint32 flags, SDL_Window **window, SDL_Renderer **renderer, const char *title, const char *icon);
void drawRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color);
void fillRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color);
void fillWindow(SDL_Renderer *renderer, Uint32 color);
void putHeader(SDL_Renderer *renderer, const char *text, TTF_Font *font, int space, SDL_Color fg, Uint32 bg, int style, int w, int h, int x, int y);
void putText(SDL_Renderer *renderer, const char *text, TTF_Font *font, int space, SDL_Color fg, Uint32 bg, int style, int w, int h, int x, int y);
void renderButtons(SDL_Renderer *renderer, std::vector<Button> controls, int current, int fg, int space, int pts);
void renderButtons(SDL_Renderer *renderer, std::vector<Button> controls, int current, int fg, int space, int pts, bool scroll_up, bool scroll_dn);
void renderCaption(SDL_Renderer *renderer, TTF_Font *font_caption, Button control);
void renderImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y);
void renderImage(SDL_Renderer *renderer, SDL_Surface *text, int x, int y, int bounds, int offset);
void renderText(SDL_Renderer *renderer, SDL_Surface *text, Uint32 bg, int x, int y, int bounds, int offset);
void renderTextButtons(SDL_Renderer *renderer, std::vector<TextButton> controls, const char *ttf, int selected, SDL_Color fg, Uint32 bg, Uint32 bgSelected, int fontsize, int style);
void renderTextButtons(SDL_Renderer *renderer, std::vector<TextButton> controls, const char *ttf, int selected, SDL_Color fg, Uint32 bg, Uint32 bgSelected, int fontsize, int offsetx, int scrolly, bool hide_scroll, int style);
void setWindowIcon(SDL_Window *window, const char *icon);
void stretchImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y, int w, int h);
void thickRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color, int pts);

// attributes list
Attribute::Type selectAttribute(SDL_Window *window, SDL_Renderer *renderer, Book::Type book, Character::Base &character, int increase);

// game screens
bool armyScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Army::Base> army);
bool armyTransfer(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party);
bool assignTeams(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Engine::TeamAssignment> teams, int min_teams);
bool cargoScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *harbour);
bool encyclopediaScreen(SDL_Window *window, SDL_Renderer *renderer, Book::Type bookID);
bool inventoryScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, Character::Base &character, int equipment_limit, bool InCombat);
bool harbourScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *harbour);
bool introScreen(SDL_Window *window, SDL_Renderer *renderer);
bool loseItems(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Character::Type character, Team::Type team, std::vector<Equipment::Base> equipment, std::vector<Equipment::Class> exceptions, int LoseLimit, bool back_button);
bool mainScreen(SDL_Window *window, SDL_Renderer *renderer, Book::Type bookID, int storyID);
bool moraleCheck(SDL_Window *window, SDL_Renderer *renderer, Book::Type book, Army::Base &unit, int combatRound);
bool mapScreen(SDL_Window *window, SDL_Renderer *renderer, Book::Type book);
bool partyDetails(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party);
bool processStory(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Book::Type book, Story::Base *story);
bool rechargeSpells(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Character::Base &character);
bool recruitAdventurer(SDL_Window *window, SDL_Renderer *renderer, Book::Type bookID, Party::Base &party, int recruitmentPrice);
bool repairScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *harbour);
bool restScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, int RestPrice, bool CanRecharge);
bool retreatArmy(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, int unit, Location::Type &location, int threshold, int rolls);
bool selectParty(SDL_Window *window, SDL_Renderer *renderer, Book::Type bookID, Party::Base &party);
bool selectTeam(SDL_Window *window, SDL_Renderer *renderer, Book::Type book, Character::Base &character, std::vector<Engine::TeamAssignment> teams);
bool shipScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Engine::ShipPrices> &shop, Story::Base *harbour);
bool shopScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Engine::EquipmentPrice> &shop, int character);
bool skillCheck(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, int team_size, Attribute::Type skill, int difficulty, int success, std::vector<int> &selection, bool useEquipment);
bool skillTestScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team_type, std::vector<int> team, Attribute::Type Skill, int difficulty, int success, bool useEquipment);
bool spellBook(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Character::Base &character, int spells_limit);
bool spellScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Spells::Base> spells, bool back_button);
bool storyScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Engine::Destination destination);
bool takeScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Equipment::Base> equipment, int TakeLimit, bool back_button);
bool testScreen(SDL_Window *window, SDL_Renderer *renderer, Book::Type bookID, int storyID);
bool vaultScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Character::Base &character);
bool viewParty(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, bool inCombat);

// Load / Save Game
Control::Type gameScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, bool save_button);

// render images
int fitImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y, int w, int h);
int fadeImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y, int w, int h, Uint8 alpha);

// attack, damage, armour saves screens
int armourSave(SDL_Window *window, SDL_Renderer *renderer, Book::Type book, Character::Base &character, int damage);
int assignDamage(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, int combat_damage);
int attackScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Monster::Base> &monsters, int combatant, int opponent, int direction, int combatRound, bool useEquipment);
int castCombatSpell(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Monster::Base> &monsters, std::vector<int> hasAttacked, int combatRound);
int castMassCombatSpell(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Army::Base> &enemyArmy, Location::Type battlefield, std::vector<int> castSpells, int combatRound);
int castSeaCombatSpell(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Ship::Base> &enemyFleet, std::vector<int> castSpells, int combatRound);
int gainAttributeScore(SDL_Window *window, SDL_Renderer *renderer, Book::Type book, Character::Base &character, Attribute::Type &attribute, int score, int rolls);
int magicAttackScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Monster::Base> &monsters, Spells::Base &spell, int combatant, int opponent, int fighting_score);
int seaAttackScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Ship::Base> &enemyFleet, int opponent, int direction);

// select single objects
int selectOpponent(SDL_Window *window, SDL_Renderer *renderer, Book::Type book, std::vector<Ship::Base> &enemyFleet, std::vector<int> previousTargets, int combatRound);
int selectOpponent(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, int attacker, std::vector<Monster::Base> &monsters, std::vector<int> previousTargets, int combatRound, Control::Type mode);
int selectPartyMember(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, Equipment::Base equipment, Control::Type mode);
int selectShip(SDL_Window *window, SDL_Renderer *renderer, Book::Type book, std::vector<Ship::Base> ships, Location::Type location, std::vector<Cargo::Type> cargo, Control::Type mode);

// combat screens
Engine::Combat combatScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, std::vector<Monster::Base> &monsters, std::vector<Allies::Type> &allies, bool storyFlee, int fleeRound, int roundLimit, bool useEquipment);
Engine::Combat deploymentScreen(SDL_Window *window, SDL_Renderer *renderer, Location::Type location, Party::Base &party, std::vector<Army::Base> &enemyArmy, std::vector<Engine::BattlefieldSpells> &enemySpells, std::vector<Engine::ArmyStatus> &enemyStatus);
Engine::Combat massCombatScreen(SDL_Window *window, SDL_Renderer *renderer, Location::Type location, Party::Base &party, std::vector<Army::Base> &enemyArmy, std::vector<Engine::BattlefieldSpells> &enemySpells, std::vector<Engine::ArmyStatus> &enemyStatus);
Engine::Combat seaCombatScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Ship::Base> &enemyFleet, bool storyFlee, int fleeRound, int roundLimit);

// get story sections
Story::Base *findStory(Engine::Destination destination);
Story::Base *processChoices(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *story);
Story::Base *renderChoices(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Story::Base *story);

// description/string utilities
std::string characterText(Character::Base &character, bool compact);
std::string getSavePath();
std::string itemString(Equipment::Base &equipment);
std::string monsterString(Monster::Base &monster);
std::string miniPreview(std::string file_name);
std::string previewGame(std::string file_name);
std::string shipString(Ship::Base &ship, bool cargo);

// miscellaneous functions
bool IsValidTransfer(Party::Base &party, Location::Type src, Location::Type dst);
bool magicRound0(Character::Base &character, int combatRound);
int FIND_CONTROL(std::vector<Button> &controls, Control::Type control);
void addBye(Story::Base *story, std::string bye);

// other game/story helper functions
void renderArmy(SDL_Renderer *renderer, TTF_Font *font, std::vector<Army::Base> &army, int boxw, int army_boxh, int offsety, SDL_Color fg, Uint32 bg);
void resolveMassCombat(SDL_Window *window, SDL_Renderer *renderer, Location::Type location, Party::Base &party, std::vector<Army::Base> &enemyArmy, std::vector<Engine::BattlefieldSpells> &enemySpells, std::vector<Engine::ArmyStatus> &enemyStatus, Location::Zone zone, int combatRound);
void storyTransition(Party::Base &party, Story::Base *story, Story::Base *next);

// popup List Scroll controls
template <typename T>
void popupScrolls(std::vector<Button> &controls, std::vector<T> &list, int start, int last, int limit, int popupw, int popuph, int infoh, int offsetx, int offsety, bool back_button);

// icon button controls
std::vector<Button> armyList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Army::Base> &army, int start, int last, int limit, int offsetx, int offsety, bool party_controls);
std::vector<Button> attributeList(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, std::vector<Attribute::Type> &attributes, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> buyCargo(SDL_Window *window, SDL_Renderer *renderer, std::vector<Engine::CargoPrices> &cargo, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> cargoList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Engine::CargoPrices> &cargo, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> cargoList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> combatantList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> party, int start, int last, int limit, int offsetx, int offsety, bool confirm_button, bool back_button);
std::vector<Button> createChoices(SDL_Window *window, SDL_Renderer *renderer, std::vector<Choice::Base> choices, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> createFileList(SDL_Window *window, SDL_Renderer *renderer, std::vector<std::string> list, int start, int last, int limit, int offsetx, int offsety, bool save_button);
std::vector<Button> equipmentList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Equipment::Base> list, int start, int last, int limit, int offsety, int scrolly);
std::vector<Button> equipmentList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Equipment::Base> list, int start, int last, int limit, bool confirm_button, bool back_button);
std::vector<Button> harbourControls(SDL_Window *window, SDL_Renderer *renderer);
std::vector<Button> monsterList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Monster::Base> &monsters, int start, int last, int limit, int offsetx, int offsety, bool confirm_button, bool back_button);
std::vector<Button> monsterList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Monster::Base> &monsters, int start, int last, int limit, int offsetx, int offsety, Control::Type mode);
std::vector<Button> popupArmy(SDL_Window *window, SDL_Renderer *renderer, std::vector<Army::Base> &army, int start, int last, int limit, int popupw, int popuph, int infoh, int offsetx, int offsety);
std::vector<Button> popupConfirm(int popupw, int popuph, int offsetx, int offsety);
std::vector<Button> popupList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Army::Base> &list, int start, int last, int limit, int popupw, int popuph, int infoh, int offsetx, int offsety, bool back_button);
std::vector<Button> popupList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Location::Type> &list, int start, int last, int limit, int popupw, int popuph, int infoh, int offsetx, int offsety);
std::vector<Button> popupList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Monster::Base> &list, int start, int last, int limit, int popupw, int popuph, int infoh, int offsetx, int offsety);
std::vector<Button> popupList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &list, int start, int last, int limit, int popupw, int popuph, int infoh, int offsetx, int offsety);
std::vector<Button> popupMoney(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, int popupw, int popuph, int infoh, int offsetx, int offsety);
std::vector<Button> restList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> party, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> romanceList(SDL_Window *window, SDL_Renderer *renderer, std::map<Character::Romance, int> &hearts, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, int start, int last, int limit, int offsetx, int offsety, Control::Type mode);
std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, int start, int last, int limit, int offsetx, int offsety, bool cargo, bool confirm_button, bool back_button);
std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, std::vector<Engine::ShipPrices> &shop, int start, int last, int limit, int offsetx, int offsety, bool buy_button, bool sell_button);
std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Engine::ShipPrices> &ships, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Engine::ShipPrices> &ships, int start, int last, int limit, int offsetx, int offsety, bool buy_button, bool sell_button);
std::vector<Button> shopList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Engine::EquipmentPrice> &shop, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> shopList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Equipment::Base> &equipment, std::vector<Engine::EquipmentPrice> &shop, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> rechargeList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Spells::Base> &spells, int start, int last, int limit, int offsetx, int offsety, int scrolly);
std::vector<Button> repairList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> spellList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Spells::Base> &spells, int start, int last, int limit, int offsetx, int offsety, int scrolly);
std::vector<Button> spellList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Spells::Base> &spells, int start, int last, int limit, int offsetx, int offsety, int scrolly, bool confirm_button, bool back_button);
std::vector<Button> teamsList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Team::Type> &teams, int start, int last, int limit, int offsetx, int offsety);
std::vector<Button> topicsList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Topics::Base> &topics, int start, int last, int limit, int offsetx, int offsety, bool compact);
std::vector<Button> vaultList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Equipment::Base> list, int start, int last, int limit, int offsety, int scrolly);

// select multiple objects
std::vector<int> selectArmyUnits(SDL_Window *window, SDL_Renderer *renderer, Book::Type book, std::vector<Army::Base> army, Location::Type garrison, int num_limit);
std::vector<int> selectArmyUnits(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Location::Type garrison, int num_limit);
std::vector<int> selectPartyMembers(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, Team::Type team, int team_size, Control::Type mode);
std::vector<int> selectShips(SDL_Window *window, SDL_Renderer *renderer, Book::Type book, std::vector<Ship::Base> ships, Location::Type harbour, int num_limit, bool back_button);
std::vector<int> selectSpell(SDL_Window *window, SDL_Renderer *renderer, Book::Type book, Character::Base &caster, std::vector<Spells::Base> &spells, int select_limit, Spells::Select mode);

// text button controls
std::vector<TextButton> createFixedTextButtons(const char **choices, int num, int text_buttonw, int text_buttonh, int textbutton_space, int text_x, int text_buttony);
std::vector<TextButton> createHTextButtons(const char **choices, int num, int text_buttonh, int text_x, int text_buttony);
std::vector<TextButton> createHTextButtons(const char **choices, int num, int text_buttonh, int text_x, int text_buttony, bool has_scrolls);

// json helpers
void followersJSON(nlohmann::json &json, std::vector<Follower::Base> followers);
void spellBookJSON(nlohmann::json &json, std::vector<Spells::Base> spells);
void attributesJSON(nlohmann::json &json, std::vector<Attribute::Base> attributes);
void equipmentJSON(nlohmann::json &json, std::vector<Equipment::Base> equipment);

// load/save games
Party::Base loadGame(std::string file_name);
void saveGame(Party::Base &party, const char *overwrite);

// ----------------------------------------------------------------------------
// START (Implementations)
// ----------------------------------------------------------------------------

// Round 0/1 conditions involving magic
bool magicRound0(Character::Base &character, int combatRound)
{
    auto result = true;

    result &= (!Engine::HAS_STATUS(character, Character::Status::EXTRA_MAGIC_ROUND0) || (Engine::HAS_STATUS(character, Character::Status::EXTRA_MAGIC_ROUND0) && combatRound != 0));
    result &= (!Engine::HAS_STATUS(character, Character::Status::UNLIMITED_MAGIC_ROUND0) || (Engine::HAS_STATUS(character, Character::Status::UNLIMITED_MAGIC_ROUND0) && combatRound != 0));

    return result;
}

// verifies if troop transfer (source/destination) is valid
bool IsValidTransfer(Party::Base &party, Location::Type src, Location::Type dst)
{
    bool valid = false;

    if (Engine::VERIFY_CODES(party, {Codes::A(33)}))
    {
        if (dst == Location::Type::SALTDAD)
        {
            valid = true;
        }
    }

    if (Engine::VERIFY_CODES(party, {Codes::A(100)}))
    {
        if (src == Location::Type::SALTDAD)
        {
            valid = true;
        }
    }

    return valid;
}

int FIND_CONTROL(std::vector<Button> &controls, Control::Type control)
{
    auto found = -1;

    for (auto i = 0; i < controls.size(); i++)
    {
        if (controls[i].Type == control)
        {
            found = i;

            break;
        }
    }

    return found;
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

SDL_Surface *createImage(const char *image)
{
    auto surface = IMG_Load(image);

    if (!surface)
    {
        std::cerr << "Unable to load image " << image << "! SDL Error: " << SDL_GetError() << std::endl;
    }

    return surface;
}

SDL_Surface *createImage(const char *image, int wrap, Uint32 bg)
{
    SDL_Surface *surface = NULL;
    SDL_Surface *converted_surface = NULL;

    auto image_surface = createImage(image);

    if (image_surface)
    {
        auto image_h = image_surface->h;

        if (image_surface->w > wrap)
        {
            auto image_scale = (double)((double)wrap / image_surface->w);

            image_h = (int)(image_surface->h * image_scale);
        }

        Uint32 amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        amask = 0x000000ff;
#else
        amask = 0xff000000;
#endif

        surface = SDL_CreateRGBSurface(0, wrap, image_h, 32, 0, 0, 0, amask);

        SDL_Rect dst;

        dst.w = surface->w;
        dst.h = surface->h;
        dst.x = 0;
        dst.y = 0;

        SDL_FillRect(surface, NULL, bg);

        dst.h = image_h;

        if (image_surface->w > wrap)
        {
            dst.w = surface->w;
        }
        else
        {
            dst.w = image_surface->w;
            dst.h = image_surface->h;
            dst.x = (surface->w - image_surface->w) / 2;
        }

        converted_surface = SDL_ConvertSurface(image_surface, surface->format, 0);

        SDL_BlitScaled(converted_surface, NULL, surface, &dst);
    }

    if (image_surface)
    {
        SDL_FreeSurface(image_surface);

        image_surface = NULL;
    }

    if (converted_surface)
    {
        SDL_FreeSurface(converted_surface);

        converted_surface = NULL;
    }

    return surface;
}

// create text image with line wrap limit
SDL_Surface *createText(const char *text, const char *ttf, int font_size, SDL_Color textColor, int wrap, int style)
{
    SDL_Surface *surface = NULL;

    TTF_Init();

    auto font = TTF_OpenFont(ttf, font_size);

    if (font)
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

// create text and image with line wrap limit
SDL_Surface *createTextAndImage(const char *text, const char *image, const char *ttf, int font_size, SDL_Color textColor, Uint32 bg, int wrap, int style)
{
    SDL_Surface *surface = NULL;
    SDL_Surface *converted_surface = NULL;

    auto image_surface = createImage(image);
    auto text_surface = createText(text, ttf, font_size, textColor, wrap, style);

    if (image_surface && text_surface)
    {
        auto image_h = image_surface->h;

        if (image_surface->w > wrap)
        {
            auto image_scale = (double)((double)wrap / image_surface->w);

            image_h = (int)(image_surface->h * image_scale);
        }

        Uint32 amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        amask = 0x000000ff;
#else
        amask = 0xff000000;
#endif
        surface = SDL_CreateRGBSurface(0, wrap, image_h + text_space + text_surface->h, 32, 0, 0, 0, amask);

        SDL_Rect dst;

        dst.w = surface->w;
        dst.h = surface->h;
        dst.x = 0;
        dst.y = 0;

        SDL_Rect text_dst;

        text_dst.w = surface->w;
        text_dst.h = surface->h;
        text_dst.x = 0;
        text_dst.y = image_h + text_space;

        SDL_FillRect(surface, NULL, bg);

        dst.h = image_h;

        if (image_surface->w > wrap)
        {
            dst.w = surface->w;
        }
        else
        {
            dst.w = image_surface->w;
            dst.h = image_surface->h;
            dst.x = (surface->w - image_surface->w) / 2;
        }

        converted_surface = SDL_ConvertSurface(image_surface, surface->format, 0);

        SDL_BlitScaled(converted_surface, NULL, surface, &dst);

        SDL_BlitSurface(text_surface, NULL, surface, &text_dst);
    }

    if (image_surface)
    {
        SDL_FreeSurface(image_surface);

        image_surface = NULL;
    }

    if (text_surface)
    {
        SDL_FreeSurface(text_surface);

        text_surface = NULL;
    }

    if (converted_surface)
    {
        SDL_FreeSurface(converted_surface);

        converted_surface = NULL;
    }

    return surface;
}

SDL_Surface *createHeaderButton(SDL_Window *window, const char *font, int font_size, const char *text, SDL_Color color, Uint32 bg, int w, int h, int x)
{
    Uint32 amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    amask = 0x000000ff;
#else
    amask = 0xff000000;
#endif

    auto button = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, amask);
    auto text_surface = createText(text, font, font_size, color, w, TTF_STYLE_NORMAL);

    if (button && text_surface)
    {
        SDL_Rect dst;

        dst.w = button->w;
        dst.h = button->h;
        dst.x = 0;
        dst.y = 0;

        SDL_FillRect(button, NULL, bg);

        dst.x = x < 0 ? (button->w - text_surface->w) / 2 : x;
        dst.y = (button->h - text_surface->h) / 2;

        SDL_BlitSurface(text_surface, NULL, button, &dst);
    }

    if (text_surface)
    {
        SDL_FreeSurface(text_surface);

        text_surface = NULL;
    }

    return button;
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

        if (character.SpellCaster)
        {
            character_text += ", SpellCaster";
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

        character_text += std::string(Attribute::Descriptions[character.Attributes[i].Type]) + ": ";

        if (character.Attributes[i].Type == Attribute::Type::FIGHTING)
        {
            mod_score = Engine::FIGHTING_SCORE(character);
        }
        else
        {
            mod_score = Engine::SCORE(character, character.Attributes[i].Type);
        }

        if (!compact)
        {
            character_text += std::to_string(raw_score);

            if (mod_score != raw_score)
            {
                character_text += "(" + std::to_string(mod_score) + ")";
            }
        }
        else
        {
            if (raw_score > mod_score)
            {
                character_text += std::to_string(raw_score);
            }
            else
            {
                character_text += std::to_string(mod_score);
            }
        }
    }

    auto raw_health = character.Health;

    auto mod_health = Engine::HEALTH(character);

    character_text += ", Health: ";

    if (!compact)
    {
        character_text += std::to_string(raw_health);

        if (raw_health != mod_health)
        {
            character_text += "(" + std::to_string(mod_health) + ")";
        }
    }
    else
    {
        if (raw_health > mod_health)
        {
            character_text += std::to_string(raw_health);
        }
        else
        {
            character_text += std::to_string(mod_health);
        }
    }

    if (!compact && character.SpellCaster)
    {
        character_text += "\n\nSpellcaster";
    }

    return character_text;
};

std::string rawCharacterText(Character::Base &character, bool compact)
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

        if (character.SpellCaster)
        {
            character_text += ", SpellCaster";
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

        character_text += std::string(Attribute::Descriptions[character.Attributes[i].Type]) + ": " + std::to_string(raw_score);
    }

    auto raw_health = character.Health;

    character_text += ", Health: " + std::to_string(raw_health);

    if (!compact && character.SpellCaster)
    {
        character_text += "\n\nSpellcaster";
    }

    return character_text;
};

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

std::string shipString(Ship::Base &ship, bool cargo)
{
    std::string ship_string = "";

    if (!cargo)
    {
        ship_string = "[" + ship.Name + "]";

        ship_string += "\nFighting: " + (ship.Fighting > 0 ? std::to_string(ship.Fighting) : std::string("Special"));

        ship_string += " Health: " + std::to_string(ship.Health);
    }
    else
    {
        ship_string = "[" + ship.Name + "] Fighting: " + std::to_string(ship.Fighting) + ", Health: " + std::to_string(ship.Health) + ", Cargo Units: " + std::to_string(ship.MaximumCargo);

        ship_string += "\nLocation: " + std::string(Location::Description[ship.Location]);
    }

    if (cargo)
    {
        if (ship.Cargo.size() > 0)
        {
            ship_string += " Cargo: ";

            for (auto i = 0; i < ship.Cargo.size(); i++)
            {
                if (i > 0)
                {
                    ship_string += ", ";
                }

                ship_string += Cargo::Description[ship.Cargo[i]];
            }
        }
    }

    return ship_string;
}

#if defined(_WIN32) || defined(__arm__) || defined(__APPLE__)
std::string time_string(long long deserialised)
{
    auto epoch = std::chrono::time_point<std::chrono::system_clock>();
    auto since_epoch = std::chrono::milliseconds(deserialised);
    std::chrono::system_clock::time_point timestamp = epoch + since_epoch;

    auto in_time_t = std::chrono::system_clock::to_time_t(timestamp);

    std::stringstream ss;

    if (in_time_t >= 0)
    {
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    }

    return ss.str();
}
#else
std::string time_string(long deserialised)
{
    auto epoch = std::chrono::time_point<std::chrono::high_resolution_clock>();
    auto since_epoch = std::chrono::milliseconds(deserialised);
    auto timestamp = epoch + since_epoch;

    auto in_time_t = std::chrono::system_clock::to_time_t(timestamp);

    std::stringstream ss;

    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");

    return ss.str();
}
#endif

std::string getSavePath()
{
#if defined(_WIN32)
    PWSTR path_str;

    SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, NULL, &path_str);

std:
shipString:
    wstring wpath(path_str);

    CoTaskMemFree(path_str);

    std::string save(wpath.length(), ' ');

    std::copy(wpath.begin(), wpath.end(), save.begin());

    save += "/Saved Games/Legendary Kingdoms";
#else
    const char *homedir;

    if ((homedir = getenv("HOME")) == NULL)
    {
        homedir = getpwuid(getuid())->pw_dir;
    }

    std::string save = std::string(homedir) + "/Documents/Saved Games/Legendary Kingdoms";
#endif

    return save;
}

std::string previewGame(std::string file_name)
{
    std::string preview = "";

    std::ifstream ifs(file_name);

    if (ifs.good())
    {
        auto data = nlohmann::json::parse(ifs);

        ifs.close();

        auto location = !data["location"].is_null() ? static_cast<Location::Type>((int)data["location"]) : Location::Type::NONE;
        auto money = !data["money"].is_null() ? (int)data["money"] : 0;
        auto book = !data["book"].is_null() ? static_cast<Book::Type>((int)data["book"]) : Book::Type::NONE;

#if defined(_WIN32) || defined(__arm__)
        auto epoch = !data["epoch"].is_null() ? (long long)(data["epoch"]) : 0;
#else
        auto epoch = !data["epoch"].is_null() ? (long)(data["epoch"]) : 0;
#endif

        preview = std::string(Book::Title[book]) + ": " + std::string(Location::Description[location]) + ", " + std::to_string(money) + " silver coins";

        // get names
        if (!data["members"].is_null() && data["members"].is_array() && data["members"].size() > 0)
        {
            preview += "\nParty: ";

            auto names = 0;

            for (auto i = 0; i < data["members"].size(); i++)
            {
                std::string name = !data["members"][i]["name"].is_null() ? data["members"][i]["name"] : "";

                if (name.length() > 0)
                {
                    if (names > 0)
                    {
                        preview += ", ";
                    }

                    preview += name;

                    names += 1;
                }
            }
        }

        preview += "\n" + time_string(epoch);
    }

    return preview;
}

std::string miniPreview(std::string file_name)
{
    std::string preview = "";

    std::ifstream ifs(file_name);

    if (ifs.good())
    {
        auto data = nlohmann::json::parse(ifs);

        ifs.close();

        auto location = !data["location"].is_null() ? static_cast<Location::Type>((int)data["location"]) : Location::Type::NONE;

#if defined(_WIN32) || defined(__arm__)
        auto epoch = !data["epoch"].is_null() ? (long long)(data["epoch"]) : 0;
#else
        auto epoch = !data["epoch"].is_null() ? (long)(data["epoch"]) : 0;
#endif

        preview = std::string(Location::Description[location]);

        // get names
        if (!data["members"].is_null() && data["members"].is_array() && data["members"].size() > 0)
        {
            preview += "\n";

            auto names = 0;

            for (auto i = 0; i < data["members"].size(); i++)
            {
                std::string name = !data["members"][i]["name"].is_null() ? data["members"][i]["name"] : "";

                if (name.length() > 0)
                {
                    if (names > 0)
                    {
                        preview += ", ";
                    }

                    preview += name;

                    names += 1;
                }
            }
        }

        preview += "\n" + time_string(epoch);
    }

    return preview;
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

template <typename T>
void popupScrolls(std::vector<Button> &controls, std::vector<T> &list, int start, int last, int limit, int popupw, int popuph, int infoh, int offsetx, int offsety, bool back_button)
{
    auto idx = (int)controls.size();

    if (list.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, offsetx + (popupw - arrow_size - button_space), offsety + infoh + 7 * border_space / 2, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (list.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, offsetx + (popupw - arrow_size - button_space), offsety + (popuph - arrow_size - 2 * border_space - buttonh - infoh), Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/yes.png", idx, back_button ? idx + 1 : idx, list.size() > 0 ? (last - start) - 1 : idx, idx, offsetx + button_space, offsety + popuph - button_space - buttonh, Control::Type::CONFIRM));

    if (back_button)
    {
        controls.push_back(Button(idx + 1, "icons/no.png", idx, idx + 1, list.size() > 0 ? (last - start) - 1 : idx + 1, idx + 1, offsetx + popupw - buttonw - button_space, offsety + popuph - button_space - buttonh, Control::Type::BACK));
    }
}

void setWindowIcon(SDL_Window *window, const char *icon)
{
    auto surface = createImage(icon);

    if (surface)
    {
        SDL_SetWindowIcon(window, surface);

        SDL_FreeSurface(surface);

        surface = NULL;
    }
}

void createWindow(Uint32 flags, SDL_Window **window, SDL_Renderer **renderer, const char *title, const char *icon)
{
    // The window we'll be rendering to
    *window = NULL;
    *renderer = NULL;

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

        SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, (SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), window, renderer);

        if (renderer)
        {
            SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_NONE);
        }

        if (!window || !renderer)
        {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        }
        else if (window)
        {
            SDL_SetWindowTitle(*window, title);

            setWindowIcon(*window, icon);
        }

        //Initialize SDL_mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
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

            src.w = text->w;
            src.h = text->h < bounds ? text->h : bounds;
            src.y = offset;
            src.x = 0;

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

// Render a portion of the image on bounded surface within the specified window
void renderImage(SDL_Renderer *renderer, SDL_Surface *text, int x, int y, int bounds, int offset)
{
    if (renderer)
    {
        if (text && renderer)
        {
            SDL_Rect dst;
            SDL_Rect src;

            src.w = text->w;
            src.h = text->h < bounds ? text->h : bounds;
            src.y = offset;
            src.x = 0;

            dst.w = text->w;
            dst.h = text->h < bounds ? text->h : bounds;
            dst.x = x;
            dst.y = y;

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

            renderImage(renderer, controls[i].Surface, controls[i].X, controls[i].Y);

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
        }
    }
}

void renderButtons(SDL_Renderer *renderer, std::vector<Button> controls, int current, int fg, int space, int pts, bool scroll_up, bool scroll_dn)
{
    if (controls.size() > 0)
    {
        for (auto i = 0; i < controls.size(); i++)
        {
            SDL_Rect rect;

            if ((controls[i].Type == Control::Type::SCROLL_UP && scroll_up) || (controls[i].Type == Control::Type::SCROLL_DOWN && scroll_dn) || (controls[i].Type != Control::Type::SCROLL_UP && controls[i].Type != Control::Type::SCROLL_DOWN))
            {
                if (controls[i].Type == Control::Type::SCROLL_UP || controls[i].Type == Control::Type::SCROLL_DOWN)
                {
                    fillRect(renderer, controls[i].W + 2 * border_space, controls[i].H + 2 * border_space, controls[i].X - border_space, controls[i].Y - border_space, intWH);
                }

                renderImage(renderer, controls[i].Surface, controls[i].X, controls[i].Y);
            }

            if (i == current)
            {
                if ((controls[i].Type == Control::Type::SCROLL_UP && scroll_up) || (controls[i].Type == Control::Type::SCROLL_DOWN && scroll_dn) || (controls[i].Type != Control::Type::SCROLL_UP && controls[i].Type != Control::Type::SCROLL_DOWN))
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
            }
        }
    }
}

std::vector<TextButton> createHTextButtons(const char **choices, int num, int text_buttonh, int text_x, int text_buttony)
{
    auto controls = std::vector<TextButton>();

    if (num > 0)
    {
        auto margin2 = (2.0 * Margin);
        auto marginleft = (1.0 - margin2);

        auto pixels = (int)(SCREEN_WIDTH * Margin) / 2;
        auto width = (int)(SCREEN_WIDTH * marginleft);

        auto text_spacew = width / num;
        auto htext_buttonw = text_spacew - pixels;
        auto text_space = pixels / 2;

        for (auto i = 0; i < num; i++)
        {
            auto left = i > 0 ? i - 1 : i;
            auto right = i < num - 1 ? i + 1 : i;
            auto up = i;
            auto down = i;

            auto x = text_x + i * (htext_buttonw + text_space * 2) + text_space;

            auto button = TextButton(i, choices[i], left, right, up, down, x, text_buttony, htext_buttonw, text_buttonh);

            controls.push_back(button);
        }
    }

    return controls;
}

std::vector<TextButton> createHTextButtons(const char **choices, int num, int text_buttonh, int text_x, int text_buttony, bool has_scrolls)
{
    if (!has_scrolls)
    {
        return createHTextButtons(choices, num, text_buttonh, text_x, text_buttony);
    }

    auto controls = std::vector<TextButton>();

    if (num > 0)
    {
        auto margin2 = (2.0 * Margin);
        auto marginleft = (1.0 - margin2);

        auto pixels = (int)(SCREEN_WIDTH * Margin) / 2;
        auto width = (int)(SCREEN_WIDTH * marginleft);

        auto text_spacew = width / num;
        auto htext_buttonw = text_spacew - pixels;
        auto text_space = pixels / 2;

        controls.push_back(TextButton(0, "", 0, 0, 0, 1, -1, -1, 32, 32, Control::Type::SCROLL_UP));
        controls.push_back(TextButton(1, "", 1, 1, 0, 2, -1, -1, 32, 32, Control::Type::SCROLL_DOWN));

        for (auto i = 0; i < num; i++)
        {
            auto left = i > 0 ? i + 1 : i + 2;
            auto right = i < num - 1 ? i + 3 : i + 2;
            auto up = 1;
            auto down = i + 2;

            auto x = text_x + i * (htext_buttonw + text_space * 2) + text_space;

            auto button = TextButton(i + 2, choices[i], left, right, up, down, x, text_buttony, htext_buttonw, text_buttonh);

            controls.push_back(button);
        }
    }

    return controls;
}

std::vector<TextButton> createFixedTextButtons(const char **choices, int num, int text_buttonw, int text_buttonh, int textbutton_space, int text_x, int text_buttony)
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

            auto button = TextButton(i, choices[i], left, right, up, down, x, text_buttony, text_buttonw, text_buttonh);

            controls.push_back(button);
        }
    }

    return controls;
}

std::vector<Button> createFileList(SDL_Window *window, SDL_Renderer *renderer, std::vector<std::string> list, int start, int last, int limit, int offsetx, int offsety, bool save_button)
{
    auto controls = std::vector<Button>();

    if (list.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto preview = previewGame(list[i]);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 22, preview.c_str(), clrBK, intBE, list_buttonw, 100, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (list.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (list.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/open.png", idx, idx + 1, list.size() > 0 ? (last - start) - 1 : idx, idx, textx, buttony, Control::Type::LOAD));

    if (save_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/disk.png", idx - 1, idx + 1, list.size() > 0 ? (last - start) - 1 : idx, idx, textx + gridsize, buttony, Control::Type::SAVE));
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/no.png", idx - 1, idx + 1, list.size() > 0 ? (last - start) - 1 : idx, idx, (save_button ? (textx + 2 * gridsize) : (textx + gridsize)), buttony, Control::Type::DELETE));

    controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, list.size() > 0 ? (last - start) - 1 : idx + 1, idx + 1, lastx, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> createChoices(SDL_Window *window, SDL_Renderer *renderer, std::vector<Choice::Base> choices, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    if (choices.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 24, choices[index].Text, clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (choices.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (choices.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/papyrus.png", idx - 1, idx + 1, idx - 1, idx, textx, buttony, Control::Type::ENCYCLOPEDIA));
    controls.push_back(Button(idx + 1, "icons/ancient-map.png", idx, idx + 2, idx - 1, idx + 1, textx + gridsize, buttony, Control::Type::MAP));
    controls.push_back(Button(idx + 2, "icons/user.png", idx + 1, idx + 3, idx - 1, idx + 2, textx + 2 * gridsize, buttony, Control::Type::PARTY));
    controls.push_back(Button(idx + 3, "icons/back-button.png", idx + 2, idx + 3, idx - 1, idx + 3, lastx, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> attributeList(SDL_Window *window, SDL_Renderer *renderer, Character::Base &character, std::vector<Attribute::Type> &attributes, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

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

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 28, attributes_string.c_str(), clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (attributes.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (attributes.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/yes.png", idx, idx, attributes.size() > 0 ? (last - start) - 1 : idx, idx, textx, buttony, Control::Type::CONFIRM));

    return controls;
}

std::vector<Button> armyList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Army::Base> &army, int start, int last, int limit, int offsetx, int offsety, bool party_controls)
{
    auto controls = std::vector<Button>();

    if (army.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto unit = army[index];

            std::string army_string = "[" + unit.Name + "] Strength: " + std::to_string(unit.Strength) + ", Morale: " + std::to_string(unit.Morale);

            army_string += "\nPosition: " + std::string(Location::BattleFieldDescription[unit.Position]) + ", Garrison: " + std::string(Location::Description[unit.Garrison]);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 22, army_string.c_str(), clrBK, intBE, list_buttonw, 100, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (army.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (army.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    idx = controls.size();

    if (party_controls)
    {
        controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "ARMY", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 1, army.size() > 0 ? (last - start) - 1 : idx, idx, startx, text_buttony, Control::Type::ARMY));
        controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "FLEET", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 2, army.size() > 0 ? (last - start) - 1 : idx + 1, idx + 1, startx + text_gridsize, text_buttony, Control::Type::FLEET));
        controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "ROMANCE", clrWH, intDB, text_buttonw, 48, -1), idx + 1, idx + 3, army.size() > 0 ? (last - start) - 1 : idx + 2, idx + 2, startx + 2 * text_gridsize, text_buttony, Control::Type::ROMANCE));
        controls.push_back(Button(idx + 3, createHeaderButton(window, FONT_DARK11, 22, "BACK", clrWH, intDB, text_buttonw, 48, -1), idx + 2, idx + 3, army.size() > 0 ? (last - start) - 1 : idx + 3, idx + 3, startx + 3 * text_gridsize, text_buttony, Control::Type::BACK));
    }
    else
    {
        controls.push_back(Button(idx, "icons/yes.png", idx, idx + 1, army.size() > 0 ? (last - start) - 1 : idx, idx, textx, buttony, Control::Type::CONFIRM));
        controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, army.size() > 0 ? (last - start) - 1 : idx + 1, idx + 1, lastx, buttony, Control::Type::BACK));
    }

    return controls;
}

std::vector<Button> armyList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Army::Base> &army, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    if (army.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto unit = army[index];

            std::string army_string = "[" + unit.Name + "] Strength: " + std::to_string(unit.Strength) + ", Morale: " + std::to_string(unit.Morale);

            army_string += "\nPosition: " + std::string(Location::BattleFieldDescription[unit.Position]) + ", Garrison: " + std::string(Location::Description[unit.Garrison]);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 24, army_string.c_str(), clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (army.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (army.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/back-button.png", idx, idx, army.size() > 0 ? (last - start) - 1 : idx, idx, lastx, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> romanceList(SDL_Window *window, SDL_Renderer *renderer, std::map<Character::Romance, int> &hearts, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    if (hearts.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto heart = hearts.begin();

            std::advance(heart, index);

            Character::Romance romance = heart->first;

            auto from = Character::ALL[Engine::FIND_CHARACTER(Character::ALL, romance.first)];
            auto to = Character::ALL[Engine::FIND_CHARACTER(Character::ALL, romance.second)];

            auto count = heart->second;

            std::string ship_string = from.Name + " has " + std::to_string(count) + " heart";

            if (count > 1)
            {
                ship_string += "s";
            }

            ship_string += " for " + to.Name + ".";

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 22, ship_string.c_str(), clrBK, intBE, list_buttonw, 100, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (hearts.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (hearts.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "ARMY", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 1, hearts.size() > 0 ? (last - start) - 1 : idx, idx, startx, text_buttony, Control::Type::ARMY));
    controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "FLEET", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 2, hearts.size() > 0 ? (last - start) - 1 : idx + 1, idx + 1, startx + text_gridsize, text_buttony, Control::Type::FLEET));
    controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "ROMANCE", clrWH, intDB, text_buttonw, 48, -1), idx + 1, idx + 3, hearts.size() > 0 ? (last - start) - 1 : idx + 2, idx + 2, startx + 2 * text_gridsize, text_buttony, Control::Type::ROMANCE));
    controls.push_back(Button(idx + 3, createHeaderButton(window, FONT_DARK11, 22, "BACK", clrWH, intDB, text_buttonw, 48, -1), idx + 2, idx + 3, hearts.size() > 0 ? (last - start) - 1 : idx + 3, idx + 3, startx + 3 * text_gridsize, text_buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> teamsList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Team::Type> &teams, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    if (teams.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            std::string teams_string = std::string(Team::Descriptions[teams[index]]);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 28, teams_string.c_str(), clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (teams.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (teams.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/yes.png", idx, idx, teams.size() > 0 ? (last - start) - 1 : idx, idx, textx, buttony, Control::Type::CONFIRM));
    controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, teams.size() > 0 ? (last - start) - 1 : idx + 1, idx + 1, lastx, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> equipmentList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Equipment::Base> list, int start, int last, int limit, bool confirm_button, bool back_button)
{
    auto font_size = 28;

    auto controls = std::vector<Button>();

    if (list.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto item_string = itemString(list[index]);

            auto text = createText(item_string.c_str(), FONT_GARAMOND, font_size, clrBK, textwidth - 4 * text_space, TTF_STYLE_NORMAL);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : texty + 2 * text_space);

            controls.push_back(Button(i, text, i, i, (i > 0 ? i - 1 : i), i + 1, textx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = textwidth - 4 * text_space;

            controls[i].H = text->h;
        }
    }

    auto idx = (int)controls.size();

    if (list.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (list.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    if (confirm_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/yes.png", idx - 1, (back_button ? idx + 1 : idx), (list.size() > 0 ? ((last - start) - 1) : idx), idx, textx, buttony, Control::Type::CONFIRM));
    }

    if (back_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/back-button.png", idx - 1, idx, (list.size() > 0 ? ((last - start) - 1) : idx), idx, lastx, buttony, Control::Type::BACK));
    }

    return controls;
}

std::vector<Button> equipmentList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Equipment::Base> list, int start, int last, int limit, int offsety, int scrolly)
{
    auto font_size = 28;

    auto controls = std::vector<Button>();

    if (list.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto item_string = itemString(list[index]);

            auto text = createText(item_string.c_str(), FONT_GARAMOND, font_size, clrBK, textwidth - 4 * text_space, TTF_STYLE_NORMAL);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, text, i, i, (i > 0 ? i - 1 : i), i + 1, textx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = textwidth - 4 * text_space;

            controls[i].H = text->h;
        }
    }

    auto idx = (int)controls.size();

    if (list.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (list.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/yes.png", idx, idx + 1, (list.size() > 0 ? ((last - start) - 1) : idx), idx, textx, buttony, Control::Type::USE));
    controls.push_back(Button(idx + 1, "icons/no.png", idx, idx + 2, (list.size() > 0 ? ((last - start) - 1) : idx + 1), idx + 1, textx + gridsize, buttony, Control::Type::DROP));
    controls.push_back(Button(idx + 2, "icons/interaction.png", idx + 1, idx + 3, (list.size() > 0 ? ((last - start) - 1) : idx + 2), idx + 2, textx + 2 * gridsize, buttony, Control::Type::TRANSFER));
    controls.push_back(Button(idx + 3, "icons/vault.png", idx + 2, idx + 4, (list.size() > 0 ? ((last - start) - 1) : idx + 3), idx + 3, textx + 3 * gridsize, buttony, Control::Type::VAULT));
    controls.push_back(Button(idx + 4, "icons/back-button.png", idx + 3, idx + 4, (list.size() > 0 ? ((last - start) - 1) : idx + 4), idx + 4, lastx, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> vaultList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Equipment::Base> list, int start, int last, int limit, int offsety, int scrolly)
{
    auto font_size = 28;

    auto controls = std::vector<Button>();

    if (list.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto item_string = itemString(list[index]);

            auto text = createText(item_string.c_str(), FONT_GARAMOND, font_size, clrBK, textwidth - 4 * text_space, TTF_STYLE_NORMAL);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, text, i, i, (i > 0 ? i - 1 : i), i + 1, textx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = textwidth - 4 * text_space;

            controls[i].H = text->h;
        }
    }

    auto idx = (int)controls.size();

    if (list.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (list.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/yes.png", idx, idx + 1, (list.size() > 0 ? ((last - start) - 1) : idx), idx, textx, buttony, Control::Type::USE));
    controls.push_back(Button(idx + 1, "icons/interaction.png", idx, idx + 2, (list.size() > 0 ? ((last - start) - 1) : idx + 1), idx + 1, textx + gridsize, buttony, Control::Type::TRANSFER));
    controls.push_back(Button(idx + 2, "icons/shop.png", idx + 1, idx + 3, (list.size() > 0 ? ((last - start) - 1) : idx + 2), idx + 2, textx + 2 * gridsize, buttony, Control::Type::MONEY));
    controls.push_back(Button(idx + 3, "icons/back-button.png", idx + 2, idx + 3, (list.size() > 0 ? ((last - start) - 1) : idx + 3), idx + 3, lastx, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> spellList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Spells::Base> &spells, int start, int last, int limit, int offsetx, int offsety, int scrolly, bool confirm_button, bool back_button)
{
    auto controls = std::vector<Button>();

    if (spells.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto spell = spells[index];

            std::string spell_string = "";

            spell_string += spell.Name;

            spell_string += "\nType: " + std::string(Spells::ScopeDescriptions[spell.Scope]) + ", Charged: " + std::string(spell.Charged ? "Yes" : "No") + ", Recharge: " + std::to_string(spell.Recharge);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 24, spell_string.c_str(), clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (spells.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (spells.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    if (confirm_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/yes.png", idx - 1, (back_button ? idx + 1 : idx), spells.size() > 0 ? (last - start) - 1 : idx, idx, textx, buttony, Control::Type::CONFIRM));
    }

    if (back_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/back-button.png", idx - 1, idx, spells.size() > 0 ? (last - start) - 1 : idx, idx, lastx, buttony, Control::Type::BACK));
    }

    return controls;
}

std::vector<Button> spellList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Spells::Base> &spells, int start, int last, int limit, int offsetx, int offsety, int scrolly)
{
    auto controls = std::vector<Button>();

    if (spells.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto spell = spells[index];

            std::string spell_string = "";

            spell_string += spell.Name;

            spell_string += "\nType: " + std::string(Spells::ScopeDescriptions[spell.Scope]) + ", Charged: " + std::string(spell.Charged ? "Yes" : "No") + ", Recharge: " + std::to_string(spell.Recharge);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 24, spell_string.c_str(), clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (spells.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (spells.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    controls.push_back(Button(idx, "icons/yes.png", idx, idx + 1, (spells.size() > 0 ? ((last - start) - 1) : idx), idx, textx, buttony, Control::Type::SPELL));
    controls.push_back(Button(idx + 1, "icons/no.png", idx, idx + 2, (spells.size() > 0 ? ((last - start) - 1) : idx + 1), idx + 1, textx + gridsize, buttony, Control::Type::UNLEARN));
    controls.push_back(Button(idx + 2, "icons/back-button.png", idx + 1, idx + 2, (spells.size() > 0 ? ((last - start) - 1) : idx + 2), idx + 2, lastx, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> rechargeList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Spells::Base> &spells, int start, int last, int limit, int offsetx, int offsety, int scrolly)
{
    auto controls = std::vector<Button>();

    if (spells.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto spell = spells[index];

            std::string spell_string = "";

            spell_string += spell.Name;

            spell_string += "\nType: " + std::string(Spells::ScopeDescriptions[spell.Scope]) + ", Charged: " + std::string(spell.Charged ? "Yes" : "No") + ", Recharge: " + std::to_string(spell.Recharge);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 24, spell_string.c_str(), clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (spells.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (spells.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    controls.push_back(Button(idx, "icons/yes.png", idx, idx + 1, (spells.size() > 0 ? ((last - start) - 1) : idx), idx, textx, buttony, Control::Type::RECHARGE));
    controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, (spells.size() > 0 ? ((last - start) - 1) : idx + 1), idx + 1, lastx, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> monsterList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Monster::Base> &monsters, int start, int last, int limit, int offsetx, int offsety, bool confirm_button, bool back_button)
{
    auto controls = std::vector<Button>();

    if (monsters.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto monster = monsters[index];

            std::string monster_string = monsterString(monster);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 24, monster_string.c_str(), clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (monsters.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (monsters.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    if (confirm_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/yes.png", idx - 1, (back_button ? idx + 1 : idx), monsters.size() > 0 ? (last - start) - 1 : idx, idx, textx, buttony, Control::Type::CONFIRM));
    }

    if (back_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/back-button.png", idx - 1, idx, monsters.size() > 0 ? (last - start) - 1 : idx, idx, lastx, buttony, Control::Type::BACK));
    }

    return controls;
}

std::vector<Button> monsterList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Monster::Base> &monsters, int start, int last, int limit, int offsetx, int offsety, Control::Type mode)
{
    auto controls = std::vector<Button>();

    if (monsters.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto monster = monsters[index];

            std::string monster_string = monsterString(monster);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 24, monster_string.c_str(), clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (monsters.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (monsters.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "VIEW PARTY", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 1, monsters.size() > 0 ? (last - start) - 1 : idx, idx, startx, text_buttony, Control::Type::PARTY));

    if (mode == Control::Type::ATTACK)
    {
        controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "ATTACK", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 2, monsters.size() > 0 ? (last - start) - 1 : idx + 1, idx + 1, startx + text_gridsize, text_buttony, Control::Type::ATTACK));
    }
    else if (mode == Control::Type::DEFEND)
    {
        controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "DEFEND", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 2, monsters.size() > 0 ? (last - start) - 1 : idx + 1, idx + 1, startx + text_gridsize, text_buttony, Control::Type::ATTACK));
    }
    else if (mode == Control::Type::NEXT)
    {
        controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "NEXT ROUND", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 2, monsters.size() > 0 ? (last - start) - 1 : idx + 1, idx + 1, startx + text_gridsize, text_buttony, Control::Type::ATTACK));
    }

    controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "CAST SPELL", clrWH, intDB, text_buttonw, 48, -1), idx + 1, idx + 3, monsters.size() > 0 ? (last - start) - 1 : idx + 2, idx + 2, startx + 2 * text_gridsize, text_buttony, Control::Type::SPELL));

    controls.push_back(Button(idx + 3, createHeaderButton(window, FONT_DARK11, 22, "FLEE", clrWH, intDB, text_buttonw, 48, -1), idx + 2, idx + 3, monsters.size() > 0 ? (last - start) - 1 : idx + 3, idx + 3, startx + 3 * text_gridsize, text_buttony, Control::Type::FLEE));

    return controls;
}

std::vector<Button> rawCharacters(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> party, int start, int last, int limit, int offsetx, int offsety, bool confirm_button, bool back_button)
{
    auto controls = std::vector<Button>();

    if (party.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto adventurer = party[index];

            std::string adventurer_string = rawCharacterText(adventurer, true);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 22, adventurer_string.c_str(), clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), ((i < (last - start) - 1) ? i + 1 : ((back_button || confirm_button) ? i + 1 : i)), offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (party.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (party.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    if (confirm_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/yes.png", idx, back_button ? idx + 1 : idx, party.size() > 0 ? (last - start) - 1 : idx, idx, textx, buttony, Control::Type::CONFIRM));
    }

    if (back_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/back-button.png", confirm_button ? idx - 1 : idx, idx, party.size() > 0 ? (last - start) - 1 : idx, idx, lastx, buttony, Control::Type::BACK));
    }

    return controls;
}

std::vector<Button> combatantList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> party, int start, int last, int limit, int offsetx, int offsety, bool confirm_button, bool back_button)
{
    auto controls = std::vector<Button>();

    if (party.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto adventurer = party[index];

            std::string adventurer_string = characterText(adventurer, true);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 22, adventurer_string.c_str(), clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), ((i < (last - start) - 1) ? i + 1 : ((back_button || confirm_button) ? i + 1 : i)), offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (party.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (party.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    if (confirm_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/yes.png", idx, back_button ? idx + 1 : idx, party.size() > 0 ? (last - start) - 1 : idx, idx, textx, buttony, Control::Type::CONFIRM));
    }

    if (back_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/back-button.png", confirm_button ? idx - 1 : idx, idx, party.size() > 0 ? (last - start) - 1 : idx, idx, lastx, buttony, Control::Type::BACK));
    }

    return controls;
}

std::vector<Button> restList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Character::Base> party, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    if (party.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto adventurer = party[index];

            std::string adventurer_string = characterText(adventurer, true);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 22, adventurer_string.c_str(), clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (party.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (party.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "HEAL 1", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 1, party.size() > 0 ? (last - start) - 1 : idx, idx, startx, text_buttony, Control::Type::HEAL1));
    controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "FULLY RECOVER", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 2, party.size() > 0 ? (last - start) - 1 : idx + 1, idx + 1, startx + text_gridsize, text_buttony, Control::Type::FULL_RECOVERY));
    controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "RECHARGE SPELLS", clrWH, intDB, text_buttonw, 48, -1), idx + 1, idx + 3, party.size() > 0 ? (last - start) - 1 : idx + 2, idx + 2, startx + 2 * text_gridsize, text_buttony, Control::Type::RECHARGE));
    controls.push_back(Button(idx + 3, createHeaderButton(window, FONT_DARK11, 22, "BACK", clrWH, intDB, text_buttonw, 48, -1), idx + 2, idx + 3, party.size() > 0 ? (last - start) - 1 : idx + 3, idx + 3, startx + 3 * text_gridsize, text_buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    if (ships.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto ship_string = shipString(ships[index], true);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 22, ship_string.c_str(), clrBK, intBE, list_buttonw, 100, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (ships.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (ships.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "ARMY", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 1, ships.size() > 0 ? (last - start) - 1 : idx, idx, startx, text_buttony, Control::Type::ARMY));
    controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "FLEET", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 2, ships.size() > 0 ? (last - start) - 1 : idx + 1, idx + 1, startx + text_gridsize, text_buttony, Control::Type::FLEET));
    controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "ROMANCE", clrWH, intDB, text_buttonw, 48, -1), idx + 1, idx + 3, ships.size() > 0 ? (last - start) - 1 : idx + 2, idx + 2, startx + 2 * text_gridsize, text_buttony, Control::Type::ROMANCE));
    controls.push_back(Button(idx + 3, createHeaderButton(window, FONT_DARK11, 22, "BACK", clrWH, intDB, text_buttonw, 48, -1), idx + 2, idx + 3, ships.size() > 0 ? (last - start) - 1 : idx + 3, idx + 3, startx + 3 * text_gridsize, text_buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, int start, int last, int limit, int offsetx, int offsety, bool cargo, bool confirm_button, bool back_button)
{
    auto controls = std::vector<Button>();

    if (ships.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto ship_string = shipString(ships[index], cargo);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 22, ship_string.c_str(), clrBK, intBE, list_buttonw, 100, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (ships.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (ships.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    if (confirm_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/yes.png", idx - 1, (back_button ? idx + 1 : idx), ships.size() > 0 ? (last - start) - 1 : idx, idx, textx, buttony, Control::Type::CONFIRM));
    }

    if (back_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/back-button.png", idx - 1, idx, ships.size() > 0 ? (last - start) - 1 : idx, idx, lastx, buttony, Control::Type::BACK));
    }

    return controls;
}

std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, int start, int last, int limit, int offsetx, int offsety, Control::Type mode)
{
    auto controls = std::vector<Button>();

    if (ships.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            std::string ship_string = shipString(ships[index], false);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 22, ship_string.c_str(), clrBK, intBE, list_buttonw, 100, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (ships.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (ships.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    if (mode == Control::Type::ATTACK)
    {
        controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "ATTACK", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 1, ships.size() > 0 ? (last - start) - 1 : idx, idx, startx, text_buttony, Control::Type::ATTACK));
    }
    else if (mode == Control::Type::DEFEND)
    {
        controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "DEFEND", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 1, ships.size() > 0 ? (last - start) - 1 : idx, idx, startx, text_buttony, Control::Type::ATTACK));
    }
    else if (mode == Control::Type::NEXT)
    {
        controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "NEXT ROUND", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 1, ships.size() > 0 ? (last - start) - 1 : idx, idx, startx, text_buttony, Control::Type::ATTACK));
    }

    controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "CAST SPELL", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 2, ships.size() > 0 ? (last - start) - 1 : idx + 1, idx + 1, startx + text_gridsize, text_buttony, Control::Type::SPELL));

    controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "FLEE", clrWH, intDB, text_buttonw, 48, -1), idx + 1, idx + 2, ships.size() > 0 ? (last - start) - 1 : idx + 2, idx + 2, startx + 2 * text_gridsize, text_buttony, Control::Type::FLEE));

    return controls;
}

std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Engine::ShipPrices> &ships, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    if (ships.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto ship = std::get<0>(ships[index]);

            auto buy = std::get<1>(ships[index]);

            auto sell = std::get<2>(ships[index]);

            std::string ship_string = "[" + ship.Name + "] Fighting: " + std::to_string(ship.Fighting) + ", Health: " + std::to_string(ship.Health) + ", Cargo: " + std::to_string(ship.MaximumCargo) + " unit(s)";

            ship_string += "\nPrice: " + std::string(buy > 0 ? std::to_string(buy) + " silver coins" : "Not available") + ", Sell: " + std::string(sell > 0 ? std::to_string(sell) + " silver coins" : "--");

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 22, ship_string.c_str(), clrBK, intBE, list_buttonw, 100, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (ships.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (ships.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "BUY/SELL SHIPS", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 1, ships.size() > 0 ? (last - start) - 1 : idx, idx, startx, text_buttony, Control::Type::BUY_SELL_SHIP));
    controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "REPAIR SHIP", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 2, ships.size() > 0 ? (last - start) - 1 : idx + 1, idx + 1, startx + text_gridsize, text_buttony, Control::Type::REPAIR_SHIP));
    controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "BUY/SELL CARGO", clrWH, intDB, text_buttonw, 48, -1), idx + 1, idx + 3, ships.size() > 0 ? (last - start) - 1 : idx + 2, idx + 2, startx + 2 * text_gridsize, text_buttony, Control::Type::BUY_SELL_CARGO));
    controls.push_back(Button(idx + 3, createHeaderButton(window, FONT_DARK11, 22, "BACK", clrWH, intDB, text_buttonw, 48, -1), idx + 2, idx + 3, ships.size() > 0 ? (last - start) - 1 : idx + 3, idx + 3, startx + 3 * text_gridsize, text_buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, std::vector<Engine::ShipPrices> &shop, int start, int last, int limit, int offsetx, int offsety, bool buy_button, bool sell_button)
{
    auto controls = std::vector<Button>();

    if (ships.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto ship = ships[index];

            auto sell = Engine::PRICE_SELL(shop, ship.Type);

            std::string ship_string = "[" + ship.Name + "] Fighting: " + std::to_string(ship.Fighting) + ", Health: " + std::to_string(ship.Health) + ", Cargo: " + std::to_string(ship.MaximumCargo) + " unit(s)";

            ship_string += "\nSell: " + std::string(sell > 0 ? std::to_string(sell) + " silver coins" : "--");

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 22, ship_string.c_str(), clrBK, intBE, list_buttonw, 100, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (ships.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (ships.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    if (buy_button)
    {
        controls.push_back(Button(idx, "icons/shop.png", idx, idx + 1, (ships.size() > 0 ? ((last - start) - 1) : idx), idx, textx, buttony, Control::Type::BUY_SHIP));

        idx += 1;
    }

    if (sell_button)
    {
        controls.push_back(Button(idx, "icons/selling.png", buy_button ? idx - 1 : idx, idx + 1, (ships.size() > 0 ? ((last - start) - 1) : idx), idx, (buy_button ? (textx + gridsize) : textx), buttony, Control::Type::SELL_SHIP));

        idx += 1;
    }

    auto grid_offset = 0;

    if (buy_button && sell_button)
    {
        grid_offset = 2;
    }
    else if (buy_button || sell_button)
    {
        grid_offset = 1;
    }

    controls.push_back(Button(idx, "icons/user.png", ((buy_button || sell_button) ? idx - 1 : idx), idx + 1, (ships.size() > 0 ? (last - start) - 1 : idx), idx, textx + grid_offset * gridsize, buttony, Control::Type::PARTY));
    controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, (ships.size() > 0 ? ((last - start) - 1) : idx + 1), idx + 1, lastx, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> shipList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Engine::ShipPrices> &ships, int start, int last, int limit, int offsetx, int offsety, bool buy_button, bool sell_button)
{
    auto controls = std::vector<Button>();

    if (ships.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto ship = std::get<0>(ships[index]);

            auto buy = std::get<1>(ships[index]);

            auto sell = std::get<2>(ships[index]);

            std::string ship_string = "[" + ship.Name + "] Fighting: " + std::to_string(ship.Fighting) + ", Health: " + std::to_string(ship.Health) + ", Cargo: " + std::to_string(ship.MaximumCargo) + " unit(s)";

            ship_string += "\nPrice: " + std::string(buy > 0 ? std::to_string(buy) + " silver coins" : "Not available") + ", Sell: " + std::string(sell > 0 ? std::to_string(sell) + " silver coins" : "--");

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 22, ship_string.c_str(), clrBK, intBE, list_buttonw, 100, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (ships.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (ships.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    if (buy_button)
    {
        controls.push_back(Button(idx, "icons/shop.png", idx, idx + 1, (ships.size() > 0 ? ((last - start) - 1) : idx), idx, textx, buttony, Control::Type::BUY_SHIP));

        idx += 1;
    }

    if (sell_button)
    {
        controls.push_back(Button(idx, "icons/selling.png", buy_button ? idx - 1 : idx, idx + 1, (ships.size() > 0 ? ((last - start) - 1) : idx), idx, (buy_button ? (textx + gridsize) : textx), buttony, Control::Type::SELL_SHIP));

        idx += 1;
    }

    auto grid_offset = 0;

    if (buy_button && sell_button)
    {
        grid_offset = 2;
    }
    else if (buy_button || sell_button)
    {
        grid_offset = 1;
    }

    controls.push_back(Button(idx, "icons/user.png", ((buy_button || sell_button) ? idx - 1 : idx), idx + 1, (ships.size() > 0 ? (last - start) - 1 : idx), idx, textx + grid_offset * gridsize, buttony, Control::Type::PARTY));
    controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, (ships.size() > 0 ? ((last - start) - 1) : idx + 1), idx + 1, lastx, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> buyCargo(SDL_Window *window, SDL_Renderer *renderer, std::vector<Engine::CargoPrices> &cargo, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

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

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 22, cargo_string.c_str(), clrBK, intBE, list_buttonw, 100, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (cargo.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (cargo.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/selling.png", idx, idx + 1, (cargo.size() > 0 ? (last - start) - 1 : idx), idx, textx, buttony, Control::Type::SELL_CARGO));
    controls.push_back(Button(idx + 1, "icons/shop.png", idx, idx + 2, (cargo.size() > 0 ? (last - start) - 1 : idx + 1), idx + 1, textx + gridsize, buttony, Control::Type::BUY_CARGO));
    controls.push_back(Button(idx + 2, "icons/user.png", idx + 1, idx + 3, (cargo.size() > 0 ? (last - start) - 1 : idx + 2), idx + 2, textx + 2 * gridsize, buttony, Control::Type::PARTY));
    controls.push_back(Button(idx + 3, "icons/back-button.png", idx + 2, idx + 3, (cargo.size() > 0 ? (last - start) - 1 : idx + 3), idx + 3, lastx, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> cargoList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Engine::CargoPrices> &cargo, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

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

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 22, cargo_string.c_str(), clrBK, intBE, list_buttonw, 100, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (cargo.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (cargo.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "BUY/SELL SHIPS", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 1, cargo.size() > 0 ? (last - start) - 1 : idx, idx, startx, text_buttony, Control::Type::BUY_SELL_SHIP));
    controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "REPAIR SHIP", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 2, cargo.size() > 0 ? (last - start) - 1 : idx + 1, idx + 1, startx + text_gridsize, text_buttony, Control::Type::REPAIR_SHIP));
    controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "BUY/SELL CARGO", clrWH, intDB, text_buttonw, 48, -1), idx + 1, idx + 3, cargo.size() > 0 ? (last - start) - 1 : idx + 2, idx + 2, startx + 2 * text_gridsize, text_buttony, Control::Type::BUY_SELL_CARGO));
    controls.push_back(Button(idx + 3, createHeaderButton(window, FONT_DARK11, 22, "BACK", clrWH, intDB, text_buttonw, 48, -1), idx + 2, idx + 3, cargo.size() > 0 ? (last - start) - 1 : idx + 3, idx + 3, startx + 3 * text_gridsize, text_buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> cargoList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    if (ships.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            std::string cargo_string = shipString(ships[index], true);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 22, cargo_string.c_str(), clrBK, intBE, list_buttonw, 100, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (ships.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (ships.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/selling.png", idx, idx + 1, (ships.size() > 0 ? (last - start) - 1 : idx), idx, textx, buttony, Control::Type::SELL_CARGO));
    controls.push_back(Button(idx + 1, "icons/shop.png", idx, idx + 2, (ships.size() > 0 ? (last - start) - 1 : idx + 1), idx + 1, textx + gridsize, buttony, Control::Type::BUY_CARGO));
    controls.push_back(Button(idx + 2, "icons/user.png", idx + 1, idx + 3, (ships.size() > 0 ? (last - start) - 1 : idx + 2), idx + 2, textx + 2 * gridsize, buttony, Control::Type::PARTY));
    controls.push_back(Button(idx + 3, "icons/back-button.png", idx + 2, idx + 3, (ships.size() > 0 ? (last - start) - 1 : idx + 3), idx + 3, lastx, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> harbourControls(SDL_Window *window, SDL_Renderer *renderer)
{
    auto controls = std::vector<Button>();

    auto idx = 0;

    controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "BUY/SELL SHIPS", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 1, idx, idx, startx, text_buttony, Control::Type::BUY_SELL_SHIP));
    controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "REPAIR SHIP", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 2, idx + 1, idx + 1, startx + text_gridsize, text_buttony, Control::Type::REPAIR_SHIP));
    controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "BUY/SELL CARGO", clrWH, intDB, text_buttonw, 48, -1), idx + 1, idx + 3, idx + 2, idx + 2, startx + 2 * text_gridsize, text_buttony, Control::Type::BUY_SELL_CARGO));
    controls.push_back(Button(idx + 3, createHeaderButton(window, FONT_DARK11, 22, "BACK", clrWH, intDB, text_buttonw, 48, -1), idx + 2, idx + 3, idx + 3, idx + 3, startx + 3 * text_gridsize, text_buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> shopList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Engine::EquipmentPrice> &shop, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

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

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 24, item_string.c_str(), clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (shop.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (shop.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/shop.png", idx, idx + 1, (shop.size() > 0 ? ((last - start) - 1) : idx), idx, textx, buttony, Control::Type::BUY));
    controls.push_back(Button(idx + 1, "icons/selling.png", idx, idx + 2, (shop.size() > 0 ? ((last - start) - 1) : idx + 1), idx + 1, textx + gridsize, buttony, Control::Type::SELL));
    controls.push_back(Button(idx + 2, "icons/items.png", idx + 1, idx + 3, (shop.size() > 0 ? ((last - start) - 1) : idx + 2), idx + 2, textx + 2 * gridsize, buttony, Control::Type::EQUIPMENT));
    controls.push_back(Button(idx + 3, "icons/previous.png", idx + 2, idx + 4, (shop.size() > 0 ? ((last - start) - 1) : idx + 3), idx + 3, textx + 3 * gridsize, buttony, Control::Type::PREVIOUS_CHARACTER));
    controls.push_back(Button(idx + 4, "icons/next.png", idx + 3, idx + 5, (shop.size() > 0 ? ((last - start) - 1) : idx + 4), idx + 4, textx + 4 * gridsize, buttony, Control::Type::NEXT_CHARACTER));
    controls.push_back(Button(idx + 5, "icons/back-button.png", idx + 4, idx + 5, (shop.size() > 0 ? ((last - start) - 1) : idx + 5), idx + 5, lastx, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> shopList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Equipment::Base> &equipment, std::vector<Engine::EquipmentPrice> &shop, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    if (equipment.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto item = equipment[index];

            auto sell = Engine::PRICE_SELL(shop, item.Type);

            auto item_string = itemString(item);

            item_string += "\nSell: " + std::string(sell > 0 ? std::to_string(sell) + " silver coins" : "--");

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 24, item_string.c_str(), clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (equipment.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (equipment.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/shop.png", idx, idx + 1, (equipment.size() > 0 ? ((last - start) - 1) : idx), idx, textx, buttony, Control::Type::BUY));
    controls.push_back(Button(idx + 1, "icons/selling.png", idx, idx + 2, (equipment.size() > 0 ? ((last - start) - 1) : idx + 1), idx + 1, textx + gridsize, buttony, Control::Type::SELL));
    controls.push_back(Button(idx + 2, "icons/items.png", idx + 1, idx + 3, (equipment.size() > 0 ? ((last - start) - 1) : idx + 2), idx + 2, textx + 2 * gridsize, buttony, Control::Type::EQUIPMENT));
    controls.push_back(Button(idx + 3, "icons/previous.png", idx + 2, idx + 4, (equipment.size() > 0 ? ((last - start) - 1) : idx + 3), idx + 3, textx + 3 * gridsize, buttony, Control::Type::PREVIOUS_CHARACTER));
    controls.push_back(Button(idx + 4, "icons/next.png", idx + 3, idx + 5, (equipment.size() > 0 ? ((last - start) - 1) : idx + 4), idx + 4, textx + 4 * gridsize, buttony, Control::Type::NEXT_CHARACTER));
    controls.push_back(Button(idx + 5, "icons/back-button.png", idx + 4, idx + 5, (equipment.size() > 0 ? ((last - start) - 1) : idx + 5), idx + 5, lastx, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> repairList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &ships, int start, int last, int limit, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    if (ships.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            std::string ship_string = shipString(ships[index], true);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 22, ship_string.c_str(), clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (ships.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, scrollx, texty + border_space, Control::Type::SCROLL_UP));

            idx += 1;
        }

        if (ships.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, scrollx, scrolly, Control::Type::SCROLL_DOWN));

            idx += 1;
        }
    }

    controls.push_back(Button(idx, createHeaderButton(window, FONT_DARK11, 22, "REPAIR 1", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 1, ships.size() > 0 ? (last - start) - 1 : idx, idx, startx, text_buttony, Control::Type::REPAIR1));
    controls.push_back(Button(idx + 1, createHeaderButton(window, FONT_DARK11, 22, "FULLY REPAIR", clrWH, intDB, text_buttonw, 48, -1), idx, idx + 2, ships.size() > 0 ? (last - start) - 1 : idx + 1, idx + 1, startx + text_gridsize, text_buttony, Control::Type::FULL_REPAIR));
    controls.push_back(Button(idx + 2, createHeaderButton(window, FONT_DARK11, 22, "BACK", clrWH, intDB, text_buttonw, 48, -1), idx + 1, idx + 2, ships.size() > 0 ? (last - start) - 1 : idx + 2, idx + 2, startx + 2 * text_gridsize, text_buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> popupConfirm(int popupw, int popuph, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    controls.push_back(Button(0, "icons/yes.png", 0, 1, 0, 0, offsetx + button_space, offsety + popuph - button_space - buttonh, Control::Type::CONFIRM));
    controls.push_back(Button(1, "icons/no.png", 0, 1, 1, 1, offsetx + popupw - buttonw - button_space, offsety + popuph - button_space - buttonh, Control::Type::BACK));

    return controls;
}

std::vector<Button> popupMoney(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, int popupw, int popuph, int infoh, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    controls.push_back(Button(0, createHeaderButton(window, FONT_GARAMOND, 24, (std::string("Vault Money: " + std::to_string(party.VaultMoney) + " silver coins")).c_str(), clrBK, intBE, popupw - 3 * button_space / 2 - button_space - arrow_size - border_space, list_buttonh, text_space), 0, 0, 0, 1, offsetx + 2 * text_space, offsety + infoh + 3 * text_space, Control::Type::ACTION));
    controls[0].W = controls[0].Surface->w;
    controls[0].H = controls[0].Surface->h;

    controls.push_back(Button(1, createHeaderButton(window, FONT_GARAMOND, 24, (std::string("Party Money: " + std::to_string(party.Money) + " silver coins")).c_str(), clrBK, intBE, popupw - 3 * button_space / 2 - button_space - arrow_size - border_space, list_buttonh, text_space), 1, 1, 0, 2, offsetx + 2 * text_space, controls[0].Y + controls[0].H + 3 * text_space, Control::Type::ACTION));
    controls[1].W = controls[1].Surface->w;
    controls[1].H = controls[1].Surface->h;

    controls.push_back(Button(2, "icons/yes.png", 2, 3, 1, 2, offsetx + button_space, offsety + popuph - button_space - buttonh, Control::Type::CONFIRM));
    controls.push_back(Button(3, "icons/add.png", 2, 4, 1, 3, offsetx + button_space + gridsize, offsety + popuph - button_space - buttonh, Control::Type::PLUS));
    controls.push_back(Button(4, "icons/minus.png", 3, 4, 1, 4, offsetx + button_space + 2 * gridsize, offsety + popuph - button_space - buttonh, Control::Type::MINUS));

    return controls;
}

std::vector<Button> popupList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Army::Base> &list, int start, int last, int limit, int popupw, int popuph, int infoh, int offsetx, int offsety, bool back_button)
{
    auto controls = std::vector<Button>();

    if (list.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto unit = list[index];

            std::string list_string = "[" + unit.Name + "] Strength: " + std::to_string(unit.Strength) + ", Morale: " + std::to_string(unit.Morale);

            list_string += "\nPosition: " + std::string(Location::BattleFieldDescription[unit.Position]) + ", Garrison: " + std::string(Location::Description[unit.Garrison]);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + infoh + 3 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 24, list_string.c_str(), clrBK, intBE, popupw - 3 * button_space / 2 - button_space - arrow_size - border_space, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    popupScrolls(controls, list, start, last, limit, popupw, popuph, infoh, offsetx, offsety, back_button);

    return controls;
}

std::vector<Button> popupArmy(SDL_Window *window, SDL_Renderer *renderer, std::vector<Army::Base> &army, int start, int last, int limit, int popupw, int popuph, int infoh, int offsetx, int offsety)
{
    return popupList(window, renderer, army, start, last, limit, popupw, popuph, infoh, offsetx, offsety, true);
}

std::vector<Button> popupList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Monster::Base> &list, int start, int last, int limit, int popupw, int popuph, int infoh, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    if (list.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            std::string list_string = monsterString(list[index]);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + infoh + 3 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 24, list_string.c_str(), clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    popupScrolls(controls, list, start, last, limit, popupw, popuph, infoh, offsetx, offsety, false);

    return controls;
}

std::vector<Button> popupList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Ship::Base> &list, int start, int last, int limit, int popupw, int popuph, int infoh, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    if (list.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto ship = list[index];

            std::string list_string = "[" + ship.Name + "] Fighting: " + std::to_string(ship.Fighting) + ", Health: " + std::to_string(ship.Health);

            if (ship.MaximumCargo > 0)
            {
                list_string += ", Cargo Units: " + std::to_string(ship.MaximumCargo);
            }

            list_string += "\nLocation: " + std::string(Location::Description[ship.Location]);

            if (ship.MaximumCargo > 0 && ship.Cargo.size() > 0)
            {
                list_string += " Cargo: ";

                for (auto j = 0; j < ship.Cargo.size(); j++)
                {
                    if (j > 0)
                    {
                        list_string += ", ";
                    }

                    list_string += Cargo::Description[ship.Cargo[j]];
                }
            }

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + infoh + 3 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 24, list_string.c_str(), clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    popupScrolls(controls, list, start, last, limit, popupw, popuph, infoh, offsetx, offsety, false);

    return controls;
}

std::vector<Button> popupList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Location::Type> &list, int start, int last, int limit, int popupw, int popuph, int infoh, int offsetx, int offsety)
{
    auto controls = std::vector<Button>();

    if (list.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            std::string list_string = Location::Description[list[index]];

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + infoh + 3 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 24, list_string.c_str(), clrBK, intBE, list_buttonw, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    popupScrolls(controls, list, start, last, limit, popupw, popuph, infoh, offsetx, offsety, true);

    return controls;
}

std::vector<Button> topicsList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Topics::Base> &topics, int start, int last, int limit, int offsetx, int offsety, bool compact)
{
    auto controls = std::vector<Button>();

    if (topics.size() > 0)
    {
        for (auto i = 0; i < last - start; i++)
        {
            auto index = start + i;

            auto topic = topics[index];

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : offsety + 2 * text_space);

            controls.push_back(Button(i, createHeaderButton(window, FONT_GARAMOND, 22, topic.Title.c_str(), clrBK, intBE, splashw - 3 * button_space / 2, list_buttonh, text_space), i, i, (i > 0 ? i - 1 : i), i + 1, offsetx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = controls[i].Surface->w;

            controls[i].H = controls[i].Surface->h;
        }
    }

    auto idx = (int)controls.size();

    if (topics.size() > limit)
    {
        auto marginx = (int)(SCREEN_WIDTH * Margin);

        auto scroll_space = (marginx - arrow_size) / 2;

        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, startx + splashw + scroll_space, texty + border_space, Control::Type::TOPICS_UP));

            idx += 1;
        }

        if (topics.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, startx + splashw + scroll_space, scrolly, Control::Type::TOPICS_DOWN));

            idx += 1;
        }
    }

    if (!compact)
    {
        idx = controls.size();

        auto topic_scrollx = (int)((1.0 - Margin) * SCREEN_WIDTH - arrow_size);

        controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, topic_scrollx, texty + border_space, Control::Type::SCROLL_UP));
        controls.push_back(Button(idx + 1, "icons/down-arrow.png", idx + 1, idx + 1, idx, idx + 2, topic_scrollx, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/previous.png", idx, idx + 1, topics.size() > 0 ? (last - start) - 1 : idx, idx, textx, buttony, Control::Type::PREVIOUS_TOPIC));
    controls.push_back(Button(idx + 1, "icons/next.png", idx, idx + 2, topics.size() > 0 ? (last - start) - 1 : idx + 1, idx + 1, textx + gridsize, buttony, Control::Type::NEXT_TOPIC));
    controls.push_back(Button(idx + 2, "icons/back-button.png", idx + 1, idx + 2, topics.size() > 0 ? (last - start) - 1 : idx + 2, idx + 2, lastx, buttony, Control::Type::BACK));

    return controls;
}

void followersJSON(nlohmann::json &json, std::vector<Follower::Base> followers)
{
    for (auto i = 0; i < followers.size(); i++)
    {
        nlohmann::json data;

        data.emplace("name", followers[i].Name);
        data.emplace("type", followers[i].Type);
        data.emplace("health", followers[i].Health);

        json.push_back(data);
    }
}

void spellBookJSON(nlohmann::json &json, std::vector<Spells::Base> spells)
{
    for (auto i = 0; i < spells.size(); i++)
    {
        nlohmann::json data;

        data.emplace("name", spells[i].Name);
        data.emplace("description", spells[i].Description);
        data.emplace("scope", spells[i].Scope);
        data.emplace("type", spells[i].Type);
        data.emplace("charged", spells[i].Charged);
        data.emplace("recharge", spells[i].Recharge);

        json.push_back(data);
    }
}

void attributesJSON(nlohmann::json &json, std::vector<Attribute::Base> attributes)
{
    for (auto i = 0; i < attributes.size(); i++)
    {
        nlohmann::json data;

        data.emplace("type", attributes[i].Type);
        data.emplace("value", attributes[i].Value);

        json.push_back(data);
    }
}

void equipmentJSON(nlohmann::json &json, std::vector<Equipment::Base> equipment)
{
    for (auto i = 0; i < equipment.size(); i++)
    {
        nlohmann::json data;

        data.emplace("name", equipment[i].Name);
        data.emplace("description", equipment[i].Description);
        data.emplace("class", equipment[i].Class);
        data.emplace("type", equipment[i].Type);
        data.emplace("attribute", equipment[i].Attribute);
        data.emplace("modifier", equipment[i].Modifier);
        data.emplace("twoHanded", equipment[i].TwoHanded);
        data.emplace("additionalSlots", equipment[i].AdditionalSlots);
        data.emplace("value", equipment[i].Value);

        json.push_back(data);
    }
}
// ----------------------------------------------------------------------------
// END
// ----------------------------------------------------------------------------
#endif
