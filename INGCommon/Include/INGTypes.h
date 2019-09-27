//----------------------------------------------------------------------------------
// INGTypes.h : Defines the common data type definitions.
//----------------------------------------------------------------------------------
#pragma once

namespace ING
{
	typedef enum
	{
		result_succeeded = 0,
		result_failed,
		result_invalid_parameter,
		result_invalid_pointer,
		result_out_of_memory,
		result_not_found,
		result_not_implemented,

		result_rendering_device_removed,

		result_count
	} result_code_t;
}

