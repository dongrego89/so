/*****************************************************************************
* DESCRIPTION:
* This example program illustrates the use of mutex variables
* in a threads program. This version performs a dot product (scalar product)
* from two vectors. The main data is made available to all threads through
* a globally accessible structure. Each thread works on a different
* part of the data. The main thread waits for all the threads to complete
* their computations, and then it prints the resulting sum.
* ESTE CÓDIGO REALIZA EL PRODUCTO ESCALAR DE DOS VECTORES Y MUESTRA LA SUMA
* DE LOS ELEMENTOS RESULTANTES DE ESE PRODUCTO. TODO ELLO SE HACE DISTRIBUYENDO
* EL TRABAJO ENTRE HEBRAS.
* SOURCE: Vijay Sonnad, IBM
* FROM: LAST REVISED: 01/29/09 Blaise Barney
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUMTHRDS 3
#define VECLEN 3

/*
la siguiente estructura contiene la info necesaria 
para permitir la funcion dotprod acceder a este
input datos y colocar este output en la estructura
*/

typedef struct{
	int *a;
	int *b;
	int sum;
	int veclen;
	}DOTDATA;

/* Define globalmente variables accesibles y el cerrojo*/

DOTDATA dotstr;

pthread_t callThd[NUMTHRDS];
pthread_mutex_t mutexsum;
/*
The function dotprod is activated when the thread is created. As before, all input to this routine is obtained from a
structure of type DOTDATA and all output from this function is written into this structure. The benefit of this approach
is apparent for the multi-threaded program: when a thread is created we pass a single argument to the activated
function - typically this argument is a thread number. All the other information required by the function is accessed
from the globally accessible structure.
*/
void *dotprod(void *arg){
	/* Define and use local variables for convenience */
	int i, start, end, len;
	long offset;
	int mysum, *x, *y;
	offset = *(long *)arg;
	len = dotstr.veclen;
	start = offset*len;
	end = start + len;
	x = dotstr.a;
	y = dotstr.b;
	
	/*
	Perform the dot product and assign result to the appropriate variable in the structure.
	*/
	
	mysum = 0;
	for (i=start; i<end ; i++)
		mysum += (x[i] * y[i]);
	/*
	Lock a mutex prior to updating the value in the shared structure, and unlock it upon updating.
	*/
	pthread_mutex_lock (&mutexsum);//COMIENZA SECCION CRITICA. BLOQUEAMOS !!!!!!
	dotstr.sum += mysum;
	printf("\nThread %ld did %d to %d: mysum=%d global sum=%d\n",offset,start,end-1,mysum,dotstr.sum);
	pthread_mutex_unlock (&mutexsum);//FIN DE SECCION CRITICA. DESBLOQUEAMOS !!!!
	pthread_exit((void*) 0);
}
/*
The main program creates threads which do all the work and then print out result upon completion. Before creating the
threads, The input data is created. Since all threads update a shared structure, we need a mutex for mutual exclusion.
The main thread needs to wait for all threads to complete, it waits for each one of the threads. We specify
a thread attribute value that allow the main thread to join with the threads it creates. Note also that we free up handles
when they are no longer needed.
*/
int main (int argc, char *argv[]){
	long i[NUMTHRDS],j;
	int *a, *b;
	void *status;
	pthread_attr_t attr;
	srand(time(NULL));
	
	/* Assign storage and initialize values */
	a = (int*) malloc (NUMTHRDS*VECLEN*sizeof(int));
	b = (int*) malloc (NUMTHRDS*VECLEN*sizeof(int));
	for (j=0; j<VECLEN*NUMTHRDS; j++){
		a[j]=rand()%11;
		b[j]=rand()%11;
	}
	printf("a:\t");
	for (j=0; j<VECLEN*NUMTHRDS; j++)
		printf("%d\t", a[j]);
	
	printf("\nb:\t");
	for (j=0; j<VECLEN*NUMTHRDS; j++)
		printf("%d\t", b[j]);
	
	dotstr.veclen = VECLEN;
	dotstr.a = a;
	dotstr.b = b;
	dotstr.sum=0;
	
	pthread_mutex_init(&mutexsum, NULL);//INICIALIZACION DE CERROJO
	/* Create threads to perform the dotproduct */
	
/*	pthread_attr_init(&attr);//???????????????
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);//???????????????????????????
*/
	/* Each thread works on a different set of data.
	* The offset is specified by 'i'. The size of
	* the data for each thread is indicated by VECLEN.
	*/
	for(j=0;j<NUMTHRDS;j++)
	i[j]=j;
	for(j=0;j<NUMTHRDS;j++)
	pthread_create(&callThd[j], NULL, dotprod, (void *)&i[j]);
	pthread_attr_destroy(&attr);
	/* Wait on the other threads */
	for(j=0;j<NUMTHRDS;j++)
	pthread_join(callThd[j], &status);
	//to complete errors...
	/* After joining, print out the results and cleanup */
	printf ("Sum = %d \n", dotstr.sum);

	free (a);
	free (b);
	pthread_mutex_destroy(&mutexsum);
}
