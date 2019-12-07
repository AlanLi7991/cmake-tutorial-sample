title: CMake后置-IDE的选择和VSCode配置细节
date: 2019-12-10 00:00:00
categories: Tutorial
tags: [CMake, IDE]

---

## IDE如何选择

IDE的概念是集成开发环境（Integrated Development Environment）

所谓集成就是集中了编译配置、编译、调试等功能，例如Xcode、Android Studio、CLion等

方便开发者专注于代码逻、快速实现功能或者减少指令输入

不过随着集成开发环境加入了越来越多的东西，变得越来越卡，现在也出现了简化的趋势，例如VSCode的流行

## CLion如何工作

使用 CLion 创建一个 “New Project —— C++ Executable” 的工程，可以发现其根目录为

```
----Project
    --cmake-build-debug
    --CMakeLists.txt
    --main.cpp
```

对应该系列的[第一篇文章HelloWorld][link01]可以发现 CLion也是使用CMake来组织工程

点击菜单栏 “Run” 存在 “Build、Run、Debug” 三个选项，指定一个断点，然后点击Debug就可以进行调试

操作工程你会发现以下几个重点

* “cmake .” 产生的一系列文件被放在了 cmake-build-debug
* 在根本的任何改动，都会使得CMakeLists.txt变化，并且刷新工程
* 和Xcode不一样，CLion没有模糊编译、运行、调试的概念
* CLion中 Build不会执行程序、Run的话断点不会停止、Debug才是Xcode的 Command+R

由于CLion 变的越来越卡了（2018年6月——2019年9月），导致很多人选择使用更加轻量级的VSCode来开发

## VSCode的配置、编译、调试

VSCode就对程序员的专业程度要求比较高了，下载官方的C/C++插件之后

根据[官方教程][link02]可以完成配置、编译、调试三个步骤

## 命令台（Command Palette）和命令行（Shell Command）工具code 

命令台有两种打开方法

* 点击菜单栏 “View —— Command Palette”
* 使用组合键 Shift + Command + P

打开 “Command Palette” 可以执行 “Shell command: Install 'code' command in PATH” 来安装code指令

code指令把一个空文件夹配置成C/C++工程的基本命令，在完成配置后存在三个文件，目录结构如下

```
----HelloWorld
    --.vscode
        --c_cpp_properties.json
        --tasks.json
        --launch.json
```

| 文件名    | 作用    | 英文原文 |
| :------------ | :------------ | :------------ |
| c_cpp_properties.json | 工程配置文件，让这个目录拥有基本的Workspace、Project的能力 | (compiler path and IntelliSense settings) |
| tasks.json            | 编译文件，指定使用哪种目录编译    | (build instructions) |
| launch.json           | 调试文件，指定调试哪个程序，调试前提是编译结果是Debug配置    | (debugger settings) |

### 工程配置文件 c_cpp_properties.json 

打开 “Command Palette” 执行以下任意一个指令来配置基本的Workspace或Project的能力

* “C/C++: Edit Configurations (UI)” 可以使用可视化界面配置
* “C/C++: Edit Configurations (JSON)” 可以使用JSON文件配置

**注意如上篇文章所陈述，工程和编译的概念很模糊，不同平台和语言的定义不完全一致，所以职责划分并不是绝对的**

如果采用可视化界面配置，最终生效的还是 “.vscode/c_cpp_properties.json” 文件

```Json
//这个文件是不能加注释的，不然VSCode会报解析失败，这里加注释只是为了讲解
{
    //配置的数组，成员是字典
    "configurations": [
        {
            // 配置名称
            "name": "Mac",
            // 头文件目录 （重要）
            // 1. 当人为引用HelloWorld目录以外的头文件时需要自己新增配置
            // 2. 当引用第三方库时需要自己新增配置，例如OpenCV的 ${OpenCV_INCLUDE_DIRS}
            "includePath": [
                "${workspaceFolder}/**"
            ],
            // 本地变量
            "defines": [],
            // Mac系统的系统库
            "macFrameworkPath": [
                "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/System/Library/Frameworks"
            ],
            // 编译器的路径
            "compilerPath": "/usr/bin/clang",
            // 编译器的C语言版本 （重要）
            "cStandard": "c11",
            // 编译器的C++版本 （重要）
            "cppStandard": "c++17",
            // 自动补全和智能提示使用哪个
            "intelliSenseMode": "clang-x64"
        }
    ],
    //c_cpp_properties 配置的版本
    "version": 4
}
```

