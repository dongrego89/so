#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>

int main(int argc, char ** argv){

pid_t pid;
int i,flag,status;

system("clear");

for(i=0;i<2;i++){

pid=fork();

switch(pid){
	case -1:
		printf("\nError en el fork");
		exit(EXIT_FAILURE);
	break;

	case 0://hijo
		if(i==0){
			execlp(argv[1],argv[1],NULL);
			exit(EXIT_SUCCESS);		
		}	
		else{
			execvp(argv[2],&argv[2]);
			exit(EXIT_SUCCESS);		
		}
	break;
}

}

while((flag=wait(&status))>0){
	if(WIFEXITED(status)){
		printf("Hijo terminado con estado %d",WEXITSTATUS(status));
	}
	if(WIFSIGNALED(status)){
		printf("Hijo terminado debido a la señal %d",WTERMSIG(status));
}

}

exit(EXIT_SUCCESS);
}
