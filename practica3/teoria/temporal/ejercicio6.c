//Sistemas Operativos
//Práctica 2. Ejercicio 6.
//Programa que crea dos hebras, cada hebra recibe un esctructura que contiene un escalar y una matriz 3x3.
//Cada hebra multiplicará el escalar por la matriz. El proceso principal imprimirá el resultado de ambas multiplicaciones.

#include <stdio.h>
#include <pthread.h>
#include <errno.h>	//Códigos de error
#include <string.h>	//Cadenas
#include <stdlib.h>
#include <time.h>	//rand

//Prototipos
void * multiplica_matriz(void * parametro);

//Estructura con los datos que recibe cada hebra.
struct informacion
{
	float escalar;		//Escalar.
	float matriz[3][3];	//Matriz de valores.
};

int main()
{
	//Declaración de variables.
	time_t t;
	pthread_t hebras_id[2];			//Para almacenar el identificador de cada hebra.
	struct informacion datos_hebras[2];	//Para almacenar las estructuras enviadas a las hebras.
	int n_hebras=2;				//Número de hebras a crear
	int i;
	int j;	//Filas
	int k;	//Columnas
	int error;

	//Generamos la semilla.
	srand ((int) time(&t));
	//Escalar para la hebra cero será 2 y para la hebra 1 será un 3. Tambien se podria pedir por pantalla
	datos_hebras[0].escalar=2.0;
	datos_hebras[1].escalar=3.0;

	//Rellenamos los valores de las matrices de las hebras de forma aleatoria.
	for(i=0;i<n_hebras;i++)
	{
		printf("\n        Matriz hebra ...: %d\n", i+1);
		for(j=0;j<3;j++)
		{
			for(k=0;k<3;k++)
			{
				datos_hebras[i].matriz[j][k]=(rand()%4)+1;
				printf("        %5.2f ",datos_hebras[i].matriz[j][k]);
			}
			printf("\n");
		}
	}

	//Creamos las hebras.
	for(i=0;i<n_hebras;i++)
	{
		error = pthread_create( &(hebras_id[i]), NULL, (void *) multiplica_matriz, (void *) &datos_hebras[i]);
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
		error = pthread_join(hebras_id[i], NULL);
		//Capturamos los errores que se pueden producir al recoger (join) una hebra.
		switch (error)	
		{
			case 0: //Correcto.
				printf("        (Hilo ppal.) Terminó correctamente la hebra ..: %lu\n", (unsigned long int)hebras_id[i]);
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

	//Imprimimos la matriz de cada hebra ya multiplicada por el escalar.
	for(i=0;i<n_hebras;i++)
	{
		printf("\n\n        Hebra %d:   Escalar %5.2f  Matriz:\n", i+1, datos_hebras[i].escalar);
		for(j=0;j<3;j++)
		{
			printf("\n");
			for(k=0;k<3;k++)			
				printf("        %5.2f ", datos_hebras[i].matriz[j][k]);
		}
		printf("\n");
	}
	printf("    (Hilo ppal.) Finalización correcta.\n");
	printf("\n\n");

	//Finalización correcta
	exit(EXIT_SUCCESS);
}

void * multiplica_matriz(void * parametro)
{
	//Función que multiplica un escalar por una matriz. Los datos se reciben en una estructura.
	//Declaración de variables.
	struct informacion *datos;		//Puntero a la estructura con los datos.
	int i;	//Filas
	int j;	//Columnas

	//Conversión de void * a struct informacion *
	datos=(struct informacion *)parametro;
	//Multiplicamos el escalar por la matriz.
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			datos->matriz[i][j]=datos->matriz[i][j] * datos->escalar;

	//Estos cambios se verán en el proceso principal porque le hemos pasado la dirección de memoria de la estructura.
	//Finalización correcta y no se devuelve nada.
	pthread_exit(NULL);
}
