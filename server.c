#if defined (WIN32)
#include <winsock2.h>
typedef int socklen_t;
#elif defined (linux)
#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#endif
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system.h"
#include "conf.c"
void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	
	#if defined (WIN32)
		WSADATA WSAData;
		int erreur = WSAStartup(MAKEWORD(2, 2), &WSAData);
	#else
		int erreur = 0;
	#endif
	confData *dataStruct=NULL;
	confData * headData=NULL;
	if (!erreur)
    	{
		int i;
		int sockfd, newsockfd, portno;
		socklen_t clilen;
		char buffer[1024];
		struct sockaddr_in serv_addr, cli_addr;
		int n;
	
		checkConf();

		dataStruct=main_data_struct;
		while(dataStruct->id!=PORT_NUMBER){
			dataStruct=dataStruct->next;
		}
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) 
			error("ERROR opening socket");
		bzero((char *) &serv_addr, sizeof(serv_addr));
		portno = atoi(dataStruct->value);
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(portno);

		if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
			error("ERROR on binding");
		else
			printf("connecting to port %d\n",portno);

		while(1){  
			printf("Waiting new request:\n");
			listen(sockfd,5);
			clilen = sizeof(cli_addr);
			newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
			if (newsockfd < 0) 
				error("ERROR on accept");
			bzero(buffer,256);
			n = read(newsockfd,buffer,255);
			if (n < 0) error("ERROR reading from socket");
				printf("Here is the message:\n%s\n",buffer);//just for know what the client send

			sprintf(buffer,"HTTP/1.1 200 OK\r\n");
			send(newsockfd,buffer,strlen(buffer),0);
			sprintf(buffer,"Content-Length: 15\r\n");
			send(newsockfd,buffer,strlen(buffer),0);
			sprintf(buffer,"Content-Type: text/html\r\n");
			send(newsockfd,buffer,strlen(buffer),0);
			sprintf(buffer,"\r\n");
			send(newsockfd,buffer,strlen(buffer),0);
			sprintf(buffer,"Hello MZ World.\r\n");
			send(newsockfd,buffer,strlen(buffer),0);
			close(newsockfd);
		}

		close(sockfd);
		#if defined (WIN32)
		        WSACleanup();
		#endif
 
	}
    	else
        	error("ERROR on initialisation");
	return 0; 
}
