#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void * convertir(void * fichero)
{
	int error=0;
	int i;
	char *entrada = (char*) malloc(sizeof(char));
	char *salida = (char*) malloc(sizeof(char));
	char * commando=  (char*) malloc(50*sizeof(char));

	entrada=(char*)(fichero);
	//cambiamos el nombre para la salida (le quitamos .mp4)

	for (i=0;i<strlen(entrada)-4;i++)
		salida[i]=entrada[i];
	salida[i]='\0';

	//Concateno para dar formato mp3
	strcat(salida,".mp3");

	//** ejecutamos el programa **
	//opcion SYSTEM()

	sprintf(commando, "ffmpeg -i %s -f mp3 -ab 192000 -ar 48000 -vn %s",entrada,salida);
	system(commando);
	pthread_exit(NULL);		
}
