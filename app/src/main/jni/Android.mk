LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

MY_C_LIST := $(wildcard $(LOCAL_PATH)/ping/*.c)
MY_C_LIST += $(wildcard $(LOCAL_PATH)/traceroute/*.c)
MY_C_LIST += $(wildcard $(LOCAL_PATH)/libsupp/*.c)

LOCAL_MODULE    := native
LOCAL_SRC_FILES := $(MY_C_LIST:$(LOCAL_PATH)/%=%) native_jni.c

# for logging
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/httping
#VERSION := 1.0
#LOCAL_CFLAGS += -DNO_SSL -DVERSION=\"$(VERSION)\"
LOCAL_LDLIBS    += -llog -lm

include $(BUILD_SHARED_LIBRARY)
