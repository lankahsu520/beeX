ifeq ($(PJ_ROOT)/user/library.mk,$(wildcard $(PJ_ROOT)/user/library.mk))
include $(PJ_ROOT)/user/library.mk
else
include $(PWD)/user/library.mk
endif
