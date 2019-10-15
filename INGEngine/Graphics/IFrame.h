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

        virtual result_code_t Initialize() = 0;
        virtual result_code_t Render() = 0;

        virtual result_code_t OnSizeChanged(const int width, const int height) = 0;
        virtual void ReleaseSizeDependentResources() = 0;
    };

}}