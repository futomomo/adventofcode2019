cmake_minimum_required(VERSION 3.13)
project(day2-1 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_C_STANDARD 99)
set(default_build_type "Release")
option(RELEASE_BUILD "Make a release build, otherwise defaults to debug." OFF)

IF(RELEASE_BUILD)
  message(STATUS "Setting build type to release.")
  set(CMAKE_BUILD_TYPE "Release")
ELSE()
  message(STATUS "Setting build type to debug.")
  set(CMAKE_BUILD_TYPE "Debug")
ENDIF()

add_executable(day2-1 "")
add_subdirectory(src)

set_target_properties(day2-1
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_compile_options(day2-1 PUBLIC "-D_CRT_SECURE_NO_WARNINGS")
configure_file("${CMAKE_SOURCE_DIR}/input.txt" "${CMAKE_BINARY_DIR}/bin/input.txt" COPYONLY)


message(STATUS "CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "CMAKE_C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG}")
