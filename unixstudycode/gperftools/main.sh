
#!/bin/sh

export LD_LIBRARY_PATH=.:/home/weizili/bin/lib
./main 
gprof -b ./main
