#
# jxfengzi@gmail.com
#
# 2011-6-24
#
#
#

LOCAL_PATH := $(call my-dir)

INC_OPENSSL             := ../../../openssl/include
INC_TINY_BASE           := ../src/Tiny/Base
INC_TINY_LOG            := ../src/Tiny/Log
INC_ALL                 := $(INC_OPENSSL)               \
                           $(INC_TINY_BASE)             \
                           $(INC_TINY_LOG)
MY_CFLAGS               := -D__ANDROID__                \
                           -fPIC                        \
                           -Wno-multichar

#MY_ARCH                := arm64-v8a
MY_ARCH                 := armeabi

$(warning $(INC_ALL))

#--------------------------------------------------------------------
# openssl
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
SRC_DIR                 := ../../src/test
LOCAL_MODULE            := srptest 
LOCAL_SRC_FILES         := $(SRC_DIR)/srptest.c
LOCAL_C_INCLUDES        := $(INC_ALL)
LOCAL_STATIC_LIBRARIES  := libcrypto libssl
LOCAL_LDLIBS            := -llog
include $(BUILD_EXECUTABLE)

#--------------------------------------------------------------------
# libtiny.a
#--------------------------------------------------------------------
include $(CLEAR_VARS)
LOCAL_MODULE            := tiny
SRC_DIR                 := ../../src/Tiny
LOCAL_SRC_FILES         := $(SRC_DIR)/Base/tiny_ret.c               \
                           $(SRC_DIR)/Base/tiny_time.c              \
                           $(SRC_DIR)/Log/tiny_log_print.c
LOCAL_C_INCLUDES        := $(INC_ALL)
include $(BUILD_STATIC_LIBRARY)

#--------------------------------------------------------------------
# libsrp.so
#--------------------------------------------------------------------
include $(CLEAR_VARS)
LOCAL_MODULE            := srp
SRC_DIR                 := ../../src/srp
LOCAL_SRC_FILES         := $(SRC_DIR)/SrpServer.c                   \
                           $(SRC_DIR)/SrpClient.c
LOCAL_C_INCLUDES        := $(INC_ALL)
LOCAL_STATIC_LIBRARIES  := libcrypto libssl libtiny
LOCAL_LDLIBS            := -llog
include $(BUILD_SHARED_LIBRARY)

#--------------------------------------------------------------------
# test_srp
#--------------------------------------------------------------------
include $(CLEAR_VARS)
SRC_DIR                 := ../../src/srp
LOCAL_MODULE            := test_srp
LOCAL_SRC_FILES         := $(SRC_DIR)/test.c
LOCAL_C_INCLUDES        := $(INC_ALL)
LOCAL_SHARED_LIBRARIES  := srp
LOCAL_LDLIBS            := -llog
include $(BUILD_EXECUTABLE)
