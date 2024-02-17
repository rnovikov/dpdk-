# SPDX-License-Identifier: BSD-3-Clause
# Copyright(c) 2010-2014 Intel Corporation

# binary name
APP = hello_world

# all source are stored in SRCS-y

SRCS-y += main.cpp



THIRD_PARTY_PATH?=../../../third_party


ERROR_FLAGS = -Werror -Wno-error=unused-result
#CXX=clang++
CXX?=g++-11
#CXX=/usr/lib/ccache/g++

DC_BUILD_DIR?=../../dpdk++/build
LDFLAGS_STATIC = $(shell $(PKGCONF) --static --libs libdpdk) 
LDFLAGS_STATIC+=-L${THIRD_PARTY_PATH}/dpdk++/build/ -l:libdpdk++.a 
CFLAGS+= -std=c++17   -I${THIRD_PARTY_PATH}/dpdk++/ 


#LDFLAGS_STATIC+= 
CFLAGS+=    -I${THIRD_PARTY_PATH}/argparse/include




ifdef LOG_LEVEL
CFLAGS+= -DLOG_LEVEL=$(LOG_LEVEL)
else
CFLAGS+= -DLOG_LEVEL=9
#$(error "PLEASE SCECIFY Logging level (from 0 to 9, where 0 - no log at all) using variable LOG_LEVEL example make LOG_LEVEL=8 ")
endif





CFLAGS+= -I$(PROJECT_DIR)  -pthread  $(ERROR_FLAGS)  



#LDFLAGS_STATIC+= 




PKGCONF = pkg-config

# Build using pkg-config variables if possible
ifneq ($(shell $(PKGCONF) --exists libdpdk && echo 0),0)
$(error "no installation of DPDK found")
endif

all: static
.PHONY: static

static: build/$(APP)


PC_FILE := $(shell $(PKGCONF) --path libdpdk 2>/dev/null)




CFLAGS +=  $(DF_API_INCLUDES) 
CFLAGS += $(shell $(PKGCONF) --cflags libdpdk) 
CFLAGS += ${DF_BUILD_FLAGS}
# Add flag to allow experimental API as l2fwd uses rte_ethdev_set_ptype API
CFLAGS += -DALLOW_EXPERIMENTAL_API 
LDFLAGS_SHARED = $(shell $(PKGCONF) --libs libdpdk)



  
#LDFLAGS_STATIC += $(PACE2_LIBRARY) -lipoque_pace2 -lz -lm


ifeq ($(MAKECMDGOALS),static)
# check for broken pkg-config
ifeq ($(shell echo $(LDFLAGS_STATIC) | grep 'whole-archive.*l:lib.*no-whole-archive'),)
$(warning "pkg-config output list does not contain drivers between 'whole-archive'/'no-whole-archive' flags.")
$(error "Cannot generate statically-linked binaries with this version of pkg-config")
endif
endif

OBJS := $(patsubst %.cpp,build/%.o,$(SRCS-y))

build/%.o: %.cpp Makefile $(PC_FILE) | build
	$(CXX) $(CFLAGS) -c $< -o $@


build/$(APP): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LDFLAGS_STATIC)
	
	
#$(LDFLAGS) $(LDFLAGS_STATIC)
#
	
        

build:
	@mkdir -p $@ 

.PHONY: clean
clean:
	rm -f build/$(APP)* build/*.o
	test -d build && rmdir -p build || true
