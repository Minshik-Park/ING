//----------------------------------------------------------------------------------
// IFileResource.cpp : Implementation of ING Graphics file resource interface.
//----------------------------------------------------------------------------------
#include "../precomp.h"
#include <IFileResource.h>
#include <fstream>

using namespace ING;
using namespace ING::Graphics;

///
/// Default Constructor.
///
IFileResource::IFileResource(resource_type_t type, const wchar_t *pName) :
    m_resourceType(type),
    m_name(pName)
{
    TraceScopeVoid();
}

///
/// Default Destructor.
///
IFileResource::~IFileResource()
{
    TraceScopeVoid();

    Dispose();
}

///
/// Load resource from file to memory.
///
result_code_t IFileResource::Load(const wchar_t* filename)
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
const byte* IFileResource::GetRaw(size_t* pSize)
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
void IFileResource::Dispose()
{
    m_byteCode.clear();
}
