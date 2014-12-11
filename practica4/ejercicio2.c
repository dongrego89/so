/************************************************************************

Recurso:

-Uso de seccion critica

Restricciones:

-No mas de 3 usuarios simultaneos y no mezclar hombres y mujeres

Solucion:

-Funcion para hombres que bloquee a la de mujeres una vez su contador 
sea mayor de uno y que bloquee su propia entrada una vez el valor de hilos 
en seccion critica llegue a 3

-Idem para mujeres.

*************************************************************************/

#include<pthread.h>
#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>

sem_t hombres;//Semaforo para acceso a hombres
sem_t mujeres;//Semaforo para acceso a mujeres
sem_t seccionCritica;//Semaforo para controlar la seccion critica

int usuarios;//Global cuenta usuarios dentro del servicio

void * usoHombres(void * n){

		sem_wait(&hombres);//Aqui se van esperando las nuevas peticiones, en la primera iteracion pasará

		sem_wait(&seccionCritica);//Bloqueamos la seccion critica

		usuarios+=1;//Incrementamos el numero de usuarios

		if(usuarios==1){
			sem_wait(&mujeres);//Una vez un hombre está en el servicio ya no podrán entrar mujeres
		}
		
		sem_post(&hombres);//Damos paso al siguiente hombre (si la seccion critica esta llena se quedará en cola)
		
		printf("\n\t[+] Hombre usando recurso");
			
		sleep(usuarios+rand()%2);	
	
		sem_post(&seccionCritica);//Desbloqueamos la seccion critica (para que pase otro hombre)

		sem_wait(&hombres);//Se hacen pasar uno por uno a los hombres para decrementar el valor de usuarios
	
		usuarios-=1;//Decrementa el valor de usuarios del recurso		

		printf("\n\t[-] Hombre dejando recurso");

		if(usuarios==0){//Si es el ultimo usuario que quedaba
			sem_post(&mujeres);//Prepara a la funcion de mujeres por si quieren entrar en la proxima iteracion
		}
		
		sem_post(&hombres);/*Abre el semaforo de los hombres por si quieren entrar (paso previo a seccion critica 
							y al decremento de usuarios*/

}


void * usoMujeres(void * n){

		sem_wait(&mujeres);//Aqui se van esperando las nuevas peticiones, en la primera iteracion pasará

		sem_wait(&seccionCritica);//Bloqueamos la seccion critica

		usuarios+=1;//Incrementamos el numero de usuarios

		if(usuarios==1){
			sem_wait(&hombres);//Una vez un hombre está en el servicio ya no podrán entrar mujeres
		}
		
		sem_post(&mujeres);//Damos paso al siguiente hombre (si la seccion critica esta llena se quedará en cola)
		
		printf("\n\t[+] Mujer usando recurso");
			
		sleep(usuarios+rand()%2);	
	
		sem_post(&seccionCritica);//Desbloqueamos la seccion critica (para que pase otro hombre)

		sem_wait(&mujeres);//Se hacen pasar uno por uno a los hombres para decrementar el valor de usuarios
	
		usuarios-=1;//Decrementa el valor de usuarios del recurso	
		printf("\n\t[-] Mujer dejando recurso");	

		if(usuarios==0){//Si es el ultimo usuario que quedaba
			sem_post(&hombres);//Prepara a la funcion de mujeres por si quieren entrar en la proxima iteracion
		}
		
		sem_post(&mujeres);/*Abre el semaforo de los hombres por si quieren entrar (paso previo a seccion critica 
							y al decremento de usuarios*/

}


int main(){
	srand(time(NULL));//Semilla random
	sem_init(&hombres,0,1);//Inicializacion de semaforos generales
	sem_init(&mujeres,0,1);
	sem_init(&seccionCritica,0,3);

	usuarios=0;
	
	int i;	

pthread_t * hilos;

hilos=(pthread_t *)malloc(sizeof(pthread_t)*10);//10 personas formarán la cola del servicio

//Creacion de hilos

for(i=0;i<=5;i++){//6 Hombres
pthread_create(&hilos[i],NULL,(void *)usoHombres,NULL);
}

for(i=6;i<=10;i++){//4 mujeres
pthread_create(&hilos[i],NULL,(void*)usoMujeres,NULL);
}

//Recogida de hilos

for(i=0;i<10;i++){
pthread_join(hilos[i],NULL);
}

printf("\n\n");

return 0;
}

