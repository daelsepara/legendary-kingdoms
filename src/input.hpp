#ifndef __INPUT__HPP__
#define __INPUT__HPP__

#include <vector>
#include <SDL.h>

#include "controls.hpp"

namespace Input
{
    int InitializeGamePads()
    {
        if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) != 1)
        {
            if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0)
            {
                std::cerr << "SDL could not initialize gamecontroller! SDL_Error: " << SDL_GetError() << std::endl;
            }
        }

        auto nJoysticks = SDL_NumJoysticks();
        auto numGamepads = 0;

        // Count how many controllers there are
        for (auto i = 0; i < nJoysticks; i++)
        {
            if (SDL_IsGameController(i))
            {
                numGamepads++;
            }
        }

        // If we have some controllers attached
        if (numGamepads > 0)
        {
            for (int i = 0; i < numGamepads; i++)
            {
                // Open the controller and add it to our list
                auto pad = SDL_GameControllerOpen(i);

                if (SDL_GameControllerGetAttached(pad) != 1)
                {
                    std::cerr << "Game pad not attached! SDL_Error: " << SDL_GetError() << std::endl;
                }
            }

            SDL_GameControllerEventState(SDL_ENABLE);
        }

        return numGamepads;
    }

    template <typename T>
    bool GetInput(SDL_Renderer *renderer, std::vector<T> choices, int &current, bool &selected, bool &scrollUp, bool &scrollDown, bool &hold)
    {
        // Update the renderer
        SDL_RenderPresent(renderer);

        SDL_Event result;

        auto quit = false;

        selected = false;
        scrollUp = false;
        scrollDown = false;

        auto sensitivity = 32000;

        auto start_ticks = SDL_GetTicks();

        while (1)
        {
            SDL_PollEvent(&result);

            if (result.type == SDL_QUIT)
            {
                quit = true;

                break;
            }
            else if (result.type == SDL_WINDOWEVENT)
            {
                if (result.window.event == SDL_WINDOWEVENT_RESTORED || result.window.event == SDL_WINDOWEVENT_MAXIMIZED || result.window.event == SDL_WINDOWEVENT_SHOWN || result.window.event == SDL_WINDOWEVENT_EXPOSED)
                {
                    SDL_RenderPresent(renderer);
                }
            }
            else if (result.type == SDL_CONTROLLERDEVICEADDED)
            {
                InitializeGamePads();

                continue;
            }
            else if (result.type == SDL_KEYDOWN)
            {
                if (result.key.keysym.sym == SDLK_PAGEUP)
                {
                    scrollUp = true;
                    scrollDown = false;

                    current = -1;

                    break;
                }
                else if (result.key.keysym.sym == SDLK_PAGEDOWN)
                {
                    scrollDown = true;
                    scrollUp = false;

                    current = -1;

                    break;
                }
                else if (current < 0)
                {
                    current = choices[0].ID;
                }
                else if (result.key.keysym.sym == SDLK_TAB || result.key.keysym.sym == SDLK_KP_TAB || result.key.keysym.sym == SDL_SCANCODE_KP_TAB)
                {
                    if (current < 0)
                    {
                        current = choices[0].ID;
                    }
                    else if (current == choices.size() - 1)
                    {
                        current = choices[0].ID;
                    }
                    else if (current >= 0 && current < choices.size() - 1)
                    {
                        current = choices[current + 1].ID;
                    }
                }
                else if (current >= 0 && current < choices.size())
                {
                    if (result.key.keysym.sym == SDLK_LEFT)
                    {
                        current = choices[current].Left;
                    }
                    else if (result.key.keysym.sym == SDLK_RIGHT)
                    {
                        current = choices[current].Right;
                    }
                    else if (result.key.keysym.sym == SDLK_UP)
                    {
                        current = choices[current].Up;
                    }
                    else if (result.key.keysym.sym == SDLK_DOWN)
                    {
                        current = choices[current].Down;
                    }
                    else if (result.key.keysym.sym == SDLK_KP_ENTER || result.key.keysym.sym == SDLK_RETURN || result.key.keysym.sym == SDLK_RETURN2)
                    {
                        selected = true;
                    }
                }

                break;
            }
            else if (result.type == SDL_CONTROLLERAXISMOTION)
            {
                if (result.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
                {
                    if (current < 0)
                    {
                        current = choices[0].ID;
                    }
                    else if (result.caxis.value < -sensitivity)
                    {
                        if (current >= 0 && current < choices.size())
                        {
                            current = choices[current].Left;
                        }
                    }
                    else if (result.caxis.value > sensitivity)
                    {
                        if (current >= 0 && current < choices.size())
                        {
                            current = choices[current].Right;
                        }
                    }

                    break;
                }
                else if (result.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
                {
                    if (current < 0)
                    {
                        current = choices[0].ID;
                    }
                    else if (result.caxis.value < -sensitivity)
                    {
                        if (current >= 0 && current < choices.size())
                        {
                            current = choices[current].Up;
                        }
                    }
                    else if (result.caxis.value > sensitivity)
                    {
                        if (current >= 0 && current < choices.size())
                        {
                            current = choices[current].Down;
                        }
                    }

                    break;
                }
            }
            else if (result.type == SDL_CONTROLLERBUTTONDOWN && result.cbutton.button == SDL_CONTROLLER_BUTTON_A)
            {
                hold = true;

                if (current >= 0 && current < choices.size() && (choices[current].Type == Control::Type::SCROLL_UP || choices[current].Type == Control::Type::SCROLL_DOWN))
                {
                    selected = true;

                    break;
                }
            }
            else if (result.type == SDL_CONTROLLERBUTTONUP)
            {
                selected = false;
                hold = false;

                if (current < 0)
                {
                    current = choices[0].ID;
                }
                else if (current >= 0 && current < choices.size())
                {
                    if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
                    {
                        current = choices[current].Left;
                    }
                    else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
                    {
                        current = choices[current].Right;
                    }
                    else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
                    {
                        current = choices[current].Up;
                    }
                    else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
                    {
                        current = choices[current].Down;
                    }
                    else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_A)
                    {
                        selected = true;
                    }
                }

                break;
            }
            else if (result.type == SDL_MOUSEMOTION)
            {
                hold = false;

                auto previous = current;

                for (auto i = 0; i < choices.size(); i++)
                {
                    if (result.motion.x >= choices[i].X && result.motion.x <= choices[i].X + choices[i].W - 1 && result.motion.y >= choices[i].Y && result.motion.y <= choices[i].Y + choices[i].H - 1)
                    {
                        current = choices[i].ID;

                        break;
                    }

                    current = -1;
                }

                if (previous != current)
                {
                    break;
                }
            }
            else if (result.type == SDL_MOUSEBUTTONDOWN && result.button.button == SDL_BUTTON_LEFT)
            {
                hold = true;

                if (current >= 0 && current < choices.size() && (choices[current].Type == Control::Type::SCROLL_UP || choices[current].Type == Control::Type::SCROLL_DOWN))
                {
                    selected = true;

                    break;
                }
            }
            else if (result.type == SDL_MOUSEBUTTONUP && result.button.button == SDL_BUTTON_LEFT)
            {
                if (hold)
                {
                    hold = false;

                    if (current >= 0 && current < choices.size())
                    {
                        selected = true;

                        break;
                    }
                }
            }
            else if (result.type == SDL_MOUSEWHEEL)
            {
                current = -1;

                if (result.wheel.y < 0 || result.wheel.x < 0)
                {
                    scrollUp = false;
                    scrollDown = true;
                }
                else
                {
                    scrollUp = true;
                    scrollDown = false;
                }

                break;
            }
            else if (hold)
            {
                if (current >= 0 && current < choices.size() && (choices[current].Type == Control::Type::SCROLL_UP || choices[current].Type == Control::Type::SCROLL_DOWN))
                {
                    SDL_Delay(50);

                    break;
                }
            }

            if (SDL_GetTicks() - start_ticks > 1000)
            {
                break;
            }
        }

        return quit;
    }

    void WaitForNext(SDL_Renderer *renderer)
    {
        SDL_Event result;

        while (1)
        {
            SDL_PollEvent(&result);

            if (result.type == SDL_QUIT)
            {
                break;
            }
            else if (result.type == SDL_WINDOWEVENT)
            {
                if (result.window.event == SDL_WINDOWEVENT_RESTORED || result.window.event == SDL_WINDOWEVENT_MAXIMIZED || result.window.event == SDL_WINDOWEVENT_SHOWN || result.window.event == SDL_WINDOWEVENT_EXPOSED)
                {
                    SDL_RenderPresent(renderer);
                }
            }
            else if (result.type == SDL_CONTROLLERDEVICEADDED)
            {
                InitializeGamePads();
            }
            else if (result.type == SDL_KEYDOWN)
            {
                if (result.key.keysym.sym == SDLK_KP_ENTER || result.key.keysym.sym == SDLK_RETURN || result.key.keysym.sym == SDLK_RETURN2)
                {
                    break;
                }
            }
            else if (result.type == SDL_CONTROLLERBUTTONUP)
            {
                if (result.cbutton.button == SDL_CONTROLLER_BUTTON_A)
                {
                    break;
                }
            }
            else if (result.type == SDL_MOUSEBUTTONUP && result.button.button == SDL_BUTTON_LEFT)
            {
                break;
            }
        }
    }
} // namespace Input
#endif
