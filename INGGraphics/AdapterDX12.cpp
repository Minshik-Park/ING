//----------------------------------------------------------------------------------
// AdapterDX12.cpp : Implements the ING Adapter for DX12 class.
//----------------------------------------------------------------------------------
#include "precomp.h"
#include "AdapterDX12.h"
#include <Exception.h>

using namespace ING;
using namespace ING::Graphics;

AdapterDX12::AdapterDX12(IDXGIAdapter1* pAdapter)
{
    if (pAdapter)
    {
        pAdapter->GetDesc1(&m_descriptor);
    }
    else
    {
        throw Exception(result_code_t::invalid_parameter, __FUNCTION__, __LINE__);
    }
}

AdapterDX12::~AdapterDX12()
{
}

const wchar_t* AdapterDX12::Description()
{
    return m_descriptor.Description;
}

bool AdapterDX12::IsHardware()
{
    return !(m_descriptor.Flags & DXGI_ADAPTER_FLAG_SOFTWARE);
}