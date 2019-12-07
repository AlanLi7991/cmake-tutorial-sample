cmake_minimum_required(VERSION 3.14)

project(HelloWorld)

include(MultiplexingFunc.cmake)

glob_sources(HEAD_FILES ${CMAKE_SOURCE_DIR})

add_executable(main main.cpp)

message("Print include outside: \n")
foreach(FILE ${HEAD_FILES})
    message(${FILE})
endforeach()
message("\n")

if(IOS)
    include(CMakeLists_iOS.cmake)
else(APPLE)
    include(CMakeLists_Apple.cmake)
endif()

