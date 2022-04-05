#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>


#define LIMITE 50


//creo estructura de semaforos
sem_t semaforo_salero;
sem_t semaforo_sarten;
sem_t semaforo_horno;
sem_t ganador;
int equipo_ganador;

struct semaforos {
    sem_t sem_mezclar;
    sem_t sem_salero;
    sem_t sem_empanar;
    sem_t sem_sarten;
    sem_t sem_hornear;
    sem_t sem_agregar_verduras;
    sem_t sem_terminar;
};

//creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];

};

//creo los parametros de los hilos
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param[8];
};

//funcion para imprimir las acciones y los ingredientes de la accion
void* imprimirAccion(void *data, char *accionIn) {
        struct parametro *mydata = data;
        //calculo la longitud del array de pasos
        int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
        //indice para recorrer array de pasos
        int i;
        for(i = 0; i < sizeArray; i ++){
                //pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
                if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
                printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
                //calculo la longitud del array de ingredientes
                int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
                //indice para recorrer array de ingredientes
                int h;
                printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param);
                        for(h = 0; h < sizeArrayIngredientes; h++) {
                                //consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion
                                if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
                                                        printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
                                }
                        }
                }
        }
}

//funcion para tomar de ejemplo
void* cortar(void *data) {
        //creo el nombre de la accion de la funcion
        char *accion = "CORTAR";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero).
        struct parametro *mydata = data;
        //llamo a la funcion imprimir le paso el struct y la accion de la funcion
        imprimirAccion(mydata,accion);
        //uso sleep para simular que que pasa tiempo
        usleep( 20000 );
        //doy la señal a la siguiente accion (cortar me habilita mezclar)
        sem_post(&mydata->semaforos_param.sem_mezclar);

    pthread_exit(NULL);
}

void* salar(void *data){
        char *accion = "SALAR!";
        struct parametro *mydata = data;
        sem_wait(&mydata -> semaforos_param.sem_salero);
        sem_wait(&semaforo_salero);
        imprimirAccion(mydata,accion);
        usleep(100000);
        sem_post(&semaforo_salero);
        sem_post(&mydata->semaforos_param.sem_empanar);

        pthread_exit(NULL);
}

void* mezclar(void *data) {
        char *accion = "MEZCLAR";
        struct parametro *mydata = data;
        sem_wait(&mydata -> semaforos_param.sem_mezclar);
        imprimirAccion(mydata,accion);
        usleep(300000);

        sem_post(&mydata->semaforos_param.sem_salero);


        pthread_exit(NULL);
}
void* aLaSarten(void *data){
        char *accion = "AL FUEGO!";
        struct parametro *mydata = data;
        sem_wait(&mydata -> semaforos_param.sem_sarten);
        sem_wait(&semaforo_sarten);
        imprimirAccion(mydata,accion);
        usleep(1000000);
        sem_post(&semaforo_sarten);
        sem_post(&mydata->semaforos_param.sem_hornear);

        pthread_exit(NULL);
}

void* hornear(void *data){
        char *accion = "HORNEANDO!";
        struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_hornear);
        sem_wait(&semaforo_horno);
        imprimirAccion(mydata,accion);
        usleep(2000000);
        sem_post(&semaforo_horno);
        sem_post(&mydata->semaforos_param.sem_agregar_verduras);

        pthread_exit(NULL);
}


void* empanar(void *data){
        char *accion = "EMPANANDO MILANGA";
        struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_empanar);
        imprimirAccion(mydata,accion);
        usleep(100000);
        sem_post(&mydata->semaforos_param.sem_sarten);
        pthread_exit(NULL);
}

void* agregar_verduras(void *data){
        char *accion = "AGREGANDO VERDURAS";
        struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_agregar_verduras);
        imprimirAccion(mydata,accion);
        usleep(200000);
        sem_post(&mydata->semaforos_param.sem_terminar);
        pthread_exit(NULL);
}
void* terminar(void *data){
        struct parametro *mydata = data;
        char *accion= "TERMINANDO";
        sem_wait(&mydata->semaforos_param.sem_terminar);
        sem_wait(&ganador);
        imprimirAccion(mydata,accion);
        if (equipo_ganador == 0){
                equipo_ganador = mydata->equipo_param;
        }
        sem_post(&ganador);
        pthread_exit(NULL);
}

