//----------------------------------------------------------------------------------
// IState.h : Definition of ING Graphics state interface.
//----------------------------------------------------------------------------------
#pragma once
#include <Common.h>
#include "IShader.h"
#include <string>

namespace ING {
namespace Graphics {

    class IState
    {
    public:
        IState(const wchar_t *pName);
        virtual ~IState();

        const std::wstring& Name() { return m_name; }

        virtual result_code_t Initialize() = 0;

    protected:
        std::wstring m_name;
    };

}}
