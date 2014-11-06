#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>

typedef struct{
	int ca;
	int cb;
	int * vector;
}pares;


void * sumaParcial(void * parejas){
	printf("aaaaaaa\n");	
	pares * aux=(pares *)parejas;
	int i,*suma;
	printf("bbbbbb\n");	
	suma=(int*)malloc(sizeof(int));
	*suma=0;

	for(i=aux->ca;i<=aux->cb;i++){
		*suma+=aux->vector[i];
	}
	printf("\nCorte A: %d y Corte B: %d",aux->ca,aux->cb);
	printf("\n \tSuma parcial vale %d",*suma);
	pthread_exit((void *)suma);
}



int main(int argc, char* argv[]){//Funcion Principal
int i,j,corteA=0,corteB=0,resto,sumaTotal=0,nHilos,nElementos;
//int * vector;
void *sumaParcial;
pares * estructura;
pares estructura_aux;


if(argc!=3){//Comprobación del número de hilos como parametro
	printf("\nSe debe pasar un numero de hilos como parametro\n");
	return -1;
}
else{	

	nHilos=atoi(argv[1]);
	//pthread_t * vectorHilos = (pthread_t *) malloc(sizeof(pthread_t)*nHilos); 
	nElementos=atoi(argv[2]);
	//vector=(int*)malloc(sizeof(int)*nElementos);
	pthread_t vectorHilos[nHilos];
	int vector[nElementos];

	estructura=(pares * )malloc(sizeof(pares)*nHilos);

	printf("Numero de hilos introducido:%d\n",nHilos);
	printf("Numero de elementos del vector:%d\n",nElementos);

	if((nHilos>10) || (nHilos<=0)){//Permitiremos solo entre 1-10 hilos
		printf("\nEl numero de hilos no debe superar a 10 ni ser inferior a 1\n");
		return -1;
	}
	
	srand(time(NULL));//Inicialización de semilla para aleatorios
	printf("\nNuestro vector aleatorio es: ");

	for(j=0;j<nElementos;j++){//Vector de aleatorios del 0 al 9
		vector[j]=rand()%10;
		printf("[%d]",vector[j]);
	}
	
	resto=nElementos%nHilos;
	for(j=0;j<nHilos;j++) //El bucle recorrera desde el hilo 1 hasta el hilo nHilos
 	{
		if(j==0){
			corteA=0;
		}
		
		else{
			corteA=corteB+1;
		}
		
		corteB=corteA+(nElementos/nHilos)-1;

		if(resto>0){
			resto--;
			corteB++;
		}
		
		estructura_aux.ca=corteA;
		estructura_aux.cb=corteB;
		estructura_aux.vector=vector;
		printf("\n -Add elemento a estructura. Iteracion: %d",j);
		estructura[j]=estructura_aux;	
	}

	for(i=0;i<nHilos;i++){
		printf("\nSoltamos hebra %d",i+1);
		pthread_create(&vectorHilos[i],NULL,(void *)sumaParcial,(void *)&estructura[i]);//Lanzamos los hilos
		//printf("\n Corte en A: %d, Corte en B: %d, vector: %d",estructura[i].ca,estructura[i].cb,estructura[i].vector[i]);
	}
		
	for(j=0;j<nHilos;j++){
		pthread_join(vectorHilos[j],(void **)&sumaParcial);
		//Como la funcion devuelve un puntero hemos de acceder por indirección al valor de suma
		sumaTotal+=*(int*)sumaParcial;
	}

	printf("\nLa suma total es: %d",sumaTotal);
	return 0;
}//else
}//main
