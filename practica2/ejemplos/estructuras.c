#include<stdio.h>
#include<stdlib.h>

typedef struct{
	int n1;
	int n2;
}numeros;

int main(){

int cantidad=4,i;
numeros auxiliar,* vector;

vector=malloc(sizeof(numeros)*cantidad);

for(i=0;i<cantidad;i++){
	printf("\n-Introduce un valor n1 para el %d elemento:",i+1);	
	scanf("%d",&auxiliar.n1);
	printf("\n-Introduce un valor n2 para el %d elemento:",i+1);	
	scanf("%d",&auxiliar.n2);

	vector[i]=auxiliar;
}

printf("\n\n");

for(i=0;i<cantidad;i++){
	printf("\n-Elemento %d: n1=%d y n2=%d",i+1,vector[i].n1,vector[i].n2);
}
return 0;
}
