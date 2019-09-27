//----------------------------------------------------------------------------------
// INGConfiguration.h : Defines any build configurations.
//----------------------------------------------------------------------------------
#pragma once

#ifdef ING_EXPORTS
#define ING_API __declspec(dllexport)
#else
#define ING_API __declspec(dllimport)
#endif

#ifdef _WIN32
// Windows specific configurations
#else
#endif
