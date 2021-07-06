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
#include "input.hpp"

// Forward declarations
bool introScreen(SDL_Window *window, SDL_Renderer *renderer);
bool mainScreen(SDL_Window *window, SDL_Renderer *renderer, int storyID);

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
    int splash_h = image->h;
    int splash_w = w;

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
    int splash_h = image->h;
    int splash_w = w;

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
    int splash_h = image->h;
    int splash_w = w;

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

void putHeader(SDL_Renderer *renderer, const char *text, TTF_Font *font, int space, SDL_Color fg, Uint32 bg, int style, int w, int h, int x, int y)
{
    if (renderer)
    {
        TTF_SetFontStyle(font, style);

        auto surface = TTF_RenderText_Blended_Wrapped(font, text, fg, w - 2 * space);

        if (surface)
        {
            auto height = (surface->h + 2 * space) < h ? h : (surface->h + 2 * space);

            fillRect(renderer, w, height, x, y, bg);

            if (space > 0)
            {
                renderText(renderer, surface, 0, x + space, y + (h - surface->h) / 2, height - 2 * space, 0);
            }
            else
            {
                renderText(renderer, surface, 0, x + (w - surface->w) / 2, y + (h - surface->h) / 2, height - 2 * space, 0);
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

            fillRect(renderer, w, height, x, y, bg);

            if (space > 0)
            {
                renderText(renderer, surface, 0, x + space, y + space, height - 2 * space, 0);
            }
            else
            {
                renderText(renderer, surface, 0, x + (w - surface->w) / 2, y + (h - surface->h) / 2, height - 2 * space, 0);
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

            int x = controls[i].X + (controls[i].W - text->w) / 2;
            int y = controls[i].Y + (controls[i].H - text->h) / 2;

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
            int left = i > 0 ? i - 1 : i;
            int right = i < num - 1 ? i + 1 : i;
            int up = i;
            int down = i;

            auto x = text_x + i * (text_buttonw + text_space * 2) + text_space;

            auto button = TextButton(i, choices[i], left, right, up, down, x, text_y, text_buttonw, text_buttonh);

            controls.push_back(button);
        }
    }

    return controls;
}

SDL_Surface *createHeaderButton(SDL_Window *window, const char *text, SDL_Color color, Uint32 bg, int w, int h, int x)
{
    auto button = SDL_CreateRGBSurface(0, w, h, arrow_size, 0, 0, 0, 0);
    auto text_surface = createText(text, FONT_BOOKMAN, 18, color, w, TTF_STYLE_NORMAL);

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

std::string characterText(Character::Base character)
{
    std::string character_text = character.Background;

    character_text += "\n\nATTRIBUTES: ";

    for (auto i = 0; i < character.Attributes.size(); i++)
    {
        if (i > 0)
        {
            character_text += ", ";
        }

        character_text += std::string(Attribute::Descriptions[character.Attributes[i].Type]) + ": " + std::to_string(character.Attributes[i].Value);
    }

    if (character.SpellCaster)
    {
        character_text += "\n\nSpellcaster";
    }

    return character_text;
};

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

        const char *choices_add[6] = {"PREVIOUS", "NEXT", "ADD TO PARTY", "START", "BACK"};
        const char *choices_del[6] = {"PREVIOUS", "NEXT", "REMOVE", "START", "BACK"};

        auto current = -1;

        auto selected = false;

        auto main_buttonh = 0.06 * SCREEN_HEIGHT;

        auto infoh = 0.07 * SCREEN_HEIGHT;

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

        std::vector<Character::Base> characters = std::vector<Character::Base>();

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

            text = createText(characterText(characters[character]).c_str(), FONT_GARAMOND, garamond_size, clrDB, textwidth - 2 * text_space, TTF_STYLE_NORMAL);
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
                putHeader(renderer, std::string("PARTY (Limit: " + std::to_string(party.Limit) + ")").c_str(), font_mason2, 8, clrWH, intDB, TTF_STYLE_NORMAL, textwidth, infoh, textx, starty + character_box + 10);

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

                        text = createText(characterText(characters[character]).c_str(), FONT_GARAMOND, garamond_size, clrDB, textwidth - 2 * text_space, TTF_STYLE_NORMAL);
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

                        text = createText(characterText(characters[character]).c_str(), FONT_GARAMOND, garamond_size, clrDB, textwidth - 2 * text_space, TTF_STYLE_NORMAL);
                    }

                    break;

                case Control::Type::NEW:

                    if (selection.size() == party.Limit)
                    {
                        for (auto i = 0; i < selection.size(); i++)
                        {
                            if (selection[i] >= 0 && selection[i] < characters.size())
                            {
                                party.Party.push_back(characters[selection[i]]);
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

    auto map_valley = createImage("images/valley-of-bones-map.png");
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

std::vector<Button> monsterList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Monster::Base> monsters, int start, int last, int limit, int offsetx, int offsety, bool back_button)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (monsters.size() > 0)
    {
        for (int i = 0; i < last - start; i++)
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

    if (back_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/back-button.png", idx - 1, idx, monsters.size() > 0 ? (last - start) : idx, idx, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));
    }

    return controls;
}

Engine::CombatResult combatScreen(SDL_Window *window, SDL_Renderer *renderer, Party::Base &party, std::vector<Monster::Base> &monsters, bool canFlee, bool useEquipment)
{
    auto combatResult = Engine::CombatResult::NONE;

    if (window && renderer)
    {
        auto flash_message = false;

        auto flash_color = intRD;

        const char *message = NULL;

        Uint32 start_ticks = 0;

        Uint32 duration = 5000;

        TTF_Init();

        auto font_garamond = TTF_OpenFont(FONT_GARAMOND, 24);

        auto font_mason = TTF_OpenFont(FONT_MASON, 24);

        auto font_dark11 = TTF_OpenFont(FONT_DARK11, 32);

        auto main_buttonh = 48;

        const char *choices[4] = {"VIEW PARTY", "ATTACK", "CAST SPELL", "FLEE"};

        auto controls = createHTextButtons(choices, 4, main_buttonh, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);

        controls[0].Type = Control::Type::PARTY;
        controls[1].Type = Control::Type::ATTACK;
        controls[2].Type = Control::Type::SPELL;
        controls[3].Type = Control::Type::FLEE;

        auto font_size = 20;
        auto text_space = 8;
        auto messageh = 0.25 * SCREEN_HEIGHT;
        auto infoh = 0.07 * SCREEN_HEIGHT;
        auto boxh = 0.125 * SCREEN_HEIGHT;
        auto box_space = 10;
        auto offset = 0;
        auto limit = (text_bounds - text_space) / ((boxh) + 3 * text_space);
        auto last = offset + limit;

        if (last > monsters.size())
        {
            last = monsters.size();
        }

        auto splash = createImage("images/legendary-kingdoms-logo-bw.png");

        auto monster_list = monsterList(window, renderer, monsters, offset, last, limit, textx, texty + infoh + text_space, false);

        while (Engine::COUNT(monsters) > 0 || Engine::COUNT(party.Party) > 0)
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

                putHeader(renderer, "Opponents", font_dark11, text_space, clrWH, intDB, TTF_STYLE_NORMAL, textwidth, infoh, textx, texty);

                putText(renderer, "Party", font_dark11, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

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
                            party_string += " (-)";
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
                        putText(renderer, message, font_garamond, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw, messageh, startx, starty);
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
                            done = true;

                            combatResult = Engine::CombatResult::FLEE;
                        }
                        else
                        {
                            flash_message = true;

                            message = "You cannot flee from this combat.";

                            start_ticks = SDL_GetTicks();

                            flash_color = intRD;
                        }
                    }
                }
            }

            if (done)
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

    if (combatResult != Engine::CombatResult::FLEE && combatResult != Engine::CombatResult::NONE)
    {
        combatResult = Engine::COUNT(party.Party) > 0 ? Engine::CombatResult::VICTORY : Engine::CombatResult::DOOM;
    }

    return combatResult;
}

bool mainScreen(SDL_Window *window, SDL_Renderer *renderer, int storyID)
{
    auto font_size = 20;

    auto *introduction = "Prepare for adventure in this huge open-world gamebook series. Legendary Kingdoms is a gamebook campaign, where you lead a party of adventurers in a world that adapts to your actions. Venture into ancient ruins, pick a side and lead an army into battle, sail the high seas on your own warship, defeat tyrants or bring them to power. Along the way your party will increase in skill, wealth and renown, allowing them to take on more challenging adventures. Reach the heights of power and you may uncover a dreadful threat to the world itself and go on a mission that spans all six gamebooks in the series.\n\nBook 1: The Valley of Bones takes place in a desert wilderness where tyrant kings oppress the teeming masses in a land strewn with ancient artefacts and ruins. But their grip on power is fragile... and the citizenry are ripe for revolution. It is a land of blood and sand, where civilisation is rare and terrible beasts roam freely.";

    auto splash = createImage("images/valley-of-bones-cover.png");

    auto text = createText(introduction, FONT_GARAMOND, 28, clrDB, SCREEN_WIDTH * (1.0 - 3.0 * Margin) - splashw);

    auto title = "Legendary Kingdoms 1 - The Valley of Bones";

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

        auto Party = Party::Base();

        while (!done)
        {
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

                    done = selectParty(window, renderer, Book::Type::BOOK1, Party);

                    current = -1;

                    selected = false;

                    storyID = 0;

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

bool testScreen(SDL_Window *window, SDL_Renderer *renderer, int storyID)
{
    auto font_size = 20;
    auto text_space = 8;

    auto *introduction = "This is the DEBUG screen. Testing facilities for various gamebook functions such as COMBAT, SKILL CHECKS, MAGIC, etc, can be accessed here. While the game is still in the ALPHA stage, this is the default screen.";

    auto text = createText(introduction, FONT_GARAMOND, 28, clrDB, textwidth - 2 * text_space);

    auto title = "Legendary Kingdoms 1 - The Valley of Bones";

    auto splash = createImage("images/legendary-kingdoms-logo.png");

    // Render window
    if (window && renderer && text && splash)
    {
        SDL_SetWindowTitle(window, title);

        const char *choices[3] = {"COMBAT", "MAP", "EXIT"};

        auto current = -1;

        auto selected = false;

        auto main_buttonh = 48;

        auto controls = createHTextButtons(choices, 3, main_buttonh, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);

        controls[0].Type = Control::Type::COMBAT;
        controls[1].Type = Control::Type::MAP;
        controls[2].Type = Control::Type::QUIT;

        auto done = false;

        auto text_space = 8;

        auto Party = Party::Base();

        std::vector<Monster::Base> monsters = {
            Monster::Base("Goblin", 4, 5, 4, 6, 0),
            Monster::Base("Orc Bodyguard", 6, 4, 4, 10, 0)};

        while (!done)
        {
            // Fill the surface with background
            fillWindow(renderer, intWH);

            fitImage(renderer, splash, startx, starty, splashw, text_bounds);

            fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

            renderText(renderer, text, intBK, startx * 2 + splashw + text_space, starty + text_space, SCREEN_HEIGHT * (1.0 - 2 * Margin) - 2 * text_space, 0);

            renderTextButtons(renderer, controls, FONT_DARK11, current, clrWH, intDB, intLB, font_size + 2, TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            Control::Type result;

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            auto combat = Engine::CombatResult::NONE;

            if (selected && current >= 0 && current < controls.size())
            {
                switch (controls[current].Type)
                {
                case Control::Type::COMBAT:

                    done = selectParty(window, renderer, Book::Type::BOOK1, Party);

                    combat = combatScreen(window, renderer, Party, monsters, true, false);

                    current = -1;

                    selected = false;

                    storyID = 0;

                    break;

                case Control::Type::QUIT:

                    done = true;

                    break;

                case Control::Type::MAP:

                    mapScreen(window, renderer);

                    done = false;

                    current = -1;

                    selected = false;

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

int main(int argc, char **argv)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    auto title = "Legendary Kingdoms 1 - The Valley of Bones";

    createWindow(SDL_INIT_VIDEO, &window, &renderer, title, "icons/toxic.png");

    auto numGamePads = Input::InitializeGamePads();

    auto quit = false;

    auto storyID = 0;

    if (argc > 1)
    {
        storyID = std::atoi(argv[1]);
    }

    auto Party = Party::Base();

    if (window && renderer)
    {
        //introScreen(window, renderer);

        //mainScreen(window, renderer, storyID);

        testScreen(window, renderer, storyID);

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
