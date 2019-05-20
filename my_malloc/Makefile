cc=gcc
objects=testcase.o my_malloc.o
testcase : $(objects)
	$(cc) -o testcase $(objects)
testcase.o:testcase.c
	$(cc) -c testcase.c my_malloc.c
my_malloc.o:my_malloc.c
	$(cc) -c my_malloc.c
.PHONY:clean
clean:
	rm testcase $(objects)

