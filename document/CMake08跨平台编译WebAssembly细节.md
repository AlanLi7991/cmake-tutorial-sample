title: CMake08-跨平台编译WebAssembly实践细节
date: 2019-12-08 20:00:00
categories: Tutorial
tags: [CMake]

---

## WebAssembly实践细节

编译成WebAssembly需要使用Emscripten作为编译器，官方网站为[emscripten.org](https://emscripten.org/index.html)

推荐实践路线

1. application: 推荐手动建立工程，给出sdk文件位置
2. sdk: 使用cmake生成，编译完成后拷贝至application应用文件夹
3. c++: 使用cmake管理，并通过sdk层引用

### 下载emsdk

官方文档[Emscripten Toolchain Requirements](https://emscripten.org/docs/building_from_source/toolchain_what_is_needed.html)指出Emscripten依赖的库包括

* Node.js、Python、Java、Git client、LLVM、Binaryen

你可以手动安装这些库，但是更加推荐的用法是根据官方文档[Download and install](https://emscripten.org/docs/getting_started/downloads.html)直接下载SDK，其内部包含完整的工具链。

### emsdk使用CMake

如果想让CMake使用emcc命令作为Compiler和LinkTool, 需要使用官方提供的工具链文件[Emscripten.cmake](https://github.com/emscripten-core/emscripten/blob/incoming/cmake/Modules/Platform/Emscripten.cmake)

```SH
# To use this toolchain file with CMake, invoke CMake with the following command line parameters
# cmake -DCMAKE_TOOLCHAIN_FILE=<EmscriptenRoot>/cmake/Modules/Platform/Emscripten.cmake
#       -DCMAKE_BUILD_TYPE=<Debug|RelWithDebInfo|Release|MinSizeRel>
#       -G "Unix Makefiles" (Linux and OSX)
#       -G "MinGW Makefiles" (Windows)
#       <path/to/CMakeLists.txt> # Note, pass in here ONLY the path to the file, not the filename 'CMakeLists.txt' itself.
```
根据官方文档提示，只需要在cmake generate的过程中指定CMAKE_TOOLCHAIN_FILE即可

### 映射C++方法的方式

映射C++的方式分为两种 Embind 和 WebIDL Binder，参考博客[使用WebAssembly编译C++到JS](https://blog.icehoney.me/posts/2019-04-28-webassmebly)更加推荐 Embind的方式因为其丰富的数据结构映射

| C++ type   | JavaScript type    |
| :------------ | :------------ |	
| void      |	undefined  |
| bool  	| true or false  |
| char	    | Number  |
| signed char	 |  Number  |
| unsigned char	 |  Number  |
| short	         |  Number  |
| unsigned short | 	Number  |
| int            | 	Number  |
| unsigned int   | 	Number  |
| long	         |  Number  |
| unsigned long  | 	Number  |
| float          | 	Number  |
| double         | 	Number  |
| std::string    | 	ArrayBuffer, Uint8Array, Uint8ClampedArray, Int8Array, or String  |
| std::wstring   | 	String (UTF-16 code units)  |
| emscripten::val| 	anything  |

其中最重要的是 **emscripten::val** 可以映射任何东西

自然也包括了std::function，可以将C++的lambda转化为js的callback, 详见[StackOverflow的回答](https://stackoverflow.com/questions/36371178/embind-pass-lambda-function-as-callback-parameter)

### 编译同步版本

根据官方文档[Building to WebAssembly](https://emscripten.org/docs/compiling/WebAssembly.html#wasm-files-and-compilation)的章节 **.wasm files and compilation** 提出

1. WebAssembly默认编译结果是异步加载
2. 可以在onRuntimeInitialized中确定何时加载完毕
3. 加载完毕后才能访问C++代码

```SH

Another noticeable effect is that WebAssembly is compiled asynchronously by default, which means you must wait for compilation to complete before calling compiled code (by waiting for main(), or the onRuntimeInitialized callback......)
```

如果想要同步加载C++ Port的结果，可以选择在编译时设置 **WASM_ASYNC_COMPILATION=0**

需要注意的是这个参数并没有在[编译配置的源代码](https://github.com/emscripten-core/emscripten/blob/master/src/settings.js)中出现，应该是文档不够严谨导致的

### 常见的错误

* CMake中 target_compile_options 和 COMPILE_FLAGS 无效 

在编译WebAssembly的过程中使用 **--bind** 参数无法通过以下两个CMake指令传入

1. target_compile_options
2. set_target_properties(\<target> PROPERTIES COMPILE_FLAGS "--bind") 

而只能使用对应Link属性，可以通过 **cmake --build path --verbose** 查看调用的指令区别

具体原因不详，猜测和Emscripten的Compiler与Link为同一个指令相关

* Electron报错unsafe-eval

因为Electron内部的Content-Security-Policy的安全策略问题，使用WASM可能会报错

```SH
'unsafe-eval' is required because of a Chrome bug(?) that disallows loading WASM from 'self'.
```

参考开源工程[electron-wasm-rust-example](https://github.com/anderejd/electron-wasm-rust-example/blob/master/index.html)修改策略

```HTML
    <!--
      https://github.com/anderejd/electron-wasm-rust-example/issues/6
      https://github.com/WebAssembly/content-security-policy/issues/7
      https://bugs.chromium.org/p/chromium/issues/detail?id=948834&can=1&q=wasm-eval
    -->
    <meta http-equiv="Content-Security-Policy" content="script-src 'self' app: 'unsafe-eval'; object-src 'self' app:;">
```
