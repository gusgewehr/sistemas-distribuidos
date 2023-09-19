
/*****************************************
   cli1.c (cliente)
*****************************************/


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


#define SERV_TCP_PORT  5555
#define MAXLINE 512

char line[MAXLINE];

char sendline[MAXLINE];

pthread_t trd_1;


//Função para depois criar uma thread q fica lendo o socket do servidor
void *readClientSocket(void *newsockfd){
  //transforma para ponteiro de int novamente
    int *teste = (int *)newsockfd;
  
  //loop eterno
    while (1)
    { 
      //lê o socket e retorna uma string
        int n = read(*teste, line, MAXLINE);
      //se tiver recebido algo
        if( n > 0){
          //imprime na tela a mensagem
          printf("%s\n",line);
          //limpa a variavel com a mensagem
          bzero(line, sizeof(line));
        }
        
    }
    

}

//Função para receber um input do usuário e enviar para o socket do servidor
void digitaAlgo(int sockfd) {
   int  n;

   //Espera o input do usuário, quando ele apertar enter entra no loop
   printf("Você diz: ");
   while (fgets(sendline, MAXLINE, stdin) != NULL) {   
     n = strlen(sendline);
     //puts(sendline);
     
     if(write(sockfd, sendline, n) != n) {
       printf("str_cli: writen error or socket\n");
       exit(3);
     }
   }
}

int main(argc, argv)
    int argc; char *argv[];
{
  int                 sockfd;
  struct sockaddr_in  serv_addr;
  struct hostent      *hp;
  char                host[10];
  char SERVER_NAME[50];

  if (argc < 2) {
     printf("Digite o nome do servidor!");
     exit(1);
  }
  strcpy(SERVER_NAME, argv[1]);

  printf("Trying to connect to server %s ...\n", SERVER_NAME);
  hp= gethostbyname(SERVER_NAME);
  bzero((char *) &serv_addr, sizeof(serv_addr));
  bcopy(hp->h_addr, (char *) &serv_addr.sin_addr, hp->h_length);

  serv_addr.sin_family         =AF_INET;
 /* serv_addr.sin_addr.s_addr  =inet_addr(SERV_HOST_ADDR); */
  serv_addr.sin_port           =htons(SERV_TCP_PORT);

  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
      printf("client: can not open stream socket\n");
      exit(1);
   }

  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
      printf("client: can not connect to server\n");
      exit(2);
    }
  printf("\nOK!\n");

  pthread_create(&trd_1, NULL, readClientSocket, (void *)&sockfd);

  digitaAlgo(sockfd);
  close(sockfd);
}