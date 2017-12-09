/*
- file: play_again2.c
- author: 2015118527_양창엽
- datetime: 2017-11-01 18:04
- description: Set tty into Char-By-Char mode and no-Echo mode read char, return result.
*/

#include <stdio.h>
#include <termios.h>

#define QUESTION	"Do you want another transaction"

int get_response(char *);
void set_cr_noecho_mode(void);
void tty_mode(int);

int main(void)
{
	int	response = 0;
	
	tty_mode(0);
	set_cr_noecho_mode();
	response = get_response(QUESTION); /* Get Some Answer */
	
	tty_mode(1);
	return response;
}

int get_response(char * question)
{
	printf("%s (y/n)?", question);
	
	while(1) {
		switch(getchar()) {
			case 'y' : case 'Y' : return 0;
			case 'n' : case 'N' : return 1;
			case EOF : return 1;
		}
	}
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
	
	if (how == 0) { tcgetattr(0, &original_mode); } /* Save Current Mode  */
	else { tcsetattr(0, TCSANOW, &original_mode);  } /* Resore Mode */
}
