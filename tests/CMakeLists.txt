# Create test executable
add_executable(list_test
        list_test.c
)

# Link the test with our library and Unity
target_link_libraries(list_test
        PRIVATE
        libfaafo
        unity
)

# Add the test
add_test(NAME ListTest COMMAND list_test)