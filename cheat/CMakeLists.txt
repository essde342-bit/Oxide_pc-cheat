cmake_minimum_required(VERSION 3.10...3.31)
project(oxide_cheat LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(oxide_cheat SHARED
    src/main.cpp
)

target_link_libraries(oxide_cheat
    log
    EGL
    GLESv3
    android
)

target_compile_options(oxide_cheat PRIVATE -O2 -fvisibility=hidden)
