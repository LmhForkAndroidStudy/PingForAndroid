//
// Created by lmhu on 2021/6/27.
//

#ifndef PINGFORANDROID_TRACEROUTE_LIB_H
#define PINGFORANDROID_TRACEROUTE_LIB_H

#include <android/log.h>

#define TAG "my_traceroute-jni" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型

void traceroute_TraceRoute_execute(char * hostName);

#endif //PINGFORANDROID_TRACEROUTE_LIB_H
