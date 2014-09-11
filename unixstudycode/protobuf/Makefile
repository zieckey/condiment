# weizili@360.cn 

CC=gcc
CXX=g++
AR=ar
ARFLAGS=cru
COMMONFLAGS=-g -D_DEBUG -fPIC  -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings -Wsign-compare -Winvalid-pch -fms-extensions -Wfloat-equal -Wextra -Wall -Wno-unused-parameter -Wno-unused-function -Wunused-variable -Wunused-value -I../../../qmodule/include -I/usr/local/include -I. 
CFLAGS=$(COMMONFLAGS)
CCFLAGS=-Woverloaded-virtual -Wsign-promo -fno-gnu-keywords $(COMMONFLAGS) 
LDFLAGS=-L /home/weizili/bin/lib -lprotobuf 
protobuf=addressbook
objs=$(protobuf).pb.o $(protobuf).o


all : $(protobuf).pb.h $(protobuf)

$(protobuf) : $(objs)
	$(CXX) $(objs) $(LDFLAGS) -o $@

$(protobuf).pb.h : $(protobuf).proto
	protoc -I=. --cpp_out=. $(protobuf).proto

%.o : %.cc
	$(CXX) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -rf *.o $(protobuf)

