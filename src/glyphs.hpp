#ifndef __GLYPHS__HPP__
#define __GLYPHS__HPP__

#include <cstring>
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>

#include "constants.hpp"

namespace Glyphs
{
    // https://stackoverflow.com/questions/49333136/removing-html-tags-from-a-string-of-text
    // Probably a slow implementation
    void Sanitize(std::string &str)
    {
        for (auto a = 0; a < str.length(); a++)
        {
            if (str[a] == '<')
            {
                for (int b = a; b < str.length(); b++)
                {
                    if (str[b] == '>')
                    {
                        str.erase(a, (b - a + 1));

                        break;
                    }
                }
            }
        }
    }

    void SizeText(const char *text, TTF_Font *font, int width, int *h)
    {
        auto current_style = TTF_STYLE_NORMAL;

        std::string copy = text;

        auto x = 0;

        auto word = false;

        auto start = 0;

        auto lines = 1;

        bool isBold = false;

        bool isItalic = false;

        bool closeBold = false;

        bool closeItalic = false;

        auto space = 0;

        TTF_GlyphMetrics(font, ' ', NULL, NULL, NULL, NULL, &space);

        for (auto i = 0; i < copy.length(); i++)
        {
            current_style = TTF_STYLE_NORMAL;

            if (isBold)
            {
                current_style |= TTF_STYLE_BOLD;
            }

            if (isItalic)
            {
                current_style |= TTF_STYLE_ITALIC;
            }

            auto c = copy.at(i);

            if (c == '\r' || c == '\n' || c == '\t' || c == ' ')
            {
                auto subw = 0;

                if (word)
                {
                    auto sub = copy.substr(start, i - start);

                    Glyphs::Sanitize(sub);

                    TTF_SetFontStyle(font, current_style);

                    TTF_SizeText(font, sub.c_str(), &subw, NULL);

                    word = false;

                    if (x + subw > width)
                    {
                        x = subw;

                        lines += 1;
                    }
                    else
                    {
                        x += subw;
                    }

                    if (c == '\t' || c == ' ')
                    {
                        x += space;

                        if (x > width)
                        {
                            x = 0;

                            lines += 1;                    
                        }
                    }
                    else if (c == '\n' || c == '\r')
                    {
                        x = 0;

                        lines += 1;
                    }

                    if (closeBold)
                    {
                        isBold = false;

                        closeBold = false;
                    }

                    if (closeItalic)
                    {
                        isItalic = false;

                        closeItalic = false;
                    }
                }
                else if (c == ' ' || c == '\t')
                {
                    x += space;

                    if (x > width)
                    {
                        x = 0;

                        lines += 1;
                    }
                }
                else if (c == '\n' || c == '\r')
                {
                    x = 0;

                    lines += 1;
                }
            }
            else if (c == '<')
            {
                if (copy.substr(i, 3) == "<b>" || copy.substr(i, 3) == "<B>")
                {
                    if (!isBold)
                    {
                        isBold = true;

                        closeBold = false;
                    }

                    i += 2;
                }
                else if (copy.substr(i, 4) == "</b>" || copy.substr(i, 4) == "</B>")
                {
                    closeBold = true;

                    i += 3;
                }
                else if (copy.substr(i, 3) == "<i>" || copy.substr(i, 3) == "<I>")
                {
                    if (!isItalic)
                    {
                        isItalic = true;

                        closeItalic = false;
                    }

                    i += 2;
                }
                else if (copy.substr(i, 4) == "</i>" || copy.substr(i, 4) == "</I>")
                {
                    closeItalic = true;

                    i += 3;
                }
            }
            else if (!word)
            {
                word = true;

                start = i;
            }
        }

        // estimate last word
        if (word)
        {
            auto subw = 0;

            auto sub = copy.substr(start, copy.length() - start);

            Glyphs::Sanitize(sub);

            TTF_SetFontStyle(font, current_style);

            TTF_SizeText(font, sub.c_str(), &subw, NULL);

            word = false;

            if (x + subw > width)
            {
                lines += 1;
            }
        }

        if (h != NULL)
        {
            *h = TTF_FontLineSkip(font) * lines;
        }
    }

    // TODO: Use faster implementation, i.e. Render from Glyph Atlas with Kernering
    void RenderText(const char *text, TTF_Font *font, SDL_Color textColor, SDL_Surface *surface, int x, int y)
    {
        if (font)
        {
            SDL_Rect dst;

            dst.w = surface->w;
            dst.h = surface->h;
            dst.x = x;
            dst.y = y;

            auto text_surface = TTF_RenderText_Blended(font, text, textColor);

            if (text_surface)
            {
                SDL_BlitSurface(text_surface, NULL, surface, &dst);

                SDL_FreeSurface(text_surface);

                text_surface = NULL;
            }
        }
    }

