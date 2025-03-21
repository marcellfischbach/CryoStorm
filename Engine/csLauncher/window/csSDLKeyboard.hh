#ifndef SDLKEYBOARD_HH
#define SDLKEYBOARD_HH

#include <csCore/input/iKeyboard.hh>
#include <SDL.h>

namespace cs::launcher
{

CS_CLASS()
class csSDLKeyboard : public iKeyboard
{
  CS_CLASS_GEN_OBJECT;
public:

  csSDLKeyboard();
  ~csSDLKeyboard() override = default;

  void Update();
  void Update(SDL_Scancode scanCode, bool down);

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
