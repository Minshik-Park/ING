//----------------------------------------------------------------------------------
// IFrame.h : Definition the ING Graphics frame interface class.
//----------------------------------------------------------------------------------
#pragma once
#include <Common.h>

namespace ING {
namespace Graphics {

    class IFrame
    {
    public:
        IFrame();
        virtual ~IFrame();

        virtual result_code_t OnSizeChanged(const int width, const int height) = 0;
    };

}}