## @file Android.mk
##
## @brief TCP server Unit test class make file
##
## @author Roman Raisin (roman.raisin@gmail.com)
##
## How to run unit test:
##     1. Compile and run
##     2. connect to server with `nc localhost 39003` (or use adb forward)

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS += -Wall -fexceptions

my_includes:= \
        $(LOCAL_PATH)/../include \

LOCAL_C_INCLUDES := $(my_includes)
LOCAL_C_INCLUDES += $(TARGET_OUT_HEADERS)/common/inc

LOCAL_SRC_FILES:= \
    ut-TcpServer.cpp \
    TcpServer.cpp

LOCAL_MODULE := ut-TcpServer
LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)
