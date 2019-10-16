//----------------------------------------------------------------------------------
// IShader.h : Definition of ING Graphics shader interface.
//----------------------------------------------------------------------------------
#pragma once
#include <Common.h>
#include <IResource.h>

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

    class IShader : public IResource
    {
    public:
        IShader(shader_type_t type, const wchar_t* pName);
        virtual ~IShader();

    protected:
        shader_type_t m_shaderType = shader_type_t::Unknown;
    };

}}
