cmake_minimum_required(VERSION 3.10)

project(FULLSTACK VERSION 1.0.0 LANGUAGES CXX)

include(cmake/StandardSettings.cmake)
message(${PROJECT_ENABLE_CLANG_TIDY})
message(${PROJECT_ENABLE_UNIT_TESTING})
message(${PROJECT_ENABLE_ASAN})
include(cmake/StaticAnalyzer.cmake)
include(cmake/Utils.cmake)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

message(STATUS "Started CMake for ${PROJECT_NAME} v${PROJECT_VERSION}...\n")

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.\n")
endif()

# Create library, setup header and source file
include(cmake/SourcesAndHeaders.cmake)

if(PROJECT_ENABLE_UNIT_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
add_subdirectory(src)

# Set the project standard and warnings
include(cmake/CompilerWarnings.cmake)
add_compile_options(--std=c++20)
add_compile_options(${PROJECT_WARNINGS})

# Format the project using the `clang-format` target (i.e: cmake --build build --target clang-format)
add_clang_format_target()
