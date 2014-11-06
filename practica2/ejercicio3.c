#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>

typedef struct{
	int ca;
	int cb;
	int * vector;
}pares;

void * sumaParcial(void * parejas){
	pares aux=*(pares *)parejas;
	int i,* suma;
	
	suma=(int*)malloc(sizeof(int));
	*suma=0;
	for(i=aux.ca;i<=aux.cb;i++){
		*suma+=aux.vector[i];
	}
	printf("\nCorte A: %d y Corte B: %d",aux.ca,aux.cb);
	printf("\n \tSuma parcial vale %d",*suma);
}

int main(int argc, char* argv[]){
system("clear");

if(argc!=2){
	printf("\nSe debe pasar un numero de hilos como parametro\n");
	return -1;
}
else{
	int i=10,j,vector[i],nHilos=atoi(argv[1]),corteA=0,corteB=0,resto;//hacerle un atoi
	pares parejas;

	if((nHilos>10) || (nHilos<=0)){
		printf("\nEl numero de hilos no debe superar a 10 ni ser inferior a 1\n");
		return -1;
	}
	
	srand(time(NULL));//Inicialización de semilla
	printf("\nNuestro vector aleatorio es: ");

	for(j=0;j<i;j++){
		vector[j]=rand()%10;
		printf("[%d]",vector[j]);
	}
	
	resto=i%nHilos;
	
	for(j=0;j<nHilos;j++){//El bucle recorrera desde el hilo 1 hasta el hilo nHilos
		if(j==0){
			corteA=0;
		}		
		else{
			corteA=corteB+1;
		}
		
		corteB=corteA+(i/nHilos)-1;

		if(resto>0){
			resto--;
			corteB++;
		}
		parejas.ca=corteA;
		parejas.cb=corteB;
		parejas.vector=vector;

		sumaParcial((void *)&parejas);

	//DEPUR	printf("\n Corte A: %d - Corte B: %d - Resto %d",corteA,corteB,resto);	
		
	}
		


	printf("\nRepartiremos las sumas en %s hilos\n",argv[1]);



	return 0;

	
}

}
