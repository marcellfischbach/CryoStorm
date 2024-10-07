
#pragma once

#include <csCore/input/eMouseButton.hh>
#include <csCore/csTypes.hh>

namespace cs::launcher
{

static eMouseButton Map(uint8_t button)
{
  switch (button) {
  case 1: return eMouseButton::eMB_Left;
  case 2: return eMouseButton::eMB_Middle;
  case 3: return eMouseButton::eMB_Right;
  case 4: return eMouseButton::eMB_Alt3;
  case 5: return eMouseButton::eMB_Alt4;
  case 6: return eMouseButton::eMB_Alt5;
  case 7: return eMouseButton::eMB_Alt6;
  case 8: return eMouseButton::eMB_Alt7;
  default: break;
  }
  return eMouseButton::eMB_Undefined;
}

}

