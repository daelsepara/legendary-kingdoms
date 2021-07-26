#ifndef __CONSTANTS__HPP__
#define __CONSTANTS__HPP__

#include <SDL.h>

const char *FONT_BOOKMAN = "fonts/bookman-old-style.ttf";
const char *FONT_CASLON = "fonts/caslon-regular.otf";
const char *FONT_DARK11 = "fonts/dark11.ttf";
const char *FONT_GARAMOND = "fonts/garamond-regular.otf";
const char *FONT_MASON = "fonts/mason-regular.ttf";

const SDL_Color clrBK = {0, 0, 0, 0};
const SDL_Color clrDB = {7, 7, 58, 0};
const SDL_Color clrWH = {255, 255, 255, 0};
const SDL_Color clrLB = {10, 100, 173, 0};
const SDL_Color clrGN = {6, 74, 61, 0};
const SDL_Color clrLG = {79, 153, 55, 0};
const SDL_Color clrYW = {247, 173, 7, 0};
const SDL_Color clrLY = {247, 237, 7, 0};
const SDL_Color clrMG = {191, 0, 191, 0};
const SDL_Color clrLM = {255, 0, 255, 0};
const SDL_Color clrBR = {132, 85, 62, 0};

const Uint32 intBK = 0x00000000;
const Uint32 intRD = 0xFFFF0000;
const Uint32 intWH = 0xFFFFFFFF;
const Uint32 intGR = 0xFF7F7F7F;
const Uint32 intGN = 0xFF064A3E;
const Uint32 intLG = 0xFF4F9937;
const Uint32 intYW = 0xFFF7AD07;
const Uint32 intLY = 0xFFF7ED07;
const Uint32 intMG = 0xFFBF00BF;
const Uint32 intLM = 0xFFFF00FF;
const Uint32 intDB = 0xFF07073A;
const Uint32 intBR = 0xFF84553E;

// Beige in ARG format
const Uint32 intBE = 0xFFF5F5DC;
const Uint32 BE_50 = 0x7FF5F5DC;
const Uint32 BE_80 = 0xBBF5F5DC;
const Uint32 drkBE = 0xFFEBEBD2;

// Light Blue in ARG format
const Uint32 intLB = 0xFF0A64AD;

Uint8 R(Uint32 c) { return (Uint8)((c & 0xFF0000) >> 16); }
Uint8 G(Uint32 c) { return (Uint8)((c & 0x00FF00) >> 8); }
Uint8 B(Uint32 c) { return (Uint8)(c & 0x0000FF); }
Uint8 A(Uint32 c) { return (Uint8)(c >> 24); }

// Screen dimension constants
int SCREEN_WIDTH = 1024;
int SCREEN_HEIGHT = 768;

double Margin = 0.05;

int splashw = 0.30 * (SCREEN_WIDTH * (1.0 - 3.0 * Margin));
int startx = (SCREEN_WIDTH * Margin);
int starty = (SCREEN_HEIGHT * Margin);
int textx = startx * 2 + splashw;
int texty = starty;
int buttonw = 64;
int buttonh = 64;
int buttony = (int)(SCREEN_HEIGHT * (1.0 - Margin) - buttonh);

int button_space = 20;
int gridsize = buttonw + button_space;
int border_space = 8;
int border_pts = 4;
int arrow_size = 32;
int text_bounds = SCREEN_HEIGHT * (1.0 - Margin * 2.0) - buttonh - button_space * 2;
int textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

void Recompute()
{
    splashw = 0.30 * (SCREEN_WIDTH * (1.0 - 3.0 * Margin));

    startx = (SCREEN_WIDTH * Margin);
    starty = (SCREEN_HEIGHT * Margin);

    textx = startx * 2 + splashw;
    texty = starty;

    buttony = (int)(SCREEN_HEIGHT * (1.0 - Margin) - buttonh);
    gridsize = buttonw + button_space;

    textwidth = ((1.0 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);
    text_bounds = SCREEN_HEIGHT * (1.0 - Margin * 2.0) - buttonh - button_space * 2;
}

#endif
