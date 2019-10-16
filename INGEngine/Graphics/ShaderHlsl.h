//----------------------------------------------------------------------------------
// ShaderHlsl.h : Definition of ING Graphics HLSL Shader class.
//----------------------------------------------------------------------------------
#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "IShader.h"
#include <vector>
#include <string>

namespace ING {
namespace Graphics {

    class ShaderHlsl : public IShader
    {
    public:
        ShaderHlsl(shader_type_t type, const wchar_t* pName);
        virtual ~ShaderHlsl();

        result_code_t LoadObject();
    };

}}
