#pragma once


#include <spcCore/input/imouse.hh>
#include <SDL.h>

namespace spc
{


class SDLMouse : public iMouse
{
public:
  SDLMouse();
  virtual ~SDLMouse() { }

  void Update();
  void Update(uint8_t button, bool down);
  void Update(Int32 wheel, Int32 wheelHorizontal);
  void Update(Int16 x, Int16 y, Int16 xrel, Int16 yrel);

  virtual Int16 GetX() const;
  virtual Int16 GetY() const;

  virtual Int16 GetDeltaX() const;
  virtual Int16 GetDeltaY() const;
  virtual Int16 GetWheel() const;
  virtual Int16 GetWheelHorizontal() const;


  virtual bool IsButtonDown(MouseButton mouseButton) const;
  virtual bool IsButtonUp(MouseButton mouseButton) const;
  virtual bool IsButtonPressed(MouseButton mouseButton) const;
  virtual bool IsButtonReleased(MouseButton mouseButton) const;
private:

  Int16 m_x;
  Int16 m_y;
  Int16 m_relX;
  Int16 m_relY;
  Int16 m_wheel;
  Int16 m_wheelHorizontal;
  bool m_current[eMB_COUNT];
  bool m_last[eMB_COUNT];

};

}