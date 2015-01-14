#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>

int main(int argc,char ** argv){

pid_t pid;

int flag,status;

system("clear");

if(argc!=3){
	printf("\nDebe pasar como argumento ejecutable [numero de segundos] [valor de estado]");
	exit(EXIT_FAILURE);
}

pid=fork();

switch(pid){
	case -1:
		printf("\nError en el fork");
		exit(EXIT_FAILURE);
	break;
	case 0:
		printf("[H]Hijo creado, dormirá %d segundos y devolverá %d como estado\n",atoi(argv[1]),atoi(argv[2]));
		sleep(atoi(argv[1]));
		exit(atoi(argv[2]));
	break;
	default:
		printf("[P]Soy el padre, y he creado a mi hijo de PID %d\n",pid);
	break;
}

while((flag=wait(&status))>0){

	if(WIFEXITED(status)){
		printf("Hijo ha terminado con estado %d\n",WEXITSTATUS(status));
	}
	if(WIFSIGNALED(status)){
		printf("Hijo ha terminado debido a la señal %d\n",WTERMSIG(status));
	}
}

exit(EXIT_SUCCESS);
}
