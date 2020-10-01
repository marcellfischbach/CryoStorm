
#pragma once

#ifdef SPC_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)




#ifdef SPC_OGL_EXPORT
#define SPC_OGL_API __declspec(dllexport)
#else
#define SPC_OGL_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define SPC_OGL_API

#endif

