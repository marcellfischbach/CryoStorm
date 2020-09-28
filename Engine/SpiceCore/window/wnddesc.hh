#pragma once

#include <SpiceCore/types.hh>


namespace Spice {


struct WndDesc
{
  UInt16 width;
  UInt16 height;
  Int16 x;
  Int16 y;

	bool fullscreen;
};

}

