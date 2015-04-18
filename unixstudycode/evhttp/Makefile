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
LDFLAGS=-lstdc++ -L/usr/local/lib -lpthread -levent

C_SRCS=$(wildcard *.c)
CPP_SRCS=$(wildcard *.cc) 

OBJS=$(patsubst %.c, %.o, $(C_SRCS) )		
OBJS+=$(patsubst %.cc, %.o, $(CPP_SRCS) )
PROGRAMS=$(patsubst %.o, %, $(OBJS) )		
		
SUBDIRS=

    
TOPDIR=`pwd`

all: $(OBJS) $(PROGRAMS) 
	for t in $(SUBDIRS); do $(MAKE) -C $$t || exit 1; done

pcre_test: pcre_test.o
	$(CC) $(LDFLAGS) $^ -lpcre -o $@
	
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@
	
%.o : %.cc
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -rf *.o $(PROGRAMS) $(OBJS)
	for t in $(SUBDIRS); do $(MAKE) clean -C $$t || exit 1; done

