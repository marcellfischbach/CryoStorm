#include <csLauncher/window/csSDLKeyboard.hh>
#include <csLauncher/window/csSDLKeyMap.hh>

namespace cs::launcher
{

csSDLKeyboard::csSDLKeyboard()
{
    for (int i=0; i < eKey::eK_COUNT; ++i)
    {
        m_current[i] = false;
        m_last[i] = false;
    }
}




bool csSDLKeyboard::IsKeyDown(eKey key) const
{
    return m_current[key];
}

bool csSDLKeyboard::IsKeyUp(eKey key) const
{
    return !m_current[key];
}

bool csSDLKeyboard::IsKeyPressed(eKey key) const
{
    return !m_last[key] && m_current[key];
}

bool csSDLKeyboard::IsKeyReleased(eKey key) const
{
    return m_last[key] && !m_current[key];
}


void csSDLKeyboard::Update()
{
    for (int i=0; i < eKey::eK_COUNT; ++i)
    {
        m_last[i] = m_current[i];
    }
}

void csSDLKeyboard::Update(SDL_Scancode scanCode, bool down)
{
    m_current[Map(scanCode)] = down;
}

}
