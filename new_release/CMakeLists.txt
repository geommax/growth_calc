cmake_minimum_required(VERSION 3.10)
project(growth_calc_modular CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
    src/config.cpp
    src/print_utils.cpp
    src/thread.cpp
    src/growth_calc.cpp
)

add_executable(growth_calc_modular
    main.cpp
    ${SOURCES}
)