void* ejecutarReceta(void *i) {

        //variables semaforos
        sem_t sem_mezclar;

        //crear variables semaforos aqui
        sem_t sem_salero;
        sem_t sem_empanar;
        sem_t sem_sarten;
        sem_t sem_hornear;
        sem_t sem_agregar_verduras;
        sem_t sem_terminar;

        //variables hilos
        pthread_t p1;

        //crear variables hilos aqui
        pthread_t p2;
        pthread_t p3;
        pthread_t p4;
        pthread_t p5;
        pthread_t p6;
        pthread_t p7;
        pthread_t p8;

         //numero del equipo (casteo el puntero a un int)
        int p = *((int *) i);

        printf("Ejecutando equipo %d \n", p);

        //reservo memoria para el struct
        struct parametro *pthread_data = malloc(sizeof(struct parametro));

        //seteo los valores al struct

        //seteo numero de grupo
        pthread_data->equipo_param = p;

        //seteo semaforos
        pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
        //setear demas semaforos al struct aqui
        pthread_data->semaforos_param.sem_salero = sem_salero;
        pthread_data->semaforos_param.sem_empanar = sem_empanar;
        pthread_data->semaforos_param.sem_sarten = sem_sarten;
        pthread_data->semaforos_param.sem_sarten = sem_hornear;

        char s_cortar[] = "CORTAR";
        char s_mezclar[] = "MEZCLAR";
        char ajo[] = "ajo";
        char perejil[] = "perejil";
        char huevo[] = "huevo";
        char carne[] = "carne";
        char s_salar[] = "SALAR!";
        char s_empanar[] = "EMPANANDO MILANGA";
        char s_sarten[] = "AL FUEGO!";
        char s_hornear[] = "HORNEANDO!";
        char s_verduras[] = "AGREGANDO VERDURAS";
        char s_terminar[] = "TERMINANDO";
        char milanesa[] = "milanesa";
        char pan[] = "pan";
        char verduras[][LIMITE] = {"Lechuga", "Tomate", "Cebolla Morada", "Pepino"};


         //seteo las acciones y los ingredientes (Faltan acciones e ingredientes) ¿Se ve hardcodeado no? ¿Les parece bien?
            strcpy(pthread_data->pasos_param[0].accion, s_cortar);
            strcpy(pthread_data->pasos_param[0].ingredientes[0], ajo);
            strcpy(pthread_data->pasos_param[0].ingredientes[1], perejil);

            // mezclar
            strcpy(pthread_data->pasos_param[1].accion, s_mezclar);
            strcpy(pthread_data->pasos_param[1].ingredientes[0], ajo);
            strcpy(pthread_data->pasos_param[1].ingredientes[1], perejil);
            strcpy(pthread_data->pasos_param[1].ingredientes[2], huevo);

            // salar
            strcpy(pthread_data->pasos_param[2].accion, s_salar);
            strcpy(pthread_data->pasos_param[2].ingredientes[0], ajo);
            strcpy(pthread_data->pasos_param[2].ingredientes[1], perejil);
            strcpy(pthread_data->pasos_param[2].ingredientes[2], huevo);

            // Agrego carne y empano
            strcpy(pthread_data->pasos_param[3].accion, s_empanar);
            strcpy(pthread_data->pasos_param[3].ingredientes[0], carne);

            // Sarten
            strcpy(pthread_data->pasos_param[4].accion, s_sarten);
            strcpy(pthread_data->pasos_param[4].ingredientes[0], milanesa);

            // Horno
            strcpy(pthread_data->pasos_param[5].accion, s_hornear);
            strcpy(pthread_data->pasos_param[5].ingredientes[0], pan);

            // Agrego verdura
            strcpy(pthread_data->pasos_param[6].accion, s_verduras);
          //  for (i=0; i < strlen(verduras); i++){
        //      int(i);
         //     strcpy(pthread_data->pasos_param[6].ingredientes[i], verduras[i]);
          // }
            strcpy(pthread_data->pasos_param[6].ingredientes[0], "lechuga");
            strcpy(pthread_data->pasos_param[6].ingredientes[1], "tomate");
            strcpy(pthread_data->pasos_param[6].ingredientes[2], "cebolla morada");
            strcpy(pthread_data->pasos_param[6].ingredientes[3], "pepino");

            // Termino
            strcpy(pthread_data->pasos_param[7].accion, s_terminar);

        //inicializo los semaforos
      sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);

        sem_init(&(pthread_data -> semaforos_param.sem_salero),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_sarten),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_empanar),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_hornear),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_agregar_verduras),0,0);
        sem_init(&(pthread_data->semaforos_param.sem_terminar),0,0);


        //creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos
    int rc;

    rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
        //crear demas hilos aqui
        rc = pthread_create(&p2,
                NULL,
                mezclar,
                pthread_data);

        rc = pthread_create(&p3,
                        NULL,
                        salar,
                        pthread_data);

        rc = pthread_create(&p4,
                        NULL,
                        empanar,
                        pthread_data);

        rc = pthread_create(&p5,
                        NULL,
                        aLaSarten,
                        pthread_data);

        rc = pthread_create(&p6,
                        NULL,
                        hornear,
                        pthread_data);

        rc = pthread_create(&p7,
                        NULL,
                        agregar_verduras,
                        pthread_data);

        rc = pthread_create(&p8,
                        NULL,
                        terminar,
                        pthread_data);
    //join de todos los hilos
        pthread_join (p1,NULL);
        //crear join de demas hilos
        pthread_join (p2,NULL);
        pthread_join (p3,NULL);
        pthread_join (p4,NULL);
        pthread_join (p5,NULL);
        pthread_join (p6,NULL);
        pthread_join (p7,NULL);
        pthread_join (p8,NULL);

        //valido que el hilo se alla creado bien
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }

        //destruccion de los semaforos
        sem_destroy(&sem_mezclar);
        //destruir demas semaforos
        sem_destroy(&sem_salero);
        sem_destroy(&sem_sarten);
        sem_destroy(&sem_empanar);
        sem_destroy(&sem_hornear);
        sem_destroy(&sem_agregar_verduras);
        sem_destroy(&sem_terminar);

        //salida del hilo
         pthread_exit(NULL);
}

