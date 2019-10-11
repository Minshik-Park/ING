//----------------------------------------------------------------------------------
// Engine.cpp : Implementation the ING Engine class.
//----------------------------------------------------------------------------------
#include "precomp.h"
#include "Engine.h"
#include <Exception.h>

using namespace ING;
using namespace ING::Graphics;

///
///
///
Engine::Engine(window_t wnd, Graphics::GraphicsType type) :
    m_window(wnd),
    m_graphicsType(type)
{
}

///
///
///
Engine::~Engine()
{
}

///
///
///
result_code_t Engine::Initialize()
{
    result_code_t result = result_code_t::succeeded;

    try
    {
        m_spGraphics.reset(IGraphics::Create(m_graphicsType));
        RETURN_IF_FAILED(m_spGraphics->Initialize(m_window));
    }
    catch (ExceptionHr hrEx)
    {
        ING_DebugWrite(L"hrException : %S\n", hrEx.what());
        result = hrEx.Result();
    }
    catch (Exception ex)
    {
        ING_DebugWrite(L"Exception : %S\n", ex.what());
        result = ex.Result();
    }

    return result;
}

///
///
///
void Engine::PauseRendering()
{
    if (m_spGraphics != nullptr)
    {
        m_spGraphics->PauseRendering();
    }
}

///
///
///
void Engine::ResumeRendering()
{
    if (m_spGraphics != nullptr)
    {
        m_spGraphics->ResumeRendering();
    }
}

///
///
///
result_code_t Engine::OnWindowSizeChanged(const int width, const int height)
{
    if (m_spGraphics != nullptr)
    {
        RETURN_IF_FAILED(m_spGraphics->OnWindowSizeChanged(width, height));
    }

    return result_code_t::succeeded;
}
