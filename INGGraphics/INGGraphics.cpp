//----------------------------------------------------------------------------------
// INGGraphics.cpp : Implements the ING Graphics class.
//----------------------------------------------------------------------------------
#include "precomp.h"
#include <INGGraphics.h>
#include "INGGraphicsDX12.h"

using namespace ING;
using namespace ING::Graphics;

INGGraphics::INGGraphics()
{

}

INGGraphics::~INGGraphics()
{

}

INGGraphics* INGGraphics::Create(GraphicsType type)
{
    INGGraphics* pRet = nullptr;

    try
    {
        switch (type)
        {
#ifdef _WIN32
        case GraphicsType::DX12:
            pRet = new INGGraphicsDX12();
            break;
#endif
        default:
            break;
        }
    }
    catch (int )
    {
    }

    return pRet;
}

void INGGraphics::Test()
{

}