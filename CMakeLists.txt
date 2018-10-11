cmake_minimum_required(VERSION 3.5.0)

project(ING LANGUAGES C CXX)

set(ING_PROJECT_DIR ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(INGEngine)

add_subdirectory(Examples)
