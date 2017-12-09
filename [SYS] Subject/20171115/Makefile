subject : bounce1d.o bounce2d.o bounce_async.o bounce_aio.o
	gcc -o bounce1d bounce1d.o set_ticker.o -lcurses
	gcc -o bounce2d bounce2d.o set_ticker.o -lcurses
	gcc -o bounce_async bounce_async.o set_ticker.o -lcurses
	gcc -o bounce_aio bounce_aio.o set_ticker.o -lrt -lcurses

bounce1d.o : bounce1d.c
	gcc -c bounce1d.c set_ticker.c

bounce2d.o : bounce2d.c
	gcc -c bounce2d.c set_ticker.c bounce.h

bounce_async.o : bounce_async.c
	gcc -c bounce_async.c set_ticker.c

bounce_aio.o : bounce_aio.c
	gcc -c bounce_aio.c set_ticker.c

all: subject

clean: rm -f *.o subject
