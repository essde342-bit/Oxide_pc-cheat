cmake_minimum_required(VERSION 3.18)
project(oxide_injector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(injector injector.cpp)
add_executable(cleanup cleanup.cpp)
