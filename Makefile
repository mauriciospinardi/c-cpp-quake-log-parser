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

# Definições básicas

CFG_NAME := \
$(if $(CFG_NAME),$(CFG_NAME),release)

PROJECT_NAME := \
quake-log-parser

PROJECT_VERSION := \
00.00.00

BIN_DIR := \
./bin/$(CFG_NAME)

BUILD_DIR := \
./build/$(CFG_NAME)

DEF := \
-D_PROJECT_NAME_="\"$(PROJECT_NAME)\"" \
-D_PROJECT_VERSION_="\"$(PROJECT_VERSION)\""

# Executáveis

CC := \
gcc

CPP := \
g++

# Opções de compilação

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
-Wall \
-Wextra

# Opções de vinculação

LDFLAGS += \
-Wl,-Map=$(BUILD_DIR)/$(PROJECT_NAME).map \
-Wl,-rpath-link=./lib

# Objetos de saída

OBJS := \
$(patsubst src/%,$(BUILD_DIR)/%.o,$(wildcard src/*.c*))

# Regras de compilação e instalação

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

$(BUILD_DIR)/%.cpp.o: src/%.cpp
	@echo - compiling with g++ $<...
	@$(CPP) -std=c++11 $(CFLAGS) -c $< -o $@

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
