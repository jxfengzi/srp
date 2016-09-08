#
# jxfengzi@gmail.com
#
# 2011-6-24
#
#
#

LOCAL_PATH := $(call my-dir)

INC_OPENSSL             := ../../../openssl/include
INC_ALL                 := $(INC_OPENSSL)
MY_CFLAGS               := -D__ANDROID__                \
                           -fPIC                        \
                           -Wno-multichar

#MY_ARCH                := arm64-v8a
MY_ARCH                 := armeabi

#--------------------------------------------------------------------
# libav
#--------------------------------------------------------------------
include $(CLEAR_VARS)
LOCAL_MODULE            := crypto
LOCAL_SRC_FILES         := ../../libs/$(MY_ARCH)/libcrypto.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE            := ssl
LOCAL_SRC_FILES         := ../../libs/$(MY_ARCH)/libssl.a
include $(PREBUILT_STATIC_LIBRARY)

#--------------------------------------------------------------------
# srptest
#--------------------------------------------------------------------
include $(CLEAR_VARS)
SRC_DIR                 := ../../src
LOCAL_MODULE            := srptest 
LOCAL_SRC_FILES         := $(SRC_DIR)/srptest.c
LOCAL_C_INCLUDES        := $(INC_ALL)
LOCAL_STATIC_LIBRARIES  := libcrypto libssl
LOCAL_LDLIBS            := -llog
include $(BUILD_EXECUTABLE)
