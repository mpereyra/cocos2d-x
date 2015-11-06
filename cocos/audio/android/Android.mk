LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := cocosdenshion_static

LOCAL_MODULE_FILENAME := libcocosdenshion

#turn off thumb for extra speed
LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := cddSimpleAudioEngine.cpp \
                   ccdandroidUtils.cpp \
                   jni/cddandroidAndroidJavaEngine.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include \
                    $(LOCAL_PATH)/../.. \
                    $(LOCAL_PATH)/../../platform/android

include $(BUILD_STATIC_LIBRARY)

#new audio engine
include $(CLEAR_VARS)

LOCAL_MODULE := audioengine_static

LOCAL_MODULE_FILENAME := libaudioengine

#turn off thumb for extra speed
LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES := AudioEngine-inl.cpp \
                   ../AudioEngine.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../include

LOCAL_EXPORT_LDLIBS := -lOpenSLES

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../include \
                    $(LOCAL_PATH)/../.. \
                    $(LOCAL_PATH)/../../platform/android

include $(BUILD_STATIC_LIBRARY)
