# weizili@360.cn 
TOPDIR := $(shell pwd)

CC=gcc
CXX=g++
AR=ar
ARFLAGS=cru
CFLAGS=-g -fPIC -Wconversion -Wall -Wformat 
CPPFLAGS=-Woverloaded-virtual -Wsign-promo -fno-gnu-keywords 
LDFLAGS=-lstdc++ -lcrypto
objs=sign_rsa.o 
program=sign_rsa 

all : $(program)
	
sign_rsa : sign_rsa.o
	$(CC) sign_rsa.o $(LDFLAGS) -o $@

clean:
	rm -rf *.o $(program)
