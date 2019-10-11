//----------------------------------------------------------------------------------
// IAdapter.h : Definition the ING Adapter interface class.
//----------------------------------------------------------------------------------
#pragma once
#include <Common.h>

namespace ING {
namespace Graphics {

    class ING_API IAdapter
    {
    public:
        IAdapter();
        virtual ~IAdapter();

        virtual const wchar_t *Description() = 0;
        virtual bool IsHardware() = 0;
    };

}}
