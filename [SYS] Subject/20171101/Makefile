subject : my_write.o my_stty.o
	gcc -o my_write  my_write.o
	gcc -o my_stty	my_stty.o

my_write.o : my_write.c
	gcc -c my_write.c

my_stty.o : my_stty.c
	gcc -c my_stty.c

all : subject

clean : rm -f *.o subject
