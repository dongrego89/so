#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

int glob = 0; //GLOBAL VARIABLE

pthread_mutex_t mtx; //GLOBAL MUTEX


void * threadFunc(void *arg) /* Bucle que incrementa argumento veces global */
{
	int loops = *((int *) arg);//n iteraciones
	int loc, j, s;//j iterador y s es estado de bloquear cerrojo
	for (j = 0; j < loops; j++)
	{
		s = pthread_mutex_lock(&mtx);/* BLOQUEAR !!!!!!!!!!!!!!!*/ 
		
		if (s != 0)
			printf("mutex_lock error...\n");
	
		/*SECCION CRITICA*/
	
		loc = glob;
		loc++;
		glob = loc;
		printf("Thread %ld increasing the global variable...\n", (unsigned long) pthread_self());
		s = pthread_mutex_unlock(&mtx); //desbloquear!!!!!!!!!!!!!!!!!!!!
		
		/*DESBLOQUEO DE CERROJO*/
		
		if (s != 0)
			printf("mutex_unlock error...\n");
	}
	return NULL;
}


int main(int argc, char *argv[])
{
	pthread_mutex_init(&mtx,NULL);

	pthread_t t1, t2;
	int loops, s;
	
	if(argc!=2)
	{
		printf("Ussage: ./a.out Number_of_loops\n");
		exit(EXIT_FAILURE);
	}
	//loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;
	loops = atoi(argv[1]);//n iteraciones es el argumento por comandos 1

	s = pthread_create(&t1, NULL, threadFunc, &loops);//creamos hilo 1 y pasamos por parametro n iteraciones
	if (s != 0)
		printf("pthread_create error...\n");//si no se crea hay error
	
	s = pthread_create(&t2, NULL, threadFunc, &loops);//creamos hilo 2 y pasamos por parametro n iteraciones
	if (s != 0)
		printf("pthread_create error...\n");//si no se crea hay error
	
	printf("MAIN Thread, Stopping in the join call...\n");//hilo principal parando en el join
	s = pthread_join(t1, NULL);//si no se para el hilo, error recibiendo hilo 1
	if (s != 0)
		printf("pthread_join error...\n");

	s = pthread_join(t2, NULL);//si no se para el hilo, error recibiendo hilo 2
	if (s != 0)
		printf("pthread_join error...\n");

	printf("MAIN Thread leaves join calls, the global variable is:%d\n", glob); //hilo principal acaba las recepciones. Imprimimos variable global
	
	exit(EXIT_SUCCESS);
}
