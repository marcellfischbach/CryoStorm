
#pragma once

#ifdef CE_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)




#ifdef CE_JAVA_EXPORT
#define CE_JAVA_API __declspec(dllexport)
#else
#define CE_JAVA_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define CE_CORE_API

#endif

