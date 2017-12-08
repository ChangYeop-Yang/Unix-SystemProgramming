
/*
 *file:server.c
 *author:양창엽 양현아 우성연 왕신자
 *datetime:2017_12_08 10:47
 *description : select_position, select_nickname
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

/* MACRO */
#define MALLOC(X, Y) (X*)malloc(sizeof(X) * Y)

/* FLAG ERROR Define */
#define FLAG_IP_ERROR 10
#define FLAG_BIND_ERROR 12
#define FLAG_LISTEN_ERROR 13
#define FLAG_SOCKET_ERROR 14
#define FLAG_CONNECT_ERROR 15

/* FLAG DATA Define */
#define FLAG_TRUE 1
#define FLAG_FALSE 0
#define FLAG_USER_NUM 6

/* MEMORY Define */
#define BUF_MSG 1024
#define BUF_REG 255

/* Struct */
typedef struct users
{
    /* Short */
    short sLoginState;
    
    /* Integer */
    int nVenID[FLAG_USER_NUM];
    int nVersion;
    int nFileDiscript;
    
    /* String */
    char sID[BUF_REG];
    char sPassword[BUF_REG];
} USER;

int anUser_Name_Flag[FLAG_USER_NUM];
USER asUser_Current[FLAG_USER_NUM];

int select_Position();
void handle_Error(const short sError);
void send_Msg(const int nSocket, const ssize_t nLength, const char * sMsg);
char * Select_Nickname(const int nSock);
void join_member(const char * sBuf, const int nSock, const int msgSz);
void login_member(const char * sBuf, const int nSock, const int msgSz);

/* Typedef */
typedef struct sockaddr_in SOCK_IN;

int main(int argc, char * argv[])
{
	/* Pthread */
    pthread_t read_id;
    pthread_mutex_init(&pMutx, NULL);

	if (argc != 2) { handle_Error(FLAG_IP_ERROR); }
    
    /* Integer */
    int nClnt_Sock = 0;
    int nServ_Sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    
    /* struct sockaddr_in */
    SOCK_IN sServ_Adr, sClnt_Adr;
    
    /* sockaddr_in Initialization */
    memset(&sServ_Adr, 0, sizeof(sServ_Adr));
    sServ_Adr.sin_family = AF_INET;
    sServ_Adr.sin_addr.s_addr = htonl(INADDR_ANY);
    sServ_Adr.sin_port = htons(atoi(argv[1]));
    
    /* Socket Bind & Listen */
    if (bind(nServ_Sock, (struct sockaddr *) &sServ_Adr, sizeof(sServ_Adr)) == EOF) { handle_Error(FLAG_BIND_ERROR); }
    if (listen(nServ_Sock, 10) == EOF) { handle_Error(FLAG_LISTEN_ERROR); }
    memset(asUser_Current, 0, sizeof(USER) * 6);
    while (FLAG_TRUE)
    {
        socklen_t unClnt_Adr_Sz = sizeof(sClnt_Adr);
        if ((nClnt_Sock = accept(nServ_Sock, (struct sockaddr *)&sClnt_Adr, &unClnt_Adr_Sz)) == EOF) { break; }

    }
    
    close(nServ_Sock);

	return 0;
}

int select_Position()
{
	/* Integer */
	int nPosition = 0;
	
	for(nPosition = 0; anUser_Name_Flag[nPosition]; nPosition ++);
	anUser_Name_Flag[nPosition] = FLAG_TRUE;

	return nPosition;
}

void send_Msg(const int nSocket, const ssize_t nLength, const char * sMsg)
{
    write(nSocket, sMsg, nLength);
}

void handle_Error(const short sError)
{
    switch (sError)
    {
        case (FLAG_IP_ERROR): { printf("Using Current IP Address Error!!!\n"); break; }
        case (FLAG_SOCKET_ERROR): { printf("Create Socket Error!!!\n"); break; }
        case (FLAG_CONNECT_ERROR): { printf("Connect Server Error!!!\n"); break; }
        case (FLAG_BIND_ERROR): { printf("Bind Socket Error!!!\n"); break; }
        case (FLAG_LISTEN_ERROR): { printf("Listen Socket Error!!!\n"); break; }
        default: { printf("Network UnKnown Error!!!\n"); break; }
    }
}

char * Select_Nickname(const int nSock)
{
	/* string */
	char * sNickname=MALLOC(char, BUFSIZ);

	/* Integer */
	int ii=0;

	/* select User Nickname */
	for(ii=0; ii<FLAG_USER_NUM; ii++)
	{
		/* select User Nickname Success */
		if(nSock == asUser_Current[ii].nFileDiscript)
		{
			sprintf(sNickname,"%s", asUser_Current[ii].sID);
		}
	}

	return sNickName;
}

