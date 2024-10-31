all: test

test: test.c fadd.c fsub.c fmul.c fdiv.c
	gcc -o test test.c fadd.c fsub.c fmul.c fdiv.c -lm

clean:
	rm -f test