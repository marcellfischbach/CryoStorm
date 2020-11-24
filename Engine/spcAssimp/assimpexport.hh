
#pragma once

#ifdef SPC_WIN32

#pragma warning( disable : 4530)
#pragma warning( disable : 4577)




#ifdef SPC_ASSIMP_EXPORT
#define SPC_ASSIMP_API __declspec(dllexport)
#else
#define SPC_ASSIMP_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#else

#define SPC_ASSIMP_API

#endif

