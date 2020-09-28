
#include <SpiceLauncher/window/sdlkeyboard.hh>

#include <iostream>
#include <SDL.h>

Spice::SDLKeyboard keyboard;


void UpdateEvents ()
{
    keyboard.Update();
    SDL_Event evt;
    while (SDL_PollEvent(&evt))
    {
        switch (evt.type)
        {
        case SDL_KEYDOWN:
            keyboard.Update(evt.key.keysym.scancode, true);
            break;
        case SDL_KEYUP:
            keyboard.Update(evt.key.keysym.scancode, false);
            break;
        }
    }

}

int main(int argc, char** argv)
{

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);


    SDL_Window* wnd = SDL_CreateWindow("Spice", 25, 25, 1024, 768, SDL_WINDOW_OPENGL);
    SDL_ShowWindow(wnd);


    while (true)
    {
        UpdateEvents();

        if (keyboard.IsKeyPressed(Spice::eKey::eK_Escape))
        {
            break;
        }

    }


    return 0;
}
