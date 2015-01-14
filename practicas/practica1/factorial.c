#include<stdio.h>
#include<stdlib.h>

int main(int argc, char ** argv){


int factorial, numero;

if(argc!=2){
	printf("Se debe pasar el numero al que calcular el factorial");
	return -1;
}

factorial=1;

for(numero=atoi(argv[1]);numero>1;numero--){
	factorial=factorial*numero;
	printf("\nFactorial parcial: %d",factorial);
	sleep(1);
}

printf("\n\nEl factorial de %d es %d\n",atoi(argv[1]),factorial);

return 0;
}
