cmake_minimum_required(VERSION 3.8)

set(PROJECT_NAME huffman)

project(${PROJECT_NAME} C CXX)

set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror")

# Исходники
file(GLOB SOURCES
    "sources/*.h"
    "sources/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${SOURCES})
