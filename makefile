GCC=gcc
GCCFLAGS=-g

.SUFFIXES: .out
.c.o:
	$(GCC) $(GCCFLAGS) $< -c

a.out :main.o charlist.o eval.o read.o print.o variable.o symbol.o ifunc.o error.o
	$(GCC) $^

main.o:main.c
strchunk.o:charlist.c
eval.o:eval.c
read.o:read.c
print.o:print.c
variable.o:variable.c
symbol.o:symbol.c
ifunc.o:ifunc.c
error.o:error.c


clean:
	rm -f *.o *.out
