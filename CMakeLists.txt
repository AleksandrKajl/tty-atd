cmake_minimum_required(VERSION 3.16)

project(atd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(atd_core)

add_executable(atd app/main.cpp)

target_link_libraries(atd PRIVATE atd_core)
