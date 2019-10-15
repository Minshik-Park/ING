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
        ShaderHlsl(shader_type_t type);
        virtual ~ShaderHlsl();

        virtual result_code_t Load(const wchar_t* pName, const wchar_t* filename) override;
        virtual const byte* Get(size_t* pSize) override;
        virtual void Dispose() override;

    private:
        std::wstring m_name;
        std::vector<byte> m_byteCode;
    };

}}