    SDL_Surface *createText(const char *text, const char *ttf, int font_size, SDL_Color textColor, int wrap)
    {
        TTF_Init();

        auto font = TTF_OpenFont(ttf, font_size);

        if (font)
        {
            if (std::strcmp(ttf, FONT_DARK11) == 0)
            {
                TTF_SetFontKerning(font, 0);
            }
        }

        auto width = wrap;

        auto height = 0;

        if (font)
        {
            Glyphs::SizeText(text, font, wrap, &height);
        }

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        auto surface = SDL_CreateRGBSurface(0, wrap, height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
#else
        auto surface = SDL_CreateRGBSurface(0, wrap, height, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
#endif
        SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

        if (font)
        {
            auto current_style = TTF_STYLE_NORMAL;

            std::string copy = text;

            auto x = 0;

            auto y = 0;

            auto word = false;

            auto start = 0;

            auto lines = 0;

            bool isBold = false;

            bool isItalic = false;

            auto space = 0;

            auto skip = TTF_FontLineSkip(font);

            TTF_GlyphMetrics(font, ' ', NULL, NULL, NULL, NULL, &space);

            auto closeBold = false;

            auto closeItalic = false;

            for (auto i = 0; i < copy.length(); i++)
            {
                current_style = TTF_STYLE_NORMAL;

                if (isBold)
                {
                    current_style |= TTF_STYLE_BOLD;
                }

                if (isItalic)
                {
                    current_style |= TTF_STYLE_ITALIC;
                }

                auto c = copy.at(i);

                if (c == '\r' || c == '\n' || c == '\t' || c == ' ')
                {
                    if (word)
                    {
                        auto subw = 0;

                        auto sub = copy.substr(start, i - start);

                        Glyphs::Sanitize(sub);

                        TTF_SetFontStyle(font, current_style);

                        TTF_SizeText(font, sub.c_str(), &subw, NULL);

                        word = false;

                        if (x + subw > width)
                        {
                            x = subw;

                            lines += 1;

                            y = lines * skip;

                            Glyphs::RenderText(sub.c_str(), font, textColor, surface, 0, y);
                        }
                        else
                        {
                            Glyphs::RenderText(sub.c_str(), font, textColor, surface, x, y);

                            x += subw;
                        }

                        if (c == '\t' || c == ' ')
                        {
                            x += space;

                            if (x > width)
                            {
                                x = 0;

                                lines += 1;                    
                            }
                        }
                        else if (c == '\n' || c == '\r')
                        {
                            x = 0;

                            lines += 1;
                        }

                        y = lines * skip;

                        if (closeBold)
                        {
                            isBold = false;

                            closeBold = false;
                        }

                        if (closeItalic)
                        {
                            isItalic = false;

                            closeItalic = false;
                        }
                    }
                    else if (c == '\t' || c == ' ')
                    {
                        x += space;

                        if (x > width)
                        {
                            x = 0;

                            lines += 1;     
                        }
                    }
                    else if (c == '\n' || c == '\r')
                    {
                        x = 0;

                        lines += 1;
                    }

                    y = lines * skip;
                }
                else if (c == '<')
                {
                    if (copy.substr(i, 3) == "<b>" || copy.substr(i, 3) == "<B>")
                    {
                        if (!isBold)
                        {
                            isBold = true;

                            closeBold = false;
                        }

                        i += 2;
                    }
                    else if (copy.substr(i, 4) == "</b>" || copy.substr(i, 4) == "</B>")
                    {
                        closeBold = true;

                        i += 3;
                    }
                    else if (copy.substr(i, 3) == "<i>" || copy.substr(i, 3) == "<I>")
                    {
                        if (!isItalic)
                        {
                            isItalic = true;

                            closeItalic = false;
                        }

                        i += 2;
                    }
                    else if (copy.substr(i, 4) == "</i>" || copy.substr(i, 4) == "</I>")
                    {
                        closeItalic = true;

                        i += 3;
                    }
                }
                else if (!word)
                {
                    word = true;

                    start = i;
                }
            }

            if (word)
            {
                auto subw = 0;

                // render last word
                auto sub = copy.substr(start, copy.length() - start);

                Glyphs::Sanitize(sub);

                TTF_SetFontStyle(font, current_style);

                TTF_SizeText(font, sub.c_str(), &subw, NULL);

                word = false;

                if (x + subw > width)
                {
                    x = subw;

                    x += space;

                    lines += 1;

                    y = lines * skip;

                    Glyphs::RenderText(sub.c_str(), font, textColor, surface, 0, y);
                }
                else
                {
                    Glyphs::RenderText(sub.c_str(), font, textColor, surface, x, y);
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();

        return surface;
    }
}
#endif