#pragma once
#include <ING/INGTypes.h>
#include <ING/INGEngine.h>

#include <INGCore_export.h>

#ifdef __cplusplus
namespace ING
{
extern "C"
{
#endif

INGCORE_EXPORT result_code_t CreateEngine(const renderer_type_t rendererType, INGEngine **ppEngine);

#ifdef __cplusplus
}
}
#endif

