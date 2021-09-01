#ifndef __SOUND__HPP__
#define __SOUND__HPP__

#include <SDL_mixer.h>

#if defined(_WIN32)
#undef ERROR
#endif

namespace Sound
{
    enum class Type
    {
        NONE = 0,
        BUTTON_CLICK,
        DICE_ROLL,
        SUCCESS,
        FAIL,
        ERROR
    };

    Mix_Chunk *gButtonClick = NULL;
    Mix_Chunk *gDiceRoll = NULL;
    Mix_Chunk *gSuccess = NULL;
    Mix_Chunk *gFail = NULL;
    Mix_Chunk *gError = NULL;
    
    void Initialize()
    {
        gButtonClick = Mix_LoadWAV("sounds/button_click.wav");
        gDiceRoll = Mix_LoadWAV("sounds/dice_roll.wav");
        gSuccess = Mix_LoadWAV("sounds/success.wav");
        gFail = Mix_LoadWAV("sounds/fail.wav");
        gError = Mix_LoadWAV("sounds/fail.wav");
    }

    void Play(Sound::Type sound)
    {
        if (sound == Sound::Type::BUTTON_CLICK && gButtonClick)
        {
            Mix_PlayChannel(-1, gButtonClick, 0);
        }
        else if (sound == Sound::Type::DICE_ROLL && gDiceRoll)
        {
            Mix_PlayChannel(-1, gDiceRoll, 0);
        }
        else if (sound == Sound::Type::SUCCESS && gSuccess)
        {
            Mix_PlayChannel(-1, gSuccess, 0);
        }
        else if (sound == Sound::Type::FAIL && gFail)
        {
            Mix_PlayChannel(-1, gFail, 0);
        }
        else if (sound == Sound::Type::ERROR && gError)
        {
            Mix_PlayChannel(-1, gError, 0);
        }
    }

    void Free()
    {
        if (gError)
        {
            Mix_FreeChunk(gError);

            gError = NULL;
        }

        if (gFail)
        {
            Mix_FreeChunk(gFail);

            gFail = NULL;
        }

        if (gSuccess)
        {
            Mix_FreeChunk(gSuccess);

            gSuccess = NULL;
        }

        if (gDiceRoll)
        {
            Mix_FreeChunk(gDiceRoll);

            gDiceRoll = NULL;
        }

        if (gButtonClick)
        {
            Mix_FreeChunk(gButtonClick);

            gButtonClick = NULL;
        }
    }
}
#endif
