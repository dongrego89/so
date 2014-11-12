#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>

void * cuentaLineas(void * fichero){
	char * auxiliar;
	int * retorno;
	int c;

	FILE * archivo;

	retorno=(int *)malloc(sizeof(int));
	auxiliar=(char *)fichero;

	archivo=fopen(auxiliar,"r");

	*retorno=0;

 	while((c=fgetc(archivo))!=EOF){//Contamos los retornos de carro para conocer el nº de lineas
       		if(c=='\n'){
			*retorno+=1;
		}
	}
	
	fclose(archivo);
	printf("\n Sumatorio de lineas: %d",*retorno);
	pthread_exit((void *)retorno);	
}

int main(int argc, char * argv[]){

system("clear");

if(argc>1){
	int nArchivos,i,lineasTotales=0;
	pthread_t * hilos=NULL;
	void * subTotal;



	nArchivos=argc-1;
	hilos=(pthread_t *)malloc(sizeof(pthread_t)*nArchivos);

	for(i=1;i<argc;i++){
		pthread_create(&hilos[i-1],NULL,(void *)cuentaLineas,(void *)argv[i]);
	}

	for(i=0;i<nArchivos;i++){
		pthread_join(hilos[i],(void **)&subTotal);
		lineasTotales+=*(int*)subTotal;
	}

	printf("\n \nTotal de lineas %d en %d ficheros\n\n",lineasTotales,nArchivos);
}
else{
	printf("\nEs necesario pasar como argumento un nombre de fichero texto como minimo\n\n");
}
return 0;
}
