cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(AsciiArt)

# global settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# test executable
# add_executable(test src/test.cpp ${SRC_FILES})
# target_include_directories(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# main executable
add_executable(main src/main.cpp ${SRC_FILES})
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/")

