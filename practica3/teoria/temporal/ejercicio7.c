//Sistemas Operativos
//Práctica 2. Ejercicio 7.
//Programa que crea N hebras, cada hebra recibe un esctructura que contiene un entero y una cadena.
//Cada hebra debe incrementar el entero en 1 y sustituir el primer carácter de la cadena por el número 9.
//El proceso principal imprimirá el array de estructuras enviada a cada hebra.

#include <stdio.h>
#include <pthread.h>
#include <errno.h>	//Códigos de error
#include <string.h>	//Cadenas
#include <stdlib.h>

//Prototipos
void * modifica_estructura(void * parametro);

//Estructura con los datos que recibe cada hebra.
struct informacion
{
	int entero;		//Valor entero
	char cadena[50];	//Cadena
};


int main(int argc, char **argv)
{
	//Declaración de variables.
	pthread_t *hebras_id;		//Para almacenar el identificador de cada hebra.
	struct informacion *datos_hebras;	//Para almacenar las estructuras enviadas a las hebras.
	int n_hebras;				//Número de hebras a crear
	int i;
	int error;

	printf("\n\n");
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
   hebras_id=(pthread_t *)malloc(sizeof(pthread_t)*n_hebras);
	datos_hebras=(struct informacion *)malloc(sizeof(struct informacion)*n_hebras);

	if(n_hebras<1)
	{
		printf("Número de hebras incorrecto.\n");
		printf("Sintaxis correcta: %s <número de hebras mayor que 1>\n", argv[0]);
		printf("Se para la ejecución del programa.\n");
		exit(-1);
	}

	//Rellenamos los datos de la esctructura de cada hebra.
	for(i=0;i<n_hebras;i++)
	{
		datos_hebras[i].entero=i+1;
 		strcpy(datos_hebras[i].cadena, "Cadena de la hebra\0");
	}

	//Creamos las hebras.
	for(i=0;i<n_hebras;i++)
	{
		error = pthread_create( &(hebras_id[i]), NULL, (void *) modifica_estructura, (void *) &datos_hebras[i]);
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

	//Imprimimos los datos ya modificados de la esctructura de cada hebra.
	for(i=0;i<n_hebras;i++)
		printf("\n          La hebra %d tiene de valor entero: %d y cadena: %s\n",i+1,datos_hebras[i].entero, datos_hebras[i].cadena);
	printf("    (Hilo ppal.) Finalización correcta.\n");
	printf("\n\n");
	//Finalización correcta
	exit(EXIT_SUCCESS);
}


void * modifica_estructura(void * parametro)
{
	//Función que modifica los valores de la estructura que recibe.
	//Declaración de variables.

	struct informacion *datos;		//Puntero a la estructura con los datos.

	//Conversión de void * a struct informacion *
	datos=(struct informacion *)parametro;

	//Hacemos los cambios.
	datos->entero++;
	datos->cadena[0]='9';

	//Estos cambios se verán en el proceso principal porque le hemos pasado la dirección de memoria de la estructura.
	//Finalización correcta y no se devuelve nada.
	pthread_exit(NULL);
}

