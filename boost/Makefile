
all : 
	$(MAKE) -C libevent++
	$(MAKE) -C test

check : all
	$(MAKE) check -C test

clean:
	$(MAKE) clean -C libevent++
	$(MAKE) clean -C test