int main ()
  {
        //creo los nombres de los equipos
        int rc;
        int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
        int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
        int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
        int *equipoNombre4 =malloc(sizeof(*equipoNombre4));
//faltan equipos 
        *equipoNombre1 = 1;
        *equipoNombre2 = 2;
        *equipoNombre3 = 3;
        *equipoNombre4 = 4;

        //creo las variables los hilos de los equipos
        pthread_t equipo1;
        pthread_t equipo2;
        pthread_t equipo3;
        pthread_t equipo4;


        sem_init(&semaforo_salero, 0, 1);
        sem_init(&semaforo_sarten, 0, 1);
        sem_init(&semaforo_horno, 0, 2);
        sem_init(&ganador, 0, 1);

        //inicializo los hilos de los equipos
    rc = pthread_create(&equipo1,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre1);

    rc = pthread_create(&equipo2,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre2);

    rc = pthread_create(&equipo3,
                        NULL,
                        ejecutarReceta,
                        equipoNombre3);

    rc = pthread_create(&equipo4,
                        NULL,
                        ejecutarReceta,
                        equipoNombre4);


   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }

        pthread_join (equipo1,NULL);
        pthread_join (equipo2,NULL);
        pthread_join (equipo3,NULL);
        pthread_join (equipo4,NULL);

        sem_destroy(&semaforo_salero);
        sem_destroy(&semaforo_sarten);
        sem_destroy(&semaforo_horno);
        sem_destroy(&ganador);

        printf("GANADOR: EQUIPO %d \n", equipo_ganador);

    pthread_exit(NULL);
}
