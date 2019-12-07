cmake_minimum_required(VERSION 3.14)

# Found header files & source files
file(GLOB_RECURSE TARGET_HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB_RECURSE TARGET_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

# Create android library target
# Comment: 
# Even tough header files can not be compiled
# They should add here for AndroidStudio
add_library(
        AlgorithmSDK SHARED
        ${TARGET_HEADERS}
        ${TARGET_SOURCES}
)

# Add C++ library as denpendencies
add_dependencies(
        AlgorithmSDK
        Algorithm
)

# Add C++ library include path for header include
target_include_directories(
        AlgorithmSDK
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../algorithm/include
)

# Add link libraries
target_link_libraries(
        AlgorithmSDK
        Algorithm
)