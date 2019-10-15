//----------------------------------------------------------------------------------
// Exception.cpp : Implementation of ING Exception classes.
//----------------------------------------------------------------------------------
#include "..\precomp.h"
#include <Exception.h>

namespace ING
{
    ///
    /// Get string description of result code.
    ///
    const char* ResultCodeToString(result_code_t result)
    {
        switch (result)
        {
        case result_code_t::succeeded:
            return "Succeeded";
        case result_code_t::failed:
            return "Generic Failure";
        case result_code_t::invalid_parameter:
            return "Invalid Parameter";
        case result_code_t::invalid_pointer:
            return "Invalid Pointer operation";
        case result_code_t::not_found:
            return "Not Found";
        case result_code_t::not_implemented:
            return "Not implemented";
        case result_code_t::not_initialized:
            return "Not initialized";
        case result_code_t::out_of_memory:
            return "Out of Memory";
        case result_code_t::out_of_bound:
            return "Not in valid range of data";
        case result_code_t::rendering_device_removed:
            return "Rendering device has been removed";
        default:
            break;
        }

        return "Unknown error";
    }
}
