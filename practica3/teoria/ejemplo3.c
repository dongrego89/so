/*  Implemente el problema para hilos teniendo en cuenta que la sección crítica va a ser un
array de enteros con una capacidad de 5 elementos. Haga una implementación usando mutexs pero no
variables de condición, por lo que se producirá espera activa en casos en los que no haya sitio donde
producir o no haya items que consumir */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h> 
#include <errno.h>

//Definiciones de macros TRUE y FALSE
#define TRUE 1
#define FALSE 0

pthread_mutex_t semaforo;//Declaracion de semáforo

int * buffer;//Declaracion de puntero array de enteros
int tamBuffer;//Numero de elementos del buffer
int usoBuffer=0;//Elementos utiles en el buffer

void * produce(void * l);//Funcion productor
void * consume(void *);//Funcion consumidor
void validaCreate(int e);//Funcion captadora de errores en creacion de hilos
void validaJoin(int e);//Funcion captadora de errores en captacion de hilos

int main(){
	srand(time(NULL));//Semilla para aleatorios	
	pthread_mutex_init(&semaforo,NULL);//Inicialización de semáforo

	pthread_t * hilos;

	hilos=(pthread_t *)malloc(sizeof(pthread_t)*2);

	int unidades;
	int * limite;
	int error;
	int i;

	limite=(int *)malloc(sizeof(int));

	printf("Introduce un espacio en unidades para el buffer: ");
	scanf("%d",&unidades);

	buffer=(int *)malloc(sizeof(int)*unidades);//Reserva de memoria para el buffer
	tamBuffer=unidades;//Asignacion del valor

	printf("Introduce el limite de producciones: ");
	scanf("%d",limite);



	error=pthread_create(&hilos[0],NULL,(void *)produce,limite);
	validaCreate(error);

	error=pthread_create(&hilos[1],NULL,(void *)consume,NULL);
	validaCreate(error);

	for(i=0;i<2;i++){
		error=pthread_join(hilos[i],NULL);
		validaJoin(error);
	}





	exit(EXIT_SUCCESS);
}

void validaCreate(int e){
	char * mensaje="\n\t-Se produjo el error";
	switch(e){
		case EAGAIN:
			printf("%s EAGAIN (Sin recursos o maximo de hilos alcanzado) \n\n",mensaje);
		break;
		case EINVAL:
			printf("%s EINVAL (Valor de atributos incorrecto)\n\n",mensaje);
		break;
		case EPERM:
			printf("%s EPERM (Sin privilegios para configurar la planificacion)\n\n",mensaje);
		break;	
		default:
			//printf("\n\t-Hilo lanzado satisfactoriamente\n\n");
		break;	
	}
}

void validaJoin(int e){
	char * mensaje="\n\t-Se produjo el error";
	switch(e){
		case EDEADLK:
			printf("%s EDEADLK (Se quiso hacer un join a si mismo) \n\n",mensaje);
		break;
		case EINVAL:
			printf("%s EINVAL (Hilo detached u otro hilo esperando a que termine)\n\n",mensaje);
		break;
		case ESRCH:
			printf("%s ESRCH (El hilo no existe)\n\n",mensaje);
		break;	
		default:
			//printf("\n\t-Hilo captado satisfactoriamente\n\n");
		break;	
	}
}

void * produce(void * l){
	int * limit=(int *)l;
	int posicion=0;
	
	while(TRUE){
		if(*limit!=0){//Si no hemos llegado al limite de producciones..
	
			while(usoBuffer==tamBuffer);//Mientras el buffer este lleno... Espera Activa
		
			pthread_mutex_lock(&semaforo);//Bloqueo de seccion critica

			buffer[posicion]=rand()%10;//Se almacena un aleatorio en esa posicion

			printf("\n+ Elemento producido: %d | Posicion: %d", buffer[posicion],posicion);
		
			posicion=(posicion+1)%tamBuffer;//Se incrementa circularmente la posicion
			usoBuffer+=1;//Hay un elemento util nuevo en el buffer
			*limit-=1;//Decrementamos el numero de producciones restantes
			
			pthread_mutex_unlock(&semaforo);//Liberacion de seccion critica
			
		}
	}
	pthread_exit(NULL);
}

void * consume(void * arg){
	int posicion=0;
	
	while(TRUE){
		while(usoBuffer==0);//Mientras el buffer este vacío... Espera activa
		
		pthread_mutex_lock(&semaforo);//Bloqueo de seccion critica
		
		printf("\n- Elemento consumido: %d | Posicion: %d",buffer[posicion],posicion);		
	
		posicion=(posicion+1)%tamBuffer;//Incrementamos circularmente la posicion		
		usoBuffer-=1;//Decrementamos el numero de elementos utiles del buffer
		pthread_mutex_unlock(&semaforo);//Liberacion de seccion critica
	}
	pthread_exit(NULL);
}


/*
pthread_cond_signal(&cond); 
pthread_cond_wait(&cond, &mtx)
*/
