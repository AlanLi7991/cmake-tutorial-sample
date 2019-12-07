title: CMake03-添加文件和产物
date: 2019-12-03 00:00:00
categories: Tutorial
tags: [CMake]

---

## 源文件和Executable

根据编译基本概念，每一个可执行程序（Executable，Target、Package）都是由许多的源文件组成

CMake很直接的表达， 参数分别为 **（名称 平台标记 源文件可变参数）** ，平台标记一般可以先不考虑

```
# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL] [source1] [source2 ...])
add_executable(HelloWorld main.cpp)
```

## 添加头文件

添加头文件有两个命令，一个在2.8.10之后生效，更加突出了Target的概念

```SH
# 2.8.10之前
include_directories([AFTER|BEFORE] [SYSTEM] dir1 [dir2 ...])

# 2.8.10之后
target_include_directories(<target> [SYSTEM] [BEFORE]
  <INTERFACE|PUBLIC|PRIVATE> [items1...]
  [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])

```

在 target_include_directories 最大的改变我们可以对头文件设置分类

PRIVATE 和 PUBLIC 会被放入Target的变量 INCLUDE_DIRECTORIES，PUBLIC 和 INTERFACE 则会被放入 INTERFACE_INCLUDE_DIRECTORIES 变量

## 文件太多怎么处理

当时用到添加源文件和头文件的命令时，随着工程增大会逐渐发现两个问题

1. 文件数目越来越多，比较难以维护CMakeLists.txt
2. 文件结构越来越复杂，目录层级变得多

为了解决这两个问题，我们可以使用 file 命令的GLOB功能

```SH
# 非递归查找
file(GLOB <variable>
     [LIST_DIRECTORIES true|false] [RELATIVE <path>] [CONFIGURE_DEPENDS]
     [<globbing-expressions>...])

# 递归查找
file(GLOB_RECURSE <variable> [FOLLOW_SYMLINKS]
     [LIST_DIRECTORIES true|false] [RELATIVE <path>] [CONFIGURE_DEPENDS]
     [<globbing-expressions>...])
```

例如我们递归选出所有的cpp文件，其中使用到了一些其他知识

1. CMAKE_CURRENT_LIST_DIR: 当前CMakeLists.txt所处于的目录，注意和 PROJECT_SOURCE_DIR 的区别
2. message: 相当于日志方法，可以接受可变参数

```SH
# 递归查找当前 CMakeLists.txt 文件下 source 内的以 cpp 结尾的文件

file(GLOB_RECURSE CPP_FILES ${CMAKE_CURRENT_LIST_DIR}/source/*.cpp)
message("Glob List file : " ${CPP_FILES} "\n")

add_executable(HelloWorld ${CPP_FILES})
```