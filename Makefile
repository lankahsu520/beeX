PWD=$(shell pwd)
-include $(SDK_CONFIG_CONFIG)

#[major].[minor].[revision].[build]
VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_REVISION = 0
VERSION_FULL = $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_REVISION)
LIBNAME = beex

#** CFLAGS & LDFLAGS **
CFLAGS += $(CFLAGS_OTHERS) $(CFLAGS_CUSTOMER)

CFLAGS += \
					-DPJ_BEEX_ZWARE \
					-I./
LDFLAGS += \
					-L./
ARFLAGS = rcs

#** LIBXXX_OBJS **
LIBXXX_OBJS += \
							beex_honeycomb.o \
							beex_tracker.o \
							beex_report.o \
							beex_translater.o

#** LIBXXX_yes **
#LIBXXX_A = lib$(LIBNAME).a
LIBXXX_SO = lib$(LIBNAME).so
#LIBXXXS_$(PJ_HAS_STATIC_LIB) += $(LIBXXX_A)
#LIBXXXS_$(PJ_HAS_SHARE_LIB) += -l$(LIBNAME)
LIBXXXS_yes += -l$(LIBNAME)

#** HEADER_FILES **
HEADER_FILES = \
							beex_api.h \
							beex_honeycomb.h \
							beex_report.h \
							beex_translater.h \
							beex_zware.h \
							beex_def.h

#** librarys **
LIBS_yes = $(LIBXXXS_yes)
#** LIBS_yes, CLEAN_BINS, DUMMY_BINS  **
-include ./library.mk

LIBS += $(LIBS_yes)
#-ldl -lpthread -lm

#** Clean **
CLEAN_OBJS = $(LIBXXX_OBJS)
CLEAN_LIBS = $(LIBXXX_A) $(LIBXXX_SO)

#** Target (CLEAN_BINS) **
CLEAN_BINS += \
							beex_123

#** Target (DUMMY_BINS) **
DUMMY_BINS += \

CLEAN_BINS += $(DUMMY_BINS)
CLEAN_OBJS += $(addsuffix .o, $(CLEAN_BINS))

#** Target (SHELL_SBINS) **
SHELL_SBINS = \

DUMMY_SBINS = $(SHELL_SBINS)

#** Target (CONFS) **
CONFS = \
				$(wildcard conf/*.conf)

TO_FOLDER =

#** include *.mk **
-include define.mk

.DEFAULT_GOAL = all
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
	$(PJ_SH_RM) Makefile.bak $(CLEAN_BINS) $(CLEAN_BINS:=.elf) $(CLEAN_BINS:=.gdb)
	$(PJ_SH_RM) .configured .patched $(addsuffix *, $(CLEAN_LIBS)) $(CLEAN_OBJS) $(CLEAN_OBJS:%.o=%.c.bak) $(CLEAN_OBJS:%.o=%.h.bak)
	@for subbin in $(CLEAN_BINS); do \
		($(PJ_SH_RM) $(SDK_BIN_DIR)/$$subbin;); \
	done
	@for sublib in $(CLEAN_LIBS); do \
		($(PJ_SH_RM) $(SDK_LIB_DIR)/$$sublib*;); \
	done
	@for subheader in $(HEADER_FILES); do \
		($(PJ_SH_RM) $(SDK_INC_DIR)/$$subheader;); \
	done
	@for subshell in $(SHELL_SBINS); do \
		($(PJ_SH_RM) $(SDK_SBIN_DIR)/$$subshell;); \
	done
	@$(PJ_SH_RMDIR) build_xxx .meson_config build.meson meson_options.txt meson_public

distclean: clean

%.a: $(LIBXXX_OBJS)
	@echo 'Building lib (static): $@'
	$(AR) $(ARFLAGS) $@ $(LIBXXX_OBJS)
	@echo 'Finished building lib (static): $@'
	@echo ' '

%.so: $(LIBXXX_OBJS)
	@echo 'Building lib (shared): $@'
	$(CC) -shared $(LDFLAGS) -Wl,-soname,$@.$(VERSION_MAJOR) -o $@.$(VERSION_FULL) $(LIBXXX_OBJS)
	ln -sf $@.$(VERSION_FULL) $@.$(VERSION_MAJOR)
	ln -sf $@.$(VERSION_MAJOR) $@
	@echo 'Finished building lib (shared): $@'
	@echo ' '

install: all
	@for subbin in $(CLEAN_BINS); do \
		$(PJ_SH_CP) $$subbin $(SDK_BIN_DIR); \
		$(STRIP) $(SDK_BIN_DIR)/`basename $$subbin`; \
	done
	@for sublib in $(CLEAN_LIBS); do \
		$(PJ_SH_CP) $$sublib* $(SDK_LIB_DIR); \
		$(STRIP) $(SDK_LIB_DIR)/$$sublib; \
	done
	@for subheader in $(HEADER_FILES); do \
		$(PJ_SH_CP) $$subheader $(SDK_INC_DIR); \
	done
	@for subshell in $(SHELL_SBINS); do \
		$(PJ_SH_CP) $$subshell $(SDK_SBIN_DIR); \
	done

romfs: install
ifneq ("$(HOMEX_ROOT_DIR)", "")
	@for subbin in $(DUMMY_BINS); do \
		$(PJ_SH_CP) $$subbin $(HOMEX_BIN_DIR); \
		$(STRIP) $(HOMEX_BIN_DIR)/`basename $$subbin`; \
	done
	@for sublib in $(CLEAN_LIBS); do \
		$(PJ_SH_CP) $$sublib* $(HOMEX_LIB_DIR); \
		$(STRIP) $(HOMEX_LIB_DIR)/$$sublib; \
	done
	#@for subheader in $(HEADER_FILES); do \
	#	$(PJ_SH_CP) $$subheader $(HOMEX_INC_DIR); \
	#done
	@for subshell in $(DUMMY_SBINS); do \
		$(PJ_SH_CP) $$subshell $(HOMEX_SBIN_DIR); \
	done
endif

