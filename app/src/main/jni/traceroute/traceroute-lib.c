#include <jni.h>
#include <string.h>
#include <malloc.h>
#include "traceroute.h"
#include <pthread.h>

#define OUTPUT_LENGTH  10000
#include "traceroute-lib.h"

JavaVM *g_jvm;
static jobject j_java_cls;
static jobjectArray j_argvs;
int exec_status = -3;
pthread_t trace_thread;



void JNU_ThrowByName(JNIEnv *env, const char *name, const char *msg) {
    jclass cls = (*env)->FindClass(env, name);
    if (cls != NULL) {
        (*env)->ThrowNew(env, cls, msg);
    }
    (*env)->DeleteLocalRef(env, cls);
}

JNIEnv *JNU_GetEnv() {
    JNIEnv *env;
    (*g_jvm)->GetEnv(g_jvm, (void **) &env, JNI_VERSION_1_6);
    return env;
}

void call_java_clear_result() {
    JNIEnv *env = JNU_GetEnv();
    jclass j_cls = (*env)->GetObjectClass(env, j_java_cls);
    jmethodID j_clear_result = (*env)->GetMethodID(env,
                                                   j_cls,
                                                   "clearResult",
                                                   "()V");
    if (j_clear_result == NULL) {
        LOGE("can't find clearResult() method. do you add proguard to proguard-rules.pro");
        return;
    }
    (*env)->CallVoidMethod(env, j_java_cls, j_clear_result);
}

void call_java_append_result(const char *text) {
    LOGE("ret --- %s",text);

//    JNIEnv *jniEnv = JNU_GetEnv();
//    jclass j_cls = (*jniEnv)->GetObjectClass(jniEnv, j_java_cls);
//    jmethodID j_append_result = (*jniEnv)->GetMethodID(jniEnv,
//                                                       j_cls,
//                                                       "appendResult",
//                                                       "(Ljava/lang/String;)V");
//    if (j_append_result == NULL) {
//        LOGE("can't find appendResult() method. do you add proguard to proguard-rules.pro");
//        return;
//    }
//    jstring message = (*jniEnv)->NewStringUTF(jniEnv, text);
//    (*jniEnv)->CallVoidMethod(jniEnv, j_java_cls, j_append_result, message);
//    (*jniEnv)->DeleteLocalRef(jniEnv, message);
}

int printf(const char *fmt, ...) {
    va_list argptr;
    int cnt;
    va_start(argptr, fmt);
    char *buffer = (char *) malloc(OUTPUT_LENGTH);
    memset(buffer, OUTPUT_LENGTH, 0);
    cnt = vsnprintf(buffer, OUTPUT_LENGTH, fmt, argptr);
    buffer[cnt] = '\0';
    call_java_append_result(buffer);
    free(buffer);
    va_end(argptr);
    return 1;
}

int fprintf(FILE *fp, const char *fmt, ...) {
    va_list argptr;
    int cnt;
    va_start(argptr, fmt);
    char *buffer = (char *) malloc(OUTPUT_LENGTH);
    memset(buffer, OUTPUT_LENGTH, 0);
    cnt = vsnprintf(buffer, OUTPUT_LENGTH, fmt, argptr);
    buffer[cnt] = '\0';
    LOGE("traceroute error message(fprintf): %s", buffer);
    free(buffer);
    va_end(argptr);
    return 1;
}

int vfprintf(FILE *fp, const char *fmt, va_list args) {
    int cnt;
    char *buffer = (char *) malloc(OUTPUT_LENGTH);
    memset(buffer, OUTPUT_LENGTH, 0);
    cnt = vsnprintf(buffer, OUTPUT_LENGTH, fmt, args);
    buffer[cnt] = '\0';
    LOGE("traceroute error message(vfprintf): %s", buffer);
    free(buffer);
    return 1;
}

void perror(const char *msg) {
    LOGE("traceroute error message(perror): %s", msg);
}

void exit(int status) {
    // avoid some device crash. eg: vivo x7
    (*g_jvm)->DetachCurrentThread(g_jvm);
    exec_status = -3;
    LOGE("traceroute error to exit program, status:%d", status);
    pthread_exit(0);
}

void *do_execute(char * hostName) {
    LOGE("AttachCurrentThread %s ", hostName);

    char *argv[2];
    char *comman="traceroute";
    argv[0]=comman;
    argv[1]=hostName;

    LOGD("execute 0000 command result:%d", exec_status);
   // call_java_clear_result();
    exec_status = exec(2, argv);

    LOGD("execute command result:%d", exec_status);
   // (*g_jvm)->DetachCurrentThread(g_jvm);
    return NULL;
}





void traceroute_TraceRoute_execute(char * hostName) {
    LOGD("start traceroute");
    pthread_create(&trace_thread, NULL, do_execute,  hostName);
    pthread_join(trace_thread, NULL);
    LOGD("finish traceroute, status:%d", exec_status);
}




/*
JNIEXPORT jint JNICALL Java_com_wandroid_traceroute_TraceRoute_execute
        (JNIEnv *env, jobject jthis, jobjectArray jarray) {
    LOGD("start traceroute");
    j_java_cls = (*env)->NewGlobalRef(env, jthis);
    j_argvs = (*env)->NewGlobalRef(env, jarray);

    pthread_create(&trace_thread, NULL, do_execute, NULL);
    pthread_join(trace_thread, NULL);

    (*env)->DeleteGlobalRef(env, j_java_cls);
    (*env)->DeleteGlobalRef(env, j_argvs);

    LOGD("finish traceroute, status:%d", exec_status);
    return exec_status;
}
*/