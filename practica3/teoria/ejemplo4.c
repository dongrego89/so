#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum {FALSE=0, TRUE=1} booleano;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int avail = 0;

void * threadFunc(void *arg)
{
	int cnt = atoi((char *) arg);
	int s, j;
	
	for (j = 0; j < cnt; j++)//de 0 a el numero de incrementos en este hilo
	{
		//Bloquea el semaforo
		s = pthread_mutex_lock(&mtx);
		
		if (s != 0)
		printf("mutex_lock error...\n");
		/* Let consumer know another unit is available */
		
		avail++;
		printf("avail variable increased by thread %ld, avail=%d\n", (unsigned long) pthread_self(),avail);
		//Unlock mutex
		s = pthread_mutex_unlock(&mtx);
		
		if (s != 0)
		printf("mutex_unlock error...\n");
		/* Wake sleeping consumer */

		s = pthread_cond_signal(&cond);
		
		if (s != 0)
		printf("pthread_cond_signal error...\n");
		
		printf("Sending cond_signal from thread %ld...\n", (unsigned long) pthread_self());
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t tid;
	int s, j;
	int totRequired; /* Total de unidades que todos los hilos produciran */
	int numConsumed; /* Total de unidades que serán consumidas como mucho */
	booleano done;
	
	if(argc<=3)
	{
		printf("Ussage: ./a.out Number_of_increasing_for_thread1 Number_of_increasing_for_thread2 Number_of_increasing_for_threadN\n");
		exit(EXIT_FAILURE);
	}
	
	/* Crear todos los hilos */
	totRequired = 0;
	
	for (j = 1; j < argc; j++)
	{
		totRequired += atoi(argv[j]);//Se suman en totrequired la cantidad de unidades a producir
		s = pthread_create(&tid, NULL, threadFunc, argv[j]);//Se crea el mismo hilo que llama a la funcion threadFunc y recibe el argumento de unidades a producir
	
		if (s != 0)
		printf("Error creando hilo...\n");
	}
	
	/* Bucle para consumir todas las unidades disponibles */
	numConsumed = 0;
	done = FALSE;
	
	while(TRUE) //Simulacion continua
	{
	//bloquea semaforo
	s = pthread_mutex_lock(&mtx);
	if (s != 0)
	printf("Error cerrando semaforo...\n");

	while (avail == 0)//Mientras no haya unidades disponibles...
	{
		/* Espera por algo para consumir */
		printf("Main llamando a pthread_cond_wait()..Bloqueando main por planificador!!!\n");
		s = pthread_cond_wait(&cond, &mtx);
		if (s != 0)
		printf("pthread_cond_wait error...\n");
		printf("Tiempo de dormir concluido. Main vuelve a funcionar. No tiempo improductivo mientras avail es menor o igual a 0!!!\n");
	}

	while (avail > 0) /* Una vez haya unidades disponibles....Consumir todas las unidades disponibles */
	{
		/* Haz algo con la unidad producida*/
		numConsumed ++;
		avail--;
		printf("Main, numConsumed=%d\n", numConsumed);
		done = numConsumed >= totRequired;
	}
	//Desbloquea semaforo
	s = pthread_mutex_unlock(&mtx);
	if (s != 0)
	printf("mutex_unlock error...\n");
	
	if (done)
	{
	printf("Saliendo... Todos los items han sido consumidos\n");
	break;
	}
	/* Aqui puede ir otro trabajo que no requiera el bloqueo del semaforo */
	}
	exit(EXIT_SUCCESS);
}
