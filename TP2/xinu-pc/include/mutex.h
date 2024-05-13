#ifndef MUTEX_H
#define MUTEX_H

#include <xinu.h>

/* Definir la estructura del mutex */
struct mutex {
    pid32 owner_pid;  // PID del proceso que ha adquirido el mutex
    sid32 sem;        // Semáforo binario asociado al mutex
};

/* Función para inicializar el mutex */
void mutex_init(struct mutex *m);

/* Función para adquirir el mutex (lock) */
void mutex_lock(struct mutex *m);

/* Función para liberar el mutex (unlock) */
void mutex_unlock(struct mutex *m);

#endif /* MUTEX_H */