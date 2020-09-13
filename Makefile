 #
 # @file Makefile
 # @author Maurício Spinardi (mauricio.spinardi@gmail.com)
 # @platform cygwin64
 # @brief Makefile
 # @date 2020-09-03
 #
 #

ifndef ROOT_DIR
$(error ROOT_DIR not defined!)
endif

.EXPORT_ALL_VARIABLES:

ROOT_DIR := \
$(subst \,/,$(ROOT_DIR))

# Basic definitions

CFG_NAME := \
$(if $(CFG_NAME),$(CFG_NAME),release)

PROJECT_NAME := \
quake-log-parser

ifeq ($(CFG_NAME),tests)
PROJECT_NAME := \
libqlp-unit-tests
endif

PROJECT_VERSION := \
01.00.00

BIN_DIR := \
./bin/$(CFG_NAME)

BUILD_DIR := \
./build/$(CFG_NAME)

DEF := \
-D_PROJECT_NAME_="\"$(PROJECT_NAME)\"" \
-D_PROJECT_VERSION_="\"$(PROJECT_VERSION)\""

ifeq ($(CFG_NAME),tests)
DEF += \
-D_UNIT_TESTS_
endif

# Executables

CC := \
gcc

CPP := \
g++

# Compiling options

CFLAGS := \
-O3

ifeq ($(CFG_NAME),debug)
CFLAGS := \
-O0 -g

DEF += \
-D_DEBUG_
endif

CFLAGS += \
$(DEF) \
-I"include" \
-I"include/libcjson" \
-Wall \
-Wextra

# Linking options

LDFLAGS += \
-Wl,-Map=$(BUILD_DIR)/$(PROJECT_NAME).map \
-Wl,-rpath-link=./lib

# Output objects

OBJS := \
$(patsubst src/%,$(BUILD_DIR)/%.o,$(wildcard src/*.c*)) \
$(patsubst src/libcjson/cJSON.c,$(BUILD_DIR)/cJSON.c.o,$(wildcard src/libcjson/cJSON.c)) \
$(patsubst src/libqlp/%,$(BUILD_DIR)/%.o,$(wildcard src/libqlp/*.c*))

ifeq ($(CFG_NAME),tests)
OBJS := \
$(subst main.c,tests.c,$(OBJS))
endif

# Building rules

.PHONY: all
all:
	@$(MAKE) --no-print-directory -j4 -Orecurse build

.PHONY: build
build: prebuild $(OBJS)
	@echo - linking with g++...
	@$(CPP) $(OBJS) -Wl,--start-group $(LDFLAGS) -Wl,--end-group -o $(BIN_DIR)/$(PROJECT_NAME).exe
	@echo -n Finished building application at & date "+ %F %T"

$(BUILD_DIR)/%.c.o: src/%.c
	@echo - compiling with gcc $<...
	@$(CC) -std=c99 $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.o: src/libcjson/%.c
	@echo - compiling with gcc $<...
	@$(CC) -std=c89 $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.o: src/libqlp/%.c
	@echo - compiling with gcc $<...
	@$(CC) -std=c99 $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.o: tests/%.c
	@echo - compiling with gcc $<...
	@$(CC) -std=c99 $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@echo Cleaning...
	@rm -fr ./bin
	@rm -fr ./build

.PHONY: prebuild
prebuild:
	@echo Starting build $(PROJECT_NAME) in $(CFG_NAME) mode...
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)
