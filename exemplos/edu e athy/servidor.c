#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h> // Para threads

#define SERV_TCP_PORT  5555
#define MAXLINE 512

char line[MAXLINE];
char *IP;
int clients[FD_SETSIZE]; // Array para manter as conexões dos clientes

void *client_handler(void *arg) {
    int newsockfd = *((int *)arg);
    int n;

    while (1) {
        n = read(newsockfd, line, MAXLINE);
        if (n <= 0) {
            printf("Cliente desconectado\n");
            close(newsockfd);
            return NULL;
        }
        line[n] = '\0';

        printf("Cliente %s enviou: %s\n", IP, line);

        // Envie a mensagem para todos os clientes conectados
        for (int i = 0; i < FD_SETSIZE; i++) {
            if (clients[i] != -1 && clients[i] != newsockfd) {
                write(clients[i], line, strlen(line));
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, clilen, n;
    struct sockaddr_in cli_addr, serv_addr;
    struct hostent *hp;
    pthread_t thread_id;

    // Inicialize o array de clientes
    for (int i = 0; i < FD_SETSIZE; i++) {
        clients[i] = -1;
    }

    /* open a TCP socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("server: can not open stream socket\n");
        exit(1);
    }

    /* bind our local address so that the client can send to us */
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_TCP_PORT);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("server: can not bind local addr\n");
        exit(2);
    }

    listen(sockfd, 5);
    printf("Servidor aguardando conexao\n");

    clilen = sizeof(cli_addr);

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

        printf("newsockfd: %d\n", newsockfd);

        if (newsockfd < 0) {
            printf("server: accept error\n");
            continue;
        }

        IP = inet_ntoa(cli_addr.sin_addr);
        printf("Cliente conectado --> IP: %s\n", IP);

        // Encontre um espaço vazio no array de clientes
        for (int i = 0; i < FD_SETSIZE; i++) {
            if (clients[i] == -1) {
                clients[i] = newsockfd;
                break;
            }
        }

        // Crie uma thread para lidar com este cliente
        pthread_create(&thread_id, NULL, client_handler, &newsockfd);
    }

    return 0;
}
