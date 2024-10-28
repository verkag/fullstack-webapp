find_package(Boost REQUIRED COMPONENTS system)

if (Boost_FOUND)
    if(${PROJECT_NAME}_VERBOSE_OUTPUT)
        verbose_message("Found the following sources:")
        foreach(source IN LISTS sources)
            verbose_message("* ${source}")
        endforeach()
    endif()

    add_library(PROJECT_lib SHARED ${sources})    

    target_include_directories(PROJECT_lib PRIVATE ${Boost_INCLUDE_DIRS}) 
    target_link_libraries(PROJECT_lib PRIVATE ${Boost_LIBRARIES})

    target_include_directories(PROJECT_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    add_executable(PROJECT_bin main.cpp) 
    target_link_libraries(PROJECT_bin PROJECT_lib)
endif()    
