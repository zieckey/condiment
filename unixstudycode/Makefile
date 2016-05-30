# weizili@360.cn 
#
# The study test code project 
#
# Please use gmake but not make in FreeBSD OS
#


CC=cc
CXX=cc
AR=ar
ARFLAGS=cru
CFLAGS=-g -D_DEBUG -D_TRACE -pg -fPIC -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings -Wsign-compare -Winvalid-pch -fms-extensions -Wall -I/usr/local/include -I.
CPPFLAGS=-Woverloaded-virtual -Wsign-promo -fno-gnu-keywords 
LDFLAGS=-lstdc++ -L/usr/local/lib -lpthread -ldl

C_SRCS=$(wildcard *.c)
CPP_SRCS=$(wildcard *.cpp) 
CC_SRCS=$(wildcard *.cc) 

OBJS=$(patsubst %.c, %.o, $(C_SRCS) )		
OBJS+=$(patsubst %.cpp, %.o, $(CPP_SRCS) )
OBJS+=$(patsubst %.cc, %.o, $(CC_SRCS) )
PROGRAMS=$(patsubst %.o, %, $(OBJS) )		
		
SUBDIRS=echo-server \
				evhttp \
				c_call_cpp \
				rsa \
				protobuf \
				shared_ptr \
				libevent \
				test_map_performance \
				boost \
				boost_asio \
				boost_program_options \
				boost_ptree \
				boost_shared_memory_map \
				url_extractor \
				tun \
				ucontext \
				string_copy 

    
TOPDIR=`pwd`

all: $(OBJS) $(PROGRAMS) 
	for t in $(SUBDIRS); do $(MAKE) -C $$t || exit 1; done
	
zlib : zlib.o 
	$(CC) $(LDFLAGS)  -lz zlib.o -o $@
	
pcre_test: pcre_test.o
	$(CC) $(LDFLAGS) $^ -lpcre -o $@
	
genrsakey: genrsakey.o
	$(CC) $(LDFLAGS) $^ -lcrypto -o $@
	
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@
	
%.o : %.cpp
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

%.o : %.cc
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -rf *.o $(PROGRAMS) $(OBJS)
	for t in $(SUBDIRS); do $(MAKE) clean -C $$t || exit 1; done

