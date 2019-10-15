//----------------------------------------------------------------------------------
// StateDX12.h : Definition of ING Graphics State for DX12 class.
//----------------------------------------------------------------------------------
#include "../precomp.h"
#include "StateDX12.h"
#include "ShaderHlsl.h"

using namespace ING;
using namespace ING::Graphics;
using namespace Microsoft::WRL;

///
/// Default Constructor.
///
StateDX12::StateDX12(ID3D12Device* pD3DDevice) :
    m_spD3DDevice(pD3DDevice)
{
}

///
/// Default Destructor.
///
StateDX12::~StateDX12()
{
}

///
///
///
result_code_t StateDX12::Initialize(const StateParameter* pParameter)
{
    HRESULT hr = S_OK;
    D3D12_DESCRIPTOR_RANGE range = {};
    D3D12_ROOT_PARAMETER parameter = {};
    D3D12_GRAPHICS_PIPELINE_STATE_DESC state = {};
    ShaderHlsl pixelShaderCode(shader_type_t::PixelShader);
    ShaderHlsl vertexShaderCode(shader_type_t::VertexShader);
    ShaderHlsl domainShaderCode(shader_type_t::DomainShader);
    ShaderHlsl hullShaderCode(shader_type_t::HullShader);
    ShaderHlsl geometryShaderCode(shader_type_t::GeometryShader);

    RETURN_IF_FALSE(pParameter, result_code_t::invalid_parameter);

    // Set State Name.
    m_name = pParameter->m_name;

    // Load Shaders
    if (!pParameter->m_pixelShaderName.empty())
    {
        RETURN_IF_FAILED(pixelShaderCode.Load(pParameter->m_pixelShaderName.c_str(), pParameter->m_pixelShaderName.c_str()));

        state.PS.pShaderBytecode = pixelShaderCode.Get(&state.PS.BytecodeLength);
    }

    if (!pParameter->m_vertexShaderName.empty())
    {
        RETURN_IF_FAILED(vertexShaderCode.Load(pParameter->m_vertexShaderName.c_str(), pParameter->m_vertexShaderName.c_str()));

        state.VS.pShaderBytecode = vertexShaderCode.Get(&state.VS.BytecodeLength);
    }

    if (!pParameter->m_domainShaderName.empty())
    {
        RETURN_IF_FAILED(domainShaderCode.Load(pParameter->m_domainShaderName.c_str(), pParameter->m_domainShaderName.c_str()));

        state.DS.pShaderBytecode = domainShaderCode.Get(&state.DS.BytecodeLength);
    }

    if (!pParameter->m_hullShaderName.empty())
    {
        RETURN_IF_FAILED(hullShaderCode.Load(pParameter->m_hullShaderName.c_str(), pParameter->m_hullShaderName.c_str()));

        state.HS.pShaderBytecode = hullShaderCode.Get(&state.HS.BytecodeLength);
    }

    if (!pParameter->m_geometryShaderName.empty())
    {
        RETURN_IF_FAILED(geometryShaderCode.Load(pParameter->m_geometryShaderName.c_str(), pParameter->m_geometryShaderName.c_str()));

        state.GS.pShaderBytecode = geometryShaderCode.Get(&state.GS.BytecodeLength);
    }

    // Create a root signature with a single constant buffer slot.
    {
        range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        range.NumDescriptors = 1;
        range.BaseShaderRegister = 0;
        range.RegisterSpace = 0;
        range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        parameter.DescriptorTable.NumDescriptorRanges = 1;
        parameter.DescriptorTable.pDescriptorRanges = &range;
        parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

        D3D12_ROOT_SIGNATURE_DESC descRootSignature;
        descRootSignature.NumParameters = 1;
        descRootSignature.pParameters = &parameter;
        descRootSignature.NumStaticSamplers = 0;
        descRootSignature.pStaticSamplers = nullptr;
        descRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |    // Only the input assembler stage needs access to the constant buffer.
                                  D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS     |
                                  D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS   |
                                  D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS       |
                                  D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;


        ComPtr<ID3DBlob> spSignature;
        ComPtr<ID3DBlob> spError;
        GOTO_IF_HR_FAILED(D3D12SerializeRootSignature(&descRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, spSignature.GetAddressOf(), spError.GetAddressOf()), Cleanup);
        GOTO_IF_HR_FAILED(m_spD3DDevice->CreateRootSignature(0, spSignature->GetBufferPointer(), spSignature->GetBufferSize(), IID_PPV_ARGS(&m_spRootSignature)), Cleanup);
    }

    // Create Pipeline State.
    {
        state.pRootSignature = m_spRootSignature.Get();

        state.StreamOutput.pSODeclaration = nullptr;
        state.StreamOutput.NumEntries = 0;
        state.StreamOutput.pBufferStrides = nullptr;
        state.StreamOutput.NumStrides = 0;
        state.StreamOutput.RasterizedStream = 0;

        state.BlendState.AlphaToCoverageEnable = FALSE;
        state.BlendState.IndependentBlendEnable = FALSE;
        const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
        {
            FALSE,FALSE,
            D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
            D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
            D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE_ALL,
        };
        for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
        {
            state.BlendState.RenderTarget[i] = defaultRenderTargetBlendDesc;
        }

        state.SampleMask = UINT_MAX;
        
        state.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
        state.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
        state.RasterizerState.FrontCounterClockwise = FALSE;
        state.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        state.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        state.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        state.RasterizerState.DepthClipEnable = TRUE;
        state.RasterizerState.MultisampleEnable = FALSE;
        state.RasterizerState.AntialiasedLineEnable = FALSE;
        state.RasterizerState.ForcedSampleCount = 0;
        state.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

        state.DepthStencilState.DepthEnable = TRUE;
        state.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        state.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
        state.DepthStencilState.StencilEnable = FALSE;
        state.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
        state.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
        state.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        state.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        state.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        state.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
        state.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
        state.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
        state.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
        state.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

        static const D3D12_INPUT_ELEMENT_DESC inputLayout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };

        state.InputLayout = { inputLayout, _countof(inputLayout) };
        
        state.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

        state.NumRenderTargets = 1;
        state.RTVFormats[0] = pParameter->m_RTVFormats;
        state.DSVFormat     = pParameter->m_DSVFormat;
        state.SampleDesc.Count = 1;
        state.SampleDesc.Quality = 0;
        GOTO_IF_HR_FAILED(m_spD3DDevice->CreateGraphicsPipelineState(&state, IID_PPV_ARGS(&m_spPipelineState)), Cleanup);
    }

Cleanup:
    return HRESULT_TO_RESULT_CODE(hr);
}
