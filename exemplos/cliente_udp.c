#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#define MY_PORT_ID 6089
#define SERVER_PORT_ID 6090
#define SERV_HOST_ADDR "10.10.5.20"
int main()
{
    int sockid, retcode;
    struct sockaddr_in my_addr, server_addr;
    char msg[15];

    printf("Cliente: criando socket\n");
    if ((sockid = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("Cliente: falha no socket: %d\n", errno);
        exit(0);
    }
    printf("Cliente: amarrando socket local\n");
    bzero((char *)&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr.sin_port = htons(MY_PORT_ID);
    if ((bind(sockid, (struct sockaddr *)&my_addr,sizeof(my_addr)) < 0))
    {
        printf("Cliente: falha na amarração:%d\n", errno);
        exit(0);
    }

    printf("Cliente: criando estrutura addr parao servidor\n");
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    server_addr.sin_port = htons(SERVER_PORT_ID);
    printf("Cliente: iniciando mensagem eenviando\n");
    sprintf(msg, "Ola para todos");
    retcode = sendto(sockid, msg, sizeof(msg), 0, (struct sockaddr *)&server_addr,sizeof(server_addr));
    if (retcode <= -1)
    {
        printf("cliente: falha no sendto: %d\n", errno);
        exit(0);
    }
    /* close socket */
    close(sockid);
}