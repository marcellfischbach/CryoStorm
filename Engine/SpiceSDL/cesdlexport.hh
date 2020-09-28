
#pragma once

#ifdef CE_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)




#ifdef CEW_EXPORT
#define CEW_API __declspec(dllexport)
#else
#define CEW_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define CEW_API

#endif

