/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	int i;
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	if (argc < 2) {
	 	fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	else
		printf("connecting to port %s\n",argv[1]);

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
	return 0; 
}