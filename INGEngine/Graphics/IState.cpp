//----------------------------------------------------------------------------------
// IState.cpp : Implementation of ING Graphics state interface.
//----------------------------------------------------------------------------------
#include "../precomp.h"
#include "IState.h"

using namespace ING;
using namespace ING::Graphics;

///
/// Default Constructor.
///
IState::IState(const wchar_t* pName) :
    m_name(pName)
{
}

///
/// Default Destructor.
///
IState::~IState()
{
}
