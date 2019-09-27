//----------------------------------------------------------------------------------
// INGTrace.h : Implements the Debugging trace utilities.
//----------------------------------------------------------------------------------
#define ING_EXPORTS
#include <INGCommon.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string>

#ifdef _DEBUG
#define DEBUG_WRITE_BUFFER_SIZE 256

void ING_DebugWrite(const wchar_t* msg, ...)
{
	// Keep two extra characters for the line terminator
	// and the null terminator.
	wchar_t buffer[DEBUG_WRITE_BUFFER_SIZE + 2] = {};
	va_list args;

	va_start(args, msg);
	//swprintf(buffer, DEBUG_WRITE_BUFFER_SIZE, msg, args);
	_vsnwprintf_s(buffer, _countof(buffer) - 1, _TRUNCATE, msg, args);
	va_end(args);
#ifdef _WIN32
	OutputDebugStringW(buffer);
#else
	// ToDo: Other platform debug message output.
#endif
}
#endif

#ifdef _WIN32
// Utility function only used by Win32
ING::result_code_t ING::HRESULT_TO_RESULT_CODE(const HRESULT hr)
{
	result_code_t result = result_failed;   // Generic failure

	switch (hr)
	{
	case S_OK:
	case S_FALSE:
		result = result_succeeded;
		break;
	case E_OUTOFMEMORY:
		result = result_out_of_memory;
		break;
	case E_INVALIDARG:
		result = result_invalid_parameter;
		break;
	case E_POINTER:
		result = result_invalid_pointer;
		break;
	case HRESULT_FROM_WIN32(ERROR_NOT_FOUND):
		result = result_not_found;
		break;
	case E_NOTIMPL:
		result = result_not_implemented;
		break;
	case DXGI_ERROR_DEVICE_REMOVED:
	case DXGI_ERROR_DEVICE_RESET:
		result = result_rendering_device_removed;
		break;
	default:
		break;
	}

	return result;
}
#endif