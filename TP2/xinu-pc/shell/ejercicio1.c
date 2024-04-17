#include <xinu.h>

void	funcion(char);


void ejercicio1(void)
{
    int pidA, pidB, pidC;

	pidA = create(funcion, 1024, 2, "send A", 1, 'A');
	pidB = create(funcion, 1024, 2, "send B", 1, 'B');
    pidC = create(funcion, 1024, 2, "send C", 1, 'C');
    resume(pidA);
    resume(pidB);
    resume(pidC);
    high_level_scheduler(pidA, 120, pidB, 60, pidC, 20);


}

void	funcion(char ch){
	for(int i=0; i < 5000; i++)
		printf("%c - %d \n ", ch, i);

}