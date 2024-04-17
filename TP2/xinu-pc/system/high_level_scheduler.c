#include <xinu.h>

void high_level_scheduler(int pidA,int  msA,int  pidB,int  msB,int  pidC,int  msC){
    int pidScheduler, priorityScheduler;
    pidScheduler = getpid();
    priorityScheduler = getprio(pidScheduler);
    
    while(1){
        changePrio(pidA, msA, priorityScheduler);
        changePrio(pidB, msB, priorityScheduler);
        changePrio(pidC, msC, priorityScheduler);
    }
}

void changePrio(int pidProceso, int ms_p, int prioScheduler){
    int prioProc = getprio(pidProceso);
    chprio(pidProceso, prioScheduler - 1);
    sleepms(ms_p);
    chprio(pidProceso, prioProc);
}