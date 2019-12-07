title: CMake08-跨平台编译iOS实践细节
date: 2019-12-08 12:00:00
categories: Tutorial
tags: [CMake]

---

## iOS平台实践细节

推荐实践路线

1. application: 推荐手动建立工程，方便团队里iOS工程师使用
2. sdk: 使用cmake生成，并设置成同手动工程配置，方便application工程师开箱即用
3. c++: 使用cmake管理，并通过sdk层引用

### iOS的Info.plist如何处理

iOS平台上的Product常见一个Info.plist文件用于描述基本信息

CMake提供了两个基本模版

1. MacOSXFrameworkInfo.plist.in
2. MacOSXBundleInfo.plist.in

并且可以通过设置一些环境变量填入这个模版, 参考[官方文档][link03]

```SH
set(MACOSX_FRAMEWORK_IDENTIFIER "demo.alanli")    
set(MACOSX_FRAMEWORK_NAME "AlgorithmSDK")    
set(MACOSX_FRAMEWORK_BUNDLE_VERSION "1.0.0")    
set(MACOSX_FRAMEWORK_SHORT_VERSION_STRING "1.0.0")
```

也可以用户模仿官方模版自己指定模版，并且指定路径

```SH
set_target_properties(
    ....
    MACOSX_FRAMEWORK_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/Info.plist.in
    ....
)
```

[link03]:https://cmake.org/cmake/help/v3.15/prop_tgt/MACOSX_FRAMEWORK_INFO_PLIST.html


### Xcode Generate 生成的工程和手工不一致

在使用Xcode Genreate的时候生成的工程有些选项和手工是不一致的，导致不能当成一个正常的Project来用

两个主要的问题

* Framework不能进行Archive操作
* Dependency和LinkLibrary的关系

解决方案分别是

* 手工生成的Project的Build Setting - SKIP_INSTALL 是YES， 而CMake生成的是 NO
* Xcode的Run指令会自动link已经dependency的target，不需要CMake的指令再link一遍，如果不是Xcode Generate则需要CMake进行Link


列出一份基本的配置表，帮助我们可以还原Xcode11 手动创建的工程配置, 更多细节可以参考源代码

```SH
set_target_properties(
    AlgorithmSDK PROPERTIES
    # Make target is framework
    FRAMEWORK TRUE
    FRAMEWORK_VERSION C
    MACOSX_FRAMEWORK_IDENTIFIER ${MACOSX_FRAMEWORK_IDENTIFIER}
    MACOSX_FRAMEWORK_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/Info.plist.in
    # Mark which headers should public
    PUBLIC_HEADER ${TARGET_HEADERS}
    ## ----------------------------------
    ## Architecture
    ## ----------------------------------
    XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH[variant=Debug] "YES"
    XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH[variant=Release] "NO"
    ## ----------------------------------
    ## Build Location
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

```

默认是MAC 要设置成iOS

### 已知的错误实践

由于CMake可以使用Xcode作为Generator，所以大部分开发者会有生成一个TestApp的想法，及通过

```SH
add_executable(test_app ${Headers} ${Sources})
```

虽然CMake支持这个功能，但是对于一些细节问题处理的不够好，有三个问题会依次出现

1. 通过 cmake --build . 指令编译失败
2. 签名问题比较复杂，如果多团队使用不同的证书，通用性比较差
3. 无法生成Asset.xcassets 文件，可以通过Resource管理图片，不方便不懂CMake的开发队友

解决这三个问题引入的方法复杂度远高于三个问题本身，所以不推荐使用CMake生成测试用App，尽量采用示例代码的结构

### 签名问题

根据CMake文档[FRAMEWORK](https://cmake.org/cmake/help/v3.16/prop_tgt/FRAMEWORK.html)的示例代码

```
set_target_properties(dynamicFramework PROPERTIES
  ........
  XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
)
```

使用"iPhone Developer"的原因是因为Xcode会自动寻找合适的证书，如果不使用这种通用字符串，可以指定具体的签名文件

根据经验推荐以下两点

1. 如果你有App需要源码依赖Framework，不推荐对Framework进行签名，而统一交给App进行签名
2. 如果发布的Release产品就是一个Framework必须签名，推荐使用 "iPhone Distribution"，并设置CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM

### Swift启用

如果想启用Swift需要使用的CMake指令[][link02]

```SH
# 注意要在写add_library add_execute 之前
enable_language(Swift)
```

并且Swift因为更新的原因常常会找不到Compiler，需要更新至最新CMake，例如[No CMAKE_Swift_COMPILER could be found][link01]

[link01]:https://gitlab.kitware.com/cmake/cmake/issues/19699
[link02]:https://cmake.org/cmake/help/latest/command/enable_language.html
