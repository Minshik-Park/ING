//----------------------------------------------------------------------------------
// INGException.cpp : Implementation of ING Exception classes.
//----------------------------------------------------------------------------------
#include "precomp.h"
#include <INGException.h>

namespace ING
{
    const char* ResultCodeToString(result_code_t result)
    {
        switch (result)
        {
        case result_code_t::result_succeeded:
            return "Succeeded";
        case result_code_t::result_failed:
            return "Generic Failure";
        case result_code_t::result_invalid_parameter:
            return "Invalid Parameter";
        case result_code_t::result_invalid_pointer:
            return "Invalid Pointer operation";
        case result_code_t::result_out_of_memory:
            return "Out of Memory";
        case result_code_t::result_not_found:
            return "Not Found";
        case result_code_t::result_not_implemented:
            return "Not implemented";
        case result_code_t::result_rendering_device_removed:
            return "Rendering device has been removed";
        default:
            break;
        }

        return "Unknown error";
    }
}
