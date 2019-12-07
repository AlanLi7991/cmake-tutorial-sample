title: CMake04-添加第三方库
date: 2019-12-04 00:00:00
categories: Tutorial
tags: [CMake]

---

## 添加库依赖

添加源文件和头文件之后，对于库的依赖可以使用 

```
target_link_libraries(<target> ... <item>... ...)
```

其中item可以使用的范围包括以下这些，其中最常用的就是库文件的路径

| 原文    | 解释    |
| :------------ | :------------ |
| A library target name             |  使用 add_library() 定义的对象     |
| A full path to a library file     |  一个库文件的路径     |
| A plain library name              |  一个库的名字，会在编译过程中默认使用，如果没有就报错     |
| A link flag                       |  一些编译标记位     |
| A generator expression            |  通配符表达式     |

## 如何找到第三方库

如何找到第三方库的路径，则是整个命令的基础，一般分为两种方式

1. 自行编译的第三方库
2. 支持CMake指令的第三方库

### 自行编译

如果是自行编译的第三方库，无非是放到一个已知的位置（例如工程目录libs下）

```
target_link_libraries(HelloWorld ${CMAKE_CURRENT_LIST_DIR}/libs/lib.a)
```

### 支持CMake指令的第三方库

CMake本身提供了一个指令叫 **find_package** 来查找存在哪些第三方库

```
# 参数列表很长此处省略
find_package(<PackageName> ..........)
```

其工作原理是根据输入的名字PackageName，全局搜索三个配置文件，来获取这个库的配置相关配置

三个配置文件分别是 FindPackageName.cmake 、PackageNameConfig.cmake、packagename-config.cmake 


## 以OpenCV举例搜索第三方库

Mac系统可以使用HomeBrew安装OpenCV, 不然只能手动编译，配置更加复杂一些

```
brew install opencv
```

### 如何查找

安装完成后在CMakeLists.txt 使用指令 find_package 即可获得两个变量

1. OpenCV_INCLUDE_DIRS 当前OpenCV库的头文件目录
2. OpenCV_LIBS 当前OpenCV的所有库

```
find_package(OpenCV REQUIRED)

target_include_directories(HelloWorld PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(HelloWorld ${OpenCV_LIBS})

```

如果打印出来可以看到其输出

```Sh
# OpenCV include directories: /usr/local/Cellar/opencv/4.1.1_1/include/opencv4
message("OpenCV include directories: " ${OpenCV_INCLUDE_DIRS} "\n")

# OpenCV libraries: opencv_calib3dopencv_core......
message("OpenCV libraries: " ${OpenCV_LIBS} "\n")
```

### 如何确定变量名

为什么OpenCV使用了 OpenCV_INCLUDE_DIRS 、OpenCV_LIBS 这两个变量名

命名规则 PackageName 加上 _INCLUDE_DIRS、_LIBS 是众多第三方库约定俗成的习惯

**CMake官方文档并没有强制要求，最终还是取决于其CMake配置文件的内容** 我们可以去自己查看

### OpenCV的配置文件在哪

根据CMake的官方要求，OpenCV的配置文件应该是

1. FindOpenCV.cmake
2. OpenCVConfig.cmake
3. opencv-config.cmake 

这三个文件之一，根据查找Homebrew或者源文件我们可以看到其位置，并且使用的是 OpenCVConfig 的名字

```Sh
# Home brew
/usr/local/Cellar/opencv/4.1.1_1/lib/cmake/opencv4/OpenCVConfig.cmake
# Source Code
../opencv-4.1.0/cmake/templates/OpenCVConfig.cmake.in
```

我们可以看到文件中存在说明定义了哪些变量

```SH
#    This file will define the following variables:
#      - OpenCV_LIBS                     : The list of all imported targets for OpenCV modules.
#      - OpenCV_INCLUDE_DIRS             : The OpenCV include directories.
```

### 两种CMake的查找模式

根据CMake的官方文档，还存在一种备用的Module模式，其配置文件类似于Config的命名规则

```
The command has two modes by which it searches for packages: “Module” mode and “Config” mode. The above signature selects Module mode. If no module is found the command falls back to Config mode, described below. This fall back is disabled if the MODULE option is given.
```


