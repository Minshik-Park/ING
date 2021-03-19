#include <pch.h>
#include "Graphics.h"
#include "GraphicsDX12.h"

using namespace ING;

/// <summary>
/// Factory method to create Graphics object.
/// </summary>
/// <param name="api">Desired graphics API to use</param>
/// <returns></returns>
Graphics* Graphics::CreateInstance(GraphicsApi api)
{
    Graphics* pGraphics = nullptr;

    switch (api)
    {
    case GraphicsApi::DX12:
        pGraphics = new GraphicsDX12();
        break;
    default:
        Debug::DebugPrintf("%s: Unsupported Graphics API - %d", __FUNCTION__, api);
        break;
    }

    return pGraphics;
}
