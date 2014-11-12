#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

typedef struct{
	float escalar;
	float matriz[3][3];
}parametros;


void * multiplicaMatriz(void * v){
	parametros * auxiliar;
	int i,j;
	auxiliar=(parametros *)malloc(sizeof(parametros));
	
	auxiliar=(parametros*)v;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			auxiliar->matriz[i][j]=auxiliar->matriz[i][j]*auxiliar->escalar;
		}
	}
	pthread_exit((void *)auxiliar);
}

int main(){

pthread_t * hilos;

parametros * valores;
parametros * resultados;

parametros auxiliar;

void * producto;

int i,j,k;

valores=(parametros *)malloc(sizeof(parametros)*2);
resultados=(parametros *)malloc(sizeof(parametros)*2);
hilos=(pthread_t *)malloc(sizeof(pthread_t)*2);

system("clear");

for(i=0;i<2;i++){//Insercion de valores a escalares y matrices
	
	printf("\nConjunto de valores %d::::::::::::::\n\nIntroduce un numero escalar: ",i+1);	
	scanf("%f",&auxiliar.escalar);
	printf("Introduce valores para la matriz:\n");
	for(j=0;j<3;j++){
		for(k=0;k<3;k++){
			printf("\t[%d][%d] : ",j,k);
			scanf("%f",&auxiliar.matriz[j][k]);
		}
	}
	valores[i]=auxiliar;
}



for(i=0;i<2;i++){//Lanzamiento de hilos
	pthread_create(&hilos[i],NULL,(void *)multiplicaMatriz,(void *)&valores[i]);
}

for(i=0;i<2;i++){//Espera de hilos
	pthread_join(hilos[i],(void **)&producto);
	resultados[i]=*(parametros *)producto;
}

for(i=0;i<2;i++){//Impresion de resultados
	
	printf("\nConjunto de valores %d::::::::::::::\n\nNumero escalar: %2.0f ",i+1,valores[i].escalar);
	printf("\nValores para la matriz:\n\t");
	for(j=0;j<3;j++){
		for(k=0;k<3;k++){
			printf("[%2.0f]",valores[i].matriz[j][k]);
		}
	printf("\n\t");
	}
}

return 0;
}
