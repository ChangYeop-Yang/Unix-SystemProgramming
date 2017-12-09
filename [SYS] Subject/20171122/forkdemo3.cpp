#include <iostream>
#include <cstdio>
#include <unistd.h>
using namespace std;

int main(void)
{
	int fork_rv;
	cout << "Before: my pid is" << getpid() << endl;

	fork_rv = fork();

	if(fork_rv == -1)
		perror("fork");
	else if (fork_rv == 0)
		cout << "I am the Child. my pid= " << getpid() << endl;
	else 
		cout << "I am parent. my child is" << fork_rv << endl;
}
