cmake_minimum_required(VERSION 3.14)

# Prepare cmake variable
set(CMAKE_CXX_STANDARD 14)
set(EMSCRIPTEN_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR}/../../emsdk/upstream/emscripten)

# Found header files & source files
file(GLOB_RECURSE TARGET_HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB_RECURSE TARGET_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

# Create default wasm target
add_executable(
    AlgorithmSDK
    ${TARGET_HEADERS}
    ${TARGET_SOURCES}
)

# Add C++ library as denpendencies
add_dependencies(
    AlgorithmSDK
    Algorithm
)

# Add C++ library & Emscripten include path for header include
target_include_directories(
    AlgorithmSDK
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../algorithm/include
    PUBLIC ${EMSCRIPTEN_ROOT_PATH}/system/include
)

# Add C++ library as link libraries
target_link_libraries(
    AlgorithmSDK
    Algorithm
)

# Add build flags
# ------------------------------------------- #
# --------------- IMPORTANT ————————————————— #
# ------------------------------------------- #
# issue:
# 1. --bind compile flag used for Embind command
# 2. target_compile_options not work for Emscripten toolchain (May be is a bug)
# tips:
# 1. Emscripten simultaneously use emcc as compiler & link tool
# 2. Tips-1 may the reason why cmake complie options & flag not work well
# 3. Flags can passed by target_link_options or set_target_properties LINK_FLAGS
# 4. Target AlgorithmSDK_Sync below will use set_target_properties LINK_FLAGS
target_link_options(
    AlgorithmSDK BEFORE
    PUBLIC "--bind"
)


# Create sychronize wasm target
add_executable(
    AlgorithmSDK_Sync
    ${TARGET_HEADERS}
    ${TARGET_SOURCES}
)

# Add C++ library as denpendencies
add_dependencies(
    AlgorithmSDK_Sync
    Algorithm
)

# Add C++ library & Emscripten include path for header include
target_include_directories(
    AlgorithmSDK_Sync
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../algorithm/include
    PUBLIC ${EMSCRIPTEN_ROOT_PATH}/system/include
)

# Add C++ library as link libraries
target_link_libraries(
    AlgorithmSDK_Sync
    Algorithm
)

# Add build flags for sychronize target
# Tips: 
# 1. WASM_ASYNC_COMPILATION=0 is not recorded in complie options
#    ref: https://github.com/emscripten-core/emscripten/blob/master/src/settings.js
# 2. WASM_ASYNC_COMPILATION is mentioned at <Building to WebAssembly>
#    ref: https://emscripten.org/docs/compiling/WebAssembly.html
# Document Abstract:
# Another noticeable effect is that WebAssembly is compiled asynchronously by default...
# ......
# You can turn off async compilation by setting WASM_ASYNC_COMPILATION=0,
# but that may not work in Chrome due to current limitations there.
set_target_properties(
    AlgorithmSDK_Sync PROPERTIES
    LINK_FLAGS  "--bind -s WASM_ASYNC_COMPILATION=0"
)
