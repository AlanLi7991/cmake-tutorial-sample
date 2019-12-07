title: CMake02-编译配置Debug或Release
date: 2019-12-02 00:00:00
categories: Tutorial
tags: [CMake]

---


## 影响编译行为的配置

CMake会[影响编译行为的参数][link01]有很多，其中最常用的就是 CMAKE_BUILD_TYPE

可能的值包括空值、非空值Debug, Release, RelWithDebInfo, MinSizeRel 等

**其中Debug和Release是最常用的两个选项，直接影响到了可不可以打断点调试**

## 如何配置 CMAKE_BUILD_TYPE

配置 CMAKE_BUILD_TYPE 有两种方式

1. 在cmake的配置指令 “cmake .” 阶段通过 -D 传入该参数
2. 在CMakeList.txt里写入 SET(CMAKE_BUILD_TYPE "Debug") 指定该参数

## 在配置阶段传入

以HelloWorld的的举例

```SH
 #切换到文件夹
cd HelloWorld
#cmake指令，帮助用户生成 makefile 文件 build.make
cmake . -D CMAKE_BUILD_TYPE="Debug"
#make指令， 帮助用户完成编译的调度
make .
 #运行 HelloWorld 程序 
./main      
```

注意在 -D 指令传入参数时有多种方法，以下都是正确的

1. -D 之后是否加空格
2. 赋值采用 = 或 : 

```SH
cmake . -D CMAKE_BUILD_TYPE="Debug"
cmake . -D CMAKE_BUILD_TYPE:"Debug"
cmake . -DCMAKE_BUILD_TYPE="Debug"
cmake . -DCMAKE_BUILD_TYPE:"Debug"
```


[link01]:https://cmake.org/cmake/help/v3.15/manual/cmake-variables.7.html#variables-that-change-behavior