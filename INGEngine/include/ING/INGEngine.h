#pragma once
#include <ING/INGTypes.h>

namespace ING
{
    class INGEngine
    {
    public:
        INGEngine();
        virtual ~INGEngine();

        result_code_t Initialize();

        result_code_t Update();

        result_code_t Render();

        // Factory methods
        result_code_t CreateScene();
        result_code_t CreateObject();

    private:
    };
}
