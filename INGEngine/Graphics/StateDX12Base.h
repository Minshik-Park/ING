//----------------------------------------------------------------------------------
// StateDX12Base.h : Definition of ING Graphics State for DX12 base class.
//----------------------------------------------------------------------------------
#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "IState.h"
#include <string>
#include <vector>

namespace ING {
namespace Graphics {

    class StateDX12Base : public IState
    {
    public:
        StateDX12Base(const wchar_t* pName, ID3D12Device *pD3DDevice);
        virtual ~StateDX12Base();

        // DX12 data accessors.
        const ID3D12PipelineState* GetPipelineStateDX12() { return m_spPipelineState.Get(); }
        void SetRenderTargetFormat(const unsigned int numRenderTarget, DXGI_FORMAT format);
        void SetDepthStencilFormat(DXGI_FORMAT format);

    protected:
        virtual result_code_t CreateRootSignature() = 0;
        virtual result_code_t CreatePipelineState() = 0;

    protected:
        std::wstring m_name;
        Microsoft::WRL::ComPtr<ID3D12Device>        m_spD3DDevice;
        Microsoft::WRL::ComPtr<ID3D12RootSignature> m_spRootSignature;
        Microsoft::WRL::ComPtr<ID3D12PipelineState> m_spPipelineState;

        D3D12_GRAPHICS_PIPELINE_STATE_DESC m_stateDX12 = {};
    };

}}
