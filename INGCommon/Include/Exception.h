//----------------------------------------------------------------------------------
// Exception.h : Definitions of ING Exception classes.
//----------------------------------------------------------------------------------
#pragma once
#include <Common.h>
#include <exception>
#include <system_error>
#include <string>

namespace ING
{
    ING_API const char* ResultCodeToString(result_code_t result);

    class Exception : public std::exception
    {
    public:
        Exception(result_code_t result) :
            std::exception(ResultCodeToString(result)),
            m_result(result)
        {
        }

        Exception(result_code_t result, const char* pMessage) :
            std::exception(pMessage),
            m_result(result)
        {
        }

        Exception(result_code_t result, const char* pszFunction, const int line) :
            std::exception(ResultCodeToString(result)),
            m_result(result),
            m_function(pszFunction),
            m_line(line)
        {
        }

        Exception(result_code_t result, const char* pszFunction, const int line, const char* pMessage) :
            std::exception(pMessage),
            m_result(result),
            m_function(pszFunction),
            m_line(line)
        {
        }

        virtual ~Exception()
        {
        }

        const result_code_t Result()
        {
            return m_result;
        }

        const char* Function()
        {
            return m_function.empty() ? nullptr : m_function.c_str();
        }

        const int Line()
        {
            return m_line;
        }

    protected:
        result_code_t m_result = result_code_t::succeeded;
        std::string m_function;
        int m_line = -1;
    };

#ifdef _WIN32
    class ExceptionHr : public Exception
    {
    public:
        ExceptionHr(HRESULT hr) :
            Exception(HRESULT_TO_RESULT_CODE(hr), std::system_category().message(hr).c_str()),
            m_hr(hr)
        {
        }

        ExceptionHr(HRESULT hr, const char* pMessage) :
            Exception(HRESULT_TO_RESULT_CODE(hr), pMessage),
            m_hr(hr)
        {
        }

        ExceptionHr(HRESULT hr, const char* pszFunction, const int line) :
            Exception(HRESULT_TO_RESULT_CODE(hr), pszFunction, line, std::system_category().message(hr).c_str()),
            m_hr(hr)
        {
        }

        ExceptionHr(HRESULT hr, const char* pszFunction, const int line, const char* pMessage) :
            Exception(HRESULT_TO_RESULT_CODE(hr), pszFunction, line, pMessage),
            m_hr(hr)
        {
        }

        virtual ~ExceptionHr()
        {
        }

        const HRESULT HResult()
        {
            return m_hr;
        }

    private:
        HRESULT m_hr;
    };
#endif
}

//----------------------------------------------------------------------------------
// Utility MACRO definitions
//----------------------------------------------------------------------------------
#define THROW_IF_FALSE(cond, resultCode) \
if(!(cond)) \
{ \
    throw ING::Exception(resultCode, __FUNCTION__, __LINE__); \
}

#define THROW_IF_FAILED(opResult) \
if((ING::result_code_t _result = (opResult)) != ING::result_code_t::succeeded) \
{ \
    throw ING::Exception(_result, __FUNCTION__, __LINE__); \
}

#ifdef _WIN32
#define THROW_HR_IF_FALSE(cond, hrResult) \
if(!(cond)) \
{ \
    throw ING::ExceptionHr(hrResult, __FUNCTION__, __LINE__); \
}

#define THROW_IF_HR_FAILED(hrResult) \
if(FAILED(hr = (hrResult))) \
{ \
    throw ING::Exception(hr, __FUNCTION__, __LINE__); \
}
#endif