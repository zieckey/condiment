
#!/bin/sh

export LD_LIBRARY_PATH=.:/home/weizili/bin/lib
CPUPROFILE=perf.out ./gmain 
pprof --text ./gmain ./perf.out
