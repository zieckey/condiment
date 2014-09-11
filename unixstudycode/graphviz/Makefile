
all :
	gcc -Wl,-Map=test.map -g -o test test.c
	grep main test.map | awk '{print $$1}'
	addr2line 0x0000000000400498 -e test -f

clean:
	rm -rf test test.map
