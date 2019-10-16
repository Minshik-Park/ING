//----------------------------------------------------------------------------------
// Graphics.cpp : Implements the ING Graphics interface class.
//----------------------------------------------------------------------------------
#include "../precomp.h"
#include "IGraphics.h"
#include "GraphicsDX12.h"
#include <Exception.h>

using namespace ING;
using namespace ING::Graphics;

///
/// Default Constructor.
///
IGraphics::IGraphics()
{

}

///
/// Default Destructor.
///
IGraphics::~IGraphics()
{

}

///
/// Factory method to create graphics instance.
///
IGraphics* IGraphics::Create(graphics_type_t type)
{
    IGraphics* pRet = nullptr;

    switch (type)
    {
#ifdef _WIN32
    case graphics_type_t::DX12:
        pRet = new GraphicsDX12();
        break;
#endif
    default:
        throw Exception(result_code_t::invalid_parameter, __FUNCTION__, __LINE__, "Not supported (valid) Graphics Type");
        break;
    }

    return pRet;
}
