CC = gcc
CFLAGS = -Wall
EXE = allocate
OBJ = allocate.o process.o scheduler.o queue.o
SRC = allocate.c process.c scheduler.c queue.c

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) -lm

clean:
	rm -f $(OBJ) $(EXE)

usage: $(EXE)
	./$(EXE)

