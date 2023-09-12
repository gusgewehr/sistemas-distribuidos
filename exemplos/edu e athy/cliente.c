#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h> 

#define SERV_TCP_PORT 5555
#define MAXLINE 512

char sendline[MAXLINE];
char line[MAXLINE];

void *thread_ouvir(void *sockfd)
{
    printf("thread ouvindo\n");
    int *sock = (int *)sockfd;
    while (1) {
        int n = read(*sock, line, MAXLINE);
        if (n > 0) {
            printf("Recebi do server=%s\n", line);
            bzero(line, sizeof(line));
        }
    }
} 

digitaAlgo(int sockfd)
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, thread_ouvir, (void *)&sockfd);

    int n;
    printf("Digite algo...\n");
    while (fgets(sendline, MAXLINE, stdin) != NULL)
    {
        n = strlen(sendline);
        puts(sendline);
        printf("\nEnviando para servidor ...\n");
        if (write(sockfd, sendline, n) != n)
        {
            printf("str_cli: writen error or socket\n");
            exit(3);
        }
    }
}

int main(argc, argv)
int argc;
char *argv[];
{
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *hp;
    char host[10];
    char SERVER_NAME[50];

    if (argc < 2)
    {
        printf("Digite o nome do servidor!");
        exit(1);
    }
    strcpy(SERVER_NAME, argv[1]);

    printf("Trying to connect to server %s ...\n", SERVER_NAME);
    hp = gethostbyname(SERVER_NAME);
    bzero((char *)&serv_addr, sizeof(serv_addr));
    bcopy(hp->h_addr, (char *)&serv_addr.sin_addr, hp->h_length);

    serv_addr.sin_family = AF_INET;
    /* serv_addr.sin_addr.s_addr  =inet_addr(SERV_HOST_ADDR); */
    serv_addr.sin_port = htons(SERV_TCP_PORT);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("client: can not open stream socket\n");
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("client: can not connect to server\n");
        exit(2);
    }
    printf("\nOK!\n");

    digitaAlgo(sockfd);
    close(sockfd);
}
