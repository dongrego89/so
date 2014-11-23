//Sistemas Operativos
//Práctica 2. Ejercicio 3
//Programa que crea N hebras para sumar los 10 elementos de un vector. Cada hebra se encargará de sumar algunos elementos del vector y retornará dicho resultado.
//En la hebra principal se acumulará la suma de todas las hebras.

#include <stdio.h>
#include <pthread.h>
#include <errno.h>	//Códigos de error
#include <string.h>	//Cadenas
#include <stdlib.h>
#include <time.h>	//rand
#define TAM 10

//Prototipos
void * suma_vector(void *);

//Estructura con los datos que recibe cada hebra.
struct info_reparto
{
	int *vector;	//Puntero al inicio del vector
	int desde;	//posición de inicio a sumar
	int hasta;	//posición de fin a sumar
};

int main(int argc, char **argv)
{
	//Declaración de variables.
	time_t t;
	int vector[TAM];				//Vector de elementos.
	pthread_t hebras_id[TAM];		//Para almacenar el identificador de cada hebra.
	struct info_reparto reparto;		//reparto de la suma asignado a una hebra.
	struct info_reparto reparto_hebras[TAM];	//Para almacenar el reparto de todas las hebras.
	int desde;				//posición de inicio a sumar
	int hasta;				//posición de fin a sumar
	int n_hebras;				//Número de hebras a crear
	int *puntero_a_suma;			//Para obtener la suma de cada hebra.
	int suma = 0;				//Suma total.
	int i;
	int error;

	//Generamos la semilla.
	srand ((int) time(&t));

	//Controlamos los parámetros.
	if(argc!=2)
	{
		printf("Parámetros incorrectos.\n");
		printf("Sintaxis correcta: %s <número de hebras>\n", argv[0]);
		printf("Se para la ejecución del programa.\n");
		exit(-1);
	}

	//Controlamos el número de hebras recibidas.
	n_hebras=atoi(argv[1]);
	if(n_hebras<1 || n_hebras>10)
	{
		printf("Número de hebras incorrecto.\n");
		printf("Sintaxis correcta: %s <número de hebras (1..10)>\n", argv[0]);
		printf("Se para la ejecución del programa.\n");
		exit(-1);
	}

	//Rellenamos el vector con números aleatorios.
	printf("\nVector de elementos ..: ");
	for(i=0;i<10;i++)
	{
		vector[i]=rand()%10;
		printf("%d, ", vector[i]);
		suma=suma+vector[i];
	}

	printf(" --> Suma: %d \n\n", suma);
	suma=0;

	//todas las hebras reciben el vector completo.
	reparto.vector=vector;
	desde=0;
	//Dividimos el vector en partes iguales.
	hasta=(int)(10/n_hebras);
	//Creamos las hebras.
	for(i=0;i<n_hebras;i++)
	{
		reparto.desde=desde;
		//A la última hebra le asignamos el resto de elementos.
		if(i==(n_hebras-1))
			reparto.hasta=10;
		else
			reparto.hasta=desde+hasta;

		reparto_hebras[i]=reparto;
		//Actualizamos la posición desde para la próxima hebra
		desde=reparto.hasta;

		error = pthread_create( &(hebras_id[i]), NULL, (void *) suma_vector, (void *) &reparto_hebras[i]);		
		//Capturamos los errores que se pueden producir al crear una hebra.
		switch (error)
		{
			case 0: //Correcto.
				printf("    (Hilo ppal.) Se ha creado la hebra ..: %lu\n", (unsigned long int)hebras_id[i]);
				printf("    (Hilo ppal.)    Tiene que sumar desde %d hasta %d \n\n", reparto.desde, (reparto.hasta-1));
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
				printf("        (Hilo ppal.) Y devolvió el valor ..: %d\n\n", *puntero_a_suma);
				suma=suma+*puntero_a_suma;
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

	printf("\n\n");
	//Finalización correcta
	exit(EXIT_SUCCESS);
}

void * suma_vector(void * parametro)
{
	//Función que suma parte de los elementos de un vector.
	//Declaración de variables.
	int *suma;			//suma de los elementos de vector.
	struct info_reparto *reparto;	//reparto de la suma que ha recibido esta hebra.
	int i;

	//Reservamos memoria para devolver la suma de los elementos del vector.
	suma=(int *) malloc(sizeof(int));
	if(suma==NULL)
	{

		printf("\nError!! No se pudo hacer la reserva de memoria solicitada.");
		printf("\nSe cancela la ejecución del programa.");
		exit(-1);
	}

	//Conversión de void * a struct info_reparto *
	reparto=(struct info_reparto *)parametro;
	*suma=0;
	for(i=reparto->desde;i<reparto->hasta;i++)
		*suma=*suma+(reparto->vector[i]);

	//Finalización correcta y devolvemos la direccion de memoria de suma.
	//Al estar reservada con malloc() permanece en el Heap del proceso
	pthread_exit((void *)suma);
}

