file(GLOB sources
    src/*.cpp
)

list(REMOVE_ITEM sources src/main.cpp)

file(GLOB exe_sources
    src/main.cpp
)

file(GLOB headers
    src/*.hpp
)

# set(test_sources
#   test/hello_test.cpp
# )
