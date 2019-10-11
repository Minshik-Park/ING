//----------------------------------------------------------------------------------
// FrameDX12.cpp : Implementation the ING Graphics frame for DX12 class.
//----------------------------------------------------------------------------------
#include "precomp.h"
#include "FrameDX12.h"
#include <Exception.h>

using namespace ING;
using namespace ING::Graphics;

FrameDX12::FrameDX12(ID3D12Device* pD3DDevice)
{
}

FrameDX12::~FrameDX12()
{
}

result_code_t FrameDX12::OnSizeChanged(const int width, const int height)
{
    return result_code_t::succeeded;
}
