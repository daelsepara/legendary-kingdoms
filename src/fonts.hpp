#ifndef __FONTS__HPP__
#define __FONTS__HPP__

#include <SDL_ttf.h>

#include "constants.hpp"

namespace Fonts
{
    TTF_Font *Caption = NULL;
    TTF_Font *Dark11 = NULL;
    TTF_Font *Garamond22 = NULL;
    TTF_Font *Garamond24 = NULL;
    TTF_Font *Garamond28 = NULL;
    TTF_Font *Garamond32 = NULL;
    TTF_Font *Mason22 = NULL;
    TTF_Font *Mason24 = NULL;
    TTF_Font *Mason28 = NULL;
    TTF_Font *Mason32 = NULL;

    void Initialize()
    {
        Caption = TTF_OpenFont(FONT_GARAMOND, 22);
        Dark11 = TTF_OpenFont(FONT_DARK11, 32);
        Garamond22 = TTF_OpenFont(FONT_GARAMOND, 22);
        Garamond24 = TTF_OpenFont(FONT_GARAMOND, 24);
        Garamond28 = TTF_OpenFont(FONT_GARAMOND, 28);
        Garamond32 = TTF_OpenFont(FONT_GARAMOND, 32);
        Mason22 = TTF_OpenFont(FONT_MASON, 22);
        Mason24 = TTF_OpenFont(FONT_MASON, 24);
        Mason28 = TTF_OpenFont(FONT_MASON, 28);
        Mason32 = TTF_OpenFont(FONT_MASON, 32);

        TTF_SetFontKerning(Dark11, 0);
    }

    void Free()
    {
        if (Caption)
        {
            TTF_CloseFont(Caption);

            Caption = NULL;
        }

        if (Dark11)
        {
            TTF_CloseFont(Dark11);

            Dark11 = NULL;
        }

        if (Garamond22)
        {
            TTF_CloseFont(Garamond22);

            Garamond22 = NULL;
        }

        if (Garamond24)
        {
            TTF_CloseFont(Garamond24);

            Garamond24 = NULL;
        }

        if (Garamond28)
        {
            TTF_CloseFont(Garamond28);

            Garamond28 = NULL;
        }

        if (Garamond32)
        {
            TTF_CloseFont(Garamond32);

            Garamond32 = NULL;
        }

        if (Mason22)
        {
            TTF_CloseFont(Mason22);

            Mason22 = NULL;
        }

        if (Mason24)
        {
            TTF_CloseFont(Mason24);

            Mason24 = NULL;
        }

        if (Mason28)
        {
            TTF_CloseFont(Mason28);

            Mason28 = NULL;
        }

        if (Mason32)
        {
            TTF_CloseFont(Mason32);

            Mason32 = NULL;
        }
    }
}
#endif
