# Define the list of test files
set(TEST_FILES
        darray_test
        list_test
)

# Handle all test files in one loop
foreach(test ${TEST_FILES})
    add_executable(${test} ${test}.c)
    target_link_libraries(${test}
            PRIVATE
            libfaafo
            unity
    )
    add_test(NAME ${test} COMMAND ${test})
endforeach()
