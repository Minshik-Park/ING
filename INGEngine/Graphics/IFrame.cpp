//----------------------------------------------------------------------------------
// IFrame.cpp : Implementation the ING Graphics frame interface class.
//----------------------------------------------------------------------------------
#include "../precomp.h"
#include "IFrame.h"

using namespace ING;
using namespace ING::Graphics;

///
/// Default Constructor.
///
IFrame::IFrame(const int index) :
    m_index(index)
{
    TraceScopeVoid();
}

///
/// Default Destructor.
///
IFrame::~IFrame()
{
    TraceScopeVoid();
}
