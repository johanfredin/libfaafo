add_executable(basic_example
        basic_example.c
)

target_link_libraries(basic_example
        PRIVATE
        libfaafo
)