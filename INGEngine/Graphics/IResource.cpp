//----------------------------------------------------------------------------------
// IResource.cpp : Implementation of ING Graphics resource interface.
//----------------------------------------------------------------------------------
#include "../precomp.h"
#include <IResource.h>
#include <fstream>

using namespace ING;
using namespace ING::Graphics;

///
/// Default Constructor.
///
IResource::IResource(resource_type_t type, const wchar_t *pName) :
    m_resourceType(type),
    m_name(pName)
{
    TraceScopeVoid(__FUNCTION__);
}

///
/// Default Destructor.
///
IResource::~IResource()
{
    TraceScopeVoid(__FUNCTION__);

    Dispose();
}

///
/// Load resource from file to memory.
///
result_code_t IResource::Load(const wchar_t* filename)
{
    result_code_t result = result_code_t::succeeded;

    std::ifstream in(filename, std::ios::in | std::ios::binary);

    if (in.good())
    {
        // Get data size
        in.seekg(0, std::ios::end);
        auto bufferSize = static_cast<std::streamsize>(in.tellg());
        in.seekg(0, std::ios::beg);

        // Resize buffer
        m_byteCode.resize(static_cast<size_t>(bufferSize));

        // Read buffer
        in.read(reinterpret_cast<char*>(m_byteCode.data()), bufferSize);
    }
    else
    {
        GOTO_IF_FAILED(result_code_t::not_found, Cleanup);
    }

Cleanup:
    if (in.is_open())
    {
        in.close();
    }

    return result;
}

///
/// Get raw byte code with size.
///
const byte* IResource::GetRaw(size_t* pSize)
{
    byte* pRet = nullptr;

    if (!m_byteCode.empty())
    {
        pRet = m_byteCode.data();
    }

    if (pSize)
    {
        *pSize = m_byteCode.size();
    }

    return pRet;
}

///
/// Dispose memory loaded data.
///
void IResource::Dispose()
{
    m_byteCode.clear();
}
