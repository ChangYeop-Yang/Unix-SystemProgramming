/*
- file: bounce_async.c
- author: 2015118527_양창엽
- datetime: 2017-11-18 20:12
- description: Purpose animation with user control, using O_ASYNC on fd.
*/

#include <unistd.h>
#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

#define MESSAGE "hello"
#define BLANK 	"	"

int row = 10, col = 0, dir = 1, delay = 200, done = 0;

int set_ticker(int);

int main(void)
{
	void 	on_alarm(int);
	void	on_input(int);
	void	enable_kbd_signals();

	initscr();
	crmode();
	noecho();
	clear();

	signal(SIGIO, on_input);
	enable_kbd_signals();
	signal(SIGALRM, on_alarm);
	set_ticker(delay);
	
	move(row, col);
	addstr(MESSAGE);
	
	while( !done ) { pause(); }

	endwin();

	return 0;
}

void on_input(int signum)
{
	int c = getch();
	if (c == 'Q' || c == EOF) done = 1;
	else if (c == ' ') dir = -dir;
}

void on_alarm(int signum)
{
	signal(SIGALRM, on_alarm);
	mvaddstr(row, col, BLANK);
	col += dir;
	mvaddstr(row, col, MESSAGE);
	refresh();

	if (dir == EOF && col <= 0) dir = 1;
	else if (dir == 1 && col + strlen(MESSAGE) >= COLS) dir = EOF;
}

void enable_kbd_signals()
{
	int fd_flags;
	
	fcntl(0, F_SETOWN, getpid());
	fd_flags = fcntl(0, F_GETFL);
	fcntl(0, F_SETFL, (fd_flags|O_ASYNC));
}
