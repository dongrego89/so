#include<stdio.h>
#include<stdlib.h>//exit, atoi
#include<errno.h>//errno ECHILD
#include<unistd.h>//fork
#include<sys/types.h>//wait

int main(int argc, char ** argv){

pid_t pid;
int i,flag,status;

system("clear");

if(argc!=2){
	printf("\nError. Pase como parámetro el número de hijos");
	exit(EXIT_FAILURE);
}
printf("\nNos proponemos a crear %d hijos en paralelo:\n",atoi(argv[1]));

for(i=0;i<atoi(argv[1]);i++){
	pid=fork();

	switch(pid){
		case -1:
			printf("\nFallo en el fork");
			exit(EXIT_FAILURE);
		break;

		case 0://Ejecución por parte del hijo
			printf("\n\t[H]Hijo creado al habla, mi PID es %d y la de mi padre es %d",getpid(),getppid());
			sleep(1);
			exit(EXIT_SUCCESS);//El hijo es creado y muere
		break;

		default://Ejecución por parte del padre, (cuando pid valga la pid del hijo)
			printf("\n[P]Proceso padre al habla, cuyo PID es %d, creador del hijo con PID %d",getpid(),pid);
		break;
	}
}
printf("\nEsperas de hijos:\n\n");
while((flag=wait(&status))>0){
	if(WIFEXITED(status)){//Si el hijo termina con normalidad
		printf("\n[F]Hijo ha terminado cuyo estado=%d",WEXITSTATUS(status));
	}
	if(WIFSIGNALED(status)){//Si el hijo a terminado por una señal
		printf("\n[F]Hijo ha terminado debido a la señal=%d",WTERMSIG(status));
	}
}

if(flag==-1 && errno==ECHILD){
	printf("\n\nNo hay más hijos que esperar\n\n");
}
else{
	perror("Error en llamada a wait");
	exit(EXIT_FAILURE);
}
exit(EXIT_SUCCESS);

}
