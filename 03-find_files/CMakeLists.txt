cmake_minimum_required(VERSION 3.14)

project(HelloWorld)

file(GLOB_RECURSE TARGET_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_executable(main ${TARGET_SOURCES})
