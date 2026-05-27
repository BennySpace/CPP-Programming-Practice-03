cmake_minimum_required(VERSION 3.16)
project(FormulaOneSimulator VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(HEADERS
        ${CMAKE_SOURCE_DIR}/include/game.h
        ${CMAKE_SOURCE_DIR}/include/item.h
        ${CMAKE_SOURCE_DIR}/include/player.h
        ${CMAKE_SOURCE_DIR}/include/probabilities.h
        ${CMAKE_SOURCE_DIR}/include/race.h
        ${CMAKE_SOURCE_DIR}/include/race_monaco.h
        ${CMAKE_SOURCE_DIR}/include/race_monza.h
        ${CMAKE_SOURCE_DIR}/include/race_spa.h
        ${CMAKE_SOURCE_DIR}/include/nlohmann/json.hpp
)
set(SOURCES
        ${CMAKE_SOURCE_DIR}/src/main.cpp
        ${CMAKE_SOURCE_DIR}/src/game.cpp
        ${CMAKE_SOURCE_DIR}/src/player.cpp
        ${CMAKE_SOURCE_DIR}/src/race.cpp
        ${CMAKE_SOURCE_DIR}/src/race_monaco.cpp
        ${CMAKE_SOURCE_DIR}/src/race_monza.cpp
        ${CMAKE_SOURCE_DIR}/src/race_spa.cpp
)

add_executable(FormulaOneSimulator ${HEADERS} ${SOURCES})
target_compile_features(FormulaOneSimulator PRIVATE cxx_std_17)
target_include_directories(FormulaOneSimulator PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(MSVC)
    target_compile_options(FormulaOneSimulator PRIVATE /W4 /permissive-)
else()
    target_compile_options(FormulaOneSimulator PRIVATE -Wall -Wextra -pedantic)
endif()
