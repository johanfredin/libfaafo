# Define the list of test files
set(TEST_FILES
        linkedlist_test
        radixmap_test
        string_test
        arraylist_test
        hashmap_test
)

# Handle all test files in one loop
foreach(test ${TEST_FILES})
    add_executable(${test} ${test}.c
            testutil.h
            testutil.c)
    target_link_libraries(${test}
            PRIVATE
            libfaafo
            unity
    )
    add_test(NAME ${test} COMMAND ${test})
endforeach()
