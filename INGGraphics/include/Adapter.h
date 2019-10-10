//----------------------------------------------------------------------------------
// Adapter.h : Definition the ING Adapter class.
//----------------------------------------------------------------------------------
#pragma once
#include <Common.h>

namespace ING {
namespace Graphics {

    class ING_API Adapter
    {
    public:
        Adapter();
        virtual ~Adapter();

        virtual const wchar_t *Description() = 0;
        virtual bool IsHardware() = 0;
    };

}}
