cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)

project(HelloOpenCV)

add_executable(find_opencv main.cpp)

find_package(OpenCV REQUIRED)
target_include_directories(
    find_opencv 
    PRIVATE 
    ${OpenCV_INCLUDE_DIRS}
    )

target_link_libraries(
    find_opencv 
    ${OpenCV_LIBS}
    )

