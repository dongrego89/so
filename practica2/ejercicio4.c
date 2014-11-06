#include<stdio.h>
#include<string.h>
#include<stdlib.h>//Memoria dinamica
#include<pthread.h>

/*
Programa que sacará el audio de cada fichero pasado por línea de argumentos paralelamente mediante hilos
*/


void * conversionMP3(void * archivo){
	char * aux;
	aux=(char *)archivo;
	char ruta[200]="ffmpeg -i ";
	strcat(ruta,aux);
	strcat(ruta," -f mp3 -ab 192000 -ar 48000 -vn ");
	aux=strtok(aux,".");
	strcat(ruta,aux);
	strcat(ruta,".mp3");
	fflush(stdout);
	printf("\n...Convirtiendo fichero '%s'",ruta);
	system(ruta);
	
}

int main(int argc,char * argv[]){

int i,nHilos=argc-1;
char ** archivos;
pthread_t * hilos;

hilos=malloc(sizeof(pthread_t)*nHilos);


archivos=malloc(sizeof(char)*nHilos);

for(i=1;i<argc;i++){
	archivos[i-1]=argv[i];
	//printf("\n - %s",archivos[i-1]);
}

for(i=0;i<nHilos;i++){
	pthread_create(&hilos[i],NULL,(void *)conversionMP3,(void *)archivos[i]);
}

for(i=0;i<nHilos;i++){
	pthread_join(hilos[i],NULL);
}

printf("\n");

return 0;
}
