/* Ejemplo con threads en Linux. Compilar con: gcc -o p p.c -lpthread */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
int total = 0;
void *thread(void * i){
    int n = *((int *) i);
    printf("Este es el thread nro: %i \n", n);
}
int main() {
    int i0=0;
    int i1=1;
    int i2=2;
    pthread_t tid[3];
    /* Create independent threads each of which will execute function */
    pthread_create( &tid[0], NULL, thread, (void *) &i0);
    pthread_create( &tid[1], NULL, thread, (void *) &i1);
    pthread_create( &tid[0], NULL, thread, (void *) &i2);
    /* Wait till threads are complete before main continues. */
    for (int i = 0; i <= 2; i++)
        pthread_join(tid[i], NULL);
        return 0;
}