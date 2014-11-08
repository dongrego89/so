#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
//#include<string.h>

typedef struct{
	char mensaje[50];
	int numero;
}estructura;

void modificaEstructura(void * e){
estructura * aux;
aux=(estructura *)malloc(sizeof(estructura));

aux=(estructura *)e;

aux->numero+=1;
aux->mensaje[0]='9';
pthread_exit((void *)aux);
}

int main(int argc, char * argv[]){

system("clear");

if(argc==2){

	int nHilos,i;
	estructura * vector,* resultado;
	pthread_t * hilos;
	void * auxiliar;

	nHilos=atoi(argv[1]);

	vector=(estructura *)malloc(sizeof(estructura)*nHilos);
	resultado=(estructura *)malloc(sizeof(estructura)*nHilos);
	hilos=(pthread_t *)malloc(sizeof(pthread_t)*nHilos);

	for(i=0;i<nHilos;i++){//Recepcion de valores para las estructuras
		
		printf("\nIntroduce un mensaje: ");
		fgets(vector[i].mensaje,50,stdin);
	
		printf("Introduce un valor: ");
		scanf("%d",&vector[i].numero);
		while(getchar()!='\n');//fflush de modo correcto
	}
		
	for(i=0;i<nHilos;i++){
		pthread_create(&hilos[i],NULL,(void *)modificaEstructura,(void *)&vector[i]);
	}

	for(i=0;i<nHilos;i++){
		pthread_join(hilos[i],(void **)&auxiliar);
		resultado[i]=*(estructura *)auxiliar;
	}

	for(i=0;i<nHilos;i++){
		printf("\nEstructura %i:\n\tNumero: %d\n\tCadena: %s",i+1,resultado[i].numero,resultado[i].mensaje);	
	}



}
else{
	printf("\nSe esperaban 2 parametros\n\n");
}

return 0;
}
