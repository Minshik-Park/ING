#include "pch.h"
#include "DebugUtility.h"

namespace ING
{
    namespace Debug
    {
        void DebugPrintf(const char* msg, ...)
        {
            // Keep an extra character for the null terminator.
            char buffer[DEBUG_PRINT_BUFFER_SIZE + 1] = {};
            va_list args;

            va_start(args, msg);
            _vsnprintf_s(buffer, _countof(buffer) - 1, _TRUNCATE, msg, args);
            va_end(args);

            DebugPrint(buffer);
        }

        void DebugPrintf(const wchar_t* msg, ...)
        {
            // Keep an extra character for the null terminator.
            wchar_t buffer[DEBUG_PRINT_BUFFER_SIZE + 1] = {};
            va_list args;

            va_start(args, msg);
            _vsnwprintf_s(buffer, _countof(buffer) - 1, _TRUNCATE, msg, args);
            va_end(args);

            DebugPrint(buffer);
        }
    }
}