### 编译任务文件 tasks.json 

打开 “Command Palette” 执行 “Tasks: Configure Default Build Task”

然后选择“Create tasks.json file from template —— Others” 来配置默认的编译任务 “.vscode/tasks.json”

编译的配置就没有UI界面了（怀疑是懒得做），为了体现VSCode的高自由度（对新人不友好）

这里举例了[官方文档的配置示例][link01]、CMake的HelloWorld、模仿CLion指令三种配置，说明了不同指令之间的关系

| 命令    | 作用    | 参考配置 |
| :------------ | :------------ | :------------ |
| clang  |  编译文件，最基本的指令     |  官方的配置示例 |
| cmake & make  |  cmake完成配置，make进行调度clang     |  CMake的HelloWorld |
| clang  |  cmake进行配置，cmake调用make使用clang进行编译     |  CLion |

**其中CLion的作法才是比较成熟的作法，这里仅仅是举例整个演进的过程**


#### 官方配置示例（不重要）

官方配置是使用最基本的clang指令，完成文件编译

```Json
//这个文件是不能加注释的，不然VSCode会报解析失败，这里加注释只是为了讲解
{
  "version": "2.0.0",
  "tasks": [
    {
      // 任务名称
      "label": "Build with Clang",
      // 任务是一个Shell命令
      "type": "shell",
      // 使用 clang++ 命令
      "command": "clang++",
      // 对命令传入的参数 “clang++ -std=c++17 ....”
      "args": [
        "-std=c++17",
        "-stdlib=libc++",
        "main.cpp",
        "-o",
        "main",
        "--debug"
      ],
      // 设置这个任务是默认的任务，可以被快捷键直接启动
      "group": {
        "kind": "build",
        "isDefault": true
      }
    },
    //此处可以添加别的Task
    ....
  ]
}

```

#### CMake和Make指令分别使用 （不重要）

当文件结构足够多以及目录结构十分复杂的情况的时候，使用一条clang编译指令的参数就过于复杂

可以使用make文件来调度clang，可以参考 CMake配置完成后的 **CMakeFiles/HelloWorld.dir/build.make** 文件

```Json
    {
      // 任务名称
      "label": "Build with CMake & Make",
      // 任务是一个Shell命令
      "type": "shell",
      // 1. 使用 cmake 进行配置
      // 2. 使用 make 指令进行编译
      "command": "cmake . && make",
      "args": [ ]
    }
```

文件中使用的指令是 **XcodeDefault.xctoolchain/usr/bin/c++** ，使用man之后可以看到是clang的一个别名指令

```Shell
CMakeFiles/HelloWorld.dir/main.cpp.o: ../main.cpp
	.......
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  .....
  ......
```


### CLion的CMake流程（成熟工程的作法）

由于CMake本身就是对make的包装，那么可以把make指令本身也对大家隐藏起来，只使用cmake

例如CLion 就是仅使用 cmake 组织工程，可以分为配置和编译两步

配置过程在 New Project的过程中已经完成，我们没有抓到日志但是可以模拟出来

```Json
    {
      // 任务名称
      "label": "Configure with CLion CMake",
      // 任务是一个Shell命令
      "type": "shell",
      // 模拟 CLion 新建工程文件
      // 1. 创建 cmake-build-debug 用于存放CMake的临时文件
      // 2. 切换到 cmake-build-debug
      // 3. 使用 cmake 指令配置 CMakeLists.txt 所在目录
      "command": "mkdir \"${workspaceFolder}/cmake-build-debug\" || cd \"${workspaceFolder}/cmake-build-debug\" && cmake ../.",
      "args": [ ]
    }
```

而在编译阶段可以看到编译指令

```S
"~/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/192.6262.62/CLion.app/Contents/bin/cmake/mac/bin/cmake" --build ~/Desktop/HelloWorld/cmake-build-debug --target main -- -j 2
```

我们在VSCode中使用Task来模拟，配置如下

