cmake_minimum_required(VERSION 3.14)


add_executable(main main.cpp)

add_dependencies(
    main
    hello
    world
)

target_include_directories(
    main
    PUBLIC ${CMAKE_SOURCE_DIR}/hello
    PUBLIC ${CMAKE_SOURCE_DIR}/world
)

target_link_libraries(
    main
    hello
    world
)
