//Sistemas Operativos
//Práctica 2. Ejercicio 1
//Programa que crea dos hebras para imprimir dos cadenas, cada hebra imprime una cadena.
// --> cadena1 'Hola'
// --> cadena2 'mundo'
// --> O bien las dos cadenas que se reciban por parámetro.


#include <stdio.h>
#include <pthread.h>
#include <errno.h>	//Códigos de error
#include <string.h>	//Cadenas
#include <stdlib.h>
#define TAM 50


void * imprime_cadena(void * parametro)
{
	//Función que imprime la cadena que recibe por parámetro.
	//Declaración de variables.
	char *cadena;
	int i;

	//Conversión de void * a char *
	cadena = (char *)parametro;

	for(i=0;i<strlen(cadena);i++)
	{
		printf("%c\n", *(cadena+i));
		sleep(1);
	}

	//Finalización correcta y no devuelve nada.
	pthread_exit(NULL);
}


int main(int argc, char **argv)
{
	//Declaración de variables. 	
	int i;
	int error;
   char cadena0[TAM] = "Hola ";
	char cadena1[TAM] = "mundo";
	pthread_t hebras[2];

	//Controlamos los parámetros.
	if(argc==2 || argc>3)
	{
		printf("Parámetros incorrectos.\n");
		printf("Sintaxis correcta: %s <cadena1> <cadena2>\n", argv[0]);
		printf("Se para la ejecución del programa.\n");
		exit(-1);
	}

	if(argc==3)
	{
		//Se reciben las dos cadenas por parámetro.
		strcpy(cadenas0,argv[1]);
		strcpy(cadenas1,argv[2]);
	}
	
	//Creamos las hebras.
	for(i=0;i<2;i++)
	{
		error = pthread_create( &(hebras[i]), NULL, (void *) imprime_cadena, (void *) cadenas[i]);

		//Capturamos los errores que se pueden producir al crear una hebra. Vienen definidos en OpenGroup
		switch (error)
		{
			case 0: //Correcto.
				printf("    (Hilo ppal.) Se ha creado la hebra ..: %lu\n", (unsigned long int)hebras[i]);
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
	for(i=0;i<2;i++)
	{
		//error = pthread_join(pthread_self(),NULL); //No se puede hacer un join de sí mismo.
		error = pthread_join(hebras[i], NULL);

		//Capturamos los errores que se pueden producir al recoger (join) una hebra.
		switch (error)
		{
			case 0: //Correcto.
				printf("    (Hilo ppal.) Terminó correctamente la hebra ..: %lu\n", (unsigned long int)hebras[i]);
				break;
			case EDEADLK: //Se quiso hacer un join a sí misma.
				printf("    (Hilo ppal.) Se quiso hacer un join a sí mismo.\n");
				break;
			case EINVAL: //La hebra ha sido detached u otra hebra está esperando a que termine.
				printf("    (Hilo ppal.) Se produjo el error EINVAL (la hebra ha sido detached u otra hebra está esperando a que termine.).\n");
				break;
			case ESRCH:  //Se ha hecho un pthread_join sobre una hebra que no existe.
				printf("    (Hilo ppal.) Se produjo el error ESRCH (la hebra no existe).\n");
				break;
		}
	}
	printf("    (Hilo ppal.) Finalización correcta.\n");
	printf("\n");

	//Finalización correcta 
	exit(EXIT_SUCCESS);
}


