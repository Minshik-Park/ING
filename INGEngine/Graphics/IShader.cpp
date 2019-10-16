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
IShader::IShader(shader_type_t type, const wchar_t* pName) :
    IResource(resource_type_t::Shader, pName),
    m_shaderType(type)
{
}

///
/// Default Destructor.
///
IShader::~IShader()
{
}
