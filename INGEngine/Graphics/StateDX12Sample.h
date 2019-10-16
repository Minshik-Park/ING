//----------------------------------------------------------------------------------
// StateDX12Sample.h : Definition of ING Graphics State for DX12 class.
//----------------------------------------------------------------------------------
#pragma once
#include "StateDX12Base.h"

namespace ING {
namespace Graphics {

    class StateDX12Sample : public StateDX12Base
    {
    public:
        StateDX12Sample(const wchar_t* pName, ID3D12Device* pD3DDevice);
        virtual ~StateDX12Sample();

        virtual result_code_t Initialize() override;

    protected:
        virtual result_code_t CreateRootSignature() override;
        virtual result_code_t CreatePipelineState() override;
    };

}}