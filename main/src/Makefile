OBJECTS = authman.o cfg.o cfg_parser.o dutils.o engine_list.o fmanager.o func_list.o mfigure.o \
	  passfd.o protoimpl.o qp.o resptrans.o somanager.o sutils.o twalker.o xmld_aggr_table.o \
	  xml.o xmld_col.o xmld_cond.o xmld_connection.o xmld_directive.o xmld_engine.o xmld_expr.o \
	  xmld_file.o xmld_func.o xmld_list.o xmld_request.o xmld_response.o xmld_row.o xmld_sockets.o \
	  xmld_work.o ptasker.o engine_xmld.o element_op.o format_manip.o \
BISON = bison
FLEX = flex
CFLAGS = -Wall -DUSE_ENGINE_XMLD -DUSE_PTASKER
CC = /usr/local/bin/gcc
VPATH = ptasker:engine_xmld

all: xmld-sql.tab.o lex.yy.o ${OBJECTS}
	${CC} $^ -lfl -lm -o xmld

xmld-sql.tab.o: xmld-sql.y $(wildcard *.h)
	bison -d xmld-sql.y
	${CC} ${CFLAGS} -c xmld-sql.tab.c

lex.yy.o: xmld-sql.lex $(wildcard *.h) xmld-sql.tab.h
	flex -i -f xmld-sql.lex
	${CC} ${CFLAGS} -c lex.yy.c
	
%.o: $(wildcard *.h)

clean:
	rm -f *.o
	rm -f ptasker/*.o
	rm -f engine_xmld/*.o
	rm -f xmld-sql.tab.h xmld-sql.tab.c lex.yy.c
