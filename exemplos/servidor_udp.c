#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#define MY_PORT_ID 6090 /* numero > 5000 */
int main()
{
    int sockid, nread, addrlen;
    struct sockaddr_in my_addr, client_addr;
    char msg[50];

    printf("Servidor: criando o socket\n");
    if ((sockid = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("Servidor: erro no socket: %d\n", errno);
        exit(0);
    }
    printf("Servidor: amarrando meu socket local\n");
    bzero((char *)&my_addr, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htons(INADDR_ANY);
    my_addr.sin_port = htons(MY_PORT_ID);

    if ((bind(sockid, (struct sockaddr *)&my_addr,
              sizeof(my_addr)) < 0))
    {
        printf("Servidor: falha na amarração:%d\n", errno);
        exit(0);
    }
    printf("Servidor: iniciando bloqueio de mensagemlida\n");
    nread = recvfrom(sockid, msg, 15, 0,
                     (struct sockaddr *)&client_addr, &addrlen);
    printf("Servidor: cod retorno lido é%d\n", nread);
    if (nread > 0)
        printf("Servidor: mensagem é:%.15s\n", msg);
    close(sockid);
}