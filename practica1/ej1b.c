#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h> // para macros de EXIT
#include <unistd.h>
#include <errno.h>

int main()
{
int n,i,pid_t,estado,flag;

system("clear");
printf("Introduce el numero de procesos:");
scanf("%d",&n);

printf("\nCASCADA DE PROCESOS | Proceso padre, PID %d | PPID %d\n",getpid(),getppid());

/*la solucion es que en cada nivel de fork() se iguale la i=n para que no se ejecute nada más que una vez
esto combinado con el ejercicio 1 es practicamente la solucion*/


for(i=0;i<n;i++){
	pid_t=fork();
	if(pid_t==-1)
	{
		printf("\nFallo al iniciar fork\n");
		exit(EXIT_FAILURE);//Salida con fallo
	}
	else if(pid_t==0)
	{//Si estamos en un hijo
		printf("\nHijo nº %d | PID: %d | PPID: %d\n",i+1,getpid(),getppid());//Muestra pid y ppid
		i=n;
	}	
}

while((flag=wait(&estado))>0)
{
	if(WIFEXITED(estado))printf("\n\tHijo exited. Estado %d\n",WEXITSTATUS(estado));//Si el hijo acaba bien muestralo
	else if(WIFSIGNALED(estado))printf("\n\tHijo killed. Senal %d\n",WTERMSIG(estado));//Si el hijo ha sido matado muestralo
	else if(WIFSTOPPED(estado))printf("\n\tHijo stopped. Senal %d\n",WSTOPSIG(estado));//Si el hijo ha sido parado muestralo	
}
if((errno==ECHILD) && (wait(estado)==-1))
{//si hubo hijos muertos o ya no se deben esperar && lo mismo ejecutando wait
	printf("\n\t\tFin de espera de hijos\n\n\n");
	exit(EXIT_SUCCESS);
}
else
{
	printf("\n\t\tError en la invocacion a wait\n");
	exit(EXIT_FAILURE);
}

return 0;
}
