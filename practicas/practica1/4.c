#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<unistd.h>

int main(int arg, char ** argv){

pid_t pid;

pid=fork();

switch(pid){
	case -1:
		printf("\nError en el fork");
		exit(EXIT_FAILURE);
	break;
	case 0:
		printf("Soy el hijo y mi PID es %d\n",getpid());
		exit(EXIT_SUCCESS);
	break;
	default:
		printf("Soy el padre con PID %d y mi hijo tiene la PID %d",getpid(),pid);
	break;
}

sleep(20);


exit(EXIT_SUCCESS);
}
