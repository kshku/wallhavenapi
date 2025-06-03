cmake_minimum_required(VERSION 3.15...3.31)

project(wallhavenapi VERSION 0.1.0 DESCRIPTION "Wallhaven API implementation in C")

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

add_subdirectory(src)
add_subdirectory("test")
add_subdirectory(docs)
