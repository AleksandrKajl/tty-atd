cmake_minimum_required(VERSION 3.16)

project(atd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(atd
    src/main.cpp
    src/command_dictionary.cpp
    src/pattern_matcher.cpp
    src/serial_port.cpp
    src/server.cpp
)

target_include_directories(atd PRIVATE include)
