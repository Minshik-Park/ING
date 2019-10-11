//----------------------------------------------------------------------------------
// IEngine.cpp : Implementation the ING Engine interface class.
//----------------------------------------------------------------------------------
#include "precomp.h"
#include <Exception.h>
#include <IEngine.h>
#include "Engine.h"

using namespace ING;
using namespace ING::Graphics;

IEngine::IEngine()
{
}

IEngine::~IEngine()
{
}

IEngine* IEngine::Create(window_t wnd, GraphicsType type)
{
    THROW_IF_FALSE(wnd != NULL, result_code_t::invalid_parameter);

    return new Engine(wnd, type);
}