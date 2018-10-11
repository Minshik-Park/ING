#include <ING/INGApi.h>

#ifdef __cplusplus
namespace ING
{
extern "C"
{
#endif

result_code_t CreateEngine(const renderer_type_t rendererType, INGEngine ** ppEngine)
{
    result_code_t result = result_not_implemented;

    if (ppEngine == nullptr)
    {
        return result_invalid_pointer;
    }

    switch (rendererType)
    {
#ifdef _WIN32
    case renderer_dx_11:
    case renderer_dx_12:
        *ppEngine = new INGEngine();
        break;
#endif
    case renderer_opengl:
        break;
    default:
        break;
    }

    return result;
}

#ifdef __cplusplus
}
}
#endif
