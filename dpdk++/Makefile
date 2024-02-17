
# binary name
LIB_NAME = libdpdk++.a

PKGCONF = pkg-config
LDFLAGS_STATIC= -lpcap
# all source are stored in SRCS-y


SRCS-y := base/base_types.cpp

SRCS-y += base/base_enums.cpp
SRCS-y += base/cfivetuplekey.cpp

SRCS-y+= connections/connection.cpp


SRCS-y += packet/sfivetyplesession.cpp 
SRCS-y += packet/cmbuf.cpp
 
SRCS-y += dpdk_cover/cdpdk.cpp
SRCS-y += dpdk_cover/cknimanager.cpp

SRCS-y += logger/csimplelogger.cpp



SRCS-y += tools/cpcapreader.cpp
SRCS-y += tools/cpcapwriter.cpp
SRCS-y += tools/json_tools.cpp
SRCS-y += tools/string_tools.cpp
SRCS-y += tools/dpdk_tools.cpp
SRCS-y += tools/tools.cpp
SRCS-y += tools/ctableview.cpp


SRCS-y += threads/ithread.cpp



THIRD_PARTY_PATH?=../../third_party/

PROJECT_DIR= .
ERROR_FLAGS = -Werror -Wno-error=unused-result

CXX?=g++-11
CFLAGS+= -O0 -ggdb 
CFLAGS+= -std=c++20 

ifdef LOG_LEVEL

else
LOG_LEVEL=8
#$(error "PLEASE SCECIFY Logging level (from 0 to 9, where 0 - no log at all) using variable LOG_LEVEL example make LOG_LEVEL=8 ")
endif
CFLAGS+= -DLOG_LEVEL=$(LOG_LEVEL)
LDFLAGS_STATIC += $(shell $(PKGCONF) --static --libs libdpdk)



CFLAGS+= -I${PROJECT_DIR}
ifneq ($(shell $(PKGCONF) --exists libdpdk && echo 0),0)
$(error "no installation of DPDK found")
endif


CFLAGS += $(shell $(PKGCONF) --cflags libdpdk) 
CFLAGS += ${BUILD_FLAGS}
# Add flag to allow experimental API as l2fwd uses rte_ethdev_set_ptype API
CFLAGS += -DALLOW_EXPERIMENTAL_API 

CFLAGS+=$(ERROR_FLAGS) 

#LDFLAGS=$(LDFLAGS_STATIC)

all: static
.PHONY: static

static: build/$(LIB_NAME)


OBJS := $(patsubst %.cpp,build/%.o,$(SRCS-y))

build/%.o: %.cpp Makefile $(PC_FILE) | build
	$(CXX) $(CFLAGS) -c $< -o $@


build/$(LIB_NAME): $(OBJS)
	ar -r -o $@ $^

       

build:
	@mkdir -p $@ $@/dpdk_cover/ $@/connections $@/tools/ $@/base  $@/logger/ $@/ports  $@/packet  $@/threads

.PHONY: clean
clean:
	rm -f build/$(LIB_NAME)* build/*.o
	test -d build && rmdir -p build || true
