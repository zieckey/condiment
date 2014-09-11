# weizili@360.cn 
#
# The study test code project 
#
# Please use gmake but not make in FreeBSD OS
#

		
SUBDIRS=src/phase1_map src/phase1_reduce src/phase2_map src/phase2_reduce

TOPDIR=$(shell echo `pwd`)
_datenow=$(shell echo `date +%Y%m%d`)
_timenow=$(shell echo `date +%H%M%S`)
_ver=1.5.1_map_thread
PACKAGE_DIR=weizili_mapred_v$(_ver)_$(_datenow)$(_timenow)
PACKAGE=$(PACKAGE_DIR).tar.gz

all:
	for t in $(SUBDIRS); do $(MAKE) -C $$t; done

deploy:	
	$(MAKE) 
	mkdir -p $(PACKAGE_DIR)/logs
	rm -rf $(PACKAGE_DIR)/*
	cp ./bin/phase1* ./bin/phase2_reduce ./bin/run.sh $(PACKAGE_DIR)
	tar zcvf $(PACKAGE) $(PACKAGE_DIR)

t:	
	echo $(PACKAGE_DIR)
	
clean:
	for t in $(SUBDIRS); do $(MAKE) clean -C $$t; done

