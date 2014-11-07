#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>

void * cuentaLineas(void * fichero){
	char * auxiliar;
	char ruta[200];
	int * retorno;

	FILE * archivo;

	retorno=(int *)malloc(sizeof(int));
	auxiliar=(char *)fichero;

	archivo=fopen(auxiliar,"r");

	//funcion contadora de lineas	PETA, REVISAR
	*retorno=0;
	
	while(!feof(archivo)){
		if(fgetc(archivo)=='\n'){
			*retorno++;
		}
	}
	
	fclose(archivo);
	pthread_exit((void *)retorno);	
}

int main(int argc, char * argv[]){

int nArchivos,i,lineasTotales=0;
pthread_t * hilos=NULL;
void * subTotal;


nArchivos=argc-1;
hilos=(pthread_t *)malloc(sizeof(pthread_t)*nArchivos);

for(i=1;i<argc;i++){
	pthread_create(&hilos[i-1],NULL,(void *)cuentaLineas,(void *)&argv[i]);
}

for(i=0;i<nArchivos;i++){
	pthread_join(hilos[i],(void **)&subTotal);
	lineasTotales+=*(int*)subTotal;
}

printf("Entre los %d ficheros hay %d lineas",nArchivos,lineasTotales);
return 0;
}
