/*
Se considera a cada usuario (lector y escritor) como un proceso distinto, y, por ejemplo, al fichero
en cuestión como un recurso. Para que el problema esté bien resuelto se tiene que cumplir:
1) Cualquier número de lectores pueden leer del fichero simultáneamente.
2) Solo un escritor al tiempo puede escribir en el fichero.
3) Si un escritor está escribiendo en el fichero ningún lector puede leerlo.
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>

#define TRUE 1
#define FALSE 0

pthread_mutex_t semaforo1,semaforo2;//Semaforos para lector y escritor

int recurso;//Recurso compartido
int numeroLectores;//Cantidad de lectores simultaneamente

void * escritor(void * limite);
void * lector(void * limite);

int main(){

srand(time(NULL));//Inicialización de semilla

pthread_t * hilos;
hilos=(pthread_t *)malloc(sizeof(hilos)*4);//Reservamos espacio para 1 hilo escritor y 3 hilos lectores

recurso=5;//Inicializamos las variables globales
numeroLectores=0;

pthread_mutex_init(&semaforo1,NULL);//Inicialización de semáforos
pthread_mutex_init(&semaforo2,NULL);


int i;//Iterador

int * limiteLecturas;
int * limiteEscrituras;

limiteLecturas=(int *)malloc(sizeof(int));
limiteEscrituras=(int *)malloc(sizeof(int));

system("clear");

printf("\nIntroduce un limite de lecturas por hilo: ");
scanf("%d",limiteLecturas);

printf("\nIntroduce un limite de escrituras por hilo: ");
scanf("%d",limiteEscrituras);


//Lanzamiento de hilos
pthread_create(&hilos[0],NULL,(void *)escritor,(void *)limiteEscrituras);//Hilo escritor

for(i=1;i<4;i++){
	pthread_create(&hilos[i],NULL,(void *)lector,(void *)limiteLecturas);//Hilos lectores
}

//Recepcion de hilos
for(i=0;i<4;i++){
	pthread_join(hilos[i],NULL);
}

return 0;
}

void * escritor(void * limite){

	int l=*(int *)limite;
	
	while(l!=0){
		pthread_mutex_lock(&semaforo2);
		recurso=rand()%10;
		printf("\n\t[+]Escritor añadiendo %d\n",recurso);
		pthread_mutex_unlock(&semaforo2);
		sleep(rand()%4);
		l--;
	}

	pthread_exit(NULL);
}

void * lector(void * limite){
	int l=*(int *)limite;

	while(l!=0){
		pthread_mutex_lock(&semaforo1);

		numeroLectores+=1;
	
		if(numeroLectores==1){
			pthread_mutex_lock(&semaforo2);
		}	
		
		pthread_mutex_unlock(&semaforo1);

		printf("\n\t[$]Lector leyendo %d\n",recurso);
		pthread_mutex_lock(&semaforo1);
		sleep(rand()%3);
		numeroLectores-=1;

		if(numeroLectores==0){
			pthread_mutex_unlock(&semaforo2);
		}

		pthread_mutex_unlock(&semaforo1);
		l--;
	}

pthread_exit(NULL);
}

