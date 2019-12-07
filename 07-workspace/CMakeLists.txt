cmake_minimum_required(VERSION 3.14)

project(HelloWorld)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/hello)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/world)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/main)
