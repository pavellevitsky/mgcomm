## @file Android.mk
##
## @brief DCI class unit test make file
##
## @author	Roman Raisin (roman.raisin@gmail.com)

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

my_includes:= \
        $(LOCAL_PATH)/../include \
        $(LOCAL_PATH)/../include/dci/include \
        $(LOCAL_PATH)/../include/dci/src

LOCAL_C_INCLUDES := $(my_includes)
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/common/inc

LOCAL_SRC_FILES:= \
	ut-DCI.cpp \
	DCIBase.cpp \
	DCI.cpp

commonSharedLibraries :=libdiag

LOCAL_MODULE := ut-DCI
LOCAL_MODULE_TAGS := optional
LOCAL_SHARED_LIBRARIES := $(commonSharedLibraries)
LOCAL_SHARED_LIBRARIES += liblog

LOCAL_MODULE_OWNER := qcom
include $(BUILD_EXECUTABLE)
