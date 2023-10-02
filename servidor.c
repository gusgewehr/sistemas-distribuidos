#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include <pthread.h>


#define SERV_TCP_PORT  5555

#define MAXLINE 512

char line[MAXLINE];
//a variável gimmic serve só para armazenar esse texto
char *IP, *gimmic = " diz: ";

//cria um struct para salvar o ip e o socket do cliente juntos
struct clientStruct{
    int client;
    char *ip;
};

char *originalPoster = "";

// cria uma variável para as threads
pthread_t trd_1;

char sendline[MAXLINE];

// cria um array pra manter os clientes
struct clientStruct clients[100];
//cria uma variável para saber a quantidade de clientes conectados
int cur_pos = 0;

// função que é usado para criar uma thread para cada cliente que se conectar para ler o que ele está enviando
void *readClientSocket(void *newsockfd){
    // pega o valor int do ponteiro
    int sockfd = *((int *)newsockfd);
    // laço infinito pra ficar lendo o que o cliente mandar
    while (1)
    {   
        // le o socket do cliente
        int n = read(sockfd, line, MAXLINE);
        if (n == 0) exit(3);                   /* connection terminated */
        else if (n < 0)
        printf("str_echo: read err\n");
        
        // descobre qual cliente q mandou
        for(int i = 0; i < cur_pos; i++){
            if(sockfd == clients[i].client){
                originalPoster = clients[i].ip;
            }
        }
        
        // laço para enviar a mensagem recebida para os demais clientes conectados
        for(int i = 0; i < cur_pos; i++){
            if(sockfd != clients[i].client){
                //envia para o client o ip originario da mensagem
                write(clients[i].client, originalPoster, 9);
                // escreve a variavel gimmic
                write(clients[i].client, gimmic, 6);
                // envia para o client a mensagem recebida
                write(clients[i].client, line, strlen(line));
            }
        }
        // esvazia a variável lida
        bzero(line, sizeof(line));
    }
    

}

int main(int argc, char *argv[]) {
   int                 sockfd, clilen, n;
   int newsockfd;
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

    // looping infinito para ficar escutando se algum cliente se conectar
    while(1) { 
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if( newsockfd < 0)
        {
        /*err_dump("server: accept error");*/
        printf("server: accept error\n");
        exit(3);
        }
        
        IP = inet_ntoa(cli_addr.sin_addr);
        printf("Cliente conectado --> IP: %s\n",IP);


        //Adiciona as informações do client no array do struct clientStruct
        clients[cur_pos].client = newsockfd;
        clients[cur_pos].ip = IP;

        // incrementa 1 na quantidade de clientes conectados
        cur_pos++;

        int result;
        
        //cria uma thread com a função de ler o socket do cliente referente aquela iteração do laço
	    result = pthread_create(&trd_1, NULL, readClientSocket, &newsockfd);


        
    }

//    Return(0);
}
