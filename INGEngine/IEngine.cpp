//----------------------------------------------------------------------------------
// IEngine.cpp : Implementation the ING Engine interface class.
//----------------------------------------------------------------------------------
#include "precomp.h"
#include <Exception.h>
#include <IEngine.h>
#include "Engine.h"

using namespace ING;
using namespace ING::Graphics;

///
/// Default Constructor.
///
IEngine::IEngine()
{
    TraceScopeVoid(__FUNCTION__);
}

///
/// Default Destructor.
///
IEngine::~IEngine()
{
    TraceScopeVoid(__FUNCTION__);
}

///
/// Factory method to create Engine instance.
///
IEngine* IEngine::Create(window_t wnd, graphics_type_t type)
{
    THROW_IF_FALSE(wnd != NULL, result_code_t::invalid_parameter);

    return new Engine(wnd, type);
}