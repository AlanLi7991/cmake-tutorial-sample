title: CMake08-跨平台编译Android实践细节
date: 2019-12-08 15:00:00
categories: Tutorial
tags: [CMake]

---

## Android平台实践细节

推荐实践路线

1. application: 推荐手动建立工程，方便团队里Android工程师使用
2. sdk: 手工创建工程，并通过build.gradle引用cmake，方便application工程师开箱即用
3. c++: 使用cmake管理，并通sdk层的CMakeLists.txt引用

### Java怎么创建库Module

Android平台的Wrapper与通过CMake generate 出来的Xcode不同，需要自己手工建立

最重要的逻辑区别在于AndroidStudio自己调用CMakeLists.txt本身，过程分为

1. 创建空的Activity，增加 AndroidLibrary Module
2. 对AndroidLibrary 添加CMake的配置文件
3. 下载AndroidStudio读取C++必须的工具NDK、Ninjia

上述步骤可以按照[Google官方文档的指导方案配置](https://developer.android.com/studio/projects/add-native-code?hl=zh-cn)

### Android配置中常见错误

* 现象：提示无Ninjia

```
CMake was unable to find a build program corresponding to "Ninja".  CMAKE_MAKE_PROGRAM is not set.
```

解决方案：通过Homebrew 或者 AndroidStudio安装Ninjia工具，Ninjia是一个Google的编译工具

* 现象：Sync不成功，提示找不到SDK

```
ERROR: Unable to resolve dependency for ':app@debug/compileClasspath': Failed to transform artifact 'sdk.aar (project :sdk)' to match attributes {artifactType=jar}.
Show Details
Affected Modules: app
```
解决方案： App使用了错误的build.gradle

setting.gradle应该加载正确的工程路径

```
include ':app', ':sdk'
rootProject.name='Render'

project(':sdk').projectDir = new File(rootDir.path, "../../wrapper/JNI/sdk")
```

build.gradle 需要使用正确的依赖方式

```
dependencies {
    ......
    implementation project(path: ':sdk')
}
```

* 现象：AndroidStudio未能加载CPP文件

解决方案：在安装Ninjia之后，点击"Build-Refresh Link C++ Project"

### JNI中的Env问题

JNI接口中有一个最重要的概念是JNIEnv，作为每一个方法的第一个参数代表着Java环境的全部上下文

根据[JNI官方文档原文](https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/design.html#interface_pointer)指出

* 每一次Java虚拟机调用JNI接口，保证同一个线程传入同一个地址的JNIEnv

```
Native methods receive the JNI interface pointer as an argument. The VM is guaranteed to pass the same interface pointer to a native method when it makes multiple calls to the native method from the same Java thread. 
```

* JNIEnv和线程强相关，不能够在一个线程访问另一个线程的JNIEnv

```
The JNI interface pointer is only valid in the current thread. A native method, therefore, must not pass the interface pointer from one thread to another.
```

根据以上描述，如果开发者选择在C++开启一个线程，并且使用lambda进行回调的话，则需要注意

1. 两个Scope中的JNIEnv并不一致
2. 一般使用静态Map，以线程ID为Key值储存JNIEnv的指针
3. C++线程需要手动Attach到Java虚拟机上

```C++
//保留Java虚拟机
static JavaVM *java_vm = nullptr;
//创建容器
static std::unordered_map<std::thread::id, JNIEnv *> jni_env_map;

//在C++线程中获取线程Env并执行Attach
void* JNI_AttachCppThread() {
    JNIEnv *env;
    if (java_vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    if （java_vm->AttachCurrentThread(&env, NULL) != JNI_OK） {
        return JNI_ERR;
    }
    jni_env_map[std::this_thread::get_id()] = env;
}

//切换ENV
void JNI_method(JNIEnv *env, jobject thiz) {
    //Java方法调用
    auto java_thread_id = std::this_thread::get_id();
    auto java_env = jni_env_map.at(thread_id)
    if (env == java_env) {
        std::cout << "Some JNIEnv * passed by JavaVM" << std::endl;
    }
    //进入C++方法
    cpp::method_with_callback([]() {
        //通过C++线程获得Env
        //此处如果访问java_env则会造成JavaVM Crash
        auto cpp_thread_id = std::this_thread::get_id();
        auto cpp_env = jni_env_map.at(thread_id)
    });
}

```
