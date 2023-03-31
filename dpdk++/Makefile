
# binary name
APP = libdpdk++.a

# all source are stored in SRCS-y


SRCS-y := dpdk_cover/cdpdk.cpp
SRCS-y += dpdk_cover/cmbuf.cpp
SRCS-y += dpdk_cover/cmempool.cpp

SRCS-y += tools/cpcapreader.cpp
SRCS-y += tools/cpcapwriter.cpp
SRCS-y += tools/json_tools.cpp
SRCS-y += tools/string_tools.cpp

SRCS-y += logger/logger_api.cpp

SRCS-y += config/ealparameters.cpp



PROJECT_DIR= .
ERROR_FLAGS = -Werror -Wno-error=unused-result

CXX?=g++-11
CFLAGS+= -O0 -ggdb 
CFLAGS+= -std=c++17 

ifdef LOG_LEVEL
CFLAGS+= -DLOG_LEVEL=$(LOG_LEVEL)
else
$(error "PLEASE SCECIFY Logging level (from 0 to 9, where 0 - no log at all) using variable LOG_LEVEL example make LOG_LEVEL=8 ")
endif

LDFLAGS_STATIC = $(shell $(PKGCONF) --static --libs libdpdk)


ifdef ENABLE_PACKET_FLOW
CFLAGS+= -DENABLE_PACKET_FLOW 
endif
CFLAGS+= -I${PROJECT_DIR}
#CFLAGS+= -DPRINT_BUILD_FLAGS='"$(BUILD_FLAGS)"'

ifeq ($(VPP_BURST_SIZE),)
$(error "Please define VPP_BURST_SIZE environment variable from 1 to 64")
endif
CFLAGS+= -DVPP_BURST_SIZE=$(VPP_BURST_SIZE)

PKGCONF = pkg-config

# Build using pkg-config variables if possible
ifneq ($(shell $(PKGCONF) --exists libdpdk && echo 0),0)
$(error "no installation of DPDK found")
endif

all: static
.PHONY: static
static: build/$(APP)
PC_FILE := $(shell $(PKGCONF) --path libdpdk 2>/dev/null)

CFLAGS += $(shell $(PKGCONF) --cflags libdpdk) 
CFLAGS += ${BUILD_FLAGS}
# Add flag to allow experimental API as l2fwd uses rte_ethdev_set_ptype API
CFLAGS += -DALLOW_EXPERIMENTAL_API 


NLOHMANN_INCLUDE?= -I$(PROJECT_DIR)/../third_party/json/include/
CFLAGS+=$(NLOHMANN_INCLUDE)
CFLAGS+=$(ERROR_FLAGS) 



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
	echo "LD FLAGS=$(LDFLAGS)"
	ar -r -o $@ $^
	

        

build:
	@mkdir -p $@ $@/dpdk_cover/ $@/tools/ $@/preprocessor_tools $@/config/ $@/logger/ 

.PHONY: clean
clean:
	rm -f build/$(APP)* build/*.o
	test -d build && rmdir -p build || true
