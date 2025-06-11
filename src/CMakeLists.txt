set(HEADERS
    "${PROJECT_SOURCE_DIR}/include/whapi/whapi.h"
    "${PROJECT_SOURCE_DIR}/include/whapi/whdef.h"
    "${PROJECT_SOURCE_DIR}/include/whapi/whstr.h"
    "${PROJECT_SOURCE_DIR}/include/whapi/whparsed.h"
    "${PROJECT_SOURCE_DIR}/include/whapi/whraw.h"
    "${PROJECT_SOURCE_DIR}/include/whapi/whrawfile.h"
    "${PROJECT_SOURCE_DIR}/include/whapi/whcore.h"
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

if(WHAPI_BUILD_STATIC)
    add_library(whapi STATIC)
    target_compile_definitions(whapi PRIVATE WH_STATIC)
else()
    add_library(whapi SHARED)
    target_compile_definitions(whapi PRIVATE WH_EXPORT)
    set_target_properties(whapi PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
    )
endif()

target_include_directories(whapi
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>
)

target_link_libraries(whapi PRIVATE whapi_flags)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(whapi PRIVATE _CRT_SECURE_NO_WARNINGS)

    find_package(CURL CONFIG REQUIRED)

    target_link_libraries(whapi PRIVATE CURL::libcurl)
else()
    target_link_libraries(whapi PRIVATE curl)
endif()

target_sources(whapi PRIVATE ${HEADERS} ${SRCS})

install(TARGETS whapi
    EXPORT whapiTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/whapi
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT whapiTargets
    FILE whapiTargets.cmake
    NAMESPACE whapi::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/whapi
)
