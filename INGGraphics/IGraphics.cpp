//----------------------------------------------------------------------------------
// Graphics.cpp : Implements the ING Graphics class.
//----------------------------------------------------------------------------------
#include "precomp.h"
#include <IGraphics.h>
#include "GraphicsDX12.h"
#include <Exception.h>

using namespace ING;
using namespace ING::Graphics;

IGraphics::IGraphics()
{

}

IGraphics::~IGraphics()
{

}

IGraphics* IGraphics::Create(GraphicsType type)
{
    IGraphics* pRet = nullptr;

    switch (type)
    {
#ifdef _WIN32
    case GraphicsType::DX12:
        pRet = new GraphicsDX12();
        break;
#endif
    default:
        throw new Exception(result_code_t::result_invalid_parameter, __FUNCTION__, __LINE__, "Not supported (valid) Graphics Type");
        break;
    }

    return pRet;
}

void ING::Graphics::Test()
{
    try
    {
        std::unique_ptr<IGraphics> pGraphics(IGraphics::Create(GraphicsType::DX12));
        auto result = pGraphics->Initialize();
    }
#ifdef _WIN32
    catch (ExceptionHr exHr)
    {
        ING_DebugWrite(L"Exception : %S\n", exHr.what());
    }
#endif
    catch (Exception ex)
    {
        ING_DebugWrite(L"Exception : %S\n", ex.what());
    }
}