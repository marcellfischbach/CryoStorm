
#pragma once

#include <ceCore/input/button.hh>
#include <ceCore/types.hh>

namespace ce
{

static MouseButton Map(uint8_t button)
{
  switch (button) {
  case 1: return eMB_Left;
  case 2: return eMB_Middle;
  case 3: return eMB_Right;
  case 4: return eMB_Alt3;
  case 5: return eMB_Alt4;
  case 6: return eMB_Alt5;
  case 7: return eMB_Alt6;
  case 8: return eMB_Alt7;
  default: break;
  }
  return eMB_Undefined;
}

}

