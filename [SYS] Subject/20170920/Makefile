subject: ls1.o ls2.o ls3.o ls4.o ls5.o
	gcc -o ls1 ls1.o
	gcc -o ls2 ls2.o
	gcc -o ls3 ls3.o
	gcc -o ls4 ls4.o
	gcc -o ls5 ls5.o

ls1.o : ls1.c
	gcc -c ls1.c

ls2.o : ls2.c
	gcc -c ls2.c

ls3.o : ls3.c
	gcc -c ls3.c

ls4.o : ls4.c
	gcc -c ls4.c

ls5.o : ls5.c
	gcc -c ls5.c

all: subject

clean:
	rm -f *.o subject
