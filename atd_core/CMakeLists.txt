add_library(atd_core STATIC
    src/application.cpp
    src/command_dictionary.cpp
    src/pattern_matcher.cpp
    src/serial_port.cpp
    src/server.cpp
)

target_compile_features(atd_core PUBLIC cxx_std_17)

target_include_directories(atd_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
