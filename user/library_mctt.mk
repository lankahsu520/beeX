CFLAGS += -include $(SDK_INC_DIR)/zwave_def.h

ifeq ("$(PJ_HAS_ZWARE)", "yes")
#CFLAGS += -DOS_LINUX -DZIP_V2 -DUSE_OPENSSL -DCONFIG_DEBUG
CFLAGS += -DOS_LINUX
CFLAGS += -I$(SDK_INC_DIR)/zware
endif

ifeq ("$(PJ_HAS_BEEX)", "yes")
ifneq (,$(wildcard $(SDK_LIB_DIR)/libbeex.so))
LIBS_yes += -lbeex
endif
endif

include $(PWD)/user/library_pub.mk

