MODULEDIR := $(ROOTDIR)/external/minizip
SOURCES :=   ioapi.c \
             unzip.c \
             zip.c 
             
SRCDIR := $(MODULEDIR)/
INCDIR := $(LOCAL_PATH)/$(MODULEDIR)/
LOCAL_SRC_FILES += $(SOURCES:%=$(SRCDIR)/%)
LOCAL_C_INCLUDES += $(INCDIR)