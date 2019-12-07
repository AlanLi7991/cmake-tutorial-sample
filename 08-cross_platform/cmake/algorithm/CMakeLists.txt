cmake_minimum_required(VERSION 3.14)

# Create C++ library project
project(Algorithm)

# Found header files & source files
file(GLOB_RECURSE TARGET_HEADERS ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
file(GLOB_RECURSE TARGET_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

# Create C++ library target
# Comment: 
# Even tough header files can not be compiled
# They should add here for GUI present(eg. Xcode, AndroidStudio)
add_library(
    Algorithm
    ${TARGET_HEADERS}
    ${TARGET_SOURCES}
)