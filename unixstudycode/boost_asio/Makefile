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
CFLAGS=-g -D_DEBUG -D_TRACE -I/usr/include/boost141 -I.
CPPFLAGS=
LDFLAGS=-lstdc++ -L/usr/local/lib -lpthread -lrt -lboost_system-mt

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

