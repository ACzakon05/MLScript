cmake_minimum_required(VERSION 3.10)
project(MLScriptCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_path(ANTLR4_INCLUDE_DIR NAMES antlr4-runtime.h PATH_SUFFIXES antlr4-runtime)
find_library(ANTLR4_LIB NAMES antlr4-runtime)

if(NOT ANTLR4_INCLUDE_DIR OR NOT ANTLR4_LIB)
    message(FATAL_ERROR "ANTLR4 C++ runtime not found on this machine! Please install it.")
endif()

include_directories(${ANTLR4_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/generated)
include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/generated/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(mlscript ${SOURCES})
target_link_libraries(mlscript ${ANTLR4_LIB})