void join_member(const char * sBuf, const int nSock, const int msgSz)
{
	/* String */
	char sID_temp[BUF_REG];
	char sPasswd_temp[BUF_REG];
	char *sMessage;
	
	/* Struct User */
	USER * sCheck_temp = MALLOC(USER, 1);
	
	/* Integer */
	size_t nPosition = 10;
	size_t nChecking = FLAG_FALSE;
	int ii;
	//ID
	for(ii = 0;sBuf[nPosition] != ' ';sID_temp[ii++]=sBuf[nPosition++]);
	//Password
	for(ii = 0;sBuf[nPosition] != '\n';sPasswd_temp[ii++]=sBuf[nPosition++]);
	
	pthread_mutex_lock(&pMutx);
	
	/* Overlap Id Check */
	FILE * fRead = fopen("member_list", "rb");
	if(fRead != NULL)
	{
		/* Read struct */
		while(fread(sCheck_temp, sizeof(USER), 1, fRead) >0)
		{
			if(strcmp(sCheck_temp->sID, sID_temp) == 0)
			{
				printf("Incorrect registration\n"):
				sMessage="Incorrect registration\n";
				nChecking = FLAG_TRUE;
				send_Msg(nSock, sizeof("Incorrect registration\n"), sMessage);
				fclose(fRead);
				pthread_mutex_unlock(&pMutx);
				return;
			}
		}
		/* Close File */
		fclose(fRead);
	}
	if(nChecking == FLAG_FALSE)
	{
		/* Not overlap ID */
		FILE * fWrite = fopen("member_list","ab");
		if(fWrite != NULL)
		{
			/* Write struct */
			fwrite(insert_UseData());
				
			/* Register Print */
			printf("%s is registered\n",sID_temp);
					
			/* Close file */	
			fclose(fWrite);
		}
	}
	pthread_mutex_unlock(&pMutx);
	sMessage = "Registration success\n";
	send_Msg(nSock, sizeof("Registration success\n"), sMessage);
	return;
}

void login_member(const char * sBuf, const int nSock, const int msgSz)
{
	/* String */
	char sID_temp[BUF_REG];
	char sPasswd_temp[BUF_REG];
	char *sMessage;
	
	/* Integer */
	size_t nPosition = 7;
	int ii;
	//ID
	for(ii = 0; sBuf[nPosition] != ' '; sID_temp[ii++] = sBuf[nPosition++]);
	//Password
	for(ii = 0; sBuf[nPosition] != '\n'; sPasswd_temp[ii++] = sBuf[nPosition++]);
	
	pthread_mutex_lock(&pMutx);
	
	/* FILE */
	FILE * fRead = fopen("member_list", "rb");
	if(fRead != NULL)
	{
		/* Struct Users */
		USER * sUser_Temp = MALLOC(USER, 1);
		while(fread(sUser_Temp, sizeof(USER),1,fRead) >0)
		{
			/* Login Success */		
			if(strcmp(sUser_Temp->sID, sID_temp) == 0 && strcmp(sUser_Temp->sPassword, sPassword_Temp)==0)
			{
				/* Select FileDescription */
				for(size_t ii=0; ii<nCurrent_User_Num; ii++)
				{			
					if(nSock == asUser_nCurrent[ii].nFileDiscript)					
					{																											{
						//ID
						strcpy(asUser_Current[ii].sID, sID_temp);
						//Password
						strcpy(asUser_Current[ii].sPassword, sPasswd_temp);
						asUser_Current[ii].sLoginState = FLAG_TRUE;
						break;
					}
				}			
				sMessage = "Login successful\n";
				send_Msg(nSock, sizeof("Login successful\n"), sMessage);
				FILE * fWrite = fopen("member_list","ab");
			
				/* Write Struct for puts Socket Discripter */
				fwrite(insert_UserData(nSock, 0, sID_temp, sPasswd_temp), sizeof(USER), 1, fWrite);
				
				/* Close File */
				fclose(fWrite);
				fclose(fRead);
				pthread_mutex_unlock(&pMutx);
				return;					
			}
		}
	
		/* File Close */
		fclose(fRead);
	}
	
	pthread_mutex_unlock(&pMutx);
	sMessage = "Login fail\n";
	send_Msg(nSock, sizeof("Login fail\n"), sMessage);
	return;
}
	
