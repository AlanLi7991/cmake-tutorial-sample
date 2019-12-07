cmake_minimum_required(VERSION 3.14)

# Set C++ version
set(CMAKE_CXX_STANDARD 14)

# Create a SDK project
project(AlgorithmSDK)

# Add shared C++ library for every platform
add_subdirectory(algorithm)


if(IOS)
    # iOS platform 
    # 1. Use *.framework target wrapper
    # 2. Perform camke generate command with "-G Xcode"
    add_subdirectory(framework)
elseif(ANDROID)
    # Android platform
    # 1. Use AndroidLibrary target as wrapper 
    # 2. Do not need cmake generate command
    # 3. AndroidStudio refer this CMakeLists.txt
    add_subdirectory(aar)
elseif(WEBASSEMBLY)
    # Desktop & Web 
    # 1. Use *.wasm with *.js as target wrapper
    # 2. Need cmake generate command with toolchain
    # 3. Toolchain was offered by Emscripten SDK
    add_subdirectory(wasm)
endif(IOS)

