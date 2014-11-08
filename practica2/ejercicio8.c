#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

void masCincuenta(void * v){
	int * aux;
	int i;
	aux=(int *)malloc(sizeof(int));
	aux=(int*)v;
	

	for(i=0;i<50;i++){
		*aux+=1;	
	}

	//pthread_exit((void *)aux);
}

int main(){

int vGlobal=0,i;
pthread_t * hilos;

hilos=(pthread_t *)malloc(sizeof(pthread_t)*2);

for(i=0;i<2;i++){
	pthread_create(&hilos[i],NULL,(void *)masCincuenta,(void *)&vGlobal);
}

for(i=0;i<2;i++){
	pthread_join(hilos[i],NULL);
}

printf("El valor de la variable global es %d \n\n",vGlobal);

return 0;
}

