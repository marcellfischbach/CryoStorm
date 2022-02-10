
#pragma once

#ifdef CE_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)




#ifdef CE_IMGLOADER_EXPORT
#define CE_IMGLOADER_API __declspec(dllexport)
#else
#define CE_IMGLOADER_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define CE_IMGLOADER_API

#endif

