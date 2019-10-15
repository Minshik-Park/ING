//----------------------------------------------------------------------------------
// StateDX12.h : Definition of ING Graphics State for DX12 class.
//----------------------------------------------------------------------------------
#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "IState.h"
#include <string>
#include <vector>

namespace ING {
namespace Graphics {

    typedef struct _StatePrameter
    {
        std::wstring m_name;

        std::wstring m_pixelShaderName;
        std::wstring m_vertexShaderName;
        std::wstring m_domainShaderName;
        std::wstring m_hullShaderName;
        std::wstring m_geometryShaderName;

        //D3D12_STREAM_OUTPUT_DESC           StreamOutput;
            //const D3D12_SO_DECLARATION_ENTRY* pSODeclaration;
                //UINT   Stream;
                //LPCSTR SemanticName;
                //UINT   SemanticIndex;
                //BYTE   StartComponent;
                //BYTE   ComponentCount;
                //BYTE   OutputSlot;
            //UINT NumEntries;
            //const UINT* pBufferStrides;
            //UINT NumStrides;
            //UINT RasterizedStream;

        //D3D12_BLEND_DESC                   BlendState;
            //BOOL AlphaToCoverageEnable;
            //BOOL IndependentBlendEnable;
            //D3D12_RENDER_TARGET_BLEND_DESC RenderTarget[8];
                //BOOL           BlendEnable;
                //BOOL           LogicOpEnable;
                //D3D12_BLEND    SrcBlend;
                //D3D12_BLEND    DestBlend;
                //D3D12_BLEND_OP BlendOp;
                //D3D12_BLEND    SrcBlendAlpha;
                //D3D12_BLEND    DestBlendAlpha;
                //D3D12_BLEND_OP BlendOpAlpha;
                //D3D12_LOGIC_OP LogicOp;
                //UINT8          RenderTargetWriteMask;

        //UINT                               SampleMask;

        //D3D12_RASTERIZER_DESC              RasterizerState;
            //D3D12_FILL_MODE                       FillMode;
            //D3D12_CULL_MODE                       CullMode;
            //BOOL                                  FrontCounterClockwise;
            //INT                                   DepthBias;
            //FLOAT                                 DepthBiasClamp;
            //FLOAT                                 SlopeScaledDepthBias;
            //BOOL                                  DepthClipEnable;
            //BOOL                                  MultisampleEnable;
            //BOOL                                  AntialiasedLineEnable;
            //UINT                                  ForcedSampleCount;
            //D3D12_CONSERVATIVE_RASTERIZATION_MODE ConservativeRaster;

        //D3D12_DEPTH_STENCIL_DESC           DepthStencilState;
            //BOOL                       DepthEnable;
            //D3D12_DEPTH_WRITE_MASK     DepthWriteMask;
            //D3D12_COMPARISON_FUNC      DepthFunc;
            //BOOL                       StencilEnable;
            //UINT8                      StencilReadMask;
            //UINT8                      StencilWriteMask;
            //D3D12_DEPTH_STENCILOP_DESC FrontFace;
                //D3D12_STENCIL_OP StencilFailOp;
                //D3D12_STENCIL_OP StencilDepthFailOp;
                //D3D12_STENCIL_OP StencilPassOp;
                //D3D12_COMPARISON_FUNC StencilFunc;
            //D3D12_DEPTH_STENCILOP_DESC BackFace;
                //D3D12_STENCIL_OP StencilFailOp;
                //D3D12_STENCIL_OP StencilDepthFailOp;
                //D3D12_STENCIL_OP StencilPassOp;
                //D3D12_COMPARISON_FUNC StencilFunc;


        //D3D12_INPUT_LAYOUT_DESC            InputLayout;
            //const D3D12_INPUT_ELEMENT_DESC* pInputElementDescs;
                //LPCSTR SemanticName;
                //UINT SemanticIndex;
                //DXGI_FORMAT Format;
                //UINT InputSlot;
                //UINT AlignedByteOffset;
                //D3D12_INPUT_CLASSIFICATION InputSlotClass;
                //UINT InstanceDataStepRate;
            //UINT                           NumElements;

        //D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBStripCutValue;

        //D3D12_PRIMITIVE_TOPOLOGY_TYPE      PrimitiveTopologyType;

        //UINT                               NumRenderTargets;

        DXGI_FORMAT m_RTVFormats;
        DXGI_FORMAT m_DSVFormat;

        //DXGI_SAMPLE_DESC                   SampleDesc;

        //UINT                               NodeMask;

        //D3D12_CACHED_PIPELINE_STATE        CachedPSO;
            //const void* pCachedBlob;
            //SIZE_T     CachedBlobSizeInBytes;

        //D3D12_PIPELINE_STATE_FLAGS         Flags;

    } StateParameter;

    class StateDX12 : public IState
    {
    public:
        StateDX12(ID3D12Device *pD3DDevice);
        virtual ~StateDX12();

        result_code_t Initialize(const StateParameter* pParameter);

        const ID3D12PipelineState* GetPipelineStateDX12() { return m_spPipelineState.Get(); }

    private:
        std::wstring m_name;
        Microsoft::WRL::ComPtr<ID3D12Device>        m_spD3DDevice;
        Microsoft::WRL::ComPtr<ID3D12RootSignature> m_spRootSignature;
        Microsoft::WRL::ComPtr<ID3D12PipelineState> m_spPipelineState;
    };

}}
