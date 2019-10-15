//----------------------------------------------------------------------------------
// IShader.h : Definition of ING Graphics shader interface.
//----------------------------------------------------------------------------------
#pragma once
#include <Common.h>

namespace ING {
namespace Graphics {

    enum class shader_type_t
    {
        Unknown = 0,
        VertexShader,
        PixelShader,
        DomainShader,
        HullShader,
        GeometryShader
    };

    class IShader
    {
    public:
        IShader(shader_type_t type);
        virtual ~IShader();

        virtual result_code_t Load(const wchar_t* pName, const wchar_t* filename) = 0;
        virtual const byte* Get(size_t* pSize) = 0;
        virtual void Dispose() = 0;

    protected:
        shader_type_t m_type = shader_type_t::Unknown;
    };

}}
