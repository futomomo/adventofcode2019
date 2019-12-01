cmake_minimum_required(VERSION 3.13)
project(aocday1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No default build type set, setting build type to ${default_build_type}.")
    set(CMAKE_BUILD_TYPE "${default_build_type}")
ENDIF()

add_executable(aocday1 "")
add_subdirectory(src)

set_target_properties(aocday1
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_compile_options(aocday1 PUBLIC "-D_CRT_SECURE_NO_WARNINGS")


message(STATUS "CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "CMAKE_C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG}")
