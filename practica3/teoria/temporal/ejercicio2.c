//Sistemas Operativos
//Práctica 2. Ejercicio 2
//Programa que crea N hebras, cada hebra obtiene dos números aleatorios y retorna la suma de ambos. En la hebra principal se acumulará la suma de todos
//los números aleatorios generados por las N hebras.
//

#include <stdio.h>
#include <pthread.h>
#include <errno.h>	//Códigos de error
#include <string.h>	//Cadenas
#include <stdlib.h>
#include <time.h>	//rand
#define TAM 10

void * suma_aleatorios(void * parametro)
{
	//Función que calcula dos números aleatorios y devuelve la suma de ambos.
	//Declaración de variables.
	int *num_hebra;
	int aleatorio1;
	int aleatorio2;
	int *suma;

	//Reservamos memoria para devolver la suma de los dos números aleatorios.
	suma=(int *)malloc(sizeof(int));
	if(suma==NULL)
	{
		printf("\nError!! No se pudo hacer la reserva de memoria solicitada.");
		printf("\nSe cancela la ejecución del programa.");
		exit(-1);
	}

	//Conversión de void * a int *
	num_hebra = (int *)parametro;

	//Calculamos los dos números aleatorios entre 0 y 10 y la suma de ambos.
	aleatorio1=rand()%11;
	aleatorio2=rand()%11;
	*suma=aleatorio1+aleatorio2;

	printf("      -> Hebra %d (%lu): %d + %d =  %d\n\n",*num_hebra,(unsigned long int)pthread_self(),aleatorio1, aleatorio2, *suma);
	//Finalización correcta y devolvemos suma (estamos devolviendo una dirección de memoria a la que hacemos casting a void *).
	pthread_exit( (void *) suma);
}


int main(int argc, char **argv)
{
	//Declaración de variables.
	time_t t;
	int i;
	int n_hebras;			//Número de hebras a crear
	int error;
	void *puntero_a_suma;
	int suma=0;
	pthread_t hebras_id[TAM];	//Para almacenar el identificador de cada hebra.
	int hebras_num[TAM];		//Para almacenar el número de hebra asignado en el bucle for, sino tenemos el problema de "hello_argc3.c de Moodle".

	//Generamos la semilla. Solo se invoca una vez
	srand ((int) time(&t));

	//Controlamos los parámetros.
	if(argc!=2)
	{
		printf("Parámetros incorrectos.\n");
		printf("Sintaxis correcta: %s <número de hebras>\n", argv[0]);
		printf("Se para la ejecución del programa.\n");
		exit(-1);
	}

	n_hebras=atoi(argv[1]);

	//Creamos las hebras.
	for(i=0;i<n_hebras;i++)
	{
		hebras_num[i]=i+1;
		error = pthread_create( &(hebras_id[i]), NULL, (void *) suma_aleatorios, (void *) &hebras_num[i]);
		//Capturamos los errores que se pueden producir al crear una hebra.
		switch (error)
		{
			case 0: //Correcto.
				printf("    (Hilo ppal.) Se ha creado la hebra ..: %lu\n", (unsigned long int)hebras_id[i]);
				break;
			case EAGAIN: //El sistema no puede crear más hebras bien porque se ha llegado al máximo PTHREAD_THREADS_MAX
				          //o bien porque se ha quedado sin recursos para ello.
				printf("    (Hilo ppal.) Se produjo el error EAGAIN (sin recursos o máximo de hebras alcanzado).\n");
				break;
			case EINVAL: //La configuración especificada en el parámetro attr no es correcta.
				printf("    (Hilo ppal.) Se produjo el error EINVAL (valores de attr incorrectos).\n");
				break;
			case EPERM:  //El proceso no tiene los permisos apropiados para configurar la política de planificacion
				          //o la configuración indicada en el parámetro attr.
				printf("    (Hilo ppal.) Se produjo el error EPERM (sin privilegios para configurar la planificación o attr).\n");
				break;
		}

	}

	//Esperamos a la finalización de las hebras.
	for(i=0;i<n_hebras;i++)
	{
		error = pthread_join(hebras_id[i], (void **) &puntero_a_suma);
		//Capturamos los errores que se pueden producir al recoger (join) una hebra.
		switch (error)
		{
			case 0: //Correcto.
				printf("        (Hilo ppal.) Terminó correctamente la hebra ..: %lu\n", (unsigned long int)hebras_id[i]);
				printf("        (Hilo ppal.) Y devolvió el valor ..: %d\n\n", *(int *)puntero_a_suma);
				suma=suma+*(int *)puntero_a_suma;
				//liberamos la memoria que reservó la hebra.
				free(puntero_a_suma);
				break;
			case EDEADLK: //Se quiso hacer un join a sí misma.
				printf("       ** (Hilo ppal.) Se quiso hacer un join a sí mismo.\n");
				break;
			case EINVAL: //La hebra ha sido detached u otra hebra está esperando a que termine.
				printf("       ** (Hilo ppal.) Se produjo el error EINVAL (la hebra ha sido detached u otra hebra está esperando a que termine.).\n");
				break;
			case ESRCH:  //Se ha hecho un pthread_join sobre una hebra que no existe.
				printf("       ** (Hilo ppal.) Se produjo el error ESRCH (la hebra no existe).\n");
				break;
		}
	}
	printf("    (Hilo ppal.) El resultado de la suma es ..: %d\n", suma);
	printf("    (Hilo ppal.) Finalización correcta.\n");

	//Finalización correcta.
	exit(EXIT_SUCCESS);
}

