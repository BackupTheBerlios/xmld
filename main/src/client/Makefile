OBJECTS = xmldclient.o protoimpl.o xmld_sockets.o sutils.o
CC = /usr/local/bin/gcc
CFLAGS = -Wall
VPATH = ../

all: ${OBJECTS}
	${CC} $^ -o xmldclient

%.o: $(wildcard *.h) $(wildcard ../*.h)

clean: 
	rm -f xmldclient
	rm -f *.o
	rm -f core
