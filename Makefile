-include $(SDK_CONFIG_CONFIG)

TOP=$(shell pwd)
PWD=$(shell pwd)

#** CFLAGS & LDFLAGS **
CFLAGS += $(CFLAGS_CUSTOMER)

CFLAGS += \
					-I./ -I$(SDK_INC_DIR) -DPJ_BEEX_ZWARE
LDFLAGS += \
					-L./ \
					-L$(SDK_LIB_DIR) \
					-Wl,-rpath -Wl,$(SDK_LIB_DIR)
ARFLAGS = rcs

#** LIBXXX_OBJS **
LIBXXX_OBJS += \
							beex_honeycomb.o \
							beex_tracker.o \
							beex_report.o \
							beex_translater.o

#** LIBXXX_yes **
#LIBXXX_A = libbeex.a
LIBXXX_SO = libbeex.so
#LIBXXXS_$(PJ_HAS_STATIC_LIB) += $(LIBXXX_A)
#LIBXXXS_$(PJ_HAS_SHARE_LIB) += -lbeex
LIBXXXS_yes += -lbeex

#** HEADER_FILES **
HEADER_FILES = beex_api.h beex_honeycomb.h beex_report.h beex_translater.h beex_zware.h

LIBS_yes = $(LIBXXXS_yes)
-include ./library.mk

#** librarys **
LIBS +=
#-ldl -lpthread -lm 

#** Clean **
CLEAN_BINS = 
CLEAN_OBJS = $(LIBXXX_OBJS)
CLEAN_LIBS = $(LIBXXX_A) $(LIBXXX_SO)

#** Target (CLEAN_BINS) **
CLEAN_BINS +=

#** Target (DUMMY_BINS) **
DUMMY_BINS =

CLEAN_BINS += $(DUMMY_BINS)
CLEAN_OBJS += $(addsuffix .o, $(CLEAN_BINS))

#** Target (SHELL_BINS) **
SHELL_BINS = 

.SUFFIXES:
.SUFFIXES: .c .o

.PHONY: all clean distclean install romfs
all: $(CLEAN_BINS) $(CLEAN_LIBS)

%.o: %.c $(HEADER_FILES)
	@echo 'Compiling file: $<'
	$(CC) $(CFLAGS) -c -o"$@" "$<"
	@echo 'Finished compiling: $<'
	@echo ' '

$(CLEAN_BINS): $(CLEAN_OBJS) $(CLEAN_LIBS)
	@echo 'Building target: $@'
	$(CC) -o $@ $@.o $(LDFLAGS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

clean:
	rm -f Makefile.bak $(CLEAN_BINS) $(CLEAN_BINS:=.elf) $(CLEAN_BINS:=.gdb)
	rm -f $(CLEAN_LIBS) $(CLEAN_OBJS)
	rm -f $(CLEAN_OBJS:%.o=%.c.bak) $(CLEAN_OBJS:%.o=%.h.bak)
	@for subbin in $(CLEAN_BINS); do \
		(rm -f $(SDK_BIN_DIR)/$$subbin;); \
	done
	@for sublib in $(CLEAN_LIBS); do \
		(rm -f $(SDK_LIB_DIR)/$$sublib;); \
	done
	@for subheader in $(HEADER_FILES); do \
		(rm -f $(SDK_INC_DIR)/$$subheader;); \
	done
	@for subshell in $(SHELL_BINS); do \
		(rm -f $(SDK_SBIN_DIR)/$$subshell;); \
	done

distclean: clean

%.a: $(LIBXXX_OBJS)
	@echo 'Building lib (static): $@'
	$(AR) $(ARFLAGS) $@ $(LIBXXX_OBJS)
	@echo 'Finished building lib (static): $@'
	@echo ' '

%.so: $(LIBXXX_OBJS)
	@echo 'Building lib (shared): $@'
	$(CC) -shared -o $@ $(LIBXXX_OBJS)
	@echo 'Finished building lib (shared): $@'
	@echo ' '

install: all
	@for subbin in $(CLEAN_BINS); do \
		cp -avf $$subbin $(SDK_BIN_DIR); \
		$(STRIP) $(SDK_BIN_DIR)/$$subbin; \
	done
	@for sublib in $(CLEAN_LIBS); do \
		cp -avf $$sublib $(SDK_LIB_DIR); \
		$(STRIP) $(SDK_LIB_DIR)/$$sublib; \
	done
	@for subheader in $(HEADER_FILES); do \
		cp -avf $$subheader $(SDK_INC_DIR); \
	done
	@for subshell in $(SHELL_BINS); do \
		cp -avf $$subshell $(SDK_SBIN_DIR); \
	done
ifneq ("$(PJ_HAS_ZWARE)", "yes")
	mkdir -p $(SDK_INC_DIR)/zware
	cp -avrf ./zware/* $(SDK_INC_DIR)/zware
endif

romfs: install
ifneq ("$(HOMEX_ROOT_DIR)", "")
	@for subbin in $(DUMMY_BINS); do \
		cp -avf $$subbin $(HOMEX_BIN_DIR); \
		$(STRIP) $(HOMEX_BIN_DIR)/$$subbin; \
	done
	@for sublib in $(CLEAN_LIBS); do \
		cp -avf $$sublib $(HOMEX_LIB_DIR); \
		$(STRIP) $(HOMEX_LIB_DIR)/$$sublib; \
	done
	#@for subheader in $(HEADER_FILES); do \
	#	cp -avf $$subheader $(HOMEX_INC_DIR); \
	#done
	@for subshell in $(SHELL_BINS); do \
		cp -avf $$subshell $(HOMEX_SBIN_DIR); \
	done
endif
