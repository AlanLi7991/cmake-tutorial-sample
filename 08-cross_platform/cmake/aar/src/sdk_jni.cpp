//
//

#include "sdk_jni.h"
#include "algorithm.h"
#include <jni.h>
#include <string>
#include <vector>


/**
 ****************************************************************************
 * Declare
 ****************************************************************************
 */

static const char* const JAVA_CLASS_NAME = "com/sdk/sdk/SDKKotlin";
static const char* const JAVA_INTERFACE_NAME = "com/sdk/sdk/IAlgorithmInterface";

struct JNI_ISDKKotlin {
    jmethodID onCallback;
};

static JNI_ISDKKotlin interface_sdk_kotlin;

/**
 ****************************************************************************
 * Register, Loader
 ****************************************************************************
 */

jint JNI_invokeAlgorithmFunc(JNIEnv *env, jobject thiz, jint number, jobject callback) {
    /**
     * JNIEnv *env 这个是JNI传递来的裸指针和Java线程绑定
     * 根据文档说明
     * 1. env 是一个Interface Point 指向一个函数表
     * 2. JVM保证相同的Java线程传入的 env是相同的（地址上相同？）
     */

    /**
     * 创建C++ 全局引用
     */
    auto callback_ref = env->NewGlobalRef(callback);

    /**
     * 创建shared_ptr
     */
    std::shared_ptr<_jobject> jni_callback(callback_ref, [&env](_jobject *ptr){
        env->DeleteGlobalRef(ptr);
    });

    /**
     * 进入C++ 方法
     */
    auto result = algorithm::invokeAlgorithmFunc(number, [&env, jni_callback](int code, std::string message) {
        auto java_str_message = env->NewStringUTF(message.c_str());
        env->CallVoidMethod(jni_callback.get(), interface_sdk_kotlin.onCallback, code, java_str_message);
    });

    return result;
    /**
     * JNIEnv在这里就会被无法访问（释放？）,所以回调里如果想CallMethod就必须保存Env
     * 1. CSDK采用的方式是保存到Static Map
     * 2. 如果传递 [env] [&env] 都会报错访问了一个非法的引用
     */
}


/**
 ****************************************************************************
 * Loader
 ****************************************************************************
 */




JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    /**
     * Get JNIEnv from JVM
     */
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    /**
     * Find JVM Interface ISDKKotlin
     */
    jclass java_interface = env->FindClass(JAVA_INTERFACE_NAME);
    if (java_interface == nullptr) {
        return JNI_ERR;
    }

    /**
     * ISDKKotlin register callback to static memory
     */
    jmethodID methodID = env->GetMethodID(java_interface, "onCallback", "(ILjava/lang/String;)V");

    if (methodID == nullptr) {
        return JNI_ERR;
    }

    interface_sdk_kotlin.onCallback = methodID;

    /**
     * Find JVM Class ISDKKotlin
     */
    jclass java_class = env->FindClass(JAVA_CLASS_NAME);

    if (java_class == nullptr) {
        return JNI_ERR;
    }

    /**
     * SDKKotlin map native_method to JNI_InvokeMethod
     */
    static const JNINativeMethod methods[] =
            {
                { "invokeAlgorithmFunc", "(ILcom/sdk/sdk/IAlgorithmInterface;)I", (void *)JNI_invokeAlgorithmFunc },
            };
    int rc = env->RegisterNatives(java_class, methods, sizeof(methods)/sizeof(JNINativeMethod));
    if (rc != JNI_OK) {
        return rc;
    }


    /**
     * Delete reference
     */
    env->DeleteLocalRef(java_class);
    env->DeleteLocalRef(java_interface);

    return JNI_VERSION_1_6;
}

