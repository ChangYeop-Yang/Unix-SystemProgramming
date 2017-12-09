/*
- file: play_again0.c
- author: 2015118527_양창엽
- datetime: 2017-11-01 17:39
- description: Ask a Question, Wait for yes/no answer
*/

#include <stdio.h>

#define QUESTION	"Do you want another transaction"

int get_response(char *);

int main(void)
{
	int	response = get_response(QUESTION); /* Get Some Answer */
	
	return response;
}

int get_response(char * question)
{
	printf("%s (y/n)?", question);
	
	while(1) {
		switch(getchar()) {
			case 'y' : case 'Y' : return 0;
			case 'n' : case 'N' : return 1;
		}
	}
}
