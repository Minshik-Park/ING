//----------------------------------------------------------------------------------
// Configuration.h : Defines any build configurations.
//----------------------------------------------------------------------------------
#pragma once

#ifdef ING_EXPORTS
#define ING_API __declspec(dllexport)
#define ING_TPL
#else
#define ING_API __declspec(dllimport)
#define ING_TPL extern
#endif

#ifdef _WIN32
// Windows specific configurations
#else
#endif
