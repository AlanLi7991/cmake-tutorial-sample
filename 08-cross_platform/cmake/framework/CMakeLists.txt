cmake_minimum_required(VERSION 3.14)

# Prepare cmake variable
set(MACOSX_FRAMEWORK_IDENTIFIER "github.alanli7991.sdk")    
set(MACOSX_FRAMEWORK_NAME "AlgorithmSDK")    
set(MACOSX_FRAMEWORK_BUNDLE_VERSION "1.0.0")    
set(MACOSX_FRAMEWORK_SHORT_VERSION_STRING "1.0.0")

# Found header files & source files
file(GLOB_RECURSE TARGET_HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB_RECURSE TARGET_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.mm)

# (Optional) Group the files with directory struct
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${TARGET_HEADERS})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${TARGET_SOURCES})

# Create target as framework
# Comment: 
# Even tough header files can not be compiled
# They should add here for GUI generator present(eg. Xcode)
add_library(
    ${MACOSX_FRAMEWORK_NAME} SHARED
    ${TARGET_HEADERS}
    ${TARGET_SOURCES}
)

# Add C++ library as denpendencies
add_dependencies(
    ${MACOSX_FRAMEWORK_NAME}
    Algorithm
)

# Find iOS platform system library assign to APPLE_UIKit & APPLE_CoreGraphics
# Comment:
# If CMAKE_SYSTEM_NAME is Apple would not find UIKit
find_library(APPLE_UIKit NAMES UIKit)
find_library(APPLE_CoreGraphics NAMES CoreGraphics)

# Add C++ library include path for header include
target_include_directories(
    ${MACOSX_FRAMEWORK_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../algorithm/include
)

# Add link libraries
# ------------------------------------------- #
# --------------- IMPORTANT ————————————————— #
# ------------------------------------------- #
# Algorithm :
# issue:
# 1. MUST use library string name "libAlgorithm.a"
# 2. Do not use cmake add_library() target name Algorithm
# reason:
# 1. CMake will use Xcode generator result a project
# 2. Next step set_target_properties will change "XCODE_ATTRIBUTE_SYMROOT"
# 3. Step 2 will make Xcode build binary directory like default
# 4. Xcode default binary dir not equal to CMAKE_BINARY_DIR
# 5. Use "Xcode-Run" will compile Algorithm.a to Xcode build binary dir
# 6. if use "PUBLIC Algorithm" Xcode will link C++ library at CMAKE_BINARY_DIR
# 7. Step 6 result make AlgorithmSDK compile error when as dependency for Render.xcodeproj
# 8. Compile error is project can not found Algorithm.a caused by wrong directory
# 9. !!!! It is complicated, but !!!!
# 10. You can try both of them and see "Xcode-Build Settings-OTHER_LDFLAGS" different
# reference: https://cmake.org/cmake/help/latest/command/target_link_libraries.html
#
# UIKit & CoreGraphics:
# issue:
# 1. Two ways for add framework
# 2. find_library(APPLE_UIKit NAMES UIKit) and PRIVATE ${APPLE_UIKit}
# 3. PRIVATE "-framework UIKit" 
# reason:
# 1. issue-2 is found library first and add
# 2. issue-3 is just add string for link
target_link_libraries(
    ${MACOSX_FRAMEWORK_NAME}
    PUBLIC libAlgorithm.a
    PRIVATE ${APPLE_UIKit}
    PRIVATE ${APPLE_CoreGraphics}
)

# Set Xcode "Build Settings"
# Make generated as manual create
# Take it can be dependency with source code  
set_target_properties(
    AlgorithmSDK PROPERTIES
    # Make target is framework
    FRAMEWORK TRUE
    FRAMEWORK_VERSION C
    MACOSX_FRAMEWORK_IDENTIFIER ${MACOSX_FRAMEWORK_IDENTIFIER}
    # Setting info.plist templete use MACOSX_* variables above
    MACOSX_FRAMEWORK_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/Info.plist.in
    # Mark which headers should public
    PUBLIC_HEADER ${TARGET_HEADERS}
    ## ----------------------------------
    ## Architecture
    ## ----------------------------------
    XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH[variant=Debug] "YES"
    XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH[variant=Release] "NO"
    ## ----------------------------------
    ## Build Location (!!Must see IMPORTANT above)
    ## ----------------------------------
    XCODE_ATTRIBUTE_SYMROOT "build"
    ## ----------------------------------
    ## Build Option
    ## ----------------------------------
    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Debug] "dwarf"
    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Release] "dwarf-with-dsym"
    ## ----------------------------------
    ## Deployment
    ## ----------------------------------
    # Enable iPhone,iPad, cmake default is iPhone
    XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
    # Enable framework can archive, cmake default is NO
    XCODE_ATTRIBUTE_SKIP_INSTALL "YES"
    XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_LIBRARY_DIR)/Frameworks"
    ## ----------------------------------
    ## Link
    ## ----------------------------------
    XCODE_ATTRIBUTE_DYLIB_INSTALL_NAME_BASE "@rpath"
    XCODE_ATTRIBUTE_LD_DYLIB_INSTALL_NAME "$(DYLIB_INSTALL_NAME_BASE:standardizepath)/$(EXECUTABLE_PATH)"
    XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "$(inherited) @executable_path/Frameworks @loader_path/Frameworks"
    XCODE_ATTRIBUTE_DYLIB_COMPATIBILITY_VERSION "1"
    XCODE_ATTRIBUTE_DYLIB_CURRENT_VERSION "1"
    ## ----------------------------------
    ## Packaging
    ## ----------------------------------
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER ${MACOSX_FRAMEWORK_IDENTIFIER}
    ## ----------------------------------
    ## Apple Clang - Language - C++
    ## ----------------------------------
    XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "gnu++14"
    XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++"
    ## ----------------------------------
    ## Apple Clang - Language - Modules
    ## ----------------------------------
    XCODE_ATTRIBUTE_CLANG_ENABLE_MODULES "YES"
    ## ----------------------------------
    ## Apple Clang - Language - Objective-C
    ## ----------------------------------
    # Enable ARC, cmake default is MRC
    XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC "YES"
    XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_WEAK "YES"
    ## ----------------------------------
    ## Apple Clang - Language - Code Generation
    ## ----------------------------------
    XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS "YES"
)


