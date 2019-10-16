//----------------------------------------------------------------------------------
// StateHlsl.h : Definition of ING Graphics HLSL Shader class.
//----------------------------------------------------------------------------------
#include "../precomp.h"
#include "ShaderHlsl.h"
#include <fstream>

using namespace ING;
using namespace ING::Graphics;

///
/// Default Constructor.
///
ShaderHlsl::ShaderHlsl(shader_type_t type, const wchar_t* pName) :
    IShader(type, pName)
{
}

///
/// Default Destructor.
///
ShaderHlsl::~ShaderHlsl()
{
}

///
/// Default Destructor.
///
result_code_t ShaderHlsl::LoadObject()
{
    return Load((m_name + L".cso").c_str());
}
