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
ShaderHlsl::ShaderHlsl(shader_type_t type) :
    IShader(type)
{
}

///
/// Default Destructor.
///
ShaderHlsl::~ShaderHlsl()
{
}

///
/// Load shader and create shader objects.
///
result_code_t ShaderHlsl::Load(const wchar_t* pName, const wchar_t* filename)
{
    result_code_t result = result_code_t::succeeded;

    std::wstring physicalFileName(filename);
    physicalFileName.append(L".cso");

    std::ifstream in(physicalFileName, std::ios::in | std::ios::binary);

    if (in.good())
    {
        in.seekg(0, std::ios::end);
        auto bufferSize = static_cast<std::streamsize>(in.tellg());
        in.seekg(0, std::ios::beg);

        m_byteCode.resize(static_cast<size_t>(bufferSize));

        in.read(reinterpret_cast<char*>(m_byteCode.data()), bufferSize);
    }
    else
    {
        GOTO_IF_FAILED(result_code_t::not_found, Cleanup);
    }

    // Set Name.
    m_name = pName;

Cleanup:
    if (in.is_open())
    {
        in.close();
    }

    return result;
}

///
/// Get shader byte code with its size
///
const byte* ShaderHlsl::Get(size_t* pSize)
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
/// Dispose all data
///
void ShaderHlsl::Dispose()
{
    m_byteCode.clear();
}
