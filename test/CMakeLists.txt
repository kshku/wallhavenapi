set(SRCS
    "test.c"
    test_parsed.c
    test_raw_to_file.c
    test_raw.c
    common.h
)

add_executable(whapitest)

target_sources(whapitest PRIVATE ${SRCS})

target_link_libraries(whapitest PRIVATE whapi)

target_include_directories(whapitest PRIVATE "${CMAKE_SOURCE_DIR}/include")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_custom_target(copy_dlls ALL
        COMMENT "Copying dlls"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_RUNTIME_DLLS:whapitest>" "$<TARGET_FILE_DIR:whapitest>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_RUNTIME_DLL_DIRS:whapitest>/zlibd1.dll" "$<TARGET_FILE_DIR:whapitest>"
        COMMAND_EXPAND_LISTS
    )
endif()
