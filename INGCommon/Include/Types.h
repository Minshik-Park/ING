//----------------------------------------------------------------------------------
// Types.h : Defines the common data type definitions.
//----------------------------------------------------------------------------------
#pragma once

namespace ING
{
	enum class result_code_t
	{
		result_succeeded = 0,
		result_failed,
		result_invalid_parameter,
		result_invalid_pointer,
		result_not_found,
		result_not_implemented,
        result_not_initialized,
        result_out_of_bound,
        result_out_of_memory,

		result_rendering_device_removed,

		result_count
	};
}

