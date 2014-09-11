# weizili@360.cn 
TOPDIR := $(shell pwd)

CC=gcc
CXX=g++
AR=ar
ARFLAGS=cru
CFLAGS=-g -O3 -fPIC -Wconversion -Wall -Wformat 
CPPFLAGS=-Woverloaded-virtual -Wsign-promo -fno-gnu-keywords 
LDFLAGS=-lstdc++
objs=cppsayhello.o main.o 
program=cppsayhello 

all : $(program)
	
$(program) : $(objs)
	$(CC) $(objs) $(LDFLAGS) -o $@

clean:
	rm -rf *.o $(program)
