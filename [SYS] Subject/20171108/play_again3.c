/*
- file: play_again3.c
- author: 2015118527_양창엽
- datetime: 2017-11-01 18:14
- description: Set tty into chr-by-chr, no-echo mode, set tty into no-delay mode.
*/

#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>

#define ASK		"Do you want another transaction"
#define TRIES		3
#define SLEEPTIME	2
#define	BEEP		putchar('\a')

int get_response(char *, int);
void set_cr_noecho_mode(void);
int get_ok_char(void);
void set_nodelay_mode(void);
void tty_mode(int);

int main(void)
{
	int	response = 0;
	
	tty_mode(0);
	set_cr_noecho_mode();
	set_nodelay_mode();
	response = get_response(ASK, TRIES); /* Get Some Answer */
	
	tty_mode(1);
	return response;
}

int get_response(char * question, int maxtries)
{
	int input;

	printf("%s (y/n)?", question);
	fflush(stdout); 	/* Clear Buffer */

	while(1) {
		sleep(SLEEPTIME);
		input = tolower(get_ok_char());
		if( input == 'y') return 0;
		if( input == 'n') return 1;
		if( maxtries-- == 0) return 2; /* Time Limit */
		BEEP;
	}
}

int get_ok_char()
{
	int c;
	while( (c = getchar()) != EOF && strchr("yYnN", c) == NULL);
	return c;
}

void set_cr_noecho_mode(void)
{
	struct termios ttystate;
	
	tcgetattr(0, &ttystate);
	ttystate.c_lflag	&= ~ICANON;	/* No Buffering */
	ttystate.c_lflag	&= ~ECHO;	/* No ECHO	*/
	ttystate.c_cc[VMIN]	= 1;
	tcsetattr(0, TCSANOW, &ttystate);
}

void tty_mode(int how)
{
	static struct termios original_mode;
	static int	original_flags;

	if (how == 0) { tcgetattr(0, &original_mode); original_flags = fcntl(0, F_GETFL);  } /* Save Current Mode  */
	else { tcsetattr(0, TCSANOW, &original_mode); fcntl(0, F_SETFL, original_flags);  } /* Resore Mode */
}

void set_nodelay_mode()
{
	int	termflags;
	termflags = fcntl(0, F_GETFL);
	termflags |= O_NDELAY;
	fcntl(0, F_SETFL, termflags);
}
