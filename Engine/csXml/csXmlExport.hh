#pragma once


#ifdef CS_WIN32

#ifdef CS_XML_EXPORT
#define CS_XML_API __declspec(dllexport)
#else
#define CS_XML_API __declspec(dllimport)
#endif

#else

#define CS_XML_API

#endif