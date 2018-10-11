#pragma once

namespace ING
{
    typedef enum
    {
        result_succeeded = 0,
        result_failed,
        result_invalid_parameter,
        result_invalid_pointer,
        result_not_implemented,

        result_count
    } result_code_t;

    typedef enum
    {
        renderer_dx_11 = 0,
        renderer_dx_12,
        renderer_opengl,

        renderer_count
    } renderer_type_t;
}