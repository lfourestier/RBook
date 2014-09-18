MODULEDIR := $(ROOTDIR)/external/zlib
SOURCES :=   adler32.c \
			compress.c \
			crc32.c \
			deflate.c \
			gzclose.c \
			gzlib.c \
			gzread.c \
			gzwrite.c \
			infback.c \
			inffast.c \
			inflate.c \
			inftrees.c \
			trees.c \
			uncompr.c \
			zutil.c 
             
SRCDIR := $(MODULEDIR)/
INCDIR := $(LOCAL_PATH)/$(MODULEDIR)/
LOCAL_SRC_FILES += $(SOURCES:%=$(SRCDIR)/%)
LOCAL_C_INCLUDES += $(INCDIR)