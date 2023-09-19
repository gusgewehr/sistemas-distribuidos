#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define SERV_TCP_PORT  5555

#define MAXLINE 512

char line[MAXLINE];
char *IP;


int main(int argc, char *argv[]) {
   int                 sockfd, newsockfd, clilen, n;
   struct sockaddr_in  cli_addr, serv_addr;
   struct hostent      *hp;

  /* open a TCP socket */
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) <0 )
  {
      printf("server: can not open stream socket\n");
      exit(1);
  }

  /* bind our local address so that the client can send to us */
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family      = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port        = htons(SERV_TCP_PORT);

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
  {
     printf("server: can not bind local addr\n");
     exit(2);
  }

  listen(sockfd, 5);
  printf ("Servidor aguardando conexao\n");


    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if( newsockfd < 0)
    {
     /*err_dump("server: accept error");*/
       printf("server: accept error\n");
       exit(3);
    }

    IP = inet_ntoa(cli_addr.sin_addr);
    printf("Cliente conectado --> IP: %s\n",IP);

    while(1) {
        n = read(newsockfd, line, MAXLINE);
        if (n == 0) exit(3);                     /* connection terminated */
        else if (n < 0)
        printf("str_echo: read err\n");
        printf("Recebi=%s\n",line);
        bzero(line, sizeof(line));
    }

//    Return(0);
}
