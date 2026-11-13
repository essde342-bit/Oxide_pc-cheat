cmake_minimum_required(VERSION 3.18)
project(oxide_cheat LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(MINHOOK_DIR ${CMAKE_SOURCE_DIR}/../include/minhook)

add_library(oxide_cheat SHARED
    src/main.cpp
    src/hooks.cpp
    src/renderer.cpp
    src/esp.cpp
    src/menu.cpp
    ${MINHOOK_DIR}/src/buffer.c
    ${MINHOOK_DIR}/src/hook.c
    ${MINHOOK_DIR}/src/trampoline.c
    ${MINHOOK_DIR}/src/hde/hde32.c
    ${MINHOOK_DIR}/src/hde/hde64.c
)

target_include_directories(oxide_cheat PRIVATE
    src
    ${MINHOOK_DIR}/include
)

target_link_libraries(oxide_cheat
    log
    EGL
    GLESv3
    android
)

target_compile_options(oxide_cheat PRIVATE -Wall -O2 -fvisibility=hidden)
