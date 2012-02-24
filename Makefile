CC=gcc
TARGET=chess
SRC=main.c chess.c io.c draggen.c tree.c
OBJ=main.o chess.o io.o draggen.o tree.o
CFLAGS=-g -Wall# -DDEBUG
#LIBS=-lefence

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)

.SUFFIXES:.c.o
.c.o:
	$(CC) -c $(CFLAGS) $<
	
main.o:main.c chess.h
chess.o:chess.c chess.h ch_stddef.h draggen.c draggen.h
draggen.o:draggen.c draggen.h tree.c tree.h io.h
io.o:io.c io.h
tree.o:tree.c tree.h tree_item.h

rebuild:clean $(TARGET)

clean:
	-rm *.o $(TARGET)
