
#pragma once

#include <ceCore/input/button.hh>
#include <ceCore/types.hh>

namespace ce::launcher
{

static eMouseButton Map(uint8_t button)
{
  switch (button) {
  case 1: return eMouseButton::Left;
  case 2: return eMouseButton::Middle;
  case 3: return eMouseButton::Right;
  case 4: return eMouseButton::Alt3;
  case 5: return eMouseButton::Alt4;
  case 6: return eMouseButton::Alt5;
  case 7: return eMouseButton::Alt6;
  case 8: return eMouseButton::Alt7;
  default: break;
  }
  return eMouseButton::Undefined;
}

}

