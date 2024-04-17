#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>


/*Guillermo Diaz FAI 3197*/
int esPrimo(int numero){
    int esPrimo = 1; 
    int i = 2;
    
    if (numero <= 1)
        return 0;
    
    //el numero de divisores que puede tener un numero es el resultado de la raiz cuadrada del numero
    while (esPrimo && (i * i) <= numero){
        if ((numero % i) == 0){
            esPrimo = 0;
        } else {
            i = i + 1;
        }
    }
    return esPrimo;
}




void encontrarPrimosPadre() {
    
    for (int i = 0; i <=1000; i++) {
        if (esPrimo(i)) 
            printf("- %d \n", i);  
    }

}
void encontrarPrimosHijo() {
    
    for (int i = 1000; i <= 5000; i++) {
        if (esPrimo(i)) 
            printf("+ %d \n", i);    
    }

}


void main(void) {
    int pid = fork();  

    if (pid == 0) {  // Proceso hijo
        printf(" Empezo  el hijo.\n");
        encontrarPrimosHijo();
        printf(" Proceso hijo finalizado.\n");
        exit(0);   
    } else {  // Proceso padre
        printf(" Proceso padre iniciado.\n");
        encontrarPrimosPadre();
        printf(" Proceso padre finalizado\n");
        kill(pid,SIGKILL);
        printf(" Elimino el hijo.\n");
        exit(0);
    } 

    /* rta teoria: el ultimo nùmero fue el 2293 (en mi ejecucion), no le fue posible calcular todos 
    debido a que el padre (en este caso) termino antes que el hijo y lo mató antes de que este finalizara, para solucionar esto 
    se podria utilizar un wait luego de que el padre termine su ejecucion antes de matar al proceso hijo*/
    
}
