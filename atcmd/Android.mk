## @file Android.mk
##
## @brief Modem AT Commands class Unit test make file
##
## @author Roman Raisin (roman.raisin@gmail.com)

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS += -Wall
LOCAL_CPPFLAGS += -fexceptions -std=c++11

my_includes:= \
    $(LOCAL_PATH)/../include \

LOCAL_C_INCLUDES := $(my_includes)
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/common/inc

LOCAL_SRC_FILES:= \
    ut-ModemAt.cpp \
    ModemAt.cpp

LOCAL_MODULE := ut-ModemAt
LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)
