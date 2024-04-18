#include<xinu.h>
int prioridadScheduler2;
int pidScheduler2;

void administrarPrioridades2(int pidProceso, int msProceso){
    int prioridadProceso=getprio(pidProceso); 
    chprio(pidProceso, prioridadScheduler2-1);//Cambia prioridad del proceso a prioriPlanificador-1
    sleepms(msProceso); //Libera CPU
    chprio(pidProceso,prioridadProceso);//Devuelve al proceso prioridad original
}
void high_level_scheduler2(int pidA, int msA, int pidB, int msB){
    pidScheduler2=getpid();
    prioridadScheduler2=getprio(pidScheduler2);
    while(1){
        administrarPrioridades2(pidA, msA);
        administrarPrioridades2(pidB, msB);
    }
}