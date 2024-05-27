# binary name
APP = sandbox
# all source are stored in SRCS-y
SRCS-y = main.cpp
PROJECT_DIR= .
#CFLAGS+= -Werror -Wno-error=unused-result
CXX=g++-11

CFLAGS+= -std=c++17 

ifdef LOG_LEVEL
CFLAGS+= -DLOG_LEVEL=$(LOG_LEVEL)
else
CFLAGS+= -DLOG_LEVEL=9
#$(error "PLEASE SPECIFY Log level (from 0 to 9, where 0 - no log at all) using variable LOG_LEVEL example make LOG_LEVEL=8 ")
endif

ifeq ($(VPP_BURST_SIZE),)
# $(error "Please define VPP_BURST_SIZE environment variable from 1 to 64")
VPP_BURST_SIZE?=32
endif
CFLAGS+= -DVPP_BURST_SIZE=$(VPP_BURST_SIZE)


LDFLAGS_STATIC = $(shell $(PKGCONF) --static --libs libdpdk) 


ifdef ENABLE_PACKET_FLOW
CFLAGS+= -DENABLE_PACKET_FLOW 
endif


ifeq ($(DPDKPP_PATH),)
$(error "Please define DPDKPP_PATH environment variable")
endif

DPDKPP_INCLUDES= -I$(DPDKPP_PATH)
DPDKPP_LIBRARY= -L$(DPDKPP_PATH)/build -ldpdk++
LDFLAGS_STATIC+=$(DPDKPP_LIBRARY)
CFLAGS+= $(DPDKPP_INCLUDES)

ARGPARSE_INCLUDE?= -I$(DPDKPP_PATH)/../third_party/argparse/include/
CFLAGS+=$(ARGPARSE_INCLUDE)


PKGCONF = pkg-config
# Build using pkg-config variables if possible
ifneq ($(shell $(PKGCONF) --exists libdpdk && echo 0),0)
$(error "no installation of DPDK found")
endif

all: static
.PHONY: static

static: build/$(APP)


PC_FILE := $(shell $(PKGCONF) --path libdpdk 2>/dev/null)

BUILD_FLAGS= -O0 -ggdb 
CFLAGS+= -I${PROJECT_DIR}
CFLAGS +=  -DVPP_BURST_SIZE=$(VPP_BURST_SIZE)
CFLAGS += $(shell $(PKGCONF) --cflags libdpdk) 
CFLAGS += ${BUILD_FLAGS}
# Add flag to allow experimental API as l2fwd uses rte_ethdev_set_ptype API
CFLAGS += -DALLOW_EXPERIMENTAL_API 
LDFLAGS_SHARED = $(shell $(PKGCONF) --libs libdpdk)
  



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

build:
	@mkdir -p $@ 

.PHONY: clean
clean:
	rm -f build/$(APP)* build/*.o
	test -d build && rmdir -p build || true
