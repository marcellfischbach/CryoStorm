#ifndef SDLKEYBOARD_HH
#define SDLKEYBOARD_HH

#include <csCore/input/iKeyboard.hh>
#include <SDL.h>

namespace cryo::launcher
{

class csSDLKeyboard : public iKeyboard
{
public:

  csSDLKeyboard();
    virtual ~csSDLKeyboard() { }

    void Update ();
    void Update (SDL_Scancode scanCode, bool down);

    virtual bool IsKeyDown(eKey key) const;
    virtual bool IsKeyUp(eKey key) const;

    virtual bool IsKeyPressed(eKey key) const;
    virtual bool IsKeyReleased(eKey key) const;

private:
    bool m_current[eK_COUNT];
    bool m_last[eK_COUNT];

};

}

#endif // SDLKEYBOARD_HH
