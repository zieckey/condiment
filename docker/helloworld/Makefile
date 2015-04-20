
GCC=gcc

all:
	$(GCC) -g -Wall main.c -o helloworld
	sudo docker build -t="zieckey/helloworld" .
	sudo docker run -i -t zieckey/helloworld helloworld

t :
	echo ${CURRENTWORKDIR}
