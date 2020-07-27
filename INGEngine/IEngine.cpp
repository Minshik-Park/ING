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
    TraceScopeVoid();
}

///
/// Default Destructor.
///
IEngine::~IEngine()
{
    TraceScopeVoid();
}

///
/// Factory method to create Engine instance.
///
IEngine* IEngine::Create(window_t wnd, graphics_type_t graphicsType)
{
    THROW_IF_FALSE(wnd != NULL, result_code_t::invalid_parameter);

    return new Engine(wnd, graphicsType);
}

std::shared_ptr<IEngine> IEngine::CreateShared(window_t wnd, Graphics::graphics_type_t graphicsType)
{
    THROW_IF_FALSE(wnd != NULL, result_code_t::invalid_parameter);

    return std::shared_ptr<IEngine>(new Engine(wnd, graphicsType), DestoryEngine);
}
