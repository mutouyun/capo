# Define exports

MODULES = \
	ut-main ut-preprocessor ut-type_name ut-type_list ut-concept ut-constant_array ut-types_to_seq

export CC = gcc
export CX = g++
export LB = ar

export CFLAGS = -pipe -frtti -Wall -Wextra -fexceptions -march=nocona -std=c++11
export LFLAGS = -Wl,-s
export AFLAGS = -csr

export SP =
export MK =
export RM =

OS = linux
DEBUG = 0
BITS = 32

# Check os

ifeq ($(OS), win)
	LFLAGS += -Wl,-subsystem,console -mthreads
	SP = \\
	MK = mkdir
	RM = rmdir /s /q
	LIB_DIR = $(OS)$(BITS)-$(CX)
else
	LFLAGS += -pthread
	SP = /
	MK = mkdir -p
	RM = rm -r
	LIB_DIR = $(OS)-$(CX)
endif

# Define workspace

export WORK_PATH = $(CURDIR)$(SP)..
export BUILD_PATH = $(CURDIR)
export THIRD_PATH = $(WORK_PATH)$(SP)third
export THIRD_LIB =
export TESTS_PATH = $(WORK_PATH)$(SP)test
export INCPATH = -I$(WORK_PATH) -I$(THIRD_PATH)

ifeq ($(DEBUG), 0)
	# release
	CFLAGS += -O2 -DNDEBUG
	CONFIG_DIR = release
	THIRD_LIB += $(THIRD_PATH)$(SP)gtest$(SP)lib$(SP)$(LIB_DIR)$(SP)gtest.a
else
	# debug
	CFLAGS += -g
	CONFIG_DIR = debug
	THIRD_LIB += $(THIRD_PATH)$(SP)gtest$(SP)lib$(SP)$(LIB_DIR)$(SP)gtestd.a
endif

ifeq ($(BITS), 32)
	# 32-bit
	CFLAGS += -m32
	LFLAGS += -m32
else
	# 64-bit
	CFLAGS += -m64
	LFLAGS += -m64
endif

# Output directory

export OUT = $(BUILD_PATH)$(SP)bin$(SP)$(CONFIG_DIR)$(SP)$(CC)
export TMP = $(BUILD_PATH)$(SP)tmp$(SP)$(CONFIG_DIR)$(SP)$(CC)

# Project

PRO_NAME = capo
SRC_PATH = $(WORK_PATH)$(SP)src$(SP)
SRC_FILES = \
	$(SRC_PATH)thread_local_ptr.cpp

BUILD_RULES = output $(PRO_NAME) $(MODULES)
include $(BUILD_PATH)$(SP)Makefile.project

# Build

include $(BUILD_PATH)$(SP)Makefile.ar

# Targets

output:
	-$(MK) "$(OUT)"
	-$(MK) "$(TMP)"

clean:
	-$(RM) "$(TMP)"
	-$(RM) "$(OUT)"

export DEPEND = $(OUT)$(SP)ut-main.a $(OUT)$(SP)$(PRO_NAME).a
$(MODULES):
	$(MAKE) -C $(TESTS_PATH)$(SP)$@

.PHONY: all clean