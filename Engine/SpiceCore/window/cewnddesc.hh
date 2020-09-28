#pragma once

#include <cecore/cetypes.hh>

struct ceWndDesc
{
	ceUInt16 width;
	ceUInt16 height;
	ceInt16 x;
	ceInt16 y;

	bool fullscreen;
};