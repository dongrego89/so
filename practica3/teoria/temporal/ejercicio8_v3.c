/*****************************
Version conhilo escuchador que
muestra estadisticas al pulsar 
ENTER. Termina con cualquier
otro caracter.
******************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int global;

int vecesFalla=0,j;
int fin = 0;

void * hilo (void * nada);
void * listen(void * nada);

int main (int argc, char ** argv){
	global = 0;
	int i;
	
	pthread_t listener;
	pthread_t hilos[2];
	
	pthread_create(&listener, NULL, listen, NULL);
	for(j=0 ; fin == 0; ++j){
		for(i=0 ; i < 2 ; ++i){
			pthread_create(hilos+i, NULL, hilo, NULL);
		}
	
		for(i=0 ; i < 2 ; ++i){
			pthread_join(hilos[i],NULL);
		}
		
		if(global != 100) ++vecesFalla;
		global = 0;
	}
	pthread_join(listener,NULL);
	
	return 0;
}

void * hilo (void * nada){
	int i;
	for(i=0 ; i < 50 ; ++i){
		global++;
	}
}

void * listen(void * nada){
	float fracaso;
	char c;
	int veces,k;
	while(fin == 0){
		c = getchar();
		veces = vecesFalla;
		k = j;
		fracaso = ((float) veces) / ((float) k) * 100.0;
		printf("Se han producido errores de concurrencia un %f%% de las veces\n(muestra: %d ejecuciones, %d errores)\n",fracaso,k, veces);
		if(c != '\n') fin = 1;
	}
}
