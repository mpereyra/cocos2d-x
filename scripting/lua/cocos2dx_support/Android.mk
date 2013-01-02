
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos_luabinding_static

LOCAL_MODULE_FILENAME := libcocoslua

LOCAL_SRC_FILES := \
	CCLuaEngine.cpp \
	Cocos2dxLuaLoader.cpp \
	LuaCocos2d.cpp \
	tolua_fix.c \

LOCAL_STATIC_LIBRARIES := \
	cocos_lua_static \
	cocos2dx_static \
	cocosdenshion_static \

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(BUILD_STATIC_LIBRARY)
