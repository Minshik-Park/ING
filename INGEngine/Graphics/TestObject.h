#pragma once
#include <Common.h>

namespace ING {
namespace Graphics {
namespace Test {
    class TestObject
    {
    public:
        TestObject();
        virtual ~TestObject();

        HRESULT Initialize();

        HRESULT Render();
    };
}}}
