#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

//crio duas threads
pthread_t trd_1, trd_2;

float shared = 0;

//dispara 2 threads

//conta 1 vez
void *thread_0() {
	long int i;
	for(i=0; i<1000000; i++)
		shared = shared + 1;
}

//conta dois milhoes de vezes

void *thread_1() {
	long int i;
	for (i=0; i<2000000; i++)
		shared = shared + 1;
}

//cria e dispara as duas
int main() {
	int result;
	result = pthread_create(&trd_1, NULL, thread_0, NULL);
	result = pthread_create(&trd_2, NULL, thread_1, NULL);
	pthread_join(trd_1, (void **) &result);
	pthread_join(trd_2, (void **) &result);
    //pega o resultado
	printf("Shared = %f\n", shared);
}




