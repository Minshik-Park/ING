//----------------------------------------------------------------------------------
// Trace.h : Defines the Debugging trace utilities.
//----------------------------------------------------------------------------------
#pragma once
#include <Types.h>
#include <string>

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef _DEBUG
	ING_API void ING_DebugWrite(const wchar_t* msg, ...);
#else
#define ING_DebugWrite(x, ...)
#endif

#ifdef __cplusplus
}
#endif

namespace ING
{
	class ScopeTrace
	{
	public:
		ScopeTrace(const char* pFunction, result_code_t* pResult = nullptr) :
			m_function(pFunction),
			m_pResult(pResult)
		{
			ING_DebugWrite(L"%S+++\n", m_function.c_str());
		}
#ifdef _WIN32
		ScopeTrace(const char* pFunction, HRESULT* pHr) :
			m_function(pFunction),
			m_pResult(nullptr),
			m_pHr(pHr)
		{
			ING_DebugWrite(L"%S+++\n", m_function.c_str());
		}
#endif

		~ScopeTrace()
		{
			if (m_pResult)
			{
				ING_DebugWrite(L"%S--- (%d)\n", m_function.c_str(), *m_pResult);
			}
#ifdef _WIN32
			else if (m_pHr)
			{
				ING_DebugWrite(L"%S--- (0x%08x)\n", m_function.c_str(), *m_pHr);
			}
#endif
			else
			{
				ING_DebugWrite(L"%S---\n", m_function.c_str());
			}
		}

	private:
		std::string m_function;
		result_code_t* m_pResult = nullptr;
#ifdef _WIN32
		HRESULT* m_pHr = nullptr;
#endif
	};

#ifdef _WIN32
	ING_API result_code_t HRESULT_TO_RESULT_CODE(const HRESULT hr);
#endif
}

//----------------------------------------------------------------------------------
// Scope tracing Macros
//----------------------------------------------------------------------------------
#ifdef _DEBUG
#define TraceScopeResult(pR) ING::ScopeTrace st(__FUNCTION__, pR);
#define TraceScopeVoid()       ING::ScopeTrace st(__FUNCTION__);
#ifdef _WIN32
#define TraceScopeHR(pHr)    ING::ScopeTrace st(__FUNCTION__, pHr);
#endif
#else
#define TraceScopeResult(f, pR)
#define TraceScopeVoid(f)
#ifdef _WIN32
#define TraceScopeHR(f, pHr)
#endif
#endif
