//----------------------------------------------------------------------------------
// INGGraphics.h : Defines the ING Graphics class.
//----------------------------------------------------------------------------------
#pragma once
#include <INGCommon.h>

namespace ING {
namespace Graphics {

    ///
    /// Type of Graphics implementation
    ///
    enum class GraphicsType
    {
        Unknown = 0,
        DX12
    };

    ///
    /// ING Graphics class
    ///
    class ING_API INGGraphics
    {
    public:
        virtual ~INGGraphics();

        // Factory
        static INGGraphics *Create(GraphicsType type);

        void Test();

    protected:
        INGGraphics();
    };

}}
