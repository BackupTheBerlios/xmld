OBJECTS = authman.o cfg.o cfg_parser.o dutils.o engine_list.o interface_list.o fmanager.o func_list.o mfigure.o \
	  passfd.o protoimpl.o resptrans.o sutils.o xmld_interface.o somanager.o twalker.o xmld_aggr_table.o \
	  xmld.o xmld_col.o xmld_connection.o xmld_directive.o xmld_engine.o xmld_expr.o \
	  xmld_file.o xmld_func.o xmld_list.o xmld_request.o xmld_response.o xmld_row.o xmld_sockets.o \
	  xmld_work.o ptasker.o engine_xmld.o lex.yy.o xmldsql.o xmldsql.tab.o
BISON = bison
FLEX = flex
CFLAGS = -Wall -DUSE_ENGINE_XMLD -DUSE_PTASKER -DUSE_XMLDSQL
CC = gcc
VPATH = ptasker:engine_xmld:xmldsql

all: generate ${OBJECTS}
	${CC} $(wordlist 2, $(words $^), $^) -lfl -lm -o xmld

%.o: $(wildcard *.h)

lex.yy.o: lex.yy.c $(wildcard *.h)
	${CC} ${CFLAGS} -c -o lex.yy.o lex.yy.c
	
xmldsql.tab.o: xmldsql.tab.c $(wildcard *.h)
	${CC} ${CFLAGS} -c -o xmldsql.tab.o xmldsql.tab.c
	
generate: xmld_sql.y xmldsql.lex $(wildcard *.h)
	${BISON} -d xmldsql/xmld_sql.y -o xmldsql.tab.c
	${FLEX} -i -f xmldsql/xmldsql.lex
	
clean:
	rm -f xmld
	rm -f *.o
	rm -f ptasker/*.o
	rm -f engine_xmld/*.o
	rm -f xmldsql/*.o
	rm -f xmldsql.tab.h xmldsql.tab.c
	rm -f lex.yy.c
	rm -f core
