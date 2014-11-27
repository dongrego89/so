//Sistemas Operativos
//Práctica 2. Ejercicio 5
//Programa que crea N hebras para contar las líneas que contienen los N ficheros de texto que se reciben por parámetro. Cada hebra retornará su resultado.
//En la hebra principal se acumulará la suma de todas las hebras.

#include <stdio.h>
#include <pthread.h>
#include <errno.h>	//Códigos de error
#include <string.h>	//Cadenas
#include <stdlib.h>


//Prototipo
void * cuenta_lineas(void *);

int main(int argc, char **argv)
{
	//Declaración de variables.
	pthread_t *hebras_id;		//Para almacenar el identificador de cada hebra.
	int n_hebras;				//Número de hebras a crear
	int *puntero_a_lineas;	//Para obtener el recuento de líneas de cada hebra.
	int suma=0;				  //Suma total.
	int i;
	int error;

	//Controlamos los parámetros.
	if(argc<2)
	{
		printf("Parámetros incorrectos.\n");
		printf("Sintaxis correcta: %s <fichero1> <fichero2> ....  <fichero n>\n", argv[0]);
		printf("Se para la ejecución del programa.\n");
		exit(-1);

	}

	n_hebras=argc-1;
   hebras_id=(pthread_t *)malloc(sizeof(pthread_t)*n_hebras);
	//Creamos las hebras.
	for(i=0;i<n_hebras;i++)
	{
		error = pthread_create( &(hebras_id[i]), NULL, (void *) cuenta_lineas, (void *) argv[i+1]);

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
		error = pthread_join(hebras_id[i], (void **) &puntero_a_lineas);
		//Capturamos los errores que se pueden producir al recoger (join) una hebra.
		switch (error)
		{
			case 0: //Correcto.
				printf("        (Hilo ppal.) Terminó correctamente la hebra ..: %lu\n", (unsigned long int)hebras_id[i]);
				printf("        (Hilo ppal.) Y devolvió el valor ..: %d\n\n", *puntero_a_lineas);
				suma=suma+*puntero_a_lineas;
				//liberamos la memoria que reservó la hebra.
				free(puntero_a_lineas);
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

	printf("    (Hilo ppal.) El total de líneas es de ..: %d\n", suma);
	printf("    (Hilo ppal.) Finalización correcta.\n");
	printf("\n\n");

	//Finalización correcta
	exit(EXIT_SUCCESS);
}

void * cuenta_lineas(void * parametro)
{
	//Función que suma las líneas que contiene el fichero de texto que recibe por parámetro.
	//Declaración de variables.
	FILE *fichero;
	int *lineas;								//número de líneas del fichero.
	char nombre_fichero[FILENAME_MAX];	//nombre del fichero a procesar. FILENAME_MAX -> Longitud maxima posible para una cadena
	char linea[FILENAME_MAX]; 

	//Reservamos memoria para devolver el número de líneas del fichero.
	lineas=(int *) malloc(sizeof(int));

	if(lineas==NULL)
	{
		printf("\nError!! No se pudo hacer la reserva de memoria solicitada.");
		printf("\nSe cancela la ejecución del programa.");
		exit(-1);
	}
	*lineas=0;

	//Conversión de void * a char *
	strcpy(nombre_fichero, (char *)parametro);

	fichero=fopen(nombre_fichero, "r");
	if(fichero==NULL)
	{
		printf("Error al abrir el fichero...\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		//Procesamos el fichero.
		fgets(linea,FILENAME_MAX,fichero);
		while(!feof(fichero))
		{
			//printf("         ( %s ) --> %s\n", nombre_fichero, linea); //Para ver el contenido de cada fichero.
			*lineas=*lineas+1;
			fgets(linea,FILENAME_MAX,fichero);
		}
		//Cerramos el fichero.
		fclose(fichero);
		printf("\n      El fichero %s contiene %d líneas.\n", nombre_fichero, *lineas);
	}	
	//Finalización correcta y devolvemos el número de líneas.
	pthread_exit((void *)lineas);
}

