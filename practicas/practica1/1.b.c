#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>

int main(int argc, char ** argv){

pid_t pid;
int status,i,flag;

system("clear");

if(argc!=2){
	printf("\nError, debes de pasar el numero de hijos como parametro");
	exit(EXIT_FAILURE);
}

printf("\nVamos a crear %d hijos en cascada a partir del proceso: %d\n\n",atoi(argv[1]),getpid());

for(i=0;i<atoi(argv[1]);i++){

	pid=fork();

	switch(pid){
		case -1://Error en fork
			{
				perror("Error invocando al fork");
				exit(EXIT_FAILURE);
				break;
			}
		

		case 0://Ejecución por parte de hijo
			{	
				printf("[H]Proceso hijo %d al habla con PID %d y cuyo padre es PPID %d\n",i+1,getpid(),getppid());			
				break;
			}

		default://Ejecución por parte del padre
			{
				while((flag=wait(&status))>0){//Recogida de hijos
					if(WIFEXITED(status)){
						printf("[P]Proceso padre al habla, mi hijo terminó con estado %d\n",WEXITSTATUS(status));			
					}
					if(WIFSIGNALED(status)){
						printf("[P]Proceso padre al habla, mi hijo terminó debido a señal %d\n",WTERMSIG(status));
					}
				}
				if(flag==-1 && errno==ECHILD){
					//printf("\n[P]Proceso padre al habla, no mas hay hijos que esperar");
				}
				else{
					perror("Error en el wait");
					exit(EXIT_FAILURE);
				}
				exit(EXIT_SUCCESS);//El padre una vez se ha replicado ya está listo para morir
			}
	
	}
}
printf("\n\n");
exit(EXIT_SUCCESS);
}
