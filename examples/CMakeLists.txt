add_executable(basic_example
        try-catch.c
        union_example.c
)

target_link_libraries(basic_example
        PRIVATE
        libfaafo
)