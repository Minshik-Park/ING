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
Engine::Engine(window_t wnd, Graphics::graphics_type_t type) :
    m_window(wnd),
    m_graphicsType(type)
{
    TraceScopeVoid();
}

///
///
///
Engine::~Engine()
{
    TraceScopeVoid();
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
    catch (const ExceptionHr &hrEx)
    {
        ING_DebugWrite(L"hrException : %S\n", hrEx.what());
        result = hrEx.Result();
    }
    catch (const Exception &ex)
    {
        ING_DebugWrite(L"Exception : %S\n", ex.what());
        result = ex.Result();
    }

    return result;
}

result_code_t Engine::Update()
{
    return result_code_t::succeeded;
}

result_code_t Engine::Render()
{
    return m_spGraphics->Render();
}

///
///
///
result_code_t Engine::OnSuspend()
{
    if (m_spGraphics != nullptr)
    {
        m_spGraphics->PauseRendering();
    }

    return result_code_t::succeeded;
}

///
///
///
result_code_t Engine::OnResume()
{
    if (m_spGraphics != nullptr)
    {
        m_spGraphics->ResumeRendering();
    }

    return result_code_t::succeeded;
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
