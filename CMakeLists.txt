cmake_minimum_required(VERSION 3.15...3.31)

project(wallhavenapi VERSION 1.1.1 DESCRIPTION "Wallhaven API client library in C" LANGUAGES C)

add_library(whapi_flags INTERFACE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(whapi_flags INTERFACE WH_DEBUG)
endif()

set(gcc_clang "$<COMPILE_LANG_AND_ID:C,Clang,GNU>")
set(msvc_comp "$<COMPILE_LANG_AND_ID:C,MSVC>")

target_compile_features(whapi_flags INTERFACE c_std_17)

target_compile_options(whapi_flags INTERFACE "$<${gcc_clang}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wformat;-Wuninitialized;-Wpedantic>>")

target_compile_definitions(whapi_flags
    INTERFACE
    WHAPI_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    WHAPI_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    WHAPI_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)

target_compile_options(whapi_flags INTERFACE "$<${gcc_clang}:-fvisibility=hidden>")

include(GNUInstallDirs)

option(WHAPI_BUILD_STATIC "Build static library" OFF)

add_subdirectory(src)
add_subdirectory("test")
add_subdirectory(docs)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/whapiConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/whapiConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/whapiConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/whapi
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/whapiConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/whapiConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/whapi
)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/whapi.pc.in
        ${CMAKE_CURRENT_BINARY_DIR}/whapi.pc
        @ONLY
    )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/whapi.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    )
endif()
