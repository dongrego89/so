#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

pthread_mutex_t semaforo; //semaforo global

void * imprimeCaracter(void * caracter){
	int i,s;	
	char * c;
	c=(char *) caracter;
		
	s=pthread_mutex_lock(&semaforo);
	if(s!=0){
		printf("\nError bloqueando semaforo");
	}
	
	//Seccion critica

	for(i=0;i<5;i++){
		printf("%c",*c);		
	}
	
	s=pthread_mutex_unlock(&semaforo);	

	if(s!=0){
		printf("\nError desbloqueando semaforo");
	}

	//Fin seccion critica
}

int main(int argc, char * argv[]){

char * vectorCaracteres="!#*%";
pthread_t * hilos;
int i;

pthread_mutex_init(&semaforo,NULL);

hilos=(pthread_t *)malloc(sizeof(pthread_t)*4);

system("clear");
printf("\n\n");

for(i=0;i<4;i++){
	pthread_create(&hilos[i],NULL,(void *)imprimeCaracter,(void*)&vectorCaracteres[i]);
}

for(i=0;i<4;i++){
	pthread_join(hilos[i],NULL);
}
printf("\n\n");

return 0;
}



/*nhilos = 3

estruct
	niteraciones=5
	caracter=?
semaforo binario ejemplo1.c*/
