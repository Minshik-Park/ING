//----------------------------------------------------------------------------------
// Common.h : Defines the common definitions.
//----------------------------------------------------------------------------------
#pragma once
#include <Configuration.h>

//----------------------------------------------------------------------------------
// Platform include headers
//----------------------------------------------------------------------------------
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>
#include <Unknwn.h>
#endif

//----------------------------------------------------------------------------------
// ING Type definitions
//----------------------------------------------------------------------------------
#include <Types.h>

//----------------------------------------------------------------------------------
// Debugging Utilities
//----------------------------------------------------------------------------------
#include <Trace.h>

//----------------------------------------------------------------------------------
// Utility MACRO definitions
//----------------------------------------------------------------------------------
#define RETURN_IF_FALSE(cond, resultCode) \
if(!(cond)) \
{ \
    ING::result_code_t _result = (resultCode); \
    ING_DebugWrite(L"%S (%d) Failed - %d\n", __FUNCTION__, __LINE__, _result); \
    return _result; \
}

#define RETURN_IF_FAILED(opResult) \
{ \
    ING::result_code_t _result = (opResult); \
    if(_result != ING::result_code_t::succeeded) \
    { \
        ING_DebugWrite(L"%S (%d) Failed - %d\n", __FUNCTION__, __LINE__, _result); \
        return _result; \
    } \
}

#define GOTO_IF_FALSE(cond, resultCode, label) \
if(!(cond)) \
{ \
    result = (resultCode); \
    ING_DebugWrite(L"%S (%d) Failed - %d\n", __FUNCTION__, __LINE__, resultCode); \
    goto label; \
}

#define GOTO_IF_FAILED(opResult, label) \
if((result = (opResult)) != ING::result_code_t::succeeded) \
{ \
    ING_DebugWrite(L"%S (%d) Failed - %d\n", __FUNCTION__, __LINE__, result); \
    goto label; \
}

#ifdef _WIN32
#define RETURN_HR_IF_FALSE(cond, hrResult) \
if(!(cond)) \
{ \
    hr = (hrResult); \
    ING_DebugWrite(L"%S (%d) Failed - 0x%08x\n", __FUNCTION__, __LINE__, hr); \
    return hr; \
}

#define RETURN_IF_HR_FAILED(hrResult) \
if(FAILED(hr = (hrResult))) \
{ \
    ING_DebugWrite(L"%S (%d) Failed - 0x%08x\n", __FUNCTION__, __LINE__, hr); \
    return HRESULT_TO_RESULT_CODE(hr); \
}

#define RETURN_HR_IF_HR_FAILED(hrResult) \
if(FAILED(hr = (hrResult))) \
{ \
    ING_DebugWrite(L"%S (%d) Failed - 0x%08x\n", __FUNCTION__, __LINE__, hr); \
    return hr; \
}

#define GOTO_IF_HR_FAILED(hrResult, label) \
if(FAILED(hr = (hrResult))) \
{ \
    ING_DebugWrite(L"%S (%d) Failed - 0x%08x\n", __FUNCTION__, __LINE__, hr); \
    goto label; \
}

#define GOTO_IF_FALSE_HR(cond, hrResult, label) \
if(!(cond)) \
{ \
    hr = (hrResult); \
    ING_DebugWrite(L"%S (%d) Failed - 0x%08x\n", __FUNCTION__, __LINE__, hr); \
    goto label; \
}

#endif