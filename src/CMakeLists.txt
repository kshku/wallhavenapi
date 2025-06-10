set(HEADERS
    "${CMAKE_SOURCE_DIR}/include/whapi/whapi.h"
    "${CMAKE_SOURCE_DIR}/include/whapi/whdef.h"
    "${CMAKE_SOURCE_DIR}/include/whapi/whstr.h"
    "${CMAKE_SOURCE_DIR}/include/whapi/whparsed.h"
    "${CMAKE_SOURCE_DIR}/include/whapi/whraw.h"
    "${CMAKE_SOURCE_DIR}/include/whapi/whrawfile.h"
    "${CMAKE_SOURCE_DIR}/include/whapi/whcore.h"
)

set(SRCS
    whcore.c
    whstr.c
    whparsed.c
    whraw.c
    whrawfile.c
    whcommon.h
    whcommon.c
    cJSON.h
    cJSON.c
    parse_funcs.h
    parse_funcs.c
)

add_library(whapi_static STATIC)
set_target_properties(whapi_static PROPERTIES OUTPUT_NAME "whapi")
target_compile_definitions(whapi_static PRIVATE _CRT_SECURE_NO_WARNINGS)

add_library(whapi_shared SHARED)
set_target_properties(whapi_shared PROPERTIES OUTPUT_NAME "whapi")
target_compile_definitions(whapi_shared PRIVATE _CRT_SECURE_NO_WARNINGS)

target_compile_definitions(whapi_static PRIVATE WH_STATIC)
target_compile_definitions(whapi_shared PRIVATE WH_EXPORT)

target_include_directories(whapi_static PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_include_directories(whapi_shared PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_link_libraries(whapi_static PRIVATE whapi_flags)
target_link_libraries(whapi_shared PRIVATE whapi_flags)

find_package(CURL CONFIG REQUIRED)

target_link_libraries(whapi_static PRIVATE CURL::libcurl)
target_link_libraries(whapi_shared PRIVATE CURL::libcurl)

target_sources(whapi_static PRIVATE ${HEADERS} ${SRCS})
target_sources(whapi_shared PRIVATE ${HEADERS} ${SRCS})
