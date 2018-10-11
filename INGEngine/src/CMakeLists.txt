add_library(INGCore SHARED
    INGApi.cpp
    INGEngine.cpp)

target_include_directories(INGCore PUBLIC
    ${ING_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/include)

include(GenerateExportHeader)
generate_export_header(INGCore EXPORT_FILE_NAME "include/INGCore_export.h")
