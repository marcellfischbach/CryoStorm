
#pragma once

#ifdef SPICE_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)




#ifdef SPICE_SDL_WINDOW_EXPORT

#define SPICE_SDL_API __declspec(dllexport)
#else
#define SPICE_SDL_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define SPICE_SDL_API

#endif

