#include <ceLauncher/window/sdlkeyboard.hh>
#include <ceLauncher/window/sdlkey_map.hh>

namespace ce
{

SDLKeyboard::SDLKeyboard()
{
    for (int i=0; i<Key::eK_COUNT; ++i)
    {
        m_current[i] = false;
        m_last[i] = false;
    }
}




bool SDLKeyboard::IsKeyDown(Key key) const
{
    return m_current[key];
}

bool SDLKeyboard::IsKeyUp(Key key) const
{
    return !m_current[key];
}

bool SDLKeyboard::IsKeyPressed(Key key) const
{
    return !m_last[key] && m_current[key];
}

bool SDLKeyboard::IsKeyReleased(Key key) const
{
    return m_last[key] && !m_current[key];
}


void SDLKeyboard::Update()
{
    for (int i=0; i<Key::eK_COUNT; ++i)
    {
        m_last[i] = m_current[i];
    }
}

void SDLKeyboard::Update(SDL_Scancode scanCode, bool down)
{
    m_current[Map(scanCode)] = down;
}

}
