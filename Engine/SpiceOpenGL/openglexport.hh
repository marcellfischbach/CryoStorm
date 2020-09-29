
#pragma once

#ifdef SPICE_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)




#ifdef SPICE_OGL_EXPORT
#define SPICE_OGL_API __declspec(dllexport)
#else
#define SPICE_OGL_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define SPICE_OGL_API

#endif

