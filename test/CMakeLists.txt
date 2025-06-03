set(SRCS
    "test.c"
)

add_executable(whapitest)

target_sources(whapitest PRIVATE ${SRCS})

target_link_libraries(whapitest PRIVATE whapi_shared)