```Json
    {
      // 任务名称
      "label": "Build with CLion CMake",
      // 任务是一个Shell命令
      "type": "shell",
      // 模拟 CLion 编译工程目标
      // 1. 使用 cmake 指令替代 make 进行编译，底层还是调用的make
      "command": "cmake",
      // 2. 给 cmake 传入参数
      "args": [
          // 3-1. 指定编译配置所在目录 cmake参数
          "--build",
          // 3-2. 编译配置所在的目录取决于 CLion 生成时指定的目录
          "${workspaceFolder}/cmake-build-debug",
          // 4-1. 指定编译目标 cmake参数 
          "--target",
          // 4-2. 编译目标由 CMakeLists.txt 内 add_executable 决定
          "main",
          // 5-1. 透传参数给底层make
          "--",
          // 5-2. jobs 任务数 make参数
          "-j",
          // 5-3. 最多并发2个job
          "2"
      ]
    }
```


### 调试启动文件 launch.json 

打开 “Command Palette” 执行 “Debug: Open launch.json”

选择 “C++(GDB/LLDB)” 就是生成文件 “.vscode/launch.json” 可以用来调试采用Debug编译的可执行程序

```Json
//这个文件是不能加注释的，不然VSCode会报解析失败，这里加注释只是为了讲解
{
  // 版本号
  "version": "0.2.0",
  "configurations": [
    {
      // 名称
      "name": "(lldb) Launch",
      // 调试类型 cpp debug 
      "type": "cppdbg",
      // 请求分为 launch 和 attach，后者是附着到已经运行的程序上
      "request": "launch",
      // 被调试的程序（重要）
      // 程序所在的位置，根据tasks编译的结果决定
      // 如果采用上述的 cmake 和 clion 的指令就会和官方的有区别
      "program": "${workspaceFolder}/helloworld.out",
      // 启动时传入的参数
      "args": [],
      // 是否在入口的默认启动一个断点
      "stopAtEntry": true,
      // 工作目录，即 program 的io时的根目录
      "cwd": "${workspaceFolder}",
      // 环境变量
      "environment": [],
      // 是否启动额外的窗口
      "externalConsole": true,
      // 微软调试引擎 microsoft/MIEngine 接入的模式
      "MIMode": "lldb",
      // 日志输出
      "logging": {
        "trace": true,
        "traceResponse": true,
        "engineLogging": true
      },
      // 调试前重新编译（重要）
      // 因为调试（Debug）是启动已经编译好（Build Success）的程序
      // 如果想类似IDE CLion和Xcode 每次调试改动前都重新编译
      // 可以将编译任务放在Launch之前
      "preLaunchTask": {
        "task": "echo",
        "type": ""
      }
    }
  ]
}

```

### 结语

在对配置、编译、调试的整个流程熟悉了之后，如果选择 “VSCode + CMake” 进行开发可以总结为以下流程图

```
      +--------------------------------------------------------------------------+
      |                                                                          |
      |                             Source Code                                  |
      |                                                                          |
      +--------------------------------------------------------------------------+

      +--------------------------------------------------------------------------+
      |                                 VSCode                                   |
      +--------------------------------------------------------------------------+

      +------------+ +--------------------------------------------+ +------------+
      |            | |                                            | |            |
      | c_cpp_     | |                tasks.json                  | |   launch   |
      | properties | |                                            | |   .json    |
      | .json      | |                                            | |            |
      |            | |                                            | |            |
      |------------+ +--------------------------------------------+ +------+-----+
                                                                          |
                    +--------------------------------------------+        |
                    |                                            |        |
                    |                  CMake                     |        |
                    |                                            |        |
                    | +----------------------------------------+ |        |
                    | |                                        | |        |
                    | |                 make                   | |        |
                    | |                                        | |        |
                    | |    +------------------------------+    | |      Debug
                    | |    |                              |    | |        |
                    | |    |           clang              |    | |        |
                    | |    |                              |    | |        |
                    | |    +------------------------------+    | |        |
                    | +----------------------------------------+ |        |
                    |                                            |        |
                    +--------------------+-----------------------+        |
                                         |                                |
                                         v  Build                         |
                     +-------------------+---------------------+          |
                     |                                         |          |
                     |             Executable(Target)          |          |
                     |                                         | <--------+
                     |                                         |
                     +-----------------------------------------+

```


[link01]:https://code.visualstudio.com/docs/cpp/config-clang-mac
[link02]:https://code.visualstudio.com/docs/cpp/config-clang-mac
