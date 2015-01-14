#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc, char ** argv){

pid_t pid;
int i, status, flag;

system("clear");

if(argc!=2){
	printf("\nError, el programa se ejecuta así programa [nombre de fichero]");
	exit(EXIT_FAILURE);
}


FILE * archivo;
archivo=fopen(argv[1],"w");

if(archivo==NULL){
	printf("\nError abriendo el fichero");
	exit(EXIT_FAILURE);
}

char cadena_hijo[]="[H]------------------------------------\n";

char cadena_padre[]="[P]++++++++++++++++++++++++++++++++++++\n";


for(i=0;i<3;i++){

pid=fork();

switch(pid){

	case -1:
		printf("\nError en el fork");
		exit(EXIT_FAILURE);
	break;
	case 0:
		fputs(cadena_hijo,archivo);
		exit(EXIT_SUCCESS);
	break;
	default:
		fputs(cadena_padre,archivo);
	break;

}


}

fclose(archivo);

while((flag=wait(&status))>0){
	if(WIFEXITED(status)){
		printf("Hijo ha salido con el estado %d\n",WEXITSTATUS(status));
	}
	if(WIFSIGNALED(status)){
		printf("Hijo ha sido detenido por la señal %d\n",WTERMSIG(status));
	}
}
if(flag==-1 && errno==ECHILD){
	printf("\nNo quedan hijos que esperar\n");
}
else{
	perror("Error en la espera de hijos en wait");
	exit(EXIT_FAILURE);
}
exit(EXIT_SUCCESS);
}
