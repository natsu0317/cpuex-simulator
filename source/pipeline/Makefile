CC = gcc
CFLAGS = -I../asm_to_binary -I../binary_to_result

all: pipeline

pipeline: pipeline.o asm_parser.o binary_parser.o
	$(CC) -o pipeline pipeline.o asm_parser.o binary_parser.o

pipeline.o: pipeline.c
	$(CC) $(CFLAGS) -c pipeline.c

asm_parser.o: ../asm_to_binary/parser.c
	$(CC) $(CFLAGS) -c ../asm_to_binary/parser.c -o asm_parser.o

binary_parser.o: ../binary_to_result/parser.c
	$(CC) $(CFLAGS) -c ../binary_to_result/parser.c -o binary_parser.o

clean:
	rm -f pipeline *.o