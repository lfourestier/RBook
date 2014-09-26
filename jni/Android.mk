LOCAL_PATH := $(call my-dir)

ROOTDIR = .
RBOOKDIR = $(ROOTDIR)/RBook
EXTERNALDIR = $(ROOTDIR)/external

include $(CLEAR_VARS)

LOCAL_MODULE    := RBook

LOCAL_C_INCLUDES := $(LOCAL_PATH)
                    
LOCAL_SRC_FILES := luc_fourestier_rbook_BookManager.cpp \
					luc_fourestier_rbook_PictManager.cpp \
					luc_fourestier_rbook_RoadBook.cpp \
					luc_fourestier_rbook_RoadPoint.cpp

LOCAL_CPP_FEATURES := exceptions 

#LOCAL_CPPFLAGS := -Wall -Werror -Wextra
ifneq ($(NDK_DEBUG),0)
LOCAL_CPPFLAGS +=  -g -O0
endif

LOCAL_LDLIBS    += -llog

include $(LOCAL_PATH)/$(RBOOKDIR)/sources.mk

include $(LOCAL_PATH)/$(EXTERNALDIR)/libjson/sources.mk
include $(LOCAL_PATH)/$(EXTERNALDIR)/zlib/sources.mk
include $(LOCAL_PATH)/$(EXTERNALDIR)/minizip/sources.mk

include $(BUILD_SHARED_LIBRARY)
