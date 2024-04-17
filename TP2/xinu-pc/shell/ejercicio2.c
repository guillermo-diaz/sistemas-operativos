#include <xinu.h>


void produce(void), consume(void);

int32 n = 0;
sid32 semProd, semCon; 

void ejercicio2(void){
    semProd = semcreate(1);
    semCon = semcreate(0);
    resume(create(consume, 4000, 20, "cons", 0));
    resume(create(produce, 4000, 20, "prod", 0));
    recvclr();
    receive();
}

void produce(void) {
    int32 i;

    for (i=1; i<=2000;i++){
        wait(semProd);
        n++;
        signal(semCon);
    }

}

void consume(void){
    int32 i;

    for (i=1; i<=2000;i++){
        wait(semCon);
        printf("The value of n is %d \n ",n);
        signal(semProd);
    }
}