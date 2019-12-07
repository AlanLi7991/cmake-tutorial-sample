title: CMake07-如何多工程协作
date: 2019-12-07 00:00:00
categories: Tutorial
tags: [CMake]

---

## 工作台的问题

在编译的基本概念里有提到一个管理器空间，iOS叫Workspace，Android叫Project

CMake本身使用Directory作为管理器空间的组织者，通过SubDirectory来组织不同的CMake项目

## 子目录

```SH
——root
    ——build
    ——main
        ——CMakeLists.txt
    ——hello
        ——CMakeLists.txt
    ——world
        ——CMakeLists.txt
    ——CMakeLists.txt
```

### 目录helloworld - CMakeLists.txt

“helloworld”这个顶级文件夹作为协调不同工程的工作空间

```SH
# 指定CMake最小版本
cmake_minimum_required(VERSION 3.14)

# 该工程为Workspace，不包含任何 target，仅起协调作用
project(HelloWorld)

# 内部协调的工程分别位于文件夹 hello、world、main
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/hello)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/world)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/main)

```

### 目录main - CMakeLists.txt

“main”工程为主程序，其依赖两个子工程分别是 hello 和 world

```SH
# 指定CMake最小版本
cmake_minimum_required(VERSION 3.14)

# hello_world内包含一个可执行文件的target
# target 的名称为 main
# 对比：
# iOS的习惯里 一般project名和target同名
# Android的习惯里 module名下可以有多个package名
add_executable(main main.cpp)

# 依赖关系为 main 依赖 hello、world两个项目产生的库
add_dependencies(
    main  # 名称来自 main - CMakeLists.txt 的 add_executable
    hello # 名称来自 hello - CMakeLists.txt 的 add_library
    world # 名称来自 world - CMakeLists.txt 的 add_library
    )

# 给main target添加头文件目录
# 帮助我们寻找头文件 #include<.h>
# CMAKE_SOURCE_DIR 指的是顶级目录 helloworld
target_include_directories(
    main
    PUBLIC ${CMAKE_SOURCE_DIR}/hello
    PUBLIC ${CMAKE_SOURCE_DIR}/world
)

# 给main target添加依赖库
# 帮助调用库中的函数
target_link_libraries(
    main
    hello
    world
)
```

### 目录依赖库 - CMakeLists.txt

其中 “hello” 和 “world” 均为我们的依赖库，使用的CMakeLists.txt文件主体结构十分类似

```SH
# 指定CMake最小版本
cmake_minimum_required(VERSION 3.14)

# 一个静态库的target 名称为 hello
# 经过workspace协调后可以被main引用
add_library(hello hello.cpp)
```
更多的源代码可以参考CMake07的源代码


## 目录并不在文件系统子目录内

在当前的举例中 hello 和 world 都在文件系统内实际为子目录，如果存在不为文件系统子目录的情况, 根据文档描述

```SH
 If binary_dir is not specified, the value of source_dir, before expanding any relative path, will be used (the typical usage).
```

所以如果我们想使用相对路径的 source_dir，可以通过以下方式指定 binary_dir

```SH
# 相当于 add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/hello  ${CMAKE_CURRENT_BINARY_DIR}/hello)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/hello)
# 参数1不为实际的子目录，只需要指定 BINARY_DIR 即可
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../outside/world ${CMAKE_CURRENT_BINARY_DIR}/world)
```
