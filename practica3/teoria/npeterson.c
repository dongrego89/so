#include <stdio.h>
#include <stdlib.h>

int * nivel, * esperando;
int nhilos;

//Sección crítica

int global = 0;

void * hilo (void * identificador);

int main (int argc, char ** argv)
{
	int i;
	int * id; //ID de cada hilo
	pthread_t * hilos; //Vector con los hilos
	
	if(argc != 2)
	{
		printf("Formato incorrecto, la forma de invocar al programa es:\n%s <no. de hilos>\n",argv[0]);
		exit(-1);
	}

	nhilos = atoi(argv[1]);
	if(nhilos < 1) exit(-1);
	
	//reserva de memoria para las hebras
	hilos = (pthread_t *) malloc(nhilos*sizeof(pthread_t));
	//reserva de memoria para identificar las hebras 
	id = (int *) malloc(nhilos*sizeof(int));
	//reserva de memoria para el vector que indica intenciones
	nivel = (int *) malloc(nhilos*sizeof(int));
	//reserva de memoria para el vector de turnos
	esperando = (int *) malloc((nhilos-1)*sizeof(int));
	//Completar control de errores
	//...	

	for(i=0 ; i<nhilos ; ++i)
	{
		id[i] = i;
		nivel[i] = -1;
		if(i < nhilos-1) 
			esperando[i] = -1;
	}
	
	for(i=0 ; i < nhilos ; ++i)
		pthread_create(hilos+i,NULL,hilo,(void *) (id+i));
	//Completar control de errores
	//...	

	for(i=0 ; i < nhilos ; ++i)
		pthread_join(hilos[i],NULL);
	//Completar control de errores
	//...	

	printf("Hebra main(), valor de global=%d\n",global);
	
	//Liberación de memoria y cierre de ficheros
	free(hilos);
	free(id);
	free(nivel);
	free(esperando);
	exit(EXIT_SUCCESS);
}

void * hilo (void * identificador)
{
	int id = *((int*) identificador);
	int l,k;
	int existe = 0;
	int local;
	
	for(l=0 ; l < nhilos-1 ; ++l)
	{
		nivel[id] = l;
		esperando[l] = id;
		
		do
		{
			existe = 0;
			for(k=0 ; k < nhilos-1 ; ++k)
				if(k != id && nivel[k] >= l) 
					existe = 1;
		}while(esperando[l] == id && existe == 1);
	}
	
	//INICIO SECCIÓN CRÍTICA
	printf("Hilo %d en sección crítica, global=%d\n",id,global);
	for(k=0 ; k < 5 ; ++k)
	{
		local = global;
		local++;
		global=local;
		usleep(100000); //milisegundos
	}
	printf("Hilo %d sale de sección crítica, global=%d\n",id, global);
	//FIN SECCIÓN CRÍTICA	
	nivel[id] = -1;
}
