#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <pthread.h>

void * generaAleatorio(void * indice){
int aleatorio1,aleatorio2,contador;
int * suma;

suma=(int*)malloc(sizeof(int));
sleep(*(int *)indice);
//Como indice es un puntero a la posición del vector de indices usamos hacemos casting para deshacer void
srand(time(NULL));

aleatorio1=rand()%10;
aleatorio2=rand()%10;

*suma=aleatorio1+aleatorio2;

fflush(stdout);
printf("\nHebra no: %d | Aleatorio 1: %d | Aleatorio 2 : %d | Suma de ambos: %d",*(int*)indice,aleatorio1,aleatorio2,*suma);


pthread_exit((void *)suma);
}

int main(){

int numHebras,i,sumaTotal=0;


void * sumaParcial;

system("clear");

printf("Introduce el numero de hebras a crear: ");
scanf("%d",&numHebras);

pthread_t vectorHebras[numHebras];
int indices[numHebras];
void * pindices;

pindices=indices;

for(i=0;i<numHebras;i++){
	indices[i]=i;
}


for(i=0;i<numHebras;i++){
	pthread_create(&vectorHebras[i],NULL,(void *)generaAleatorio,(void *)pindices);
}

for(i=0;i<numHebras;i++){
	pthread_join(vectorHebras[i],(void **)&sumaParcial);
	/*Como la funcion devuelve un puntero hemos de acceder por indirección al valor de suma*/
	sumaTotal+=*(int*)sumaParcial;
}

printf("\n\n El resultado de sumar todos los aleatorios es: %d \n\n", sumaTotal);
return 0;
}
