
CC=cc
CXX=cc
AR=ar
ARFLAGS=cru
CPPFLAGS=-g -O -fPIC -Wall -I/usr/local/include -I. 
LDFLAGS=-lstdc++ -L/usr/local/lib -lpthread -lboost_thread-mt

CPP_SRCS=$(wildcard *.cc) 

OBJS=$(patsubst %.cc, %.o, $(CPP_SRCS) )
TARGET=main

all: $(OBJS) $(TARGET)
	
%.o : %.cc
	$(CXX) -c $(CPPFLAGS) $< -o $@

clean:
	rm -rf *.o $(OBJS) $(TARGET)

