cmake_minimum_required(VERSION 3.31)

set(CMAKE_C_COMPILER clang)

project(libfaafo C)

set(CMAKE_C_STANDARD 11)

# Include FetchContent for downloading Unity
include(FetchContent)

# Fetch Unity testing framework
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.5.2
)
FetchContent_MakeAvailable(unity)

# Create the library
add_library(libfaafo STATIC
        include/list.h
        src/list.c
        include/dbg.h
        include/ptr_deref.h
        include/defer.h
        src/darray.c
        include/darray.h
        include/radixmap.h
        src/radixmap.c
        include/bstrlib.h
        src/bstrlib.c
        include/arraylist.h
        src/arraylist.c
        include/commons.h
        src/commons.c
        include/hashmap.h
        src/hashmap.c
)

# Set up include directories
target_include_directories(libfaafo
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Enable testing
enable_testing()

# Add the tests subdirectory
add_subdirectory(tests)

# Add custom target that just runs the tests
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running tests..."
)

# Optional: Add examples
add_subdirectory(examples)