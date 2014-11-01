#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void imprimeCadena(void * cadena){//funcion para imprimir por pantalla
	int i;
	char * aux = (char *)cadena;
	for(i=0;i<strlen(aux);i++){
		printf("%c",aux[i]);
		fflush(stdout);
		sleep(1);
		}
}

int main(int argc,char * argv[]){
	char * cadena1="Hola";//char * argv???
	char * cadena2="Mundo";
	system("clear");
	pthread_t hilo1,hilo2;
	
	pthread_create(&hilo1,NULL,(void *)imprimeCadena,(void *) cadena1);
	pthread_create(&hilo2,NULL,(void *)imprimeCadena,(void *) cadena2);

	pthread_join(hilo1,NULL);
	pthread_join(hilo2,NULL);

	printf("\nFin de ejercicio\n\n");
	return 0;
}
