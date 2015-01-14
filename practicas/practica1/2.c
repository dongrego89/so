#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc, char ** argv){

pid_t pid;
int i,status,flag;

system("clear");

if(argc!=2){
	printf("\nSe necesita pasar el numero de hijos como argumento");
	exit(EXIT_FAILURE);
}

printf("\nCrearemos %d hijos en paralelo desde nuestro proceso cuya PID es %d\n\n\n",atoi(argv[1]),getpid());

for(i=0;i<atoi(argv[1]);i++){
	pid=fork();
	switch(pid){
		case -1:
			perror("Error en el fork");
			exit(EXIT_FAILURE);
		break;
		case 0://Ejecución por el hijo
			printf("[H]Soy el hijo numero %d con PID %d | Mi padre tiene PID %d \n",i+1,getpid(),getppid());
			sleep((i+1)*10);//En el enunciado dice que sea n+10, pero si me espero tanto acabo muñeco
			exit(0);
		break;
		default://Ejecución por el padre
			printf("[P]Soy el padre, acabo de crear al proceso numero %d con PID %d\n",i+1,pid);
		break;
	}
}

while((flag=wait(&status))>0){
	if(WIFEXITED(status)){
		printf("Hijo terminado con estado %d\n",WEXITSTATUS(status));
	}
	if(WIFSIGNALED(status)){
		printf("Hijo terminado debido a la señal %d\n",WTERMSIG(status));
	}
}

if(flag==-1 && errno==ECHILD){
	printf("\nNo quedan hijos que esperar");
}
else{
	perror("Error en el wait");
}

exit(EXIT_SUCCESS);
}
