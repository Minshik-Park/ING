#include <ING/INGEngine.h>

using namespace ING;

INGEngine::INGEngine()
{
}

INGEngine::~INGEngine()
{
}

result_code_t INGEngine::Initialize()
{
    return result_not_implemented;
}

result_code_t INGEngine::Update()
{
    return result_not_implemented;
}

result_code_t INGEngine::Render()
{
    return result_not_implemented;
}

//
// Factory methods
//
result_code_t INGEngine::CreateScene()
{
    return result_not_implemented;
}

result_code_t INGEngine::CreateObject()
{
    return result_not_implemented;
}
