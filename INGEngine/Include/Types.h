//----------------------------------------------------------------------------------
// Types.h : Defines the common data type definitions.
//----------------------------------------------------------------------------------
#pragma once

namespace ING
{
	enum class result_code_t
	{
		succeeded = 0,
		failed,
		invalid_parameter,
		invalid_pointer,
		not_found,
		not_implemented,
        not_initialized,
        out_of_bound,
        out_of_memory,

		rendering_device_removed,

		count
	};

#ifdef _WIN32
    // Windows dependent types
    typedef HWND window_t;
    typedef IUnknown* core_window_t;
#else
    // ToDo: Define other platform dependent types
#endif

    namespace Graphics
    {
        ///
        /// Type of Graphics implementation
        ///
        enum class GraphicsType
        {
            Unknown = 0,
            DX12
        };

    }
}

