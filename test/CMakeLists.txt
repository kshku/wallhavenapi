set(SRCS
    "test.c"
    test_parsed.c
    test_raw_to_file.c
    test_raw.c
    common.h
)

add_executable(whapitest)

target_sources(whapitest PRIVATE ${SRCS})

target_link_libraries(whapitest PRIVATE whapi_shared)
