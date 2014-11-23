/*****************************
Version normal, tal como se pide
en la practica
******************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Prototipos
void * hilo (void * nada);
int global;

int main ()
{
	global = 0;
	int i;
	
	pthread_t hilos[2];
	
	for(i=0 ; i < 2 ; ++i)
		pthread_create(hilos+i, NULL, hilo, NULL);

	for(i=0 ; i < 2 ; ++i)
		pthread_join(hilos[i],NULL);
	
	printf("\nVariable global = %d\n",global);
	exit(EXIT_SUCCESS);
}

void * hilo (void * nada)
{
	int i;
	for(i=0 ; i < 50 ; ++i)
		global++;
}
