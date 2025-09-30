cmake_minimum_required(VERSION 4.00)
project(FormulaOneSimulator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB HEADERS
        "${CMAKE_SOURCE_DIR}/include/*.h"
        "${CMAKE_SOURCE_DIR}/include/nlohmann/json.hpp"
)
file(GLOB SOURCES
        "${CMAKE_SOURCE_DIR}/main.cpp"
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(FormulaOneSimulator ${HEADERS} ${SOURCES})
target_compile_features(FormulaOneSimulator PRIVATE cxx_std_17)
target_include_directories(FormulaOneSimulator PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(FormulaOneSimulator PRIVATE -Wall -Wextra -pedantic)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(FormulaOneSimulator PRIVATE -O3)
endif()