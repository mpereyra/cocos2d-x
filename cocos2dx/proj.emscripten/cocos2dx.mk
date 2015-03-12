all:

DEFINES += -DEMSCRIPTEN -DCC_KEYBOARD_SUPPORT

THIS_MAKEFILE := $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
ifndef COCOS_ROOT
#COCOS_ROOT ?= $(realpath $(dir $(THIS_MAKEFILE))/../..)
$(error COCOS_ROOT not defined)
else
RPATH_REL = ../..
endif
COCOS_SRC = $(COCOS_ROOT)/cocos2dx
OBJ_DIR ?= obj

EMSCRIPTEN_VERSION = 1.29.0
EMSCRIPTEN_ROOT ?= $(realpath $(COCOS_ROOT)/external/emsdk_portable/emscripten/$(EMSCRIPTEN_VERSION))
PACKAGER := $(EMSCRIPTEN_ROOT)/tools/file_packager.py

AR  := $(EMSCRIPTEN_ROOT)/emar
CC  := $(EMSCRIPTEN_ROOT)/emcc
CXX := $(EMSCRIPTEN_ROOT)/em++
ARFLAGS = cr

# XXX: Not entirely sure why main, malloc and free need to be explicitly listed
# here, but after adding a --js-library library, these symbols seem to get
# stripped unless enumerated here.
##EXPORTED_FLAGS := -s EXPORTED_FUNCTIONS="['_CCTextureCacheEmscripten_addImageAsyncCallBack','_CCTextureCacheEmscripten_preMultiplyImageRegion','_malloc','_free','_main']"
##JSLIBS := --js-library $(COCOS_SRC)/platform/emscripten/CCTextureCacheEmscripten.js
#EXPORTED_FLAGS := -s EXPORTED_FUNCTIONS="['_malloc','_free','_main']"

CCFLAGS  += -MMD -Wall -fPIC -Qunused-arguments -Wno-overloaded-virtual -s TOTAL_MEMORY=268435456 -s VERBOSE=1 -U__native_client__ -Wno-deprecated-declarations $(EXPORTED_FLAGS) $(JSLIBS) -s DISABLE_EXCEPTION_CATCHING=0
CXXFLAGS += -MMD -Wall -fPIC -Qunused-arguments -Wno-overloaded-virtual -s TOTAL_MEMORY=268435456 -s VERBOSE=1 -U__native_client__ -Wno-deprecated-declarations $(EXPORTED_FLAGS) $(JSLIBS) -s DISABLE_EXCEPTION_CATCHING=0 -std=c++11

LIB_DIR = $(COCOS_ROOT)/lib/emscripten
BIN_DIR = bin

INCLUDES +=  \
    -I$(COCOS_SRC) \
    -I$(COCOS_SRC)/cocoa \
    -I$(COCOS_SRC)/include \
    -I$(COCOS_SRC)/kazmath/include \
    -I$(COCOS_SRC)/platform/emscripten \
    -I$(COCOS_SRC)/platform/third_party/emscripten/libpng \
    -I$(COCOS_SRC)/platform/third_party/emscripten/libz \
    -I$(COCOS_SRC)/platform/third_party/emscripten/libtiff/include \
    -I$(COCOS_SRC)/platform/third_party/emscripten/libjpeg \
    -I$(COCOS_SRC)/platform/third_party/emscripten/libwebp \

LBITS := $(shell getconf LONG_BIT)
INCLUDES += -I$(COCOS_SRC)/platform/third_party/linux

ifeq ($(VERBOSE), 1)
CCFLAGS  += -O1 -s GL_UNSAFE_OPTS=0 -s INVOKE_RUN=0
CXXFLAGS += -O1 -s GL_UNSAFE_OPTS=0 -s INVOKE_RUN=0
DEFINES += -D_DEBUG -DCOCOS2D_DEBUG=0 -DCP_USE_DOUBLES=0 -DDEBUG=1
SUBDIR := verbose
else ifeq ($(DEBUG), 1)
CCFLAGS  += -O1 -s GL_UNSAFE_OPTS=0 -s INVOKE_RUN=0
CXXFLAGS += -O1 -s GL_UNSAFE_OPTS=0 -s INVOKE_RUN=0
DEFINES += -D_DEBUG -DCOCOS2D_DEBUG=0 -DCP_USE_DOUBLES=0 -DDEBUG=1 -DSUPPRESS_DLOG=1
SUBDIR := debug
else ifeq ($(ADHOC), 1)
CCFLAGS += -O2 -s GL_UNSAFE_OPTS=0
CXXFLAGS += -O2 -s GL_UNSAFE_OPTS=0
DEFINES += -DNDEBUG -DCP_USE_DOUBLES=0 -DADHOC=1 -DSUPPRESS_DLOG=1
SUBDIR := adhoc
else ifeq ($(RELEASE), 1)
CCFLAGS += -O2 -s GL_UNSAFE_OPTS=0
CXXFLAGS += -O2 -s GL_UNSAFE_OPTS=0
DEFINES += -DNDEBUG -DCP_USE_DOUBLES=0 -DSUPPRESS_DLOG=1
SUBDIR := release
endif

OBJ_DIR := $(OBJ_DIR)/$(SUBDIR)
LIB_DIR := $(LIB_DIR)/$(SUBDIR)
BIN_DIR := $(BIN_DIR)/$(SUBDIR)


ifndef V
LOG_CC = @echo " CC $@";
LOG_CXX = @echo " CXX $@";
LOG_AR = @echo " AR $@";
LOG_LINK = @echo " LINK $@";
endif

OBJECTS := $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.c=.o)
OBJECTS := $(subst ../,,$(OBJECTS))
OBJECTS := $(subst $(COCOS_ROOT)/,,$(OBJECTS))
OBJECTS := $(addprefix $(OBJ_DIR)/, $(OBJECTS))
DEPS = $(OBJECTS:.o=.d)
CORE_MAKEFILE_LIST := $(MAKEFILE_LIST)
-include $(DEPS)

STATICLIBS_DIR = $(COCOS_SRC)/platform/third_party/emscripten/libraries
STATICLIBS = \
    $(STATICLIBS_DIR)/libtiff.a \
    $(STATICLIBS_DIR)/libjpeg.a \
    $(STATICLIBS_DIR)/libxml2.a \
    $(STATICLIBS_DIR)/libwebp.a \
    $(STATICLIBS_DIR)/libz.a \
    $(STATICLIBS_DIR)/libpng.a
    

# SHAREDLIBS += -L$(LIB_DIR) -Wl,-rpath,$(RPATH_REL)/$(LIB_DIR)
# LIBS = -lrt -lz

SHAREDLIBS += -L$(LIB_DIR)
# LIBS = -lrt -lz

HTMLTPL_DIR = $(COCOS_ROOT)/tools/emscripten-templates/basic
HTMLTPL_FILE = index.html

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(TARGET).js $(TARGET).data $(TARGET).data.js $(BIN_DIR) core

.PHONY: all clean

# If the parent Makefile defines $(EXECUTABLE) then define this as the target
# and create a 'make run' rule to run the app.
ifdef EXECUTABLE
TARGET := $(BIN_DIR)/$(EXECUTABLE)

all: $(TARGET).js $(TARGET).data $(BIN_DIR)/$(HTMLTPL_FILE)

run: $(TARGET)
	cd $(dir $^) && ./$(notdir $^)

.PHONY: run
endif
