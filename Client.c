/*
- file: Client.c
- author: 2015115939_양현아
- datetime: 2017-11-30 20:06
- description: sock() -> connect() -> read()/write() -> close 구성의 클라이언트 구현. 입력받은 메시지를 서버에게 write한다.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

void error_handling(char * msg);
void * send_msg(void * arg);
void * recv_msg(void * arg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;

	pthread_t snd_thread, rcv_thread;
	void * thread_return;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	//socket
	sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	//connet
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");

	pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
	pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);

	//close
	close(sock);

	return 0;
}

// send thread main
void * send_msg(void * arg)   
{
   int sock = *((int*)arg);
   char name_msg[NAME_SIZE + BUF_SIZE];

   while(1)
   {
      fgets(msg, BUF_SIZE, stdin);
      if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
      {
		 close(sock); //close로 sock 닫아서 연결 종료
         exit(0);
      }
      sprintf(name_msg,  msg);
      write(sock, name_msg, strlen(name_msg));
   }

   return NULL;
}

// read thread main
void * recv_msg(void * arg)
{
	int sock = *((int*)arg);
	char name_msg[NAME_SIZE + BUF_SIZE];
	int str_len;

	while (1)
	{
		str_len = read(sock, name_msg, NAME_SIZE + BUF_SIZE - 1);
		if( str_len == -1 )
		{
			return (void*)-1;
		}
		name_msg[str_len] = 0;
		fputs(name_msg, stdout);
   }

   return NULL;
}

//error check
void error_handling(char *msg)
{
   fputs(msg, stderr);
   fputc('\n', stderr);
   exit(1);
}
