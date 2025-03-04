CC = g++
CFLAGS = -I../asm_to_binary -I../binary_to_result -I../cache -I../float/math -O2 -pg -march=native -mtune=native -ffast-math -funroll-loops
LDFLAGS = -lm -pg

all: fast_parser

fast_parser: fast_parser.o asm_parser.o fpu.o
	$(CC) -o fast_parser fast_parser.o asm_parser.o fpu.o $(LDFLAGS)

fast_parser.o: fast_parser.c
	$(CC) $(CFLAGS) -c fast_parser.c

asm_parser.o: ../asm_to_binary/parser.c
	$(CC) $(CFLAGS) -c ../asm_to_binary/parser.c -o asm_parser.o

fpu.o: ../float/math/fpu.cpp ../float/math/fpu.hpp
	$(CC) $(CFLAGS) -c ../float/math/fpu.cpp -o fpu.o

clean:
	rm -f fast_parser *.o gmon.out

gprof: fast_parser
	./fast_parser
	gprof fast_parser gmon.out > analysis.txt