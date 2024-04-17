//#include <xinu.h>
#include "mutex.h"


void operar(void), incrementar(void);

unsigned char x = 0;
struct mutex miMutex; // Declarar una variable global para el mutex

void ejercicio3(void)
{
  mutex_init(&miMutex);

    resume(create(operar, 8000, 20, "procces 1", 0));
    resume(create(incrementar, 8000, 20, "procces 2", 0));

    sleep(10);
    recvclr();
receive();
}

void operar(void)
{
    int y = 0;
    printf("Si no existen mensajes de error entonces todo va ok!\n");
    while (1)
    {
        // Adquirir el mutex antes de acceder a la variable x
        mutex_lock(&miMutex);

        if ((x % 10) == 0)
        {
            y = x * 2;

            if ((y % 10) != 0)
            {
                printf("\r ERROR!! y=%d , x=%d \r ", y, x);
            }
        }

        // Liberar el mutex despuess de acceder a la variable x
        mutex_unlock(&miMutex);
    }
}

void incrementar(void)
{
    while (1)
    {
        // Adquirir el mutex antes de acceder a la variable x
        mutex_lock(&miMutex);

        x = x + 1;

        // Liberar el mutex despue de acceder a la variable x
        mutex_unlock(&miMutex);
    }
}