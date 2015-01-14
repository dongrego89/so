#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>

int main(int argc, char ** argv){

pid_t pid;
int i,status,flag;

if(argc!=3){
	printf("\nSe deben de pasar 2 numeros como argumento");
	exit(EXIT_FAILURE);
}

for(i=0;i<2;i++){

	pid=fork();

	switch(pid){
		case -1:
			perror("Error en el fork");
			exit(EXIT_FAILURE);
		break;
		case 0://Ejecucion en el hijo
			printf("[H]Hijo %d con PID %d ejecutando su correspondencia de factorial\n",i+1,getpid());
			execlp("./factorial","./factorialeches",argv[i+1],0,NULL);
			exit(EXIT_SUCCESS);
		break;

		default://Ejecucion en el padre
			printf("[P]Padre ha creado un hijo con PID %d\n",pid);
		break;	
	}

}

while((flag=wait(&status))>0){

	if(WIFEXITED(status)){
		printf("Hijo ha termindo con estado %d\n",WEXITSTATUS(status));
	}
	if(WIFSIGNALED(status)){
		printf("Hijo ha terminado por envio de señal %d\n",WTERMSIG(status));
	}
}
if(flag==-1 && errno==ECHILD){
	printf("No quedan hijos que esperar\n");
}
else{
	perror("Error esperando hijos en el wait");
	exit(EXIT_FAILURE);
}

exit(EXIT_SUCCESS);
}
