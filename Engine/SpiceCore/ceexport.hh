
#pragma once

#ifdef CE_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)




#ifdef CEC_EXPORT
#define CEC_API __declspec(dllexport)
#else
#define CEC_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define CEC_API

#endif

