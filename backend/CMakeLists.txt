cmake_minimum_required(VERSION 3.20)

project(BACKEND VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE DEBUG)

include(cmake/SourcesAndHeaders.cmake)
include(cmake/CompilerWarnings.cmake)

include(FetchContent)

add_compile_options(${PROJECT_WARNINGS})
add_compile_options(-std=c++20)

find_package(Boost REQUIRED COMPONENTS system)

if (Boost_FOUND)
    FetchContent_Declare(spdlog 
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.15.1
    )
    FetchContent_MakeAvailable(spdlog)

    message("Found the following sources:")
    foreach(source IN LISTS sources)
        message("* ${source}")
    endforeach()

    add_executable(back src/server.cpp src/main.cpp src/util.cpp)    

    target_include_directories(back PRIVATE ${Boost_INCLUDE_DIRS}) 
    target_link_libraries(back PRIVATE ${Boost_LIBRARIES})
    target_include_directories(back SYSTEM PRIVATE ${spdlog_SOURCE_DIR}/include)

    target_include_directories(back PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif()    
