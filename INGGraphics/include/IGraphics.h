//----------------------------------------------------------------------------------
// Graphics.h : Defines the ING Graphics class.
//----------------------------------------------------------------------------------
#pragma once
#include <Common.h>
#include <Adapter.h>
#include <vector>
#include <memory>

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
    /// Graphics class
    ///
    class ING_API IGraphics
    {
    public:
        virtual ~IGraphics();

        // Factory
        static IGraphics *Create(GraphicsType type);

        virtual result_code_t Initialize() = 0;

        virtual result_code_t GetAdapterAt(const int index, Adapter **ppAdapter) = 0;

    protected:
        IGraphics();
    };

    // Test function
    ING_API void Test();
}}
