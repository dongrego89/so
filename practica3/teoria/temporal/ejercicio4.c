/*Obtenga los dos ficheros .mp4 que se facilitan en la plataforma Moodle u obtenga usted dos vídeos cualesquiera que no sean demasiado largos de la Web. Cree un programa que de forma paralela convierta dichos ficheros a .mp3 (extracción de audio) con el programa “ffmpeg” (disponible en Linux). Pida el nombre de los dos ficheros por la linea de argumentos. Use la llamada al sistema system() para invocar a “ffmpeg”.
Para extraer el audio en formato mp3 de un fichero de video mp4, puedo utilizar la siguiente linea de comandos (si tienen curiosidad por el significado de los argumentos busque en la Web):
ffmpeg -i ficheroOriginal.mp4 -f mp3 -ab 192000 -ar 48000 -vn ficheroNuevoMP3.mp3 Pruebe a realizar el ejercicio usando alguna de las funciones exec() para procesos que se explicaron en la Práctica 1, en vez de usar la funció n system().*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "ejercicio4.h"

int main (int argc , char **argv)
{
	int numHilos=2;
	int i;

	//Comprobacion de linea de argumentos
	if(argc != 3){
		printf("Error de sintaxis: %s <fichero.mp4> <fichero.mp4>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	//Declaracion de hilos	
	pthread_attr_t attr;
	pthread_t hilo[numHilos];

	//Creamos los atributos. Vamos a hacer una hebra detached dede su inicio		
	if( pthread_attr_init(&attr) != 0 )
	{
		perror("Attribute init failed");
		exit(EXIT_FAILURE); 
	}
	
	if( pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
	{ 
		perror("Setting detached state failed");
		exit(EXIT_FAILURE); 
	}
		
	//Creamos los hilos	
	for (i=0;i<numHilos;i++)
	{		
		printf("\nHilos creandose con pthread_create()...\n");
		if (pthread_create (&hilo[i], &attr , (void *) convertir , (void*) argv[i+1] ) != 0) 
		{
			perror("Fallo en el create()...\n");
			printf("errno value= %d\n", errno); 
			exit(EXIT_FAILURE);
		}		
	}
	
	pthread_attr_destroy(&attr); //destruimos el atributo	
	printf("Conversión de video a audio finalizada\n");
	//Necesario cuando se crean jebras detached!!!
	pthread_exit(NULL);
}
