

all:
	g++ --shared -fPIC -g -O0 -o libtestprofiler.so libtestprofiler.cpp
	g++ -g -O0 -o main main.cpp -ltestprofiler -L. -pg
	g++ -g -O0 -o gmain main.cpp -ltestprofiler -L. -lprofiler -L/home/weizili/bin/lib

run:
	export LD_LIBRARY_PATH=. ;  ./main ; gprof -b ./main

clean :
	rm -rf *.o
	rm -rf main gmain *.so
	rm -rf main.gmon
