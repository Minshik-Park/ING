//----------------------------------------------------------------------------------
// IShader.cpp : Implementation of ING Graphics shader interface.
//----------------------------------------------------------------------------------
#include "../precomp.h"
#include "IShader.h"

using namespace ING;
using namespace ING::Graphics;

///
/// Default Constructor.
///
IShader::IShader(shader_type_t type) :
    m_type(type)
{
}

///
/// Default Destructor.
///
IShader::~IShader()
{
}
