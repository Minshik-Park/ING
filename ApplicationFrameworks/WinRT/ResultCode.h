#pragma once
#ifdef _WIN32
#include <Windows.h>
#endif
#ifdef _DEBUG
#include <DebugUtility.h>
#endif

namespace ING
{
    /// <summary>
    /// ING Error codes
    /// </summary>
    enum class ErrorCode
    {
        EOutOfMemory        = (int)(0x8007000EL),   // E_OUTOFMEMORY
        EInvalidArgs        = (int)(0x80070057L),   // E_INVALIDARG
        EInvalidPointer     = (int)(0x80004003L),   // E_POINTER
        EAlreadyInitialized = (int)(0x800704DFL),   // ERROR_ALREADY_INITIALIZED
        ENotFound           = (int)(0x80070490L),   // ERROR_NOT_FOUND
        Succeeded           = 0,
    };

    /// <summary>
    /// 
    /// </summary>
    class ResultCode
    {
    public:
        ResultCode(const char *pczFunction) : m_strFunction(pczFunction)
        {
#ifdef _DEBUG
            Debug::DebugPrintf("<%s>\n", m_strFunction.c_str());
#endif
        }
        ResultCode(const char* pczFunction, ErrorCode err) : m_strFunction(pczFunction), m_result(err)
        {
#ifdef _DEBUG
            Debug::DebugPrintf("<%s>\n", m_strFunction.c_str());
#endif
        }
        virtual ~ResultCode()
        {
#ifdef _DEBUG
            Debug::DebugPrintf("</%s ErrorCode=0x%08x>\n", m_strFunction.c_str(), m_result);
#endif
        }

        ErrorCode Result() { return m_result; }

        void operator=(const ErrorCode& err) { m_result = err; }
#ifdef _WIN32
        void operator=(const HRESULT hr)
        {
            m_result = static_cast<ErrorCode>(hr);
        }
#endif
        bool IsSucceeded() { return m_result >= ErrorCode::Succeeded; }
        bool IsFailed()    { return m_result <  ErrorCode::Succeeded; }

    private:
        ErrorCode m_result = ErrorCode::Succeeded;
        std::string m_strFunction;
    };
}

// Utility Macros.
#define IFNULL_GOTO(_ptr, _err, _label)     if ((_ptr) == nullptr) { rc = _err; goto _label; }
#define IFNULL_EXIT(_ptr, _err)             if ((_ptr) == nullptr) { rc = _err; return rc.Result(); }
#define IFFALSE_GOTO(_cond, _err, _label)   if (!(_cond)) { rc = _err; goto _label; }
#define IFFALSE_EXIT(_cond, _err)           if (!(_cond)) { rc = _err; return rc.Result(); }
#define IFFAILED_GOTO(_result, _label)      rc = _result; if (rc.IsFailed()) { goto _label; }
#define IFFAILED_EXIT(_result)              rc = _result; if (rc.IsFailed()) { return rc.Result(); }

#ifdef _DEBUG
#define FunctionTrace()     ING::ResultCode __rc(__FUNCTION__);
#else
#define FunctionTrace()
#endif
