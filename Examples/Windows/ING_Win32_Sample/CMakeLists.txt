add_executable(ING_Win32_Sample WIN32 
    ING_Win32_Sample.cpp)

target_link_libraries(ING_Win32_Sample PRIVATE
    INGCore)
