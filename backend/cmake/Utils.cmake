# Print a message only if the `VERBOSE_OUTPUT` option is on
function(verbose_message content)
    if(PROJECT_VERBOSE_OUTPUT)
        message(STATUS ${content})
    endif()
endfunction()

# Add a target for formating the project using `clang-format` (i.e: cmake --build build --target clang-format)
function(add_clang_format_target)
    message(STATUS "Configuring clang-format...")
    if(NOT PROJECT_CLANG_FORMAT_BINARY)
        find_program(PROJECT_CLANG_FORMAT_BINARY clang-format)
    endif()

    if(PROJECT_CLANG_FORMAT_BINARY)
        if(PROJECT_BUILD_EXECUTABLE)
            add_custom_target(clang-format
                        COMMAND ${PROJECT_CLANG_FORMAT_BINARY}
                        -i ${exe_sources} ${headers}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
        else()
            add_custom_target(clang-format
                        COMMAND ${PROJECT_CLANG_FORMAT_BINARY}
                        -i ${sources} ${headers}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
        endif()

        message(STATUS "Format the project using the `clang-format` target (i.e: cmake --build build --target clang-format).\n")
    else()
        message(STATUS "Clang-format not fount")
    endif()

endfunction()
