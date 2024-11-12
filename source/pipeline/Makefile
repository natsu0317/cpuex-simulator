CC = gcc
CFLAGS = -I../asm_to_binary

all: pipeline

pipeline: pipeline.o parser.o
	$(CC) -o pipeline pipeline.o parser.o

pipeline.o: pipeline.c
	$(CC) $(CFLAGS) -c pipeline.c

parser.o: ../asm_to_binary/parser.c
	$(CC) $(CFLAGS) -c ../asm_to_binary/parser.c -o parser.o

clean:
	rm -f pipeline *.o