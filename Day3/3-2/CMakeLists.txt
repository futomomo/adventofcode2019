cmake_minimum_required(VERSION 3.13)
project(mainapp LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_C_STANDARD 11)
option(RELEASE_BUILD "Make a release build, otherwise defaults to debug." OFF)

add_executable(mainapp "")

target_compile_options(mainapp PUBLIC "-D_CRT_SECURE_NO_WARNINGS")

add_subdirectory(src)

set_target_properties(mainapp
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
	)


if(EXISTS "${CMAKE_SOURCE_DIR}/input.txt")
	configure_file("${CMAKE_SOURCE_DIR}/input.txt" "${CMAKE_BINARY_DIR}/bin/input.txt" COPYONLY)
endif()


if(WIN32)
	add_custom_target(ccmds
		COMMAND copy build\\compile_commands.json .\\
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Copying compile_commands.json to source dir."
		VERBATIM)
endif()
