//----------------------------------------------------------------------------------
// IResource.h : Definition of ING Graphics resource interface.
//----------------------------------------------------------------------------------
#pragma once
#include <Common.h>
#include <string>
#include <vector>

namespace ING {
namespace Graphics {
    ///
    /// Type of Graphics resources
    ///
    enum class resource_type_t
    {
        Unknown = 0,
        Shader,
        Texture,
        Geometry
    };

    class IResource
    {
    public:
        IResource(resource_type_t type, const wchar_t *pName);
        virtual ~IResource();

        const std::wstring& Name() { return m_name; }

        result_code_t Load(const wchar_t* filename);
        const byte* GetRaw(size_t* pSize);
        bool IsLoaded() { return !m_byteCode.empty(); }

        virtual void Dispose();

    protected:
        resource_type_t m_resourceType = resource_type_t::Unknown;
        std::wstring m_name;

        std::vector<byte> m_byteCode;
    };

}}
