CC = gcc
CFLAGS = -I../asm_to_binary -I../binary_to_result -I../cache -I../float/math -O2 -pg -march=native -mtune=native -ffast-math -funroll-loops
LDFLAGS = -lm -pg

all: fast_parser

fast_parser: fast_parser.o asm_parser.o set_associative.o fadd.o fsub.o fmul.o fdiv.o finv.o fsqrt.o fabsf.o fcvtsw.o fcvtws.o feq.o flt.o fneg.o fsgnjn.o fsgnjx.o floor.o
	$(CC) -o fast_parser fast_parser.o asm_parser.o set_associative.o fadd.o fsub.o fmul.o fdiv.o finv.o fsqrt.o fabsf.o fcvtsw.o fcvtws.o feq.o flt.o fneg.o fsgnjn.o fsgnjx.o floor.o $(LDFLAGS)

fast_parser.o: fast_parser.c
	$(CC) $(CFLAGS) -c fast_parser.c

asm_parser.o: ../asm_to_binary/parser.c
	$(CC) $(CFLAGS) -c ../asm_to_binary/parser.c -o asm_parser.o

set_associative.o: ../cache/direct.c
	$(CC) $(CFLAGS) -c ../cache/set_associative.c -o set_associative.o

fadd.o: ../float/math/fadd.c
	$(CC) $(CFLAGS) -c ../float/math/fadd.c

fsub.o: ../float/math/fsub.c
	$(CC) $(CFLAGS) -c ../float/math/fsub.c

fmul.o: ../float/math/fmul.c
	$(CC) $(CFLAGS) -c ../float/math/fmul.c

fdiv.o: ../float/math/fdiv.c
	$(CC) $(CFLAGS) -c ../float/math/fdiv.c

finv.o: ../float/math/finv.c
	$(CC) $(CFLAGS) -c ../float/math/finv.c

fsqrt.o: ../float/math/fsqrt.c
	$(CC) $(CFLAGS) -c ../float/math/fsqrt.c

fabsf.o: ../float/math/fabsf.c
	$(CC) $(CFLAGS) -c ../float/math/fabsf.c

fcvtsw.o: ../float/math/fcvtsw.c
	$(CC) $(CFLAGS) -c ../float/math/fcvtsw.c

fcvtws.o: ../float/math/fcvtws.c
	$(CC) $(CFLAGS) -c ../float/math/fcvtws.c

feq.o: ../float/math/feq.c
	$(CC) $(CFLAGS) -c ../float/math/feq.c

flt.o: ../float/math/flt.c
	$(CC) $(CFLAGS) -c ../float/math/flt.c

fneg.o: ../float/math/fneg.c
	$(CC) $(CFLAGS) -c ../float/math/fneg.c

fsgnjn.o: ../float/math/fsgnjn.c
	$(CC) $(CFLAGS) -c ../float/math/fsgnjn.c

fsgnjx.o: ../float/math/fsgnjx.c
	$(CC) $(CFLAGS) -c ../float/math/fsgnjx.c

floor.o: ../float/math/floor.c
	$(CC) $(CFLAGS) -c ../float/math/floor.c

clean:
	rm -f fast_parser *.o gmon.out

gprof: fast_parser
	./fast_parser
	gprof fast_parser gmon.out > analysis.txt