#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//Declaración de una estructura
struct param
{
char * frase;
int numero;
};

void imprimeMensaje(struct param * mensaje){
	struct param * auxiliar=(struct param *)mensaje;
	//system("clear");
	sleep(1);
	printf("%s es mi palabra\n",auxiliar->frase);
	sleep(1);
	printf("y %d es mi matricula\n",auxiliar->numero);
	sleep(1);
}

/* Función que se asignará a los hilos que se creen. Recibe un puntero a estructura */
void hiloMensaje (struct param * mensa)
{
//Casting siempre necesario, ya que realmente lo que recibe es un puntero a void<-----
struct param * aux = (struct param *) mensa;
sleep(1);
printf("%s %d\n", mensa->frase, mensa->numero);
	sleep(1);
}

int main()
{
pthread_t thd1, thd2, thd3; //Declaración de dos hebras, hilos o procesos ligeros. NO CREACION
//Inicializacion de 2 estructuras de tipo “struct param”
struct param param1 = {"Soy el hilo: ", 1};
struct param param2 = {"Digo otra cosa ", 2};
struct param param3 = {"Aqui vamos de nuevo ",66};

/*Creamos dos hilos. La función la pasaremos como (void *) nombreFuncion. Es decir, hacemos un casting a (void *),
aunque por defecto no es necesario, ya que el nombre de una función es su dirección de memoria. También es
importante realizar esto con la dirección de memoria de la variable que contiene los parámetros que se le pasan a la
función */

pthread_create (&thd1, NULL, (void *) hiloMensaje, (void *) &param1);//<-----

//pasamos como parametros:

//direccion de hebra - atributos (por defecto null) - funcion a ejecutar casteada como void - estructura de parametros casteada como void

pthread_create (&thd2, NULL, (void *) hiloMensaje, (void *) &param2);

pthread_create (&thd3,NULL,(void *) imprimeMensaje,(void *) &param3);
/*Esperamos la finalización de los hilos. Si la función devolviera algo habría que recogerlo con el segundo argumento,
que en este caso esta a NULL. Cuando el segundo argumento no es NULL, se recogen los resultados que vienen de
pthread_exit(), que se explicará a continuación.*/
/*Si no se ponen estos join() en el programa principal y simplemente lanzamos los dos hilos y finalizamos, lo más
probable es que los hilos no lleguen a ejecutarse completamente o incluso que no lleguen ni a terminar de arrancar

antes de que el programa principal termine.*/
pthread_join(thd1, NULL);
pthread_join(thd2, NULL);
pthread_join(thd3, NULL);
printf("Han finalizado los thread.\n");
exit(EXIT_SUCCESS);
}
