OBJECTS = cfg.o sutils.o opendaemon.o error.o module.o modman.o interfaceman.o cfg_directive.o cfg_section.o \
          cfg_value.o sockets.o process_pool.o assoc.o assoc_walker.o lex.yy.o cfg_parser.tab.o
BISON = bison
FLEX = flex
CFLAGS = -Wall
CC = gcc -g
VPATH = classes:apis

all: generate ${OBJECTS}
	${CC} $(wordlist 2, $(words $^), $^) -lfl -lm -ldl -o opendaemon

%.o: $(wildcard *.h)

lex.yy.o: lex.yy.c $(wildcard *.h)
	${CC} ${CFLAGS} -c -o lex.yy.o lex.yy.c
	
xmldsql.tab.o: cfg_parser.tab.c $(wildcard *.h)
	${CC} ${CFLAGS} -c -o cfg_parser.tab.o cfg_parser.tab.c
	
generate: cfg_parser.y cfg_parser.lex $(wildcard *.h)
	${BISON} -d cfg_parser.y -o cfg_parser.tab.c
	${FLEX} -f cfg_parser.lex
	
clean:
	rm -f opendaemon
	rm -f *.o
	rm -f apis/*.o
	rm -f classes/*.o
	rm -f cfg_parser.tab.h cfg_parser.tab.c
	rm -f lex.yy.c
	rm -f core
	rm -f core.*
