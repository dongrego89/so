#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>//usar booleanos
#include<pthread.h>
#include<time.h>

#define true 1
#define false 0

int recurso=100;
int turno=0;

typedef struct{
	int id;
	bool quieroEntrar;
}datos;


void * sumaleDos(void * datosHilo){
	datos * d;
	d=(datos *)datosHilo;
	//sleep(1);

	//Inicio de seccion critica

	recurso+=2;
	printf("\nRecurso pasa de valer %d a %d .. %c",recurso-2,recurso,rand()%10+115);


	//Fin de seccion critica
}

int main(int argc, char * argv[]){
srand(time(NULL));

pthread_t * hilos;
datos * datosHilo;
 
int nHilos=atoi(argv[1]);
int i;

hilos=(pthread_t *)malloc(sizeof(pthread_t) * nHilos);
datosHilo=(datos *)malloc(sizeof(datos) * nHilos);
	
for(i=0;i<nHilos;i++){
	datosHilo[i].id=i;
	datosHilo[i].quieroEntrar=false;
	if(i==0){//Para el primer hilo le daremos acceso
	datosHilo[i].quieroEntrar=true;
	}
}


for(i=0;i<nHilos;i++){
	pthread_create(&hilos[i],NULL,(void *)sumaleDos,(void *)&datosHilo[i]);
}

for(i=0;i<nHilos;i++){
	pthread_join(hilos[i],NULL);


}
printf("\n\n\tVariable Global: %d",recurso);
printf("\n\n");

return 0;
}
