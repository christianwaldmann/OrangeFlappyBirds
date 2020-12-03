#pragma once


#ifdef OG_PLATFORM_WINDOWS
#ifdef OG_BUILD_DLL
#define ORANGE_API __declspec(dllexport)
#else
#define ORANGE_API __declspec(dllimport)
#endif
#else
#error Orange only supports Windows!
#endif


#define BIT(x) (1 << x)