#include <csLauncher/window/sdlkeyboard.hh>
#include <csLauncher/window/sdlkey_map.hh>

namespace cryo::launcher
{

SDLKeyboard::SDLKeyboard()
{
    for (int i=0; i < eKey::eK_COUNT; ++i)
    {
        m_current[i] = false;
        m_last[i] = false;
    }
}




bool SDLKeyboard::IsKeyDown(eKey key) const
{
    return m_current[key];
}

bool SDLKeyboard::IsKeyUp(eKey key) const
{
    return !m_current[key];
}

bool SDLKeyboard::IsKeyPressed(eKey key) const
{
    return !m_last[key] && m_current[key];
}

bool SDLKeyboard::IsKeyReleased(eKey key) const
{
    return m_last[key] && !m_current[key];
}


void SDLKeyboard::Update()
{
    for (int i=0; i < eKey::eK_COUNT; ++i)
    {
        m_last[i] = m_current[i];
    }
}

void SDLKeyboard::Update(SDL_Scancode scanCode, bool down)
{
    m_current[Map(scanCode)] = down;
}

}
