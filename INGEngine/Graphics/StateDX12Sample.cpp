//----------------------------------------------------------------------------------
// StateDX12Sample.h : Definition of ING Graphics State for DX12 class.
//----------------------------------------------------------------------------------
#include "../precomp.h"
#include "StateDX12Sample.h"
#include "ShaderHlsl.h"

using namespace ING;
using namespace ING::Graphics;
using namespace Microsoft::WRL;

///
/// Default Constructor.
///
StateDX12Sample::StateDX12Sample(const wchar_t* pName, ID3D12Device* pD3DDevice) :
    StateDX12Base(pName, pD3DDevice)
{
    TraceScopeVoid();
}

///
/// Default Destructor.
///
StateDX12Sample::~StateDX12Sample()
{
    TraceScopeVoid();
}

///
/// Initialize Sample State.
///
result_code_t StateDX12Sample::Initialize()
{
    // Create Root Signature.
    RETURN_IF_FAILED(CreateRootSignature());

    // Create Pipeline state instance.
    RETURN_IF_FAILED(CreatePipelineState());

    return result_code_t::succeeded;
}

///
/// Create Root Signature.
///
result_code_t StateDX12Sample::CreateRootSignature()
{
    HRESULT hr = S_OK;

    D3D12_DESCRIPTOR_RANGE range = {};
    range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    range.NumDescriptors = 1;
    range.BaseShaderRegister = 0;
    range.RegisterSpace = 0;
    range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    D3D12_ROOT_PARAMETER parameter = {};
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

    // Assign root signature to the state parameter.
    m_stateDX12.pRootSignature = m_spRootSignature.Get();

Cleanup:
    return HRESULT_TO_RESULT_CODE(hr);
}

///
/// Create Pipeline State.
///
result_code_t StateDX12Sample::CreatePipelineState()
{
    HRESULT hr = S_OK;

    // Configure Shaders
    ShaderHlsl vertexShader(shader_type_t::VertexShader, L"SampleVertexShader");
    ShaderHlsl pixelShader(shader_type_t::PixelShader, L"SamplePixelShader");

    RETURN_IF_FAILED(vertexShader.LoadObject());
    RETURN_IF_FAILED(pixelShader.LoadObject());

    m_stateDX12.VS.pShaderBytecode = vertexShader.GetRaw(&m_stateDX12.VS.BytecodeLength);
    m_stateDX12.PS.pShaderBytecode = pixelShader.GetRaw(&m_stateDX12.PS.BytecodeLength);

    // Set input layer and primitive topoloty
    static const D3D12_INPUT_ELEMENT_DESC inputLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    m_stateDX12.InputLayout = { inputLayout, _countof(inputLayout) };
    m_stateDX12.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    // Configure Stream Output
    m_stateDX12.StreamOutput.pSODeclaration = nullptr;
    m_stateDX12.StreamOutput.NumEntries = 0;
    m_stateDX12.StreamOutput.pBufferStrides = nullptr;
    m_stateDX12.StreamOutput.NumStrides = 0;
    m_stateDX12.StreamOutput.RasterizedStream = 0;

    // Configure Blend state
    m_stateDX12.BlendState.AlphaToCoverageEnable = FALSE;
    m_stateDX12.BlendState.IndependentBlendEnable = FALSE;
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
        m_stateDX12.BlendState.RenderTarget[i] = defaultRenderTargetBlendDesc;
    }

    // Configure Rasterizer state
    m_stateDX12.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    m_stateDX12.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
    m_stateDX12.RasterizerState.FrontCounterClockwise = FALSE;
    m_stateDX12.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    m_stateDX12.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    m_stateDX12.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    m_stateDX12.RasterizerState.DepthClipEnable = TRUE;
    m_stateDX12.RasterizerState.MultisampleEnable = FALSE;
    m_stateDX12.RasterizerState.AntialiasedLineEnable = FALSE;
    m_stateDX12.RasterizerState.ForcedSampleCount = 0;
    m_stateDX12.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    // Configure Depth stencil state
    m_stateDX12.DepthStencilState.DepthEnable = TRUE;
    m_stateDX12.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    m_stateDX12.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    m_stateDX12.DepthStencilState.StencilEnable = FALSE;
    m_stateDX12.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
    m_stateDX12.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
    m_stateDX12.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    m_stateDX12.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
    m_stateDX12.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
    m_stateDX12.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    m_stateDX12.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    m_stateDX12.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
    m_stateDX12.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
    m_stateDX12.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

    // Set Sampler
    m_stateDX12.SampleMask = UINT_MAX;
    m_stateDX12.SampleDesc.Count = 1;
    m_stateDX12.SampleDesc.Quality = 0;

    // Create Graphics pipeline state
    GOTO_IF_HR_FAILED(m_spD3DDevice->CreateGraphicsPipelineState(&m_stateDX12, IID_PPV_ARGS(&m_spPipelineState)), Cleanup);

Cleanup:
    return HRESULT_TO_RESULT_CODE(hr);
}