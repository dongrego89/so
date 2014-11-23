/*****************************
Version que indica estadisticas
******************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int global;

void * hilo (void * nada);

int main (int argc, char ** argv){
	global = 0;
	int i,j;
	int nveces;
	int vecesFalla=0;
	float fracaso;
	
	pthread_t hilos[2];
	
	if(argc != 2){
		printf("Error de formato. La forma correcta de invocar al programa es:\n%s <no. ejecuciones>\n",argv[0]);
		exit(-1);
	}
	
	nveces = atoi(argv[1]);
	
	for(j=0 ; j < nveces ; ++j){
		for(i=0 ; i < 2 ; ++i){
			pthread_create(hilos+i, NULL, hilo, NULL);
		}
	
		for(i=0 ; i < 2 ; ++i){
			pthread_join(hilos[i],NULL);
		}
		
		if(global != 100) ++vecesFalla;
		global = 0;
	}
	
	fracaso = ((float) vecesFalla) / ((float) nveces) * 100.0;
	printf("Se han producido errores de concurrencia un %f%% de las veces\n(muestra: %d ejecuciones, %d errores)\n",fracaso,nveces, vecesFalla);
	
	return 0;
}

void * hilo (void * nada){
	int i;
	for(i=0 ; i < 50 ; ++i){
		global++;
	}
}
