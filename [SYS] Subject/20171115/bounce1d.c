/*
- file: bounce1d.c
- author: 2015118527_양창엽
- datetime: 2017-11-18 17:26
- description: Purpose animation with user controlled apped and direction.
*/

#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <string.h>
#include <termios.h>

#define MESSAGE "hello"
#define BLANK	"	"

int row = 0, col = 0, dir = 0;

int set_ticker(int);

int main(void)
{
	int delay;
	int ndelay;
	int c;
	void move_msg(int);

	initscr();
	crmode();
	noecho();
	clear();

	row = 10;
	col = 0;
	dir = 1;
	delay = 200;

	move(row, col);
	addstr(MESSAGE);
	signal(SIGALRM, move_msg);
	set_ticker(delay);

	while(1)
	{
		ndelay = 0;
		c = getchar();
		if ( c == 'Q' ) break;
		if ( c == ' ' ) dir = -dir;
		if ( c == 'f' && delay > 2) ndelay  = delay / 2;
		if ( c == 's' ) ndelay = delay * 2;
		if (ndelay > 0) set_ticker(delay = ndelay);
	}

	endwin();
	return 0;
}

void move_msg(int signum)
{
	signal(SIGALRM, move_msg);
	move(row, col);
	addstr(BLANK);
	col += dir;
	move(row, col);
	addstr(MESSAGE);
	refresh();

	if (dir == EOF && col <= 0) { dir = 1; }
	else if (dir == 1  && col + strlen(MESSAGE) >= COLS) { dir = -1;  }
}
