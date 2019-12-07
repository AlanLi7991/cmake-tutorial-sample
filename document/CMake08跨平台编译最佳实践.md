title: CMake08-跨平台编译最佳实践细节
date: 2019-12-08 00:00:00
categories: Tutorial
tags: [CMake]

---

## 目标

CMake最大的作用之一就是整合各个平台的工具链来管理共享的代码

本文根据实际工作给出一份[最佳实践源代码cmake-tutorial-sample](https://github.com/AlanLi7991/cmake-tutorial-sample)，所要达成的目标

1. 一个可以共享的C++算法库
2. 代码逻辑可以应用于 Mobile、Desktop、Web 三种环境
3. 一份对应平台的中间层SDK Wrapper

在真正的团队合作中，还需要补充的最重要的两点

* 不给业务开发者带来额外的负担，因为不同平台业务开发者可能不了解cmake运作机制
* 中间层SDK可以通过IDE进行源代码引用提供调试能力，因为SDK的UnitTest无法达到100%覆盖，方便出现问题时定位原因位于业务层还是SDK层

基本的目录结构包括

```SH
#根目录
--root
  --application #可以运行的App，包含iOS、Android、Electron、Web
  --cmake #使用cmake编译的库，包含共享的C++库、iOS的framework、Android的aar，WebAssembly的wasm
  --wrapper #中间层工程，包含cmake生成的Xcode工程、WebAssembly的Make工程，手工建立的Android中间层

```

## 路线

根据不同的平台提供不同的Target，并且产生Wrapper工程的方式不同

| 平台    | Target    |  Project  |
| :------------ | :------------ | :------------ |
| iOS     | Framework  |  使用cmake -G "Xcode" 产生 |
| Android | AndroidLibrary | 需要通过AndroidStudio建立配置gradle引用CMakeLists.txt |
| Desktop & Web | WebAssembly  | 使用 cmake -DCMAKE_TOOLCHAIN_FILE="file" 产生 |


## 已知的关键点

不同的平台细节上会有一些需要注意的地方，大致包含以下几个方面，会在后续的文章中进行详解

| 平台    | 关键点    |
| :------------ | :------------ |
|  iOS     | 1. App签名问题问题的细节<br>2. 使用cmake -G "Xcode" 生成等同手工创建的工程<br>3. 使用模板处理info.plist |
|  Android     | 1. 安装前置工具 Ninjia、NDK、CMake<br>2. 手动创建JNI中间层的工程<br>3. JNI中间层如何连接C++源码 |
|  WebAssembly | 1. 下载emsdk编译器工具<br>2. CMake编译时设置工具链<br>3. CMake无法设置CompilerFlag<br>4. 编译成同步和异步版本 |
