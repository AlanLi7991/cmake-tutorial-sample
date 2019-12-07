title: CMake06-加载文件进行复用
date: 2019-12-06 00:00:00
categories: Tutorial
tags: [CMake]

---

## CMake文件种类

CMake除了配置文件CMakeList.txt 之外，**还可以通过include的方式加载文件** 类似于头文件

## 加载公用函数

最常见的使用方法就是用来加载一些公用的函数

```SH
include(MultiplexingFunc.cmake)
```

## 根据平台不同组织逻辑

还可以根据内部逻辑来组织CMake代码，例如不同的平台加载不同的文件

```SH
if(IOS)
    include(CMakeLists_iOS.cmake)
else(APPLE)
    include(CMakeLists_Apple.cmake)
endif()
```
其中 IOS 和 APPLE 变量来自于[CMake官方文档Variables that Describe the System](https://cmake.org/cmake/help/v3.15/manual/cmake-variables.7.html#variables-that-describe-the-system)



