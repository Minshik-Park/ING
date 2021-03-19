#pragma once
#include "ResultCode.h"

namespace ING
{
    namespace Debug
    {
        const int DEBUG_PRINT_BUFFER_SIZE = 256;

        inline void DebugPrint(const char* msg) { OutputDebugStringA(msg); }
        inline void DebugPrint(const wchar_t* msg) { OutputDebugString(msg); }

        void DebugPrintf(const char* msg, ...);
        void DebugPrintf(const wchar_t* msg, ...);
    }
}
