cmake_minimum_required(VERSION 4.00)
project(PaleontologistSimulator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")
file(GLOB SOURCES
        "${CMAKE_SOURCE_DIR}/main.cpp"
        "${CMAKE_SOURCE_DIR}/src/*.cpp"

)

add_executable(PaleontologistSimulator ${HEADERS} ${SOURCES}
        src/Game.cpp)
target_compile_features(PaleontologistSimulator PRIVATE cxx_std_17)
target_include_directories(PaleontologistSimulator PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(PaleontologistSimulator PRIVATE -Wall -Wextra -pedantic)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(PaleontologistSimulator PRIVATE -O3)
endif()