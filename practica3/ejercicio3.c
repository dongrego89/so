#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

pthread_mutex_t semaforo;//Semaforo

int nElementos=5;
int buffer[5];//Vector
int n=0;//Elementos utiles dentro del vector

void * productor(void * arg){

int * aux=(int *)arg;
int i;

int posEscribir=0;//Posicion donde comienza a escribir
	while(1){//While TRUE
		while(n==nElementos);//Espera activa mientras el buffer este lleno

	pthread_mutex_lock(&semaforo);
	//Seccion critica

	if(*aux!=0){
		buffer[posEscribir]=rand()%10;
		//imprimir que he generado y en que posicion
	
		printf("\n + Se ha producido %d en la posicion %d",buffer[posEscribir],posEscribir);
	
		posEscribir= (posEscribir+1)%nElementos;//Generamos el proximo indice
		n+=1;
		*aux-=1;
	}
	
	
	
	//Fin seccion critica
	pthread_mutex_unlock(&semaforo);
		
	}
pthread_exit(NULL);
}

void * consumidor(void * arg){
int posLeer=0;//Posicion donde comienza a leer
	
	while(1){//While TRUE
		while(n==0);//Espera activa mientras el buffer este vacío	
	//Seccion critica	
	pthread_mutex_lock(&semaforo);
	fflush(stdout);

	printf("\n - Se ha consumido %d en la posicion %d",buffer[posLeer],posLeer);

	buffer[posLeer]=0;

	posLeer=(posLeer+1)%nElementos;

	n-=1;//Un elemento util menos en el buffer

	pthread_mutex_unlock(&semaforo);
	
	//Fin de seccion critica
	}
pthread_exit(NULL);
}

int main(){

srand(time(NULL));
int limite;

pthread_mutex_init(&semaforo,NULL);
pthread_t * hilos;

hilos=(pthread_t *)malloc(sizeof(pthread_t)*2);

printf("Indica el número maximo de producciones: ");
scanf("%d",&limite);

pthread_create(&hilos[0],NULL,(void *)productor,(void *)&limite);
pthread_create(&hilos[1],NULL,(void *)consumidor,NULL);

pthread_join(hilos[0],NULL);
pthread_join(hilos[1],NULL);

return 0;
}


