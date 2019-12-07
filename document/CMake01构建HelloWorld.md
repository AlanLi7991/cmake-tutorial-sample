title: CMake01-CMake构建HelloWorld
date: 2019-12-01 10:00:00
categories: Tutorial
tags: [CMake]

---

## CMake的基本概念

在阅读这个系列文章之前，需要掌握一些简单的前置概念

* 编译器: gcc、llvm 作用于高级语言的文件类似 .cpp .m 将其变化成目标文件 .o
* Make: 一种执行顺序的调度工具, 因为编译是有顺序的，通过make脚本可以确定调度顺序

而CMake是用于组织工程结构的面向过程语言，和以上三个概念的关系可以简单解释为

1. CMake通过解析自己的语言文件 CMakeLists.txt
2. 帮助用户生成 makefile 文件 build.make
3. build.make 中根据平台不同使用不同的编译命令完成编译

## Hello World

分析一个简单的 Hello World 工程, 首先新建文件夹，并且建立对应的文件

```
----HelloWorld
    --CMakeLists.txt    //CMake的配置文件
    --main.cpp          //工程源文件
```

然后我们可以通过以下三个指令，分别完成上述的文章开头的三个功能

```SH
# 切换到文件夹
cd HelloWorld
# cmake指令，帮助用户生成 makefile 文件 build.make
cmake .
# make指令， 帮助用户完成编译的调度
make
# 运行 HelloWorld 程序     
./main        
```

整个过程中有两个入门级的问题

* 为什么会生成一堆文件

CMake会以输入的目录为参考，根据CMakeLists.txt生成一套 **Scheme（Apple的叫法）**，作用是用来描述如何编译当前的代码

在实际工程中一般不会这么粗暴的使用当前目录 “.” 干扰工程的文件结构 ，而是使用一个专门的Build目录，参考CMake02章节。

* build.make 在哪

执行完CMake会生成不止一个 .make 文件，build.make 位于 “./CMakeFiles/HelloWorld.dir/build.make”

原因在于make本身是一个调度工具，不仅仅只用来编译，可以用于任何的指令调度


## CMakeLists.txt 配置

根据前文，CMakeLists.txt 是一个对工程文件描述的过程语言，HelloWorld中配置内容如下

```SH
# 当前CMake的最低版本
cmake_minimum_required(VERSION 3.14)
# 当前工程的名字
project(HelloWorld)           
# 生成程序的名字，以及使用哪些源文件
add_executable(main main.cpp)
```

和成熟的IDE对比，这份配置文件十分的简陋，而且缺少了很多常见的概念，例如头文件、第三方库等等

后续文章我们会以 Xcode 、Android Studio为对比一步步讲解和对比这些细节

最后补充main.cpp的内容

```C++
#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
```

## CMake的周期

CMake对其能力划分了功能周期, 更详细的内容可以阅读[官方文档][link02]，其中大致分为

1. Generate a Project Buildsystem
2. Build a Project
3. Install a Project

我们刚刚的指令 “camke .” 就是 Generate， “make .” 就是 Build，官方文档给出了更方便的命令 “cmake --build”

[link02]:https://cmake.org/cmake/help/v3.15/manual/cmake.1.html






