CC = gcc
CFLAGS = -I../asm_to_binary -I../binary_to_result

all: pipeline

pipeline: pipeline.o asm_parser.o binary_parser.o fadd.o fsub.o fmul.o fdiv.o finv.o fsqrt.o
	$(CC) -o pipeline pipeline.o asm_parser.o binary_parser.o fadd.o fsub.o fmul.o fdiv.o finv.o fsqrt.o

pipeline.o: pipeline.c
	$(CC) $(CFLAGS) -c pipeline.c

asm_parser.o: ../asm_to_binary/parser.c
	$(CC) $(CFLAGS) -c ../asm_to_binary/parser.c -o asm_parser.o

binary_parser.o: ../binary_to_result/parser.c
	$(CC) $(CFLAGS) -c ../binary_to_result/parser.c -o binary_parser.o

fadd.o: ../float/math/fadd.c
	$(CC) $(CFLAGS) -c ../float/math/fadd.c -o fadd.o

fsub.o: ../float/math/fsub.c
	$(CC) $(CFLAGS) -c ../float/math/fsub.c -o fsub.o

fmul.o: ../float/math/fmul.c
	$(CC) $(CFLAGS) -c ../float/math/fmul.c -o fmul.o

fdiv.o: ../float/math/fdiv.c
	$(CC) $(CFLAGS) -c ../float/math/fdiv.c -o fdiv.o

finv.o: ../float/math/finv.c
	$(CC) $(CFLAGS) -c ../float/math/finv.c -o finv.o

fsqrt.o: ../float/math/fsqrt.c
	$(CC) $(CFLAGS) -c ../float/math/fsqrt.c -o fsqrt.o

clean:
	rm -f pipeline *.o