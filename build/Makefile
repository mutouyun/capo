# Define exports

export CC ?= gcc
export CX ?= g++
export LB ?= ar

export CFLAGS ?= -pipe -frtti -Wall -Wextra -fexceptions -march=nocona -std=c++1y
export LFLAGS ?= -Wl,-s
export AFLAGS ?= -csr

# Check os

export os    = linux
export debug = 0
export bits  = 32

ifeq ($(os), win)
	# windows
	LFLAGS += -Wl,-subsystem,console -mthreads
	MK = mkdir
	RM = rmdir /s /q
	LIB_DIR = mingw$(bits)
	export THIRD_PATH = $(WORK_PATH)/third
    ifeq ($(debug), 0)
        export THIRD_LIB = $(THIRD_PATH)/gtest/lib/$(LIB_DIR)/gtest.a
    else
        export THIRD_LIB = $(THIRD_PATH)/gtest/lib/$(LIB_DIR)/gtestd.a
	endif
else
	# linux or others
	LFLAGS += -pthread
	MK = mkdir -p
	RM = rm -r
	LIB_DIR = $(os)-$(CX)
	export THIRD_PATH =
	export THIRD_LIB = -lgtest
endif

# Define workspace

export WORK_PATH  ?= $(CURDIR)/..
export BUILD_PATH ?= $(CURDIR)
export TESTS_PATH = $(WORK_PATH)/test
export INCPATH    = -I$(WORK_PATH)
ifneq ($(THIRD_PATH),)
	INCPATH += -I$(THIRD_PATH)
endif

ifeq ($(debug), 0)
	# release
	CFLAGS += -O2 -DNDEBUG
	CONFIG_DIR = release
else
	# debug
	CFLAGS += -g
	CONFIG_DIR = debug
endif

ifeq ($(bits), 32)
	# 32-bit
	CFLAGS += -m32
	LFLAGS += -m32
else
	# 64-bit
	CFLAGS += -m64
	LFLAGS += -m64
endif

# Output directory

export OUT = $(BUILD_PATH)/bin/$(CONFIG_DIR)/$(CC)
export TMP = $(BUILD_PATH)/tmp/$(CONFIG_DIR)/$(CC)

# Project

PRO_NAME = capo
SRC_PATH = $(WORK_PATH)/src
SRC_FILES = \
	$(SRC_PATH)/thread_local_ptr.cpp

MODULES = \
	ut-main ut-preprocessor ut-type_name ut-type_list \
	ut-concept ut-constant_array ut-types_to_seq ut-countof \
	ut-operator ut-max_min ut-sequence ut-range \
	ut-stopwatch ut-printf ut-assert ut-spin_lock \
	ut-semaphore ut-waiter ut-construct ut-signal \
	ut-gc ut-singleton ut-memory ut-trackable ut-any ut-cmdline

BUILD_RULES = $(PRO_NAME) $(MODULES)
include $(BUILD_PATH)/Makefile.Project

# Build

BUILD_TYPE    = lib
DEPEND_TARGET = output begin
include $(BUILD_PATH)/Makefile.Build

export DEPEND = $(OUT)/ut-main.a $(OUT)/$(PRO_NAME).a
$(MODULES): output
	@$(MAKE) -C $(TESTS_PATH)/$@

# Targets

output:
	@echo Making directorys...
	@-$(MK) "$(OUT)"
	@-$(MK) "$(TMP)"
	@echo Making complete.

clean:
	@echo Cleaning directorys...
	@-$(RM) "$(TMP)"
	@-$(RM) "$(OUT)"
	@echo Clean complete.

distclean:
	@echo Cleaning directorys...
	@-$(RM) "$(BUILD_PATH)/tmp"
	@-$(RM) "$(BUILD_PATH)/bin"
	@echo Clean complete.

# PHONY

.PHONY: all clean distclean
