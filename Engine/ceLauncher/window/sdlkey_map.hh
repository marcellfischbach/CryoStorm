#ifndef SDLKEY_MAP_HH
#define SDLKEY_MAP_HH

#include <ceCore/input/key.hh>
#include <SDL.h>

namespace cryo::launcher
{

static Key Map(SDL_Scancode scanCode)
{
    switch (scanCode) {
    case SDL_SCANCODE_ESCAPE: return eK_Escape;
    case SDL_SCANCODE_A: return eK_A;
    case SDL_SCANCODE_B: return eK_B;
    case SDL_SCANCODE_C: return eK_C;
    case SDL_SCANCODE_D: return eK_D;
    case SDL_SCANCODE_E: return eK_E;
    case SDL_SCANCODE_F: return eK_F;
    case SDL_SCANCODE_G: return eK_G;
    case SDL_SCANCODE_H: return eK_H;
    case SDL_SCANCODE_I: return eK_I;
    case SDL_SCANCODE_J: return eK_J;
    case SDL_SCANCODE_K: return eK_K;
    case SDL_SCANCODE_L: return eK_L;
    case SDL_SCANCODE_M: return eK_M;
    case SDL_SCANCODE_N: return eK_N;
    case SDL_SCANCODE_O: return eK_O;
    case SDL_SCANCODE_P: return eK_P;
    case SDL_SCANCODE_Q: return eK_Q;
    case SDL_SCANCODE_R: return eK_R;
    case SDL_SCANCODE_S: return eK_S;
    case SDL_SCANCODE_T: return eK_T;
    case SDL_SCANCODE_U: return eK_U;
    case SDL_SCANCODE_V: return eK_V;
    case SDL_SCANCODE_W: return eK_W;
    case SDL_SCANCODE_X: return eK_X;
    case SDL_SCANCODE_Y: return eK_Y;
    case SDL_SCANCODE_Z: return eK_Z;
    case SDL_SCANCODE_1: return eK_1;
    case SDL_SCANCODE_2: return eK_2;
    case SDL_SCANCODE_3: return eK_3;
    case SDL_SCANCODE_4: return eK_4;
    case SDL_SCANCODE_5: return eK_5;
    case SDL_SCANCODE_6: return eK_6;
    case SDL_SCANCODE_7: return eK_7;
    case SDL_SCANCODE_8: return eK_8;
    case SDL_SCANCODE_9: return eK_9;
    case SDL_SCANCODE_0: return eK_0;

    case SDL_SCANCODE_LSHIFT: return eK_LeftShift;
    case SDL_SCANCODE_LALT: return eK_LeftAlt;
    case SDL_SCANCODE_LCTRL: return eK_LeftCtrl;
    case SDL_SCANCODE_LGUI: return eK_LeftWin;
    case SDL_SCANCODE_RSHIFT: return eK_RightShift;
    case SDL_SCANCODE_RALT: return eK_RightAlt;
    case SDL_SCANCODE_RCTRL: return eK_RightCtrl;
    case SDL_SCANCODE_RGUI: return eK_RightWin;

    case SDL_SCANCODE_SPACE: return eK_Space;

    case SDL_SCANCODE_LEFT: return eK_Left;
    case SDL_SCANCODE_RIGHT: return eK_Right;
    case SDL_SCANCODE_UP: return eK_Up;
    case SDL_SCANCODE_DOWN: return eK_Down;
    default:
        break;
    }
    return eK_Undefined;
}

}

#endif // SDLKEY_MAP_HH
