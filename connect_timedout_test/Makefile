# weizili@360.cn 

CC=gcc
CXX=g++
AR=ar
ARFLAGS=cru
MUDUOPATH=$(HOME)/mcx/3rdparty/muduo
CFLAGS= -g -c -fPIC \
	   -Wshadow -Wcast-qual -Wcast-align \
	   -Wwrite-strings -Wsign-compare \
	   -Winvalid-pch -fms-extensions -Wall \
	   -I$(HOME)/opensource/boost_1_53_0 \
	   -I$(MUDUOPATH) \
	   -MMD
CPPFLAGS=$(CFLAGS) -Woverloaded-virtual -Wsign-promo -fno-gnu-keywords 
LDFLAGS=-L/usr/local/lib $(MUDUOPATH)/../build/debug/lib/libmuduo_net.a $(MUDUOPATH)/../build/debug/lib/libmuduo_base.a -lpthread 

SRCS := $(wildcard *.cc)
OBJS := $(patsubst %.cc, .obj/%.o, $(SRCS))
DEPS := $(patsubst %.o, %.d, $(OBJS))
PROGRAM := $(shell basename `pwd`)

all: prepare $(PROGRAM)

$(PROGRAM) : $(OBJS) 
	$(CXX) $^ $(RTFLAGS) $(LDFLAGS) -o $@

.obj/%.o : %.cc
	$(CXX) $< $(CPPFLAGS) -o $@

-include $(DEPS)

prepare : 
	@mkdir -p .obj

clean:
	rm -rf *.o *.d $(OBJS) $(DEPS) $(PROGRAM)

