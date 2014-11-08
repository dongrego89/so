#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>

typedef struct{
	int corteA;
	int corteB;
	int * vector;
}datos;

void * sumaVector(void * sum){
	datos * aux;
	int * suma;
	int i;

	aux=(datos*)sum;
	suma=(int *)malloc(sizeof(int));
	*suma=0;
	
	for(i=aux->corteA;i<=aux->corteB;i++){
		*suma+=aux->vector[i];
	}
	printf("\n\t %10s %4d","Subtotal",*suma);

	pthread_exit((void *)suma);
}

int main(int argc, char * argv[]){
system("clear");

if(argc==2){

	int nHilos=atoi(argv[1]);
	int aleatorios[10],i,resto,indiceA,indiceB,sumaTotal=0;

	pthread_t * hilos;
	hilos=(pthread_t *)malloc(sizeof(pthread_t)*nHilos);
	
	datos * sumandos;
	sumandos=(datos *)malloc(sizeof(datos)*nHilos);
	
	void * sumaSubtotal;
	

	srand(time(NULL));

	printf("\nVector aleatorio generado: ");
	for(i=0;i<10;i++){
		aleatorios[i]=rand()%10;
		printf("[%d]",aleatorios[i]);
	}
	printf("\n");
	resto=10%nHilos;

	for(i=0;i<nHilos;i++){
	
		if(i!=0){//Si no es la primera iteracion
			indiceA=indiceB+1;
			indiceB=indiceA+(10/nHilos)-1;
		}
		else{//Si es la primera iteracion	
			indiceA=0;
			indiceB=indiceA+(10/nHilos)-1;
		}	
		
		if(resto!=0){
			indiceB++;
			resto--;
		}	
		
		sumandos[i].corteA=indiceA;
		sumandos[i].corteB=indiceB;
		sumandos[i].vector=aleatorios;
		
	}
	
	for(i=0;i<nHilos;i++){
		pthread_create(&hilos[i],NULL,(void *)sumaVector,(void *)&sumandos[i]);
	}
	for(i=0;i<nHilos;i++){
		pthread_join(hilos[i],(void **)&sumaSubtotal);
		sumaTotal+=*(int *)sumaSubtotal;
	}

printf("\n\n\t %10s %4d \n\n","Total",sumaTotal);

}
else{
	printf("\nSe esperaba un parametro numero de hilos\n\n");
}

return 0;
}
