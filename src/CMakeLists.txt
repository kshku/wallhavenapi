set(HEADERS
    "${CMAKE_SOURCE_DIR}/include/whapi/whapi.h"
)

set(SRCS
    whapi.c
    whstr.h
    whstr.c
)

add_library(whapi_static STATIC)
set_target_properties(whapi_static PROPERTIES OUTPUT_NAME "whapi")

add_library(whapi_shared SHARED)
set_target_properties(whapi_shared PROPERTIES OUTPUT_NAME "whapi")

target_compile_definitions(whapi_shared PRIVATE WH_EXPORT)

target_include_directories(whapi_static PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_include_directories(whapi_shared PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(whapi_static PRIVATE whapi_flags)
target_link_libraries(whapi_shared PRIVATE whapi_flags)

target_link_libraries(whapi_static PRIVATE curl)
target_link_libraries(whapi_shared PRIVATE curl)

target_sources(whapi_static PRIVATE ${HEADERS} ${SRCS})
target_sources(whapi_shared PRIVATE ${HEADERS} ${SRCS})
