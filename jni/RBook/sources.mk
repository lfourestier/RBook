MODULEDIR := $(ROOTDIR)/RBook
SOURCES :=   	BookManager.cpp \
				RoadBook.cpp \
                RoadPoint.cpp \
                Error.cpp \
                Log.cpp \
                Archive.cpp \
                FileUtils.cpp

SRCDIR := $(MODULEDIR)/
INCDIR := $(LOCAL_PATH)/$(MODULEDIR)/
LOCAL_SRC_FILES += $(SOURCES:%=$(SRCDIR)/%)
LOCAL_C_INCLUDES += $(INCDIR)