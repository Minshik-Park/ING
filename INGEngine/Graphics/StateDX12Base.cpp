//----------------------------------------------------------------------------------
// StateDX12Base.h : Definition of ING Graphics State for DX12 base class.
//----------------------------------------------------------------------------------
#include "../precomp.h"
#include "StateDX12Base.h"

using namespace ING;
using namespace ING::Graphics;
using namespace Microsoft::WRL;

///
/// Default Constructor.
///
StateDX12Base::StateDX12Base(const wchar_t* pName, ID3D12Device* pD3DDevice) :
    IState(pName),
    m_spD3DDevice(pD3DDevice)
{
}

///
/// Default Destructor.
///
StateDX12Base::~StateDX12Base()
{
}

void StateDX12Base::SetRenderTargetFormat(const unsigned int numRenderTarget, DXGI_FORMAT format)
{
    unsigned int i = 0;
    for (; i < numRenderTarget && i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
    {
        m_stateDX12.RTVFormats[i] = format;
    }

    for (; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
    {
        m_stateDX12.RTVFormats[i] = DXGI_FORMAT_UNKNOWN;
    }

    m_stateDX12.NumRenderTargets = numRenderTarget;
}

void StateDX12Base::SetDepthStencilFormat(DXGI_FORMAT format)
{
    m_stateDX12.DSVFormat = format;
